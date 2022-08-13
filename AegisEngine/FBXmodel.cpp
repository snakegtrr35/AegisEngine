#include "FBXmodel.h"

#include "manager.h"
#include "Scene.h"
#include "ShadowMap.h"
#include "camera.h"
#include "Renderer.h"

#include "TextureImporter.h"

using namespace aegis;

static aegis::string textype;
static aegis::string directory;

XMMATRIX aiMatrixToMatrix(aiMatrix4x4 matrix)
{
	XMMATRIX m;

	Vector4 vec = Vector4(matrix.a1, matrix.a2, matrix.a3, matrix.a4);

	m.r[0] = XMLoadFloat4(&vec);
	m.r[1] = XMLoadFloat4( &(vec = Vector4(matrix.b1, matrix.b2, matrix.b3, matrix.b4)) );
	m.r[2] = XMLoadFloat4( &(vec = Vector4(matrix.c1, matrix.c2, matrix.c3, matrix.c4)) );
	m.r[3] = XMLoadFloat4( &(vec = Vector4(matrix.d1, matrix.d2, matrix.d3, matrix.d4)) );

	return m;
}

aiMatrix4x4 MatrixToaiMatrix(XMMATRIX matrix)
{
	aiMatrix4x4 m;
	m.a1 = XMVectorGetX(matrix.r[0]);
	m.a2 = XMVectorGetY(matrix.r[0]);
	m.a3 = XMVectorGetZ(matrix.r[0]);
	m.a4 = XMVectorGetW(matrix.r[0]);

	m.b1 = XMVectorGetX(matrix.r[1]);
	m.b2 = XMVectorGetY(matrix.r[1]);
	m.b3 = XMVectorGetZ(matrix.r[1]);
	m.b4 = XMVectorGetW(matrix.r[1]);

	m.c1 = XMVectorGetX(matrix.r[2]);
	m.c2 = XMVectorGetY(matrix.r[2]);
	m.c3 = XMVectorGetZ(matrix.r[2]);
	m.c4 = XMVectorGetW(matrix.r[2]);
	
	m.d1 = XMVectorGetX(matrix.r[3]);
	m.d2 = XMVectorGetY(matrix.r[3]);
	m.d3 = XMVectorGetZ(matrix.r[3]);
	m.d4 = XMVectorGetW(matrix.r[3]);
	
	return m;
}



bool FBXmodel::Load(const aegis::string& FileName)
{
	CRenderer* render = CRenderer::getInstance();
	HRESULT hr;

	//モデルの読み込み
	m_Scene = aiImportFile(FileName.c_str(), aiProcess_Triangulate | aiProcess_LimitBoneWeights | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality);
	
	if (nullptr == m_Scene)
	{
		return false;
	}

	if (false == m_Scene->HasAnimations())
	{
		FAILDE_ASSERT;
		return false;
	}

	directory = FileName.substr(0, FileName.find_last_of('/'));

	CreateBone(m_Scene->mRootNode);

	for (UINT m = 0; m < m_Scene->mNumMeshes; m++)
	{		
		if (false == m_Scene->mMeshes[m]->HasBones())
		{
			FAILDE_ASSERT;
			return false;
		}

		aiMesh* mesh = m_Scene->mMeshes[m];
		UINT vertex_Num = mesh->mNumVertices;

		aegis::vector<UINT> Bone_num;
		Bone_num.reserve(mesh->mNumVertices);

		aegis::vector<ANIME_VERTEX> vertex;
		vertex.reserve(mesh->mNumVertices);

		for (UINT i = 0; i < mesh->mNumVertices; i++)
		{
			ANIME_VERTEX anime_vertex;

			anime_vertex.Position = Vector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			anime_vertex.Normal = Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			anime_vertex.Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

			if (mesh->mTextureCoords[0])
			{
				anime_vertex.TexCoord.x = (float)mesh->mTextureCoords[0][i].x;
				anime_vertex.TexCoord.y = (float)mesh->mTextureCoords[0][i].y;
			}

			vertex.emplace_back(anime_vertex);
			Bone_num.emplace_back(0);
		}

		
		for (UINT b = 0; b < mesh->mNumBones; b++)
		{

			aiBone* bone = mesh->mBones[b];
			m_Bone[m_BoneIndex[bone->mName.C_Str()]].OffsetMatrix =aiMatrixToMatrix(bone->mOffsetMatrix);

  			for (unsigned int w = 0; w < bone->mNumWeights; w++)
			{
				aiVertexWeight weight = bone->mWeights[w];
				vertex[weight.mVertexId].bone_Index[Bone_num[weight.mVertexId]] = m_BoneIndex[bone->mName.C_Str()];
				vertex[weight.mVertexId].weight[Bone_num[weight.mVertexId]] = weight.mWeight;
				Bone_num[weight.mVertexId]++;
			}
		}


		//頂点バッファ生成
		Buffer* vertex_Buffer;
		{
			BufferDesc  bd{};
			bd.Usage = Usage::Default;
			bd.ByteWidth = sizeof(ANIME_VERTEX) * (UINT)vertex.size();
			bd.BindFlags = BindFlag::Vertexbuffer;
			bd.CPUAccessFlags = CpuAccessFlag::None;

			SubresourceData sd{};
			sd.pSysMem = vertex.data();

			vertex_Buffer = render->CreateBuffer(bd, sd);
		}
		

		// インデックスバッファ生成
		UINT index_Num;
		Buffer* index_Beffer;
		{
			aegis::vector<WORD> index;
			for (UINT f = 0; f < mesh->mNumFaces; f++)
			{
				aiFace* face = &mesh->mFaces[f];
				for (UINT i = 0; i < face->mNumIndices; i++)
				{
					index.emplace_back(face->mIndices[i]);
				}
			}

			index_Num = (UINT)index.size();

			BufferDesc  bd{};
			bd.Usage = Usage::Default;
			bd.ByteWidth = sizeof(uint16) * (UINT)index.size();
			bd.BindFlags = BindFlag::Indexbuffer;
			bd.CPUAccessFlags = CpuAccessFlag::None;

			SubresourceData sd{};
			sd.pSysMem = index.data();

			index_Beffer = render->CreateBuffer(bd, sd);
		}

		MESH temp_mesh;
		temp_mesh.VertexBuffer.reset(vertex_Buffer);
		temp_mesh.IndexBuffer.reset(index_Beffer);
		temp_mesh.VertexNum = vertex_Num;
		temp_mesh.IndexNum = index_Num;

		m_Meshes.emplace_back(std::move(temp_mesh));

		// テクスチャの設定
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = m_Scene->mMaterials[mesh->mMaterialIndex];

			if (textype.empty()) textype = determineTextureType(m_Scene, material);

			aegis::vector<TEXTURE_S> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", m_Scene);
			Textures.insert(Textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			for (auto i : diffuseMaps)
			{
				i.FileName.clear();
				i.Texture = nullptr;
			}
			diffuseMaps.clear();
		}

		Bone_num.clear();
		vertex.clear();
	}

	// 定数バッファ作成
	{
		BufferDesc  bd{};
		bd.Usage = Usage::Default;
		bd.ByteWidth = sizeof(XMMATRIX) * m_BoneNum;
		bd.BindFlags = BindFlag::Constantbuffer;
		bd.CPUAccessFlags = CpuAccessFlag::None;

		SubresourceData sd{};

		MatrixBuffer.reset(render->CreateBuffer(bd, sd));
	}

	return true;
}


void FBXmodel::UnLoad()
{
	for (auto& mesh : m_Meshes)
	{
		mesh.VertexBuffer.reset();
		mesh.IndexBuffer.reset();
	}

	m_Meshes.clear();
	
	aiReleaseImport(m_Scene);
}

void FBXmodel::Update(float delta_time)
{
	static float fr = 0.0f;

	fr += delta_time;

	if (ANIMETION_FRAME_30 <= fr)
	{
		frame++;

		fr = 0.0f;
	}

	{
		aiAnimation* animation = m_Scene->mAnimations[0];
		for (unsigned int c = 0; c < animation->mNumChannels; c++)
		{
			aiNodeAnim* nodeAnim = animation->mChannels[c];
			BONE* bone = &m_Bone[m_BoneIndex[nodeAnim->mNodeName.C_Str()]];

			int f = frame % nodeAnim->mNumRotationKeys;
			aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

			f = frame % nodeAnim->mNumPositionKeys;
			aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

			//f = frame % nodeAnim->mNumScalingKeys;
			//aiVector3D scale = nodeAnim->mScalingKeys[f].mValue;

			bone->AnimationMatrix = aiMatrixToMatrix(aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos));
		}
	}

	XMMATRIX Identity = XMMatrixIdentity();

	UpdateBoneMatrix(m_Scene->mRootNode, Identity);
}

void FBXmodel::Draw(XMMATRIX &Matrix)
{
	CRenderer* render = CRenderer::getInstance();
	{
		aegis::vector<XMMATRIX> bone;
		bone.reserve(m_BoneNum);

		/*for (UINT b = 0; b < m_BoneNum; b++)
		{
			bone.emplace_back(m_Bone[b].Matrix);
		}*/

		set_bone(m_Scene->mRootNode, bone);

		SetBoneMatrix(bone);
	}

	//render->Set_Shader(SHADER_INDEX_V::ANIMATION, SHADER_INDEX_P::DEFAULT);
	render->Set_InputLayout(INPUTLAYOUT::ANIMATION);

	render->SetPrimitiveTopology(PrimitiveTopology::TriangleList);

	DrawMesh(m_Scene->mRootNode, Matrix);

	render->Set_InputLayout();
	render->Set_Shader();
}

void FBXmodel::Draw_DPP(XMMATRIX& Matrix)
{
	CRenderer* render = CRenderer::getInstance();
	{
		aegis::vector<XMMATRIX> bone;
		bone.reserve(m_BoneNum);

		set_bone(m_Scene->mRootNode, bone);

		SetBoneMatrix(bone);
	}

	render->Set_InputLayout(INPUTLAYOUT::ANIMATION);
	render->Set_Shader(SHADER_INDEX_V::DEPTH_PRE_ANIME, SHADER_INDEX_P::MAX);

	render->SetPrimitiveTopology(PrimitiveTopology::TriangleList);

	DrawMesh_DPP(m_Scene->mRootNode, Matrix);

	render->Set_InputLayout();
	render->Set_Shader(SHADER_INDEX_V::DEPTH_PRE, SHADER_INDEX_P::MAX);
}

void FBXmodel::set_bone(const aiNode* Node, aegis::vector<XMMATRIX>& v)
{
	v.emplace_back( m_Bone[m_BoneIndex[Node->mName.C_Str()] ].Matrix );

	for (unsigned int i = 0; i < Node->mNumChildren; i++)
	{
		set_bone(Node->mChildren[i], v);
	}
}

void FBXmodel::SetBoneMatrix(aegis::vector<XMMATRIX>& matrix) const
{
	CRenderer* render = CRenderer::getInstance();

	render->UpdateSubresource(MatrixBuffer.get(), matrix.data());

	auto buffer = MatrixBuffer.get();

	render->VSSetConstantBuffers(6, 1, &buffer);
}

void FBXmodel::DrawMesh(const aiNode* Node, const XMMATRIX& Matrix)
{
	CRenderer* render = CRenderer::getInstance();

	XMMATRIX world;
	world = XMMatrixTranspose(aiMatrixToMatrix(Node->mTransformation));
	world *= Matrix;

	auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
	auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

	XMMATRIX view = CManager::Get_Instance()->Get_ShadowMap()->Get_View();
	XMMATRIX proj = CManager::Get_Instance()->Get_ShadowMap()->Get_Plojection();

	for (UINT n = 0; n < Node->mNumMeshes; n++)
	{
		UINT m = Node->mMeshes[n];

		// 3Dマトリックス設定
		{
			// 普通のカメラかデバッグカメラか?
			if (!camera01.expired() /*&& Empty_weak_ptr<CCamera>(camera01)*/)
			{
				// シャドウマップ用の描画か?
				if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
				{
					render->Set_MatrixBuffer(world, view, proj);

					render->Set_Shader(SHADER_INDEX_V::SHADOW_MAP_ANIMATION, SHADER_INDEX_P::SHADOW_MAP);
				}
				else
				{
					render->Set_MatrixBuffer(world, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());

					render->Set_MatrixBuffer01(*camera01.lock()->Get_Pos());

					render->Set_Shader(SHADER_INDEX_V::ANIMATION, SHADER_INDEX_P::DEFAULT);
				}
			}
			else
			{
				// シャドウマップ用の描画か?
				if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
				{
					render->Set_MatrixBuffer(world, view, proj);

					render->Set_Shader(SHADER_INDEX_V::SHADOW_MAP_ANIMATION, SHADER_INDEX_P::SHADOW_MAP);
				}
				else
				{

					render->Set_MatrixBuffer(world, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());

					render->Set_MatrixBuffer01(*camera02.lock()->Get_Pos());

					render->Set_Shader(SHADER_INDEX_V::ANIMATION, SHADER_INDEX_P::DEFAULT);
				}
			}
		}

		// テクスチャの設定
		{
			render->PSSetShaderResources(0, 1, &Textures[0].Texture);
		}

		// 頂点バッファの設定
		{
			render->SetVertexBuffers(m_Meshes[m].VertexBuffer.get(), sizeof(ANIME_VERTEX));
		}

		// インデックスバッファの設定
		{
			render->SetIndexBuffer(m_Meshes[m].IndexBuffer.get());
		}

		render->DrawIndexed(m_Meshes[m].IndexNum, 0, 0);
	}

	for (unsigned int i = 0; i < Node->mNumChildren; i++)
	{
		DrawMesh(Node->mChildren[i], world);
	}

	//camera01.reset();
	//camera02.reset();
}

void FBXmodel::DrawMesh_DPP(const aiNode* Node, const XMMATRIX& Matrix)
{
	CRenderer* render = CRenderer::getInstance();

	XMMATRIX world;
	world = XMMatrixTranspose(aiMatrixToMatrix(Node->mTransformation));
	world *= Matrix;

	const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
	const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

	for (UINT n = 0; n < Node->mNumMeshes; n++)
	{
		UINT m = Node->mMeshes[n];

		// 3Dマトリックス設定
		{
			// 普通のカメラかデバッグカメラか?
			if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
			{
				render->Set_MatrixBuffer(world, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());
			}
			else
			{
				render->Set_MatrixBuffer(world, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());
			}
		}

		// 頂点バッファの設定
		{
			render->SetVertexBuffers(m_Meshes[m].VertexBuffer.get(), sizeof(ANIME_VERTEX));
		}

		// インデックスバッファの設定
		{
			render->SetIndexBuffer(m_Meshes[m].IndexBuffer.get());
		}

		render->DrawIndexed(m_Meshes[m].IndexNum, 0, 0);
	}

	for (unsigned int i = 0; i < Node->mNumChildren; i++)
	{
		DrawMesh_DPP(Node->mChildren[i], world);
	}
}


void FBXmodel::CreateBone(const aiNode* Node)
{
	m_BoneIndex[Node->mName.C_Str()] = m_BoneNum;

	m_Bone[m_BoneNum] = BONE();

	m_BoneNum++;

	for (unsigned int i = 0; i < Node->mNumChildren; i++)
	{
		CreateBone(Node->mChildren[i]);
	}
}


void FBXmodel::UpdateBoneMatrix(const aiNode* Node, const XMMATRIX& Matrix)
{
	BONE* bone = &m_Bone[m_BoneIndex[Node->mName.C_Str()]];
	XMMATRIX world;
	XMMATRIX matrix, aMatrix;

	matrix = Matrix;
	aMatrix = bone->AnimationMatrix;
	
	world = matrix * aMatrix;
	
	bone->Matrix = world * bone->OffsetMatrix;

	for (unsigned int i = 0; i < Node->mNumChildren; i++)
	{
		UpdateBoneMatrix(Node->mChildren[i], world);
	}
}




aegis::vector<TEXTURE_S> FBXmodel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, aegis::string typeName, const aiScene* scene)
{
	aegis::vector<TEXTURE_S> textures;
	for (UINT i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (UINT j = 0; j < Textures.size(); j++)
		{
			if (Textures[j].FileName.c_str() == str.C_Str())
			{
				textures.push_back(Textures[j]);
				skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // If texture hasn't been loaded already, load it
			TEXTURE_S texture;
			if (textype == "embedded compressed texture")
			{
				int textureindex = getTextureIndex(&str);
				texture.Texture = getTextureFromModel(scene, textureindex);
			}
			else
			{
				aegis::string filename = aegis::string(str.C_Str());
				filename = directory + "/" + filename;
				std::wstring filenamews = std::wstring(filename.begin(), filename.end());

				{
					CRenderer* render = CRenderer::getInstance();

					TextureImporter* textureImporter = TextureImporter::getInstance();

					texture.Texture = textureImporter->GetShaderResourceView(filenamews.c_str());
				}
			}

			texture.FileName = str.C_Str();
			textures.push_back(texture);
			this->Textures.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}

aegis::string FBXmodel::determineTextureType(const aiScene* scene, aiMaterial* mat)
{
	aiString textypeStr;
	mat->GetTexture(aiTextureType_DIFFUSE, 0, &textypeStr);
	aegis::string textypeteststr = textypeStr.C_Str();
	if (textypeteststr == "" || textypeteststr == "*0" || textypeteststr == "*1" || textypeteststr == "*2" || textypeteststr == "*3" || textypeteststr == "*4" || textypeteststr == "*5")
	{
		if (scene->mTextures[0]->mHeight == 0)
		{
			return "embedded compressed texture";
		}
		else
		{
			return "embedded non-compressed texture";
		}
	}
	if (textypeteststr.find('.') != aegis::string::npos)
	{
		return "textures are on disk";
	}
}

int FBXmodel::getTextureIndex(aiString* str)
{
	aegis::string tistr;
	tistr = str->C_Str();
	tistr = tistr.substr(1);
	return std::stoi(tistr.c_str());
}

aegis::ShaderResourceView* FBXmodel::getTextureFromModel(const aiScene* scene, int textureindex)
{
	TextureImporter* textureImporter = TextureImporter::getInstance();

	aegis::ShaderResourceView* texture;

	texture = textureImporter->GetShaderResourceView(scene->mTextures[textureindex]->pcData, scene->mTextures[textureindex]->mWidth);

	return texture;
}