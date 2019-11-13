#include	"ModelLoader.h"
#include	"Library/DirectXTex/WICTextureLoader.h"

#include	"manager.h"
#include	"Scene.h"
#include	"camera.h"

Anim createAnimation(const aiAnimation* anim);
NodeAnim createNodeAnim(const aiNodeAnim* anim);

VectorKey fromAssimp(const aiVectorKey& key);
QuatKey fromAssimp(const aiQuatKey& key);

Bone createBone(const aiBone* b);
//void createBone(const aiBone* bone, vector<VERTEX_ANIME_3D>& vertices, map<string, UINT>& bone_mapping, vector<BONEINFO>& bone_info, MESH_SUBSET& subset, UINT& num_bone);

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

bool CMODEL::Load(string& filename)
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

	// �A�j���[�V�������̐ݒ�
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

		//
		pScene = importer.ReadFile("asset/model/human01_Walk.fbx",
			aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_LimitBoneWeights | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality);

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

		//
		pScene = importer.ReadFile("asset/model/human01_Jump.fbx",
			aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_LimitBoneWeights | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality);

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
	}

	Anime_State_Machine.Set_Anime_Name("Stop");

	return true;
}

bool CMODEL::Reload(string& filename)
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

	{
		vector<Anim> animation;

		if (pScene->HasAnimations())
		{
			aiAnimation** anim = pScene->mAnimations;

			for (UINT i = 0; i < pScene->mNumAnimations; i++)
			{
				animation.push_back(createAnimation(anim[i]));
			}

			// 1�Ԗڂ̃��b�V���ɃA�j���[�V��������ۑ�����
			//Meshes.begin()->second.SetAnimation(animation);
			//Meshes.SetAnimation(animation);
		}
	}

	return true;
}

void CMODEL::Draw()
{
	auto camera = CManager::Get_Scene()->Get_Game_Object<CCamera>("camera");

	if (nullptr != camera)
	{
		if (false == camera->Get_Visibility(Position))
		{
			return;
		}
	}


	XMMATRIX matrix = XMMatrixIdentity();
	XMMATRIX scaling = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
	XMMATRIX rotation  = XMMatrixIdentity();/*= XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));*/
	XMMATRIX transform = XMMatrixTranslation(Position.x, Position.y, Position.z);

	// �N�H�[�^�j�I��
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


		rotation = XMMatrixRotationQuaternion(Math::Quaternion::Euler(Rotation));
	}

	matrix = XMMatrixMultiply(matrix, scaling);

	matrix = XMMatrixMultiply(matrix, rotation);

	matrix = XMMatrixMultiply(matrix, transform);

	if (Meshes.GetAnime())
	{
		// �A�j���[�V����
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
		// ���ʂ̕`��
		for (auto mesh : Meshes.Get())
		{
			for (auto i : mesh.second.Get())
			{
				i.second.Draw(matrix);
			}
		}
	}
}

void CMODEL::Update()
{
	Meshes.Update();

	Anime_State_Machine.Update();

	Frame++;
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

	vector<Bone> bones;

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

		// ���_�f�[�^�̐ݒ�
		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;

		// ���_�J���[�̐ݒ�
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

		// �@���x�N�g���̐ݒ�
		vertex.Normal.x = mesh->mNormals[i].x;
		vertex.Normal.y = mesh->mNormals[i].y;
		vertex.Normal.z = mesh->mNormals[i].z;

		// UV���W�̐ݒ�
		if (mesh->mTextureCoords[0])
		{
			vertex.TexCoord.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.TexCoord.y = (float)mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
	}

	// �C���f�b�N�X�̐ݒ�
	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// �e�N�X�`���̐ݒ�
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		vector<TEXTURE_S> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	}

	// �{�[�����̐ݒ�
	if (mesh->HasBones())
	{
		auto bone = Meshes.Get_Bone();

		aiBone** b = mesh->mBones;
		for (UINT i = 0; i < mesh->mNumBones; ++i)
		{
			bones.push_back(createBone(b[i]));

			aiVertexWeight* w = b[i]->mWeights;

			for (u_int j = 0; j < b[i]->mNumWeights; ++j)
			{
				for (auto vertex : vertices)
				{
					//vertex.BoneIndex = w[j].mVertexId;
				}
			}
		}
	}

	// �}�g���b�N�X�̐ݒ�
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
			if (std::strcmp(textures_loaded[j].path.c_str(), str.C_Str()) == 0)
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
			texture.type = typeName;
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


// �A�j���[�V���������쐬
Anim createAnimation(const aiAnimation* anim)
{
	Anim animation;

	{
		// �K�w�A�j���[�V����
		aiNodeAnim** node_anim = anim->mChannels;
		for (UINT i = 0; i < anim->mNumChannels; ++i) {
			animation.body.push_back(createNodeAnim(node_anim[i]));
		}
	}

	{
		// ���b�V���A�j���[�V����
#if 0
		aiMeshAnim * *mesh_anim = anim->mMeshChannels;
		for (UINT i = 0; i < anim->mNumMeshChannels; ++i) {

		}
#endif
	}

	return animation;
}

// �m�[�h�ɕt������A�j���[�V���������쐬
NodeAnim createNodeAnim(const aiNodeAnim * anim)
{
	NodeAnim animation;

	animation.node_name = anim->mNodeName.C_Str();

	// ���s�ړ�
	for (UINT i = 0; i < anim->mNumPositionKeys; ++i)
	{
		animation.translate.push_back(fromAssimp(anim->mPositionKeys[i]));
	}
	animation.translate.begin()->time = anim->mNumPositionKeys;

	// �X�P�[�����O
	for (UINT i = 0; i < anim->mNumScalingKeys; ++i)
	{
		animation.scaling.push_back(fromAssimp(anim->mScalingKeys[i]));
	}
	animation.scaling.begin()->time = anim->mNumScalingKeys;

	// ��]
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

// �{�[���̏����쐬
Bone createBone(const aiBone* b)
{
	Bone bone;

	bone.name = b->mName.C_Str();

	// �}�g���b�N�X�̐ݒ�
	bone.offset = Covert_Matrix(&(b->mOffsetMatrix));

	const aiVertexWeight* w = b->mWeights;
	for (u_int i = 0; i < b->mNumWeights; ++i)
	{
		Weight weight{ w[i].mVertexId, w[i].mWeight };
		bone.weights.push_back(weight);
	}

	return bone;
}


XMMATRIX Covert_Matrix(const aiMatrix4x4* matrix)
{
	aiMatrix4x4 mtr = *matrix;

	aiTransposeMatrix4(&mtr);		// �]�u�s�� DirectX�p�ɂ���

	return XMLoadFloat4x4((XMFLOAT4X4*)& mtr);
}


//CMODEL::CMODEL()
//{
//	Enable = true;
//	Frame = 0;
//}
//
//CMODEL::~CMODEL()
//{
//	Uninit();
//}
//
//bool CMODEL::Load(string& filename)
//{
//	Assimp::Importer importer;
//
//	const aiScene* pScene = importer.ReadFile(filename,
//		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_LimitBoneWeights | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality);
//
//	if (pScene == NULL)
//		return false;
//
//	this->directory = filename.substr(0, filename.find_last_of('/'));
//
//	processNode(pScene->mRootNode, pScene, Meshes.Get());
//
//	// �A�j���[�V�������̐ݒ�
//	{
//		vector<Anim> animation;
//
//		if (pScene->HasAnimations())
//		{
//			aiAnimation** anim = pScene->mAnimations;
//
//			for (UINT i = 0; i < pScene->mNumAnimations; i++)
//			{
//				animation.push_back(createAnimation(anim[i]));
//			}
//
//			// 1�Ԗڂ̃��b�V���ɃA�j���[�V��������ۑ�����
//			Meshes.SetAnimation(animation);
//		}
//	}
//
//	return true;
//}
//
//bool CMODEL::Reload(string& filename)
//{
//	Assimp::Importer importer;
//
//	const aiScene* pScene = importer.ReadFile(filename,
//		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality);
//
//	if (pScene == NULL)
//		return false;
//
//	this->directory = filename.substr(0, filename.find_last_of('/'));
//
//	processNode(pScene->mRootNode, pScene, Meshes.Get());
//
//	{
//		vector<Anim> animation;
//
//		if (pScene->HasAnimations())
//		{
//			aiAnimation** anim = pScene->mAnimations;
//
//			for (UINT i = 0; i < pScene->mNumAnimations; i++)
//			{
//				animation.push_back(createAnimation(anim[i]));
//			}
//
//			// 1�Ԗڂ̃��b�V���ɃA�j���[�V��������ۑ�����
//			Meshes.SetAnimation(animation);
//		}
//	}
//
//	return true;
//}
//
//void CMODEL::Draw()
//{
//	XMMATRIX matrix = XMMatrixIdentity();
//	XMMATRIX scaling = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
//	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));
//	XMMATRIX transform = XMMatrixTranslation(Position.x, Position.y, Position.z);
//
//	matrix = XMMatrixMultiply(matrix, scaling);
//
//	matrix = XMMatrixMultiply(matrix, rotation);
//
//	matrix = XMMatrixMultiply(matrix, transform);
//
//	if (Meshes.GetAnime())
//	{
//		// �A�j���[�V����
//		for (auto mesh : Meshes.Get())
//		{
//			auto anime = Meshes.Get_Anime();
//
//			for (auto i : mesh.second.Get())
//			{
//				//i.second.Draw(matrix, anime, Frame);
//				///i.second.Draw_Animation(matrix, anime, Frame);
//			}
//		}
//	}
//	else
//	{
//		// ���ʂ̕`��
//		for (auto mesh : Meshes.Get())
//		{
//			for (auto i : mesh.second.Get())
//			{
//				i.second.Draw(matrix);
//			}
//		}
//	}
//}
//
//void CMODEL::Update()
//{
//	//Meshes.Update(Meshes.Get_Anime(), Frame);
//
//	Frame++;
//}
//
//void CMODEL::Uninit()
//{
//	Meshes.Uninit();
//
//	for (auto tex : textures_loaded)
//	{
//		SAFE_RELEASE(tex.Texture);
//	}
//	textures_loaded.clear();
//}
//
//void CMODEL::Set_Enable(const bool flag)
//{
//	Enable = flag;
//}
//
//const bool CMODEL::Get_Enable()
//{
//	return Enable;
//}
//
//MESH CMODEL::processMesh(aiMesh* mesh, aiNode* node, const aiScene* scene)
//{
//	// Data to fill
//	vector<VERTEX_ANIME_3D> vertices;//
//	vertices.reserve(mesh->mNumVertices);//
//
//	vector<TEXTURE_S> textures;
//
//	XMMATRIX matrix;
//
//	MESH_SUBSET subset;//
//	map<string, UINT> bone_mapping;//
//	vector<BONEINFO> bone_info;//
//	UINT num_bone = 0;//
//
//	//vector<UINT> indices;
//
//	string name = node->mName.C_Str();
//
//	UINT num_vertices = 0;//
//	UINT num_indices = 0;//
//	UINT vertices_prim = 3;//
//
//	if (mesh->mMaterialIndex >= 0)
//	{
//		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
//
//		if (textype.empty()) textype = determineTextureType(scene, mat);
//	}
//
//	// �`��T�u�Z�b�g�̐ݒ�
//	{
//		subset.IndexNum = mesh->mNumFaces * vertices_prim;
//		subset.StartVertex = num_vertices;
//		subset.StartIndex = num_indices;
//
//		num_vertices += mesh->mNumVertices;
//		num_indices += subset.IndexNum;
//
//		for (UINT i = 0; i < mesh->mNumFaces; i++)
//		{
//			aiFace face = mesh->mFaces[i];
//
//			for (UINT j = 0; j < face.mNumIndices; j++)
//				subset.Indeces.push_back(face.mIndices[j]);
//		}
//	}
//
//	//vector<MESH_SUBSET> subset2;//
//	//subset2.reserve(node->mNumMeshes);//
//	//subset2.resize(node->mNumMeshes);//
//
//	//num_vertices = 0;//
//	//num_indices = 0;//
//	//vertices_prim = 3;//
//
//	//// �`��T�u�Z�b�g�̐ݒ�
//	//for (UINT i = 0; i < node->mNumMeshes; i++) {
//	//	subset2[i].IndexNum = scene->mMeshes[i]->mNumFaces * vertices_prim;
//	//	subset2[i].StartVertex = num_vertices;
//	//	subset2[i].StartIndex = num_indices;
//
//	//	num_vertices += scene->mMeshes[i]->mNumVertices;
//	//	num_indices += subset2[i].IndexNum;
//	//}
//
//	for (UINT i = 0; i < mesh->mNumVertices; i++)
//	{
//		VERTEX_ANIME_3D vertex;
//
//		// ���_�f�[�^�̐ݒ�
//		vertex.Position.x = mesh->mVertices[i].x;
//		vertex.Position.y = mesh->mVertices[i].y;
//		vertex.Position.z = mesh->mVertices[i].z;
//
//		// ���_�J���[�̐ݒ�
//		if (mesh->HasVertexColors(i))
//		{
//			vertex.Diffuse.x = mesh->mColors[i]->r;
//			vertex.Diffuse.y = mesh->mColors[i]->g;
//			vertex.Diffuse.z = mesh->mColors[i]->b;
//			vertex.Diffuse.w = mesh->mColors[i]->a;
//		}
//		else
//		{
//			vertex.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//		}
//
//		// �@���x�N�g���̐ݒ�
//		vertex.Normal.x = mesh->mNormals[i].x;
//		vertex.Normal.y = mesh->mNormals[i].y;
//		vertex.Normal.z = mesh->mNormals[i].z;
//
//		// UV���W�̐ݒ�
//		if (mesh->mTextureCoords[0])
//		{
//			vertex.TexCoord.x = (float)mesh->mTextureCoords[0][i].x;
//			vertex.TexCoord.y = (float)mesh->mTextureCoords[0][i].y;
//		}
//
//		vertices.push_back(vertex);
//	}
//
//	/*// �C���f�b�N�X�̐ݒ�
//	for (UINT i = 0; i < mesh->mNumFaces; i++)
//	{
//		aiFace face = mesh->mFaces[i];
//
//		for (UINT j = 0; j < face.mNumIndices; j++)
//			indices.push_back(face.mIndices[j]);
//	}*/
//
//	// �e�N�X�`���̐ݒ�
//	if (mesh->mMaterialIndex >= 0)
//	{
//		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
//
//		vector<TEXTURE_S> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
//		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
//	}
//
//	// �{�[�����̐ݒ�
//	if (mesh->HasBones())
//	{
//		for (int i = 0; i < mesh->mNumBones; i++)
//		{
//			createBone(mesh->mBones[i], vertices, bone_mapping, bone_info, subset, num_bone);
//		}
//	}
//
//	// �}�g���b�N�X�̐ݒ�
//	{
//		matrix = Covert_Matrix(&node->mTransformation);
//	}
//
//	return MESH(vertices, subset, textures, matrix, bone_mapping, bone_info, num_bone, name);
//}
//
//vector<TEXTURE_S> CMODEL::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName, const aiScene* scene)
//{
//	vector<TEXTURE_S> textures;
//	for (UINT i = 0; i < mat->GetTextureCount(type); i++)
//	{
//		aiString str;
//		mat->GetTexture(type, i, &str);
//
//		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
//		bool skip = false;
//		for (UINT j = 0; j < textures_loaded.size(); j++)
//		{
//			if (std::strcmp(textures_loaded[j].path.c_str(), str.C_Str()) == 0)
//			{
//				textures.push_back(textures_loaded[j]);
//				skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
//				break;
//			}
//		}
//		if (!skip)
//		{   // If texture hasn't been loaded already, load it
//			HRESULT hr;
//			TEXTURE_S texture;
//			if (textype == "embedded compressed texture")
//			{
//				int textureindex = getTextureIndex(&str);
//				texture.Texture = getTextureFromModel(scene, textureindex);
//			}
//			else
//			{
//				string filename = string(str.C_Str());
//				filename = directory + "/" + filename;
//				wstring filenamews = wstring(filename.begin(), filename.end());
//
//				{
//					hr = CreateWICTextureFromFile(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), filenamews.c_str(), nullptr, &texture.Texture);
//				}
//
//				if (FAILED(hr))
//					FAILDE_ASSERT
//			}
//			texture.type = typeName;
//			texture.path = str.C_Str();
//			textures.push_back(texture);
//			this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
//		}
//	}
//	return textures;
//}
//
//void CMODEL::processNode(aiNode* node, const aiScene* scene, map<string, MESH>& mesh_map)
//{
//	for (UINT i = 0; i < node->mNumMeshes; i++)
//	{
//		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//
//		MESH ms = processMesh(mesh, node, scene);
//
//		mesh_map[node->mName.C_Str()] = ms;
//	}
//
//	for (UINT i = 0; i < node->mNumChildren; i++)
//	{
//		processNode(node->mChildren[i], scene, mesh_map[node->mName.C_Str()].Get());
//	}
//}
//
//string CMODEL::determineTextureType(const aiScene* scene, aiMaterial* mat)
//{
//	aiString textypeStr;
//	mat->GetTexture(aiTextureType_DIFFUSE, 0, &textypeStr);
//	string textypeteststr = textypeStr.C_Str();
//	if (textypeteststr == "*0" || textypeteststr == "*1" || textypeteststr == "*2" || textypeteststr == "*3" || textypeteststr == "*4" || textypeteststr == "*5")
//	{
//		if (scene->mTextures[0]->mHeight == 0)
//		{
//			return "embedded compressed texture";
//		}
//		else
//		{
//			return "embedded non-compressed texture";
//		}
//	}
//	if (textypeteststr.find('.') != string::npos)
//	{
//		return "textures are on disk";
//	}
//}
//
//int CMODEL::getTextureIndex(aiString* str)
//{
//	string tistr;
//	tistr = str->C_Str();
//	tistr = tistr.substr(1);
//	return stoi(tistr);
//}
//
//ID3D11ShaderResourceView* CMODEL::getTextureFromModel(const aiScene* scene, int textureindex)
//{
//	HRESULT hr;
//	ID3D11ShaderResourceView* texture;
//
//	int* size = reinterpret_cast<int*>(&scene->mTextures[textureindex]->mWidth);
//
//	hr = CreateWICTextureFromMemory(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), reinterpret_cast<unsigned char*>(scene->mTextures[textureindex]->pcData), *size, nullptr, &texture);
//	if (FAILED(hr))
//		FAILDE_ASSERT
//
//		return texture;
//}
//
//
//// �A�j���[�V���������쐬
//Anim createAnimation(const aiAnimation* anim)
//{
//	Anim animation;
//
//	{
//		// �K�w�A�j���[�V����
//		aiNodeAnim** node_anim = anim->mChannels;
//		for (UINT i = 0; i < anim->mNumChannels; ++i) {
//			animation.body.push_back(createNodeAnim(node_anim[i]));
//		}
//	}
//
//	{
//		// ���b�V���A�j���[�V����
//#if 1
//		aiMeshAnim * *mesh_anim = anim->mMeshChannels;
//		for (UINT i = 0; i < anim->mNumMeshChannels; ++i) {
//
//		}
//#endif
//	}
//
//	return animation;
//}
//
//// �m�[�h�ɕt������A�j���[�V���������쐬
//NodeAnim createNodeAnim(const aiNodeAnim* anim)
//{
//	NodeAnim animation;
//
//	animation.node_name = anim->mNodeName.C_Str();
//
//	// ���s�ړ�
//	for (UINT i = 0; i < anim->mNumPositionKeys; ++i)
//	{
//		animation.translate.push_back(fromAssimp(anim->mPositionKeys[i]));
//	}
//	animation.translate.begin()->time = anim->mNumPositionKeys;
//
//	// �X�P�[�����O
//	for (UINT i = 0; i < anim->mNumScalingKeys; ++i)
//	{
//		animation.scaling.push_back(fromAssimp(anim->mScalingKeys[i]));
//	}
//	animation.scaling.begin()->time = anim->mNumScalingKeys;
//
//	// ��]
//	for (UINT i = 0; i < anim->mNumRotationKeys; ++i)
//	{
//		animation.rotation.push_back(fromAssimp(anim->mRotationKeys[i]));
//	}
//	animation.rotation.begin()->time = anim->mNumRotationKeys;
//
//	return animation;
//}
//
//VectorKey fromAssimp(const aiVectorKey& key)
//{
//	VectorKey v;
//
//	v.value.x = key.mValue.x;
//	v.value.y = key.mValue.y;
//	v.value.z = key.mValue.z;
//
//	return v;
//}
//
//QuatKey fromAssimp(const aiQuatKey& key)
//{
//	QuatKey v;
//
//	v.value.x = key.mValue.x;
//	v.value.y = key.mValue.y;
//	v.value.z = key.mValue.z;
//	v.value.w = key.mValue.w;
//
//	return v;
//}
//
//// �{�[���̏����쐬
//void createBone(const aiBone* bone, vector<VERTEX_ANIME_3D>& vertices, map<string, UINT>& bone_mapping, vector<BONEINFO>& bone_info, MESH_SUBSET& subset, UINT& num_bone)
//{
//	UINT BoneIndex = 0;
//	string BoneName(bone->mName.data);
//
//	if (bone_mapping.find(BoneName) == bone_mapping.end()) {
//		// Allocate an index for a new bone
//		BoneIndex = num_bone;
//		num_bone++;
//		BONEINFO bi;
//		bone_info.push_back(bi);
//		bone_info[BoneIndex].Offset = Covert_Matrix(&bone->mOffsetMatrix);
//		bone_mapping[BoneName] = BoneIndex;
//	}
//	else {
//		BoneIndex = bone_mapping[BoneName];
//	}
//
//	for (UINT i = 0; i < bone->mNumWeights; i++) {
//		UINT VertexID = subset.StartVertex + bone->mWeights[i].mVertexId;
//		float Weight = bone->mWeights[i].mWeight;
//
//		for (int j = 0; j < NUM_BONES; j++)
//		{
//			if (0.f == vertices[VertexID].BoneWeight[j])
//			{
//				vertices[VertexID].BoneIndex[j] = BoneIndex;
//				vertices[VertexID].BoneWeight[j] = Weight;
//
//				break;
//			}
//		}
//	}
//}
//
//
//XMMATRIX Covert_Matrix(const aiMatrix4x4* matrix)
//{
//	aiMatrix4x4 mtr = *matrix;
//
//	aiTransposeMatrix4(&mtr);		// �]�u�s�� DirectX�p�ɂ���
//
//	return XMLoadFloat4x4((XMFLOAT4X4*)& mtr);
//}