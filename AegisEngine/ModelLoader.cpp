#include	"ModelLoader.h"
#include	"WICTextureLoader.h"

#include	<set>

Anim createAnimation(const aiAnimation* anim);
NodeAnim createNodeAnim(const aiNodeAnim* anim);

VectorKey fromAssimp(const aiVectorKey& key);
QuatKey fromAssimp(const aiQuatKey& key);

Bone createBone(const aiBone* b);

//static string g_ModelFiles[] = {
//	{"number.png"},
//	{"number02.png"},
//};
//
//
//// 読み込みモデル数
//static const int MODEL_FILE_COUNT = sizeof(g_ModelFiles) / sizeof(g_ModelFiles[0]);


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

	processNode(pScene->mRootNode, pScene);

	

	return true;
}

bool CMODEL::Reload(string& filename)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(filename,
		aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality);

	if (pScene == NULL)
		return false;

	this->directory = filename.substr(0, filename.find_last_of('/'));

	for (auto mesh : meshes)
	{
		mesh.Uninit();
	}
	meshes.clear();

	processNode(pScene->mRootNode, pScene);

	return true;
}

void CMODEL::Draw()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(Position, Rotation, Scaling);
	}

	for (auto child : children)
	{
		if(children[child.first].model->Get_Enable()) children[child.first].model->Draw();
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

	for (auto child : children)
	{
		SAFE_DELETE(children[child.first].model);
	}
	children.clear();
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

Mesh CMODEL::processMesh(aiMesh * mesh, const aiScene * scene)
{
	// Data to fill
	vector<VERTEX_3D> vertices;
	vector<UINT> indices;
	vector<TEXTURE_S> textures;

	vector<Bone> bones;

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

		if (mesh->HasVertexColors(i))
		{
			// 頂点カラーの設定
			vertex.Diffuse.x = mesh->mColors[i]->r;
			vertex.Diffuse.y = mesh->mColors[i]->g;
			vertex.Diffuse.y = mesh->mColors[i]->b;
			vertex.Diffuse.z = mesh->mColors[i]->a;
		}
		else
		{
			// 頂点カラーの設定
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

	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		vector<TEXTURE_S> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	}

	if (mesh->HasBones())
	{
		aiBone** b = mesh->mBones;
		for (UINT i = 0; i < mesh->mNumBones; ++i)
		{
			bones.push_back(createBone(b[i]));
		}
	}

	return Mesh(vertices, indices, textures);
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
				//hr = CreateWICTextureFromFile(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), filenamews.c_str(), nullptr, &texture.Texture);

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

void CMODEL::processNode(aiNode * node, const aiScene * scene)
{
	if (scene->HasAnimations())
	{
		vector<Anim> animation;

		aiAnimation** anim = scene->mAnimations;

		for (UINT i = 0; i < scene->mNumAnimations; i++)
		{
			animation.push_back(createAnimation(anim[i]));
		}
	}

	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(this->processMesh(mesh, scene));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
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
}


CMODEL::CHILD_DATE& const CMODEL::Add_Child(const string& chile_name, const string& file_name)
{
	children[chile_name].file_name = file_name;
	children[chile_name].model = new CMODEL();
	children[chile_name].model->Load(children[chile_name].file_name);

	return children[chile_name];
}

CMODEL::CHILD_DATE& const CMODEL::Get_Child(const string& chile_name)
{
	return children[chile_name];
}


void CMODEL::Set_Chile_Position(string& const child_name, const XMFLOAT3& position)
{
	children[child_name].model->Position = position;
}

void CMODEL::Set_Chile_Rotation(string& const child_name, const XMFLOAT3& rotation)
{
	children[child_name].model->Rotation = rotation;
}

void CMODEL::Set_Chile_Scaling(string& const child_name, const XMFLOAT3& scaling)
{
	children[child_name].model->Scaling = scaling;
}

void CMODEL::Add_Chile_Position(string child_name, const XMFLOAT3& position)
{
	children[child_name].model->Position.x += position.x;
	children[child_name].model->Position.y += position.y;
	children[child_name].model->Position.z += position.z;
}

void CMODEL::Add_Chile_Rotation(string child_name, const XMFLOAT3& rotation)
{
	children[child_name].model->Rotation.x += rotation.x;
	children[child_name].model->Rotation.y += rotation.y;
	children[child_name].model->Rotation.z += rotation.z;
}

void CMODEL::Add_Chile_Scaling(string child_name, const XMFLOAT3& scaling)
{
	children[child_name].model->Scaling.x += scaling.x;
	children[child_name].model->Scaling.y += scaling.y;
	children[child_name].model->Scaling.z += scaling.z;
}


XMFLOAT3& const CMODEL::Get_Chile_Position(const string& child_name)
{
	return children[child_name].model->Position;
}

XMFLOAT3& const CMODEL::Get_Chile_Rotation(const string& child_name)
{
	return children[child_name].model->Rotation;
}

XMFLOAT3& const CMODEL::Get_Chile_Scaling(const string& child_name)
{
	return children[child_name].model->Scaling;
}

void CMODEL::Set_Child_Enable(string& const child_name, const bool flag)
{
	children[child_name].model->Set_Enable(flag);
}

const bool CMODEL::Get_Child_Enable(string& const child_name)
{
	return children[child_name].model->Get_Enable();
}






//map<string, CMODEL*> MODEL_MANEGER::ModelResource;
//
//void MODEL_MANEGER::Init()
//{
//	for (auto model : ModelResource)
//	{
//		model.second = nullptr;
//	}
//
//	Load();
//}
//
//void MODEL_MANEGER::Uninit()
//{
//	for (auto model : ModelResource)
//	{
//		SAFE_DELETE(model.second);
//	}
//	ModelResource.clear();
//}
//
//void MODEL_MANEGER::Load()
//{
//	string path = "asset/model/";
//	string file_name;
//
//	for (int i = 0; i < MODEL_FILE_COUNT; i++)
//	{
//		// char から wchar_t への変換
//		file_name = g_ModelFiles[i];
//
//		path = "asset/model/";
//
//		path = path + file_name;
//
//		// モデルの読み込み
//		if (nullptr == ModelResource[file_name])
//		{
//			ModelResource[file_name] = new CMODEL();
//			ModelResource[file_name]->Load(path);
//		}
//
//		path.clear();
//		file_name.clear();
//	}
//}
//
//void MODEL_MANEGER::Add(const string& const name)
//{
//	string path = "asset/model/";
//
//	path = path + name;
//
//	// モデルの読み込み
//	if (nullptr == ModelResource[name])
//	{
//		ModelResource[name] = new CMODEL();
//		ModelResource[name]->Load(path);
//	}
//}
//
//void MODEL_MANEGER::Unload(const string& const name)
//{
//	SAFE_DELETE(ModelResource[name]);
//
//	ModelResource[name] = nullptr;
//}
//
//
//MODEL::MODEL(const string& const name)
//{
//	Name = name;
//}
//
//MODEL::~MODEL()
//{
//}
//
//void MODEL::Set_Model_Name(const string& const name)
//{
//	Name = name;
//}
//
//const string& MODEL::Get_Model_Name()
//{
//	return Name;
//}
//
//void MODEL::Draw()
//{
//
//}



// アニメーション情報を作成
Anim createAnimation(const aiAnimation* anim) {
	Anim animation;

	animation.duration = anim->mDuration;

	{
		// 階層アニメーション
		aiNodeAnim** node_anim = anim->mChannels;
		for (UINT i = 0; i < anim->mNumChannels; ++i) {
			animation.body.push_back(createNodeAnim(node_anim[i]));
		}
	}

	{
		// メッシュアニメーション
#if 1
		aiMeshAnim * *mesh_anim = anim->mMeshChannels;
		for (UINT i = 0; i < anim->mNumMeshChannels; ++i) {

		}
#endif
	}

	return animation;
}

// ノードに付随するアニメーション情報を作成
NodeAnim createNodeAnim(const aiNodeAnim * anim) {
	NodeAnim animation;

	animation.node_name = anim->mNodeName.C_Str();

	// 平行移動
	for (UINT i = 0; i < anim->mNumPositionKeys; ++i) {
		animation.translate.push_back(fromAssimp(anim->mPositionKeys[i]));
	}

	// スケーリング
	for (UINT i = 0; i < anim->mNumScalingKeys; ++i) {
		animation.scaling.push_back(fromAssimp(anim->mScalingKeys[i]));
	}

	// 回転
	for (UINT i = 0; i < anim->mNumRotationKeys; ++i) {
		animation.rotation.push_back(fromAssimp(anim->mRotationKeys[i]));
	}

	return animation;
}

VectorKey fromAssimp(const aiVectorKey& key) {
	VectorKey v;

	v.time = key.mTime;

	v.value.x = key.mValue.x;
	v.value.y = key.mValue.y;
	v.value.z = key.mValue.z;

	return v;
}

QuatKey fromAssimp(const aiQuatKey& key) {
	QuatKey v;

	v.time = key.mTime;

	v.value.x = key.mValue.x;
	v.value.y = key.mValue.y;
	v.value.z = key.mValue.z;
	v.value.w = key.mValue.w;

	return v;
}

// ボーンの情報を作成
Bone createBone(const aiBone* b) {
	Bone bone;

	bone.name = b->mName.C_Str();

	bone.offset = XMMatrixSet(	b->mOffsetMatrix.a1, b->mOffsetMatrix.a2, b->mOffsetMatrix.a3, b->mOffsetMatrix.a4,
								b->mOffsetMatrix.b1, b->mOffsetMatrix.b2, b->mOffsetMatrix.b3, b->mOffsetMatrix.b4,
								b->mOffsetMatrix.c1, b->mOffsetMatrix.c2, b->mOffsetMatrix.c3, b->mOffsetMatrix.c4,
								b->mOffsetMatrix.d1, b->mOffsetMatrix.d2, b->mOffsetMatrix.d3, b->mOffsetMatrix.d4
	);

	const aiVertexWeight* w = b->mWeights;
	for (u_int i = 0; i < b->mNumWeights; ++i)
	{
		Weight weight{ w[i].mVertexId, w[i].mWeight };
		bone.weights.push_back(weight);
	}

	return bone;
}