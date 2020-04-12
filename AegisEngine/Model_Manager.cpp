#include	"Model_Manager.h"

#include	"Library/DirectXTex/WICTextureLoader.h"

unique_ptr<MODEL_MANEGER> MODEL_MANEGER::ModelManager;

static string textype;

void MODEL_MANEGER::Init()
{
	if (nullptr == ModelManager.get())
	{
		ModelManager = make_unique<MODEL_MANEGER>();
	}

	bool flag;

	{
		std::ifstream file("model.dat", std::ios::binary);

		flag = file.is_open();

		if (flag)
		{
			size_t size = filesystem::file_size("model.dat");

			if (0 < size)
			{
				cereal::BinaryInputArchive archive(file);
				archive(*ModelManager.get());
			}
		}
	}

	// 画像データの読み込み
	ModelManager->Load(flag);
}

void MODEL_MANEGER::Uninit()
{

}

void MODEL_MANEGER::Load(const bool flag)
{
	// バイナリファイルがない
	if (false == flag)
	{
		string path;			// ファイル名(パス付き) 
		string file_name;		// ファイル名(パスなし)
		size_t key;
		size_t pos;

		std::filesystem::directory_iterator e = std::filesystem::directory_iterator("./asset/model");
		for (auto file : e)
		{
			// 一つ一つのファイル名(パス付き)
			path = file.path().string();

			// 置換
			replace(path.begin(), path.end(), '\\', '/');

			pos = path.find_last_of("/");

			file_name = path.substr(pos + 1);

			key = hash<string>()(file_name);//

			//バイナリファイルがない
			{
				//テクスチャの登録
				ModelFile[key].Path = path;
			}

			// モデルの読み込み
			if (ModelFile.find(key) != ModelFile.end())
			{
				MESHS Mesh;

				Assimp::Importer importer;

				const aiScene* pScene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_LimitBoneWeights | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality);

				if (pScene == NULL)
				{
					FAILDE_ASSERT;
				}

				//this->directory = FileName.substr(0, FileName.find_last_of('/'));

				processNode(pScene->mRootNode, pScene, Mesh.Get_Meshs());

				ModelData[key].Meshes = Mesh;
				ModelData[key].Cnt = 0;
			}
		}
	}
	else
	{
		string path;			// ファイル名(パス付き) 
		string file_name;		// ファイル名(パスなし)
		string type;
		size_t key;
		size_t pos;

		for (auto f : ModelFile)
		{
			path = f.second.Path;

			pos = path.find_last_of("/");

			file_name = path.substr(pos + 1);

			key = hash<string>()(file_name);//

			pos = path.find_last_of(".");
			type = path.substr(pos + 1);

			// モデルの読み込み
			{
				MESHS Mesh;

				Assimp::Importer importer;

				const aiScene* pScene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_LimitBoneWeights | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality);

				if (pScene == NULL)
				{
					FAILDE_ASSERT;
				}

				//this->directory = FileName.substr(0, FileName.find_last_of('/'));

				processNode(pScene->mRootNode, pScene, Mesh.Get_Meshs());

				ModelData[key].Meshes = Mesh;
				ModelData[key].Cnt = 0;
			}
		}
	}
}

MODEL_MANEGER* MODEL_MANEGER::Get_Instance()
{
	return ModelManager.get();
}

MESHS* const MODEL_MANEGER::Get_Mesh(const size_t key)
{
	if (ModelData.find(key) != ModelData.end())
	{
		return &(ModelData[key].Meshes);
	}
	return nullptr;
}

void MODEL_MANEGER::Add(const string& file_name)
{
}

const bool MODEL_MANEGER::Unload(const string& const file_name)
{
	size_t first = hash<string>()(file_name);//

#ifdef _DEBUG
	if (ModelData[first].Cnt != 0)
	{
		// 参照しているこのがある
		return false;
	}
#endif // _DEBUG

	// モデルの削除

	ModelData.erase(first);

	ModelFile.erase(first);

	return true;
}

void MODEL_MANEGER::Add_ReferenceCnt(const size_t file)
{
	ModelData[file].Cnt++;
}

void MODEL_MANEGER::Sub_ReferenceCnt(const size_t file)
{
	ModelData[file].Cnt--;

#ifdef _DEBUG
	if (ModelData[file].Cnt < 0)
	{
		FAILDE_ASSERT;
		return;
	}
#endif // _DEBUG
}




MESHS MODEL_MANEGER::processMesh(aiMesh* mesh, aiNode* node, const aiScene* scene)
{
	// Data to fill
	vector<VERTEX_3D> vertices;
	vertices.resize(mesh->mNumVertices);

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
		// 頂点データの設定
		vertices[i].Position.x = mesh->mVertices[i].x;
		vertices[i].Position.y = mesh->mVertices[i].y;
		vertices[i].Position.z = mesh->mVertices[i].z;

		// 頂点カラーの設定
		if (mesh->HasVertexColors(i))
		{
			vertices[i].Diffuse.x = mesh->mColors[i]->r;
			vertices[i].Diffuse.y = mesh->mColors[i]->g;
			vertices[i].Diffuse.z = mesh->mColors[i]->b;
			vertices[i].Diffuse.w = mesh->mColors[i]->a;
		}
		else
		{
			vertices[i].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// 法線ベクトルの設定
		vertices[i].Normal.x = mesh->mNormals[i].x;
		vertices[i].Normal.y = mesh->mNormals[i].y;
		vertices[i].Normal.z = mesh->mNormals[i].z;

		// UV座標の設定
		if (mesh->mTextureCoords[0])
		{
			vertices[i].TexCoord.x = (float)mesh->mTextureCoords[0][i].x;
			vertices[i].TexCoord.y = (float)mesh->mTextureCoords[0][i].y;
		}
	}

	// インデックスの設定
	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
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

	return MESHS(vertices, indices, textures, matrix, name);
}

vector<TEXTURE_S> MODEL_MANEGER::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName, const aiScene* scene)
{
	vector<TEXTURE_S> textures;
	for (UINT i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		//for (UINT j = 0; j < textures_loaded.size(); j++)
		{
			//if (textures_loaded[j].path.c_str() == str.C_Str())
			{
				//textures.push_back(textures_loaded[j]);
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
				//filename = directory + "/" + filename;
				wstring filenamews = wstring(filename.begin(), filename.end());

				{
					hr = CreateWICTextureFromFile(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), filenamews.c_str(), nullptr, &texture.Texture, nullptr, nullptr);
				}

				if (FAILED(hr))
					FAILDE_ASSERT
			}

			texture.path = str.C_Str();
			textures.push_back(texture);
			//this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}

void MODEL_MANEGER::processNode(aiNode* node, const aiScene* scene, vector<MESHS>& meshs)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		meshs.emplace_back(processMesh(mesh, node, scene));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		for (auto& m : meshs)
		{
			if (node->mName.C_Str() == m.Get_Name())
			{
				processNode(node->mChildren[i], scene, m.Get_Meshs());
				break;
			}
		}
	}
}

string MODEL_MANEGER::determineTextureType(const aiScene* scene, aiMaterial* mat)
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

int MODEL_MANEGER::getTextureIndex(aiString* str)
{
	string tistr;
	tistr = str->C_Str();
	tistr = tistr.substr(1);
	return stoi(tistr);
}

ID3D11ShaderResourceView* MODEL_MANEGER::getTextureFromModel(const aiScene* scene, int textureindex)
{
	HRESULT hr;
	ID3D11ShaderResourceView* texture;

	UINT size = scene->mTextures[textureindex]->mWidth;

	hr = CreateWICTextureFromMemory(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), reinterpret_cast<BYTE*>(scene->mTextures[textureindex]->pcData), size, nullptr, &texture);
	if (FAILED(hr))
		FAILDE_ASSERT

	return texture;
}