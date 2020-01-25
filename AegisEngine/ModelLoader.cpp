#include	"ModelLoader.h"
#include	"Library/DirectXTex/WICTextureLoader.h"

#include	"manager.h"
#include	"Scene.h"
#include	"camera.h"

#include	"Renderer.h"

Anim createAnimation(const aiAnimation* anim);
NodeAnim createNodeAnim(const aiNodeAnim* anim);

VectorKey fromAssimp(const aiVectorKey& key);
QuatKey fromAssimp(const aiQuatKey& key);

XMMATRIX Covert_Matrix(const aiMatrix4x4* matrix);

static string textype;

bool flag = true;

CMODEL::CMODEL()
{
	Enable = true;
	Frame = 0;
}

CMODEL::~CMODEL()
{
	Uninit();
}

bool CMODEL::Load(const string& filename)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(filename,
		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_LimitBoneWeights | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality);

	if (pScene == NULL)
		return false;

	this->directory = filename.substr(0, filename.find_last_of('/'));

	//Meshes[pScene->mRootNode->mName.C_Str()] = MESH();
	//processNode(pScene->mRootNode, nullptr, pScene, Meshes);
	processNode(pScene->mRootNode, pScene, Meshes.Get());

	// アニメーション情報の設定
	{
		Anim animation;

		if (pScene->HasAnimations())
		{
			aiAnimation** anim = pScene->mAnimations;

			for (UINT i = 0; i < pScene->mNumAnimations; i++)
			{
				animation = createAnimation(anim[i]);

				Meshes.SetAnimation(anim[i]->mName.data, animation);

				Anime_State_Machine.Add_Name(anim[i]->mName.data);
			}
		}

		////
		//pScene = importer.ReadFile("asset/model/human01_Walk.fbx",
		//	aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_LimitBoneWeights | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality);

		//if (pScene->HasAnimations())
		//{
		//	aiAnimation** anim = pScene->mAnimations;

		//	for (UINT i = 0; i < pScene->mNumAnimations; i++)
		//	{
		//		animation = createAnimation(anim[i]);

		//		Meshes.SetAnimation(anim[i]->mName.data, animation);

		//		Anime_State_Machine.Add_Name(anim[i]->mName.data);
		//	}
		//}

		////
		//pScene = importer.ReadFile("asset/model/human01_Jump.fbx",
		//	aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_LimitBoneWeights | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality);

		//if (pScene->HasAnimations())
		//{
		//	aiAnimation** anim = pScene->mAnimations;

		//	for (UINT i = 0; i < pScene->mNumAnimations; i++)
		//	{
		//		animation = createAnimation(anim[i]);

		//		Meshes.SetAnimation(anim[i]->mName.data, animation);

		//		Anime_State_Machine.Add_Name(anim[i]->mName.data);
		//	}
		//}
	}

	Anime_State_Machine.Set_Anime_Name("Stop");

	return true;
}

bool CMODEL::Reload(const string& filename)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(filename,
		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_LimitBoneWeights | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality);

	if (pScene == NULL)
		return false;

	this->directory = filename.substr(0, filename.find_last_of('/'));

	//Meshes[pScene->mRootNode->mName.C_Str()] = MESH();
	//processNode(pScene->mRootNode, nullptr, pScene, Meshes);if (camera
	processNode(pScene->mRootNode, pScene, Meshes.Get());

	{
		vector<Anim> animation;

		if (pScene->HasAnimations())
		{
			aiAnimation** anim = pScene->mAnimations;

			for (UINT i = 0; i < pScene->mNumAnimations; i++)
			{
				animation.push_back(createAnimation(anim[i]));
			}

			// 1番目のメッシュにアニメーション情報を保存する
			//Meshes.begin()->second.SetAnimation(animation);
			//Meshes.SetAnimation(animation);
		}
	}

	return true;
}

void CMODEL::Draw()
{
	const auto camera = CManager::Get_Scene()->Get_Game_Object<CCamera>("camera");

	if (camera.expired() && Empty_weak_ptr<CCamera>(camera))
	{
		if (false == camera.lock()->Get_Visibility(Position))
		{
			return;
		}
	}

	XMMATRIX matrix = XMMatrixIdentity();
	XMMATRIX scaling = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
	XMMATRIX rotation  = /*XMMatrixIdentity();=*/ XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));
	XMMATRIX transform = XMMatrixTranslation(Position.x, Position.y, Position.z);

	// クォータニオン
	if(0.0f != (Rotation.x + 0.0001f) && 0.0f != (Rotation.y + 0.0001f) && 0.0f != (Rotation.z + 0.0001f))
	{
		//XMVECTOR Quaternion = XMQuaternionIdentity();

		//XMVECTOR axisX = XMVectorSet(1.0f, 0.f, 0.f, 0.f);
		//XMVECTOR axisY = XMVectorSet(0.0f, 1.0f, 0.f, 0.f);
		//XMVECTOR axisZ = XMVectorSet(0.0f, 0.f, 1.0f, 0.f);

		//XMVECTOR rotateX = XMQuaternionRotationAxis(axisX, XMConvertToRadians(Rotation.x));
		//rotateX = XMVector4Normalize(rotateX);
		//XMVECTOR rotateY = XMQuaternionRotationAxis(axisY, XMConvertToRadians(Rotation.y));
		//rotateY = XMVector4Normalize(rotateY);
		//XMVECTOR rotateZ = XMQuaternionRotationAxis(axisZ, XMConvertToRadians(Rotation.z));
		//rotateZ = XMVector4Normalize(rotateZ);

		//Quaternion = XMQuaternionNormalize(Quaternion);

		//rotation = XMMatrixRotationQuaternion(Quaternion);


		//rotation = XMMatrixRotationQuaternion(Math::Quaternion::Euler(Rotation));
	}

	matrix = XMMatrixMultiply(matrix, scaling);

	matrix = XMMatrixMultiply(matrix, rotation);

	matrix = XMMatrixMultiply(matrix, transform);

	{
		const auto camera01 = CManager::Get_Scene()->Get_Game_Object<CCamera>("camera");
		const auto camera02 = CManager::Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

		{
			XMVECTOR camera_pos;
			LIGHT light = *CRenderer::Get_Light();

			XMVECTOR light_pos = XMVectorSet(light.Direction.x, light.Direction.y, light.Direction.z, light.Direction.w);

			light_pos = XMVectorScale(light_pos, 10.0f);

			if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
			{
				camera_pos = *camera01.lock()->Get_Pos();

				XMFLOAT4 pos;
				XMStoreFloat4(&pos, camera_pos);
				//pos.w = 1.0f;
				camera_pos = XMLoadFloat4(&pos);
				
			}
			else
			{
				camera_pos = *camera02.lock()->Get_Pos();

				XMFLOAT4 pos;
				XMStoreFloat4(&pos, camera_pos);
				//pos.w = 1.0f;
				camera_pos = XMLoadFloat4(&pos);
			}

			CRenderer::Set_MatrixBuffer01(camera_pos);
		}

		if (CManager::Get_ShadowMap()->Get_Enable())
		{
			CRenderer::Set_Shader(SHADER_INDEX_V::SHADOW_MAP, SHADER_INDEX_P::SHADOW_MAP);
		}
		else
		{
			CRenderer::Set_Shader();
		}
	}

	if (Meshes.GetAnime())
	{
		// アニメーション
		for (auto mesh : Meshes.Get())
		{
			auto anime = Meshes.Get_Anime();

			for (auto i : mesh.second.Get())
			{
				if (Anime_State_Machine.Get_Enable())
				{
					i.second.Draw_Animation(matrix, anime, Frame, Anime_State_Machine.Get_Anime_Name(), Anime_State_Machine.Get_Next_Anime_Name(), Anime_State_Machine.Get_Ratio());
				}
				else
				{
					i.second.Draw_Animation(matrix, anime, Frame, Anime_State_Machine.Get_Anime_Name());
				}
			}
		}
	}
	else
	{
		// 普通の描画
		for (auto mesh : Meshes.Get())
		{
			for (auto i : mesh.second.Get())
			{
				i.second.Draw(matrix);
			}
		}
	}

	CRenderer::Set_Shader();
}

void CMODEL::Draw_DPP()
{
	const auto camera = CManager::Get_Scene()->Get_Game_Object<CCamera>("camera");

	if (camera.expired() && Empty_weak_ptr<CCamera>(camera))
	{
		if (false == camera.lock()->Get_Visibility(Position))
		{
			return;
		}
	}

	XMMATRIX matrix = XMMatrixIdentity();
	XMMATRIX scaling = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));
	XMMATRIX transform = XMMatrixTranslation(Position.x, Position.y, Position.z);

	matrix = XMMatrixMultiply(matrix, scaling);
	matrix = XMMatrixMultiply(matrix, rotation);
	matrix = XMMatrixMultiply(matrix, transform);

	{
		auto camera01 = CManager::Get_Scene()->Get_Game_Object<CCamera>("camera");
		auto camera02 = CManager::Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

	}

	if (Meshes.GetAnime())
	{
		// アニメーション
		for (auto mesh : Meshes.Get())
		{
			auto anime = Meshes.Get_Anime();

			for (auto i : mesh.second.Get())
			{
				if (Anime_State_Machine.Get_Enable())
				{
					i.second.Draw_DPP_Animation(matrix, anime, Frame, Anime_State_Machine.Get_Anime_Name(), Anime_State_Machine.Get_Next_Anime_Name(), Anime_State_Machine.Get_Ratio());
				}
				else
				{
					i.second.Draw_DPP_Animation(matrix, anime, Frame, Anime_State_Machine.Get_Anime_Name());
				}
			}
		}
	}
	else
	{
		// 普通の描画
		for (auto mesh : Meshes.Get())
		{
			for (auto i : mesh.second.Get())
			{
				i.second.Draw_DPP(matrix);
			}
		}
	}

	CRenderer::Set_Shader();
}

void CMODEL::Update(float delta_time)
{
	Meshes.Update();

	Anime_State_Machine.Update();

	static float f = 0.f;

	f += delta_time;

	if (0.01666f <= f)
	{
		Frame++;
		f = 0.f;
	}
}

void CMODEL::Uninit()
{
	Meshes.Uninit();

	for (auto tex : textures_loaded)
	{
		SAFE_RELEASE(tex.Texture);
	}
	textures_loaded.clear();
}

void CMODEL::Set_Enable(const bool flag)
{
	Enable = flag;
}

const bool CMODEL::Get_Enable()
{
	return Enable;
}

MESH CMODEL::processMesh(aiMesh* mesh, aiNode* node, const aiScene* scene)
{
	// Data to fill
	vector<VERTEX_3D> vertices;
	vertices.reserve(mesh->mNumVertices);

	vector<UINT> indices;
	vector<TEXTURE_S> textures;

	XMMATRIX matrix;

	string name = node->mName.C_Str();

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

		if (textype.empty()) textype = determineTextureType(scene, mat);
	}

	// Walk through each of the mesh's vertices
	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		VERTEX_3D vertex;

		// 頂点データの設定
		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;

		// 頂点カラーの設定
		if (mesh->HasVertexColors(i))
		{
			vertex.Diffuse.x = mesh->mColors[i]->r;
			vertex.Diffuse.y = mesh->mColors[i]->g;
			vertex.Diffuse.z = mesh->mColors[i]->b;
			vertex.Diffuse.w = mesh->mColors[i]->a;
		}
		else
		{
			vertex.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// 法線ベクトルの設定
		vertex.Normal.x = mesh->mNormals[i].x;
		vertex.Normal.y = mesh->mNormals[i].y;
		vertex.Normal.z = mesh->mNormals[i].z;

		// UV座標の設定
		if (mesh->mTextureCoords[0])
		{
			vertex.TexCoord.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.TexCoord.y = (float)mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
	}

	// インデックスの設定
	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// テクスチャの設定
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		vector<TEXTURE_S> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	}

	// マトリックスの設定
	{
		matrix = Covert_Matrix(&node->mTransformation);
	}

	return MESH(vertices, indices, textures, matrix, name);
}

vector<TEXTURE_S> CMODEL::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName, const aiScene* scene)
{
	vector<TEXTURE_S> textures;
	for (UINT i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (UINT j = 0; j < textures_loaded.size(); j++)
		{
			if (textures_loaded[j].path.c_str() == str.C_Str())
			{
				textures.push_back(textures_loaded[j]);
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
					FAILDE_ASSERT
			}

			texture.path = str.C_Str();
			textures.push_back(texture);
			this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}

void CMODEL::processNode(aiNode* node, const aiScene* scene, map<string, MESH>& mesh_map)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		MESH ms = processMesh(mesh, node, scene);

		mesh_map[node->mName.C_Str()] = ms;
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene, mesh_map[node->mName.C_Str()].Get() );
	}
}

string CMODEL::determineTextureType(const aiScene* scene, aiMaterial* mat)
{
	aiString textypeStr;
	mat->GetTexture(aiTextureType_DIFFUSE, 0, &textypeStr);
	string textypeteststr = textypeStr.C_Str();
	if (textypeteststr == "*0" || textypeteststr == "*1" || textypeteststr == "*2" || textypeteststr == "*3" || textypeteststr == "*4" || textypeteststr == "*5")
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

int CMODEL::getTextureIndex(aiString* str)
{
	string tistr;
	tistr = str->C_Str();
	tistr = tistr.substr(1);
	return stoi(tistr);
}

ID3D11ShaderResourceView* CMODEL::getTextureFromModel(const aiScene* scene, int textureindex)
{
	HRESULT hr;
	ID3D11ShaderResourceView* texture;

	int* size = reinterpret_cast<int*>(&scene->mTextures[textureindex]->mWidth);

	hr = CreateWICTextureFromMemory(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), reinterpret_cast<unsigned char*>(scene->mTextures[textureindex]->pcData), *size, nullptr, &texture);
	if (FAILED(hr))
		FAILDE_ASSERT

		return texture;
}


// アニメーション情報を作成
Anim createAnimation(const aiAnimation* anim)
{
	Anim animation;

	// 階層アニメーション
	aiNodeAnim** node_anim = anim->mChannels;
	for (UINT i = 0; i < anim->mNumChannels; ++i)
	{
		animation.body.push_back(createNodeAnim(node_anim[i]));
	}

	return animation;
}

// ノードに付随するアニメーション情報を作成
NodeAnim createNodeAnim(const aiNodeAnim * anim)
{
	NodeAnim animation;

	animation.node_name = anim->mNodeName.C_Str();

	// 平行移動
	for (UINT i = 0; i < anim->mNumPositionKeys; ++i)
	{
		animation.translate.push_back(fromAssimp(anim->mPositionKeys[i]));
	}
	animation.translate.begin()->time = anim->mNumPositionKeys;

	// スケーリング
	for (UINT i = 0; i < anim->mNumScalingKeys; ++i)
	{
		animation.scaling.push_back(fromAssimp(anim->mScalingKeys[i]));
	}
	animation.scaling.begin()->time = anim->mNumScalingKeys;

	// 回転
	for (UINT i = 0; i < anim->mNumRotationKeys; ++i)
	{
		animation.rotation.push_back(fromAssimp(anim->mRotationKeys[i]));
	}
	animation.rotation.begin()->time = anim->mNumRotationKeys;

	return animation;
}

VectorKey fromAssimp(const aiVectorKey& key)
{
	VectorKey v;

	v.value.x = key.mValue.x;
	v.value.y = key.mValue.y;
	v.value.z = key.mValue.z;

	return v;
}

QuatKey fromAssimp(const aiQuatKey& key)
{
	QuatKey v;

	v.value.x = key.mValue.x;
	v.value.y = key.mValue.y;
	v.value.z = key.mValue.z;
	v.value.w = key.mValue.w;

	return v;
}


XMMATRIX Covert_Matrix(const aiMatrix4x4* matrix)
{
	aiMatrix4x4 mtr = *matrix;

	aiTransposeMatrix4(&mtr);		// 転置行列 DirectX用にする

	return XMLoadFloat4x4((XMFLOAT4X4*)& mtr);
}