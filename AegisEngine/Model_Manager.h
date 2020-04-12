#pragma once

#ifndef MODEL_MANEGER_H
#define MODEL_MANEGER_H

#include	"Mesh.h"

#include	<assimp/Importer.hpp>
#include	<assimp/cimport.h>
#include	<assimp/scene.h>
#include	<assimp/postprocess.h>
#include	<assimp/matrix4x4.h>

struct MODEL_FILE {
	string Path;		//! モデルファイルのファイルパス

	MODEL_FILE() {}

	template<class T>
	void serialize(T& archive) {
		archive(Path);
	}
};

struct MODEL_DATA {
	MESHS Meshes;			//! メッシュ
	UINT Cnt;		//! 参照回数

	MODEL_DATA() : Cnt(0) {}

	template<class T>
	void serialize(T& archive) {
		//archive(Meshes);
	}
};

class MODEL_MANEGER {
private:

	static unique_ptr<MODEL_MANEGER> ModelManager;

	unordered_map<size_t, MODEL_FILE> ModelFile;			//! モデルのファイルデータ
	unordered_map<size_t, MODEL_DATA> ModelData;			//! モデルデータ

	void Load(const bool flag);								// モデルの読み込み

	void processNode(aiNode* node, const aiScene* scene, vector<MESHS>& mesh_map);
	MESHS processMesh(aiMesh* mesh, aiNode* node, const aiScene* scene);

	vector<TEXTURE_S> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName, const aiScene* scene);
	string determineTextureType(const aiScene* scene, aiMaterial* mat);
	int getTextureIndex(aiString* str);
	ID3D11ShaderResourceView* getTextureFromModel(const aiScene* scene, int textureindex);

public:

	MODEL_MANEGER() {}

	~MODEL_MANEGER() { Uninit(); }

	static void Init();

	void Uninit();

	static MODEL_MANEGER* Get_Instance();

	MESHS* const Get_Mesh(const size_t key);

	void Add(const string& file_name);
	const bool Unload(const string& const file_name);

	void Add_ReferenceCnt(const size_t file);
	void Sub_ReferenceCnt(const size_t file);

	template<class T>
	void serialize(T& archive) {
		archive(ModelFile);
	}
};

#endif // !MODEL_MANEGER_H
