#pragma once

#ifndef MODEL_MANEGER_H
#define MODEL_MANEGER_H

#include	"Mesh.h"

#undef min
#undef max

#include	<assimp/Importer.hpp>
#include	<assimp/cimport.h>
#include	<assimp/scene.h>
#include	<assimp/postprocess.h>
#include	<assimp/matrix4x4.h>

struct MODEL_FILE {
	std::string Path;		//! モデルファイルのファイルパス

	MODEL_FILE() {}

	template<class T>
	void serialize(T& archive) {
		archive(Path);
	}
};

struct MODEL_DATA {
	MESH Meshes;			//! メッシュ
	UINT Cnt;				//! 参照回数

	MODEL_DATA() : Cnt(0) {}

	template<class T>
	void serialize(T& archive) {
		//archive(Meshes);
	}
};

class MODEL_MANEGER {
private:

	friend MESH;

	static std::unique_ptr<MODEL_MANEGER> ModelManager;

	aegis::unordered_map<size_t, MODEL_FILE> ModelFile;			//! モデルのファイルデータ
	aegis::unordered_map<size_t, MODEL_DATA> ModelData;			//! モデルデータ

	void Load(const bool flag);								// モデルの読み込み

	void processNode(aiNode* node, const aiScene* scene, aegis::vector<MESH>& mesh_map, aegis::vector<TEXTURE_S>& textures_loaded);
	MESH processMesh(aiMesh* mesh, aiNode* node, const aiScene* scene, aegis::vector<TEXTURE_S>& textures_loaded);

	//vector<TEXTURE_S> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene, vector<TEXTURE_S>& textures_loaded);
	std::string loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene, aegis::vector<TEXTURE_S>& textures_loaded);
	std::string determineTextureType(const aiScene* scene, aiMaterial* mat);
	int getTextureIndex(aiString* str);
	ID3D11ShaderResourceView* getTextureFromModel(const aiScene* scene, int textureindex);

public:

	MODEL_MANEGER() {}

	~MODEL_MANEGER() { Uninit(); }

	static void Init();

	void Uninit();

	static MODEL_MANEGER* Get_Instance();

	const aegis::unordered_map<size_t, MODEL_FILE>& Get_ModelFile();

	MESH* const Get_Mesh(const size_t key);

	void Add(const std::string& file_name);
	const bool Unload(const std::string& const file_name);

	void Add_ReferenceCnt(const size_t file);
	void Sub_ReferenceCnt(const size_t file);

	template<class Archive>
	void serialize(Archive& archive) {
		archive(ModelFile);
	}
};

#endif // !MODEL_MANEGER_H
