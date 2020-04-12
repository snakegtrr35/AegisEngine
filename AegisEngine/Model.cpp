#include	"Model.h"
#include	"Library/DirectXTex/WICTextureLoader.h"

static string textype;

MODEL::MODEL()
{

}

MODEL::~MODEL()
{

}

void MODEL::Init()
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(FileName,
		aiProcess_Triangulate | aiProcess_LimitBoneWeights | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality);

	if (pScene == NULL)
	{
		FAILDE_ASSERT;
	}

	this->directory = FileName.substr(0, FileName.find_last_of('/'));

	processNode(pScene->mRootNode, pScene, Meshes.Get());
}

void MODEL::Draw()
{

}

void MODEL::Draw_DPP()
{

}

void MODEL::Update(float delta_time)
{

}

void MODEL::Uninit()
{

}

void MODEL::Draw_Inspector()
{

}

MESH MODEL::processMesh(aiMesh* mesh, aiNode* node, const aiScene* scene)
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

vector<TEXTURE_S> MODEL::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName, const aiScene* scene)
{
	vector<TEXTURE_S> textures;
	for (UINT i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (UINT j = 0; j < textures.size(); j++)
		{
			if (textures[j].path.c_str() == str.C_Str())
			{
				textures.push_back(textures[j]);
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
			this->textures.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}

void MODEL::processNode(aiNode* node, const aiScene* scene, unordered_map<string, MESH>& mesh_map)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		MESH ms = processMesh(mesh, node, scene);

		mesh_map[node->mName.C_Str()] = ms;
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene, mesh_map[node->mName.C_Str()].Get());
	}
}

string MODEL::determineTextureType(const aiScene* scene, aiMaterial* mat)
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

int MODEL::getTextureIndex(aiString* str)
{
	string tistr;
	tistr = str->C_Str();
	tistr = tistr.substr(1);
	return stoi(tistr);
}

ID3D11ShaderResourceView* MODEL::getTextureFromModel(const aiScene* scene, int textureindex)
{
	HRESULT hr;
	ID3D11ShaderResourceView* texture;

	int* size = reinterpret_cast<int*>(&scene->mTextures[textureindex]->mWidth);

	hr = CreateWICTextureFromMemory(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), reinterpret_cast<unsigned char*>(scene->mTextures[textureindex]->pcData), *size, nullptr, &texture);
	if (FAILED(hr))
		FAILDE_ASSERT

		return texture;
}