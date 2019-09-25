#pragma once

#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H


#include	<assimp/Importer.hpp>
#include	<assimp/cimport.h>
#include	<assimp/scene.h>
#include	<assimp/postprocess.h>
#include	<assimp/matrix4x4.h>

#pragma comment (lib, "assimp-vc141-mt.lib")

#include	"Renderer.h"
#include	"Mesh.h"
#include	"Game_Object.h"

class CMODEL : public GAME_OBJECT {
private:
	//map<string, MESH> Meshes;		// メッシュ
	MESH Meshes;		// メッシュ
	string directory;
	vector<TEXTURE_S> textures_loaded;

	void processNode(aiNode* node, const aiScene* scene, vector<MESH>& mesh_map);
	MESH processMesh(aiMesh* mesh, aiNode* node, const aiScene* scene);

	vector<TEXTURE_S> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName, const aiScene* scene);
	string determineTextureType(const aiScene* scene, aiMaterial* mat);
	int getTextureIndex(aiString* str);
	ID3D11ShaderResourceView* getTextureFromModel(const aiScene* scene, int textureindex);

	bool Enable;

	DWORD Frame;

public:
	CMODEL();

	~CMODEL();

	bool Load(string& filename);
	bool Reload(string& filename);

	void Init() override {};
	void Draw() override;
	void Update() override;
	void Uninit() override;

	void Set_Enable(const bool flag);
	const bool Get_Enable();
};

#endif // !MODEL_LOADER_H