#include	"ModelLoader.h"
#include	"Library/DirectXTex/WICTextureLoader.h"

Anim createAnimation(const aiAnimation* anim);
NodeAnim createNodeAnim(const aiNodeAnim* anim);

VectorKey fromAssimp(const aiVectorKey& key);
QuatKey fromAssimp(const aiQuatKey& key);

Bone createBone(const aiBone* b);

XMMATRIX Covert_Matrix(aiMatrix4x4* matrix);


/*
CMODEL::CMODEL()
{
	Enable = true;
}

CMODEL::CMODEL(XMFLOAT3& position, XMFLOAT3& rotation, XMFLOAT3& scaling)
{
	Position = position;
	Rotation = rotation;
	Scaling = scaling;

	Enable = true;
}

CMODEL::~CMODEL()
{
	Uninit();
}

bool CMODEL::Load(string& filename)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(filename,
		aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality);
	
	if (pScene == NULL)
		return false;

	this->directory = filename.substr(0, filename.find_last_of('/'));

	processNode(pScene->mRootNode, pScene, Covert_Matrix(&(pScene->mRootNode->mTransformation)) );
	//processNode(pScene->mRootNode, nullptr, pScene);

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
			meshes.begin()->SetAnimation(animation);
		}
	}

	return true;
}

bool CMODEL::Reload(string& filename)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(filename,
		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality);

	if (pScene == NULL)
		return false;

	this->directory = filename.substr(0, filename.find_last_of('/'));

	processNode(pScene->mRootNode, pScene, Covert_Matrix(&(pScene->mRootNode->mTransformation)) );
	//processNode(pScene->mRootNode, nullptr, pScene);

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
			meshes.begin()->SetAnimation(animation);
		}
	}

	return true;
}

void CMODEL::Draw()
{
	for (auto mesh : meshes)
	{
		mesh.Draw(Position, Rotation, Scaling);
	}
}

void CMODEL::Uninit()
{
	for (auto mesh : meshes)
	{
		mesh.Uninit();
	}
	meshes.clear();

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

void CMODEL::SetPosition(const XMFLOAT3& position)
{
	Position = position;
}

void CMODEL::SetRotation(const XMFLOAT3& rotation)
{
	Rotation = rotation;
}

void CMODEL::SetScaling(const XMFLOAT3& scaling)
{
	Scaling = scaling;
}

static string textype;

Mesh CMODEL::processMesh(aiMesh* mesh, const aiScene* scene, XMMATRIX& matrix, XMMATRIX& parent_matrix)
{
	// Data to fill
	vector<VERTEX_3D> vertices;
	vector<UINT> indices;
	vector<TEXTURE_S> textures;

	vector<Bone> bones;

	string Name, ParentName;

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
		aiBone** b = mesh->mBones;
		for (UINT i = 0; i < mesh->mNumBones; ++i)
		{
			bones.push_back(createBone(b[i]));
		}
	}

	return Mesh(vertices, indices, textures, bones, matrix, parent_matrix);
}

vector<TEXTURE_S> CMODEL::loadMaterialTextures(aiMaterial * mat, aiTextureType type, string typeName, const aiScene * scene)
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
					hr = DirectX::CreateWICTextureFromFile(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), filenamews.c_str(), nullptr, &texture.Texture);
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

void CMODEL::processNode(aiNode* node, const aiScene* scene, XMMATRIX parent_matrix)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		XMMATRIX matrix = Covert_Matrix(&node->mTransformation);

		meshes.push_back(this->processMesh(mesh, scene, matrix, parent_matrix) );
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		// �e�}�g���b�N�X�̍쐬
		XMMATRIX matrix = Covert_Matrix(&node->mTransformation);
		matrix *= parent_matrix;
		this->processNode(node->mChildren[i], scene, matrix);
	}
}

string CMODEL::determineTextureType(const aiScene * scene, aiMaterial * mat)
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

int CMODEL::getTextureIndex(aiString * str)
{
	string tistr;
	tistr = str->C_Str();
	tistr = tistr.substr(1);
	return stoi(tistr);
}

ID3D11ShaderResourceView * CMODEL::getTextureFromModel(const aiScene * scene, int textureindex)
{
	HRESULT hr;
	ID3D11ShaderResourceView *texture;

	int* size = reinterpret_cast<int*>(&scene->mTextures[textureindex]->mWidth);

	hr = CreateWICTextureFromMemory(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), reinterpret_cast<unsigned char*>(scene->mTextures[textureindex]->pcData), *size, nullptr, &texture);
	if (FAILED(hr))
		FAILDE_ASSERT

	return texture;
}*/


CMODEL::CMODEL()
{
	Enable = true;
}

CMODEL::~CMODEL()
{
	Uninit();
}

bool CMODEL::Load(string& filename)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(filename,
		aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality);

	if (pScene == NULL)
		return false;

	this->directory = filename.substr(0, filename.find_last_of('/'));

	//Meshes[pScene->mRootNode->mName.C_Str()] = MESH();
	//processNode(pScene->mRootNode, nullptr, pScene, Meshes);
	processNode(pScene->mRootNode, nullptr, pScene, Meshes.Get());

	// �A�j���[�V�������̐ݒ�
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
			Meshes.SetAnimation(animation);
		}
	}

	return true;
}

bool CMODEL::Reload(string& filename)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(filename,
		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality);

	if (pScene == NULL)
		return false;

	this->directory = filename.substr(0, filename.find_last_of('/'));

	//Meshes[pScene->mRootNode->mName.C_Str()] = MESH();
	//processNode(pScene->mRootNode, nullptr, pScene, Meshes);
	processNode(pScene->mRootNode, nullptr, pScene, Meshes.Get());

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
			Meshes.SetAnimation(animation);
		}
	}

	return true;
}

void CMODEL::Draw()
{
	XMMATRIX matrix = XMMatrixIdentity();

	//XMMATRIX scaling = XMMatrixScaling(0.1f, 0.1f, 0.1f);

	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(XMConvertToRadians(0.0f), XMConvertToRadians(90.0f), XMConvertToRadians(0.0f));

	XMMATRIX transform = XMMatrixTranslation(5.0f, 0.0f, 0.0f);

	//matrix = XMMatrixMultiply(matrix, scaling);

	matrix = XMMatrixMultiply(matrix, rotation);

	matrix = XMMatrixMultiply(matrix, transform);

	/*for (auto mesh : Meshes)
	{
		mesh.second.Get().begin()->second.Draw(matrix);
	}*/


	for (auto mesh : Meshes.Get())
	{
		mesh.second.Get().begin()->second.Draw(matrix);
	}
}

void CMODEL::Uninit()
{
	/*if(false == Meshes.empty())
	{
		Meshes.begin()->second.Get().begin()->second.Uninit();
		Meshes.clear();
	}*/

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

void CMODEL::SetPosition(const XMFLOAT3& position)
{
	Position = position;
}

void CMODEL::SetRotation(const XMFLOAT3& rotation)
{
	Rotation = rotation;
}

void CMODEL::SetScaling(const XMFLOAT3& scaling)
{
	Scaling = scaling;
}

static string textype;

MESH CMODEL::processMesh(aiMesh* mesh, aiNode* node, const aiScene* scene)
{
	// Data to fill
	vector<VERTEX_3D> vertices;
	vector<UINT> indices;
	vector<TEXTURE_S> textures;

	vector<Bone> bones;

	XMMATRIX matrix;

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
		aiBone** b = mesh->mBones;
		for (UINT i = 0; i < mesh->mNumBones; ++i)
		{
			bones.push_back(createBone(b[i]));
		}
	}

	{
		matrix = Covert_Matrix(&node->mTransformation);
	}

	return MESH(vertices, indices, textures, bones, matrix);
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
					hr = CreateWICTextureFromFile(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), filenamews.c_str(), nullptr, &texture.Texture);
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

void CMODEL::processNode(aiNode* node, aiNode* parent_node, const aiScene* scene, map<string, MESH>& mesh_map)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		MESH ms = processMesh(mesh, node, scene);

		mesh_map[node->mName.C_Str()] = ms;
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], node, scene, mesh_map[node->mName.C_Str()].Get() );
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

	animation.duration = anim->mDuration;

	{
		// �K�w�A�j���[�V����
		aiNodeAnim** node_anim = anim->mChannels;
		for (UINT i = 0; i < anim->mNumChannels; ++i) {
			animation.body.push_back(createNodeAnim(node_anim[i]));
		}
	}

	{
		// ���b�V���A�j���[�V����
#if 1
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
	for (UINT i = 0; i < anim->mNumPositionKeys; ++i) {
		animation.translate.push_back(fromAssimp(anim->mPositionKeys[i]));
	}

	// �X�P�[�����O
	for (UINT i = 0; i < anim->mNumScalingKeys; ++i) {
		animation.scaling.push_back(fromAssimp(anim->mScalingKeys[i]));
	}

	// ��]
	for (UINT i = 0; i < anim->mNumRotationKeys; ++i) {
		animation.rotation.push_back(fromAssimp(anim->mRotationKeys[i]));
	}

	return animation;
}

VectorKey fromAssimp(const aiVectorKey& key)
{
	VectorKey v;

	v.time = key.mTime;

	v.value.x = key.mValue.x;
	v.value.y = key.mValue.y;
	v.value.z = key.mValue.z;

	return v;
}

QuatKey fromAssimp(const aiQuatKey& key)
{
	QuatKey v;

	v.time = key.mTime;

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
	aiMatrix4x4 ai_matrix = b->mOffsetMatrix;

	aiTransposeMatrix4(&ai_matrix);		// �]�u�s�� DirectX�p�ɂ���

	bone.offset = XMLoadFloat4x4((XMFLOAT4X4*)& ai_matrix);

	const aiVertexWeight* w = b->mWeights;
	for (u_int i = 0; i < b->mNumWeights; ++i)
	{
		Weight weight{ w[i].mVertexId, w[i].mWeight };
		bone.weights.push_back(weight);
	}

	return bone;
}



XMMATRIX Covert_Matrix(aiMatrix4x4* matrix)
{
	aiMatrix4x4 mtr = *matrix;

	aiTransposeMatrix4(&mtr);		// �]�u�s�� DirectX�p�ɂ���

	return XMLoadFloat4x4((XMFLOAT4X4*)& mtr);
}

//XMMATRIX Covert_Matrix(aiMatrix4x4& matrix)
//{
//	aiMatrix4x4 mtr = matrix;
//
//	aiTransposeMatrix4(&mtr);		// �]�u�s�� DirectX�p�ɂ���
//
//	return XMLoadFloat4x4((XMFLOAT4X4*)& mtr);
//}