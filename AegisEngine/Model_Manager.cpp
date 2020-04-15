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
	//ModelManager->Load(flag);
}

void MODEL_MANEGER::Uninit()
{
	for (auto& mesh : ModelData)
	{
		mesh.second.Meshes.Uninit();
	}
}

void MODEL_MANEGER::Load(const bool flag)
{
	// バイナリファイルがない
	if (false == flag)
	{
		string path;			// ファイル名(パス付き) 
		string file_name;		// ファイル名(パスなし)
		string type;
		size_t key;
		size_t pos;

		std::filesystem::directory_iterator di = std::filesystem::directory_iterator("./asset/model");
		for (auto file : di)
		{
			// 一つ一つのファイル名(パス付き)
			path = file.path().string();

			// 置換
			replace(path.begin(), path.end(), '\\', '/');

			// fbxファイルか判断
			{
				pos = path.find_last_of(".");

				type = path.substr(pos + 1);

				if ("fbx" != type)
				{
					continue;
				}
			}

			pos = path.find_last_of("/");

			file_name = path.substr(pos + 1);

			key = hash<string>()(file_name);//

			//テクスチャの登録
			ModelFile[key].Path = path;

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

				processNode(pScene->mRootNode, pScene, Mesh.Get_Meshs(), Mesh.Get_Textures());

				ModelData[key].Meshes = Mesh;
				ModelData[key].Cnt = 0;

				textype.clear();
			}
		}
	}
	else
	{
		// 現在は普通のファイルから読み込んでいるが、将来的にはバイナリファイルと普通のファイルどちらからも読み込めるようにする

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

			key = hash<string>()(file_name);

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

				processNode(pScene->mRootNode, pScene, Mesh.Get_Meshs(), Mesh.Get_Textures());

				ModelData[key].Meshes = Mesh;
				ModelData[key].Cnt = 0;

				textype.clear();
			}
		}
	}
}

MODEL_MANEGER* MODEL_MANEGER::Get_Instance()
{
	return ModelManager.get();
}

const unordered_map<size_t, MODEL_FILE>& MODEL_MANEGER::Get_ModelFile()
{
	return ModelFile;
}

MESHS* const MODEL_MANEGER::Get_Mesh(const size_t key)
{
	if (ModelData.find(key) != ModelData.end())
	{
		return &(ModelData[key].Meshes);
	}
	return nullptr;
}

using is = std::pair<size_t, MESHS>;

void MODEL_MANEGER::Add(const string& file_name)
{
	size_t key = hash<string>()(file_name);

	//テクスチャの登録
	ModelFile[key].Path = "./asset/model/" + file_name;

	// モデルの読み込み
	if (ModelFile.find(key) != ModelFile.end())
	{
		MESHS Mesh;

		Assimp::Importer importer;

		const aiScene* pScene = importer.ReadFile(ModelFile[key].Path, aiProcess_Triangulate | aiProcess_LimitBoneWeights | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality);

		if (pScene == NULL)
		{
			FAILDE_ASSERT;
		}

		processNode(pScene->mRootNode, pScene, Mesh.Get_Meshs(), Mesh.Get_Textures());

		ModelData[key].Meshes.Set(Mesh);
		ModelData[key].Meshes.Init();
		ModelData[key].Cnt = 0;

		textype.clear();
	}
}

const bool MODEL_MANEGER::Unload(const string& const file_name)
{
	size_t first = hash<string>()(file_name);//

#ifdef _DEBUG
	if (0 != ModelData[first].Cnt)
	{
		// 参照しているものがある
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




MESHS MODEL_MANEGER::processMesh(aiMesh* mesh, aiNode* node, const aiScene* scene, vector<TEXTURE_S>& textures_loaded)
{
	// Data to fill
	vector<VERTEX_3D> vertices;
	vertices.resize(mesh->mNumVertices);

	vector<UINT> indices;

	XMMATRIX matrix;

	string name = node->mName.C_Str();

	string texture_name;

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
			indices.emplace_back(face.mIndices[j]);
		}
	}

	// テクスチャの設定
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		texture_name = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene, textures_loaded);
	}

	// マトリックスの設定
	{
		matrix = Covert_Matrix(&node->mTransformation);
	}

	return MESHS(vertices, indices, texture_name, matrix, name);
}

//vector<TEXTURE_S> MODEL_MANEGER::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName, const aiScene* scene, vector<TEXTURE_S>& textures_loaded)
string MODEL_MANEGER::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName, const aiScene* scene, vector<TEXTURE_S>& textures_loaded)
{
	string file_name;
	const string directory = "./asset/model";

	for (UINT i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (UINT j = 0; j < textures_loaded.size(); j++)
		{
			if ( textures_loaded[j].FileName == string(str.C_Str()) )
			{
				file_name = textures_loaded[j].FileName;
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
				//texture.Texture = getTextureFromModel(scene, textureindex);
			}
			else
			{
				file_name = string(str.C_Str());
				string path = directory + "/" + file_name;
				wstring filenamews = wstring(path.begin(), path.end());

				{
					//hr = CreateWICTextureFromFile(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), filenamews.c_str(), nullptr, &texture.Texture, nullptr, nullptr);
				}

				//if (FAILED(hr))
				//	FAILDE_ASSERT
			}

			texture.FileName = str.C_Str();
			textures_loaded.emplace_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return file_name;
}

void MODEL_MANEGER::processNode(aiNode* node, const aiScene* scene, vector<MESHS>& meshs, vector<TEXTURE_S>& textures_loaded)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		//MESHS m = std::move(processMesh(mesh, node, scene, textures_loaded));

		meshs.emplace_back(processMesh(mesh, node, scene, textures_loaded));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		if (!meshs.empty())
		{
			for (auto& m : meshs)
			{
				if (node->mName.C_Str() == m.Get_Name())
				{
					processNode(node->mChildren[i], scene, m.Get_Meshs(), textures_loaded);
				}
			}
		}
		else
		{
			meshs.emplace_back(MESHS());

			processNode(node->mChildren[i], scene, meshs.begin()->Get_Meshs(), textures_loaded);
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

	FAILDE_ASSERT
	return "textures are nothing";
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