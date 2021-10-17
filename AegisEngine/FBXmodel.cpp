#include "FBXmodel.h"

#include	"manager.h"
#include	"Scene.h"
#include	"ShadowMap.h"
#include	"camera.h"

#include	"external/DirectXTex/WICTextureLoader.h"

using namespace Aegis;

static string textype;
static string directory;

XMMATRIX aiMatrixToMatrix(aiMatrix4x4 matrix)
{
	XMMATRIX m;
	m.r[0] = XMLoadFloat4(&Vector4(matrix.a1, matrix.a2, matrix.a3, matrix.a4));
	m.r[1] = XMLoadFloat4(&Vector4(matrix.b1, matrix.b2, matrix.b3, matrix.b4));
	m.r[2] = XMLoadFloat4(&Vector4(matrix.c1, matrix.c2, matrix.c3, matrix.c4));
	m.r[3] = XMLoadFloat4(&Vector4(matrix.d1, matrix.d2, matrix.d3, matrix.d4));

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



bool FBXmodel::Load(const string& FileName)
{
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

		vector<UINT> Bone_num;
		Bone_num.reserve(mesh->mNumVertices);

		vector<ANIME_VERTEX> vertex;
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
		ID3D11Buffer* vertex_Buffer;
		{
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(ANIME_VERTEX) * (UINT)vertex.size();
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA initData;
			ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));

			initData.pSysMem = vertex.data();
			initData.SysMemPitch = 0;
			initData.SysMemSlicePitch = 0;

			hr = CRenderer::GetDevice()->CreateBuffer(&bd, &initData, &vertex_Buffer);

			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}
		}
		

		// インデックスバッファ生成
		UINT index_Num;
		ID3D11Buffer* index_Beffer;
		{
			vector<WORD> index;
			for (UINT f = 0; f < mesh->mNumFaces; f++)
			{
				aiFace* face = &mesh->mFaces[f];
				for (UINT i = 0; i < face->mNumIndices; i++)
				{
					index.emplace_back(face->mIndices[i]);
				}
			}

			index_Num = (UINT)index.size();
	
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(WORD) * (UINT)index.size();
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA initData;
			ZeroMemory(&initData, sizeof(initData));
			initData.pSysMem = index.data();
			initData.SysMemPitch = 0;
			initData.SysMemSlicePitch = 0;

			hr = CRenderer::GetDevice()->CreateBuffer(&bd, &initData, &index_Beffer);
			
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}
		}

		MESH temp_mesh;
		temp_mesh.VertexBuffer= vertex_Buffer;
		temp_mesh.IndexBuffer = index_Beffer;
		temp_mesh.VertexNum = vertex_Num;
		temp_mesh.IndexNum = index_Num;

		m_Meshes.emplace_back(temp_mesh);

		// テクスチャの設定
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = m_Scene->mMaterials[mesh->mMaterialIndex];

			if (textype.empty()) textype = determineTextureType(m_Scene, material);

			vector<TEXTURE_S> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", m_Scene);
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
		ID3D11Buffer* buffer = nullptr;

		D3D11_BUFFER_DESC hBufferDesc;
		hBufferDesc.ByteWidth = sizeof(XMMATRIX) * m_BoneNum;
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		hBufferDesc.MiscFlags = 0;
		hBufferDesc.StructureByteStride = sizeof(float);

		{
			hr = CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, NULL, &buffer);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			MatrixBuffer.reset(buffer);
		}
	}

	return true;
}


void FBXmodel::UnLoad()
{
	for (UINT m = 0; m < m_MeshNum; m++)
	{
		SAFE_RELEASE(m_Meshes[m].VertexBuffer);
		SAFE_RELEASE(m_Meshes[m].IndexBuffer);
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
	{
		vector<XMMATRIX> bone;
		bone.reserve(m_BoneNum);

		/*for (UINT b = 0; b < m_BoneNum; b++)
		{
			bone.emplace_back(m_Bone[b].Matrix);
		}*/

		set_bone(m_Scene->mRootNode, bone);

		SetBoneMatrix(bone);
	}

	//CRenderer::Set_Shader(SHADER_INDEX_V::ANIMATION, SHADER_INDEX_P::DEFAULT);
	CRenderer::Set_InputLayout(INPUTLAYOUT::ANIMATION);

	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DrawMesh(m_Scene->mRootNode, Matrix);

	CRenderer::Set_InputLayout();
	CRenderer::Set_Shader();
}

void FBXmodel::Draw_DPP(XMMATRIX& Matrix)
{
	{
		vector<XMMATRIX> bone;
		bone.reserve(m_BoneNum);

		set_bone(m_Scene->mRootNode, bone);

		SetBoneMatrix(bone);
	}

	CRenderer::Set_InputLayout(INPUTLAYOUT::ANIMATION);
	CRenderer::Set_Shader(SHADER_INDEX_V::DEPTH_PRE_ANIME, SHADER_INDEX_P::MAX);

	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DrawMesh_DPP(m_Scene->mRootNode, Matrix);

	CRenderer::Set_InputLayout();
	CRenderer::Set_Shader(SHADER_INDEX_V::DEPTH_PRE, SHADER_INDEX_P::MAX);
}

void FBXmodel::set_bone(const aiNode* Node, vector<XMMATRIX>& v)
{
	v.emplace_back( m_Bone[m_BoneIndex[Node->mName.C_Str()] ].Matrix );

	for (unsigned int i = 0; i < Node->mNumChildren; i++)
	{
		set_bone(Node->mChildren[i], v);
	}
}

void FBXmodel::SetBoneMatrix(const vector<XMMATRIX>& matrix)
{
	CRenderer::GetDeviceContext()->UpdateSubresource(MatrixBuffer.get(), 0, NULL, matrix.data(), 0, 0);

	auto buffer = MatrixBuffer.get();

	CRenderer::GetDeviceContext()->VSSetConstantBuffers(6, 1, &buffer);
}

void FBXmodel::DrawMesh(const aiNode* Node, const XMMATRIX& Matrix)
{
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
					CRenderer::Set_MatrixBuffer(world, view, proj);

					CRenderer::Set_Shader(SHADER_INDEX_V::SHADOW_MAP_ANIMATION, SHADER_INDEX_P::SHADOW_MAP);
				}
				else
				{
					CRenderer::Set_MatrixBuffer(world, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());

					CRenderer::Set_MatrixBuffer01(*camera01.lock()->Get_Pos());

					CRenderer::Set_Shader(SHADER_INDEX_V::ANIMATION, SHADER_INDEX_P::DEFAULT);
				}
			}
			else
			{
				// シャドウマップ用の描画か?
				if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
				{
					CRenderer::Set_MatrixBuffer(world, view, proj);

					CRenderer::Set_Shader(SHADER_INDEX_V::SHADOW_MAP_ANIMATION, SHADER_INDEX_P::SHADOW_MAP);
				}
				else
				{

					CRenderer::Set_MatrixBuffer(world, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());

					CRenderer::Set_MatrixBuffer01(*camera02.lock()->Get_Pos());

					CRenderer::Set_Shader(SHADER_INDEX_V::ANIMATION, SHADER_INDEX_P::DEFAULT);
				}
			}
		}

		// テクスチャの設定
		{
			CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &Textures[0].Texture);
		}

		// 頂点バッファの設定
		{
			const UINT stride = sizeof(ANIME_VERTEX);
			const UINT offset = 0;
			ID3D11Buffer* vb[1] = { m_Meshes[m].VertexBuffer };
			CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, vb, &stride, &offset);
		}

		// インデックスバッファの設定
		{
			CRenderer::SetIndexBuffer(m_Meshes[m].IndexBuffer);
		}

		CRenderer::DrawIndexed(m_Meshes[m].IndexNum, 0, 0);
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
				CRenderer::Set_MatrixBuffer(world, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());
			}
			else
			{
				CRenderer::Set_MatrixBuffer(world, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());
			}
		}

		// 頂点バッファの設定
		{
			const UINT stride = sizeof(ANIME_VERTEX);
			const UINT offset = 0;
			ID3D11Buffer* vb[1] = { m_Meshes[m].VertexBuffer };
			CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, vb, &stride, &offset);
		}

		// インデックスバッファの設定
		{
			CRenderer::SetIndexBuffer(m_Meshes[m].IndexBuffer);
		}

		CRenderer::DrawIndexed(m_Meshes[m].IndexNum, 0, 0);
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




vector<TEXTURE_S> FBXmodel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName, const aiScene* scene)
{
	vector<TEXTURE_S> textures;
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
			HRESULT hr;
			TEXTURE_S texture;
			if (textype == "embedded compressed texture")
			{
				int textureindex = getTextureIndex(&str);
				texture.Texture = getTextureFromModel(scene, textureindex);
			}
			else
			{
				string filename = string(str.C_Str());
				filename = directory + "/" + filename;
				wstring filenamews = wstring(filename.begin(), filename.end());

				{
					hr = CreateWICTextureFromFile(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), filenamews.c_str(), nullptr, &texture.Texture, nullptr, nullptr);
				}

				if (FAILED(hr))
					FAILDE_ASSERT;
			}

			texture.FileName = str.C_Str();
			textures.push_back(texture);
			this->Textures.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}

string FBXmodel::determineTextureType(const aiScene* scene, aiMaterial* mat)
{
	aiString textypeStr;
	mat->GetTexture(aiTextureType_DIFFUSE, 0, &textypeStr);
	string textypeteststr = textypeStr.C_Str();
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
	if (textypeteststr.find('.') != string::npos)
	{
		return "textures are on disk";
	}
}

int FBXmodel::getTextureIndex(aiString* str)
{
	string tistr;
	tistr = str->C_Str();
	tistr = tistr.substr(1);
	return stoi(tistr);
}

ID3D11ShaderResourceView* FBXmodel::getTextureFromModel(const aiScene* scene, int textureindex)
{
	HRESULT hr;
	ID3D11ShaderResourceView* texture;

	int* size = reinterpret_cast<int*>(&scene->mTextures[textureindex]->mWidth);

	hr = CreateWICTextureFromMemory(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), reinterpret_cast<unsigned char*>(scene->mTextures[textureindex]->pcData), *size, nullptr, &texture);
	if (FAILED(hr))
		FAILDE_ASSERT

		return texture;
}