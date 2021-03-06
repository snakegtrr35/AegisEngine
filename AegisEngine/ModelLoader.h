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
#include	"Animetion_State_Machine.h"

class GAME_OBJECT;

class CMODEL : public GAME_OBJECT {
private:
	//map<string, MESH> Meshes;		// メッシュ
	MESH Meshes;		// メッシュ
	string directory;
	vector<TEXTURE_S> textures_loaded;

	void processNode(aiNode* node, const aiScene* scene, unordered_map<string, MESH>& mesh_map);
	MESH processMesh(aiMesh* mesh, aiNode* node, const aiScene* scene);

	vector<TEXTURE_S> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName, const aiScene* scene);
	string determineTextureType(const aiScene* scene, aiMaterial* mat);
	int getTextureIndex(aiString* str);
	ID3D11ShaderResourceView* getTextureFromModel(const aiScene* scene, int textureindex);

	bool Enable;

	DWORD Frame;

	ANIMETION_STATE Anime_State_Machine;

	XMVECTOR Quaternion;

public:
	CMODEL();

	~CMODEL();

	bool Load(const string& filename);
	bool Reload(const string& filename);

	void Init() override {};
	void Draw() override;
	void Draw_DPP() override;
	void Update(float delta_time) override;
	void Uninit() override;

	void Set_Enable(const bool flag);
	const bool Get_Enable();

	ANIMETION_STATE& Get() {
		return Anime_State_Machine;
	}

	void Set_Quaternion(const XMVECTOR& vec) {
		Quaternion = vec;
	}

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<GAME_OBJECT>(this));
	}

};

CEREAL_REGISTER_TYPE(CMODEL)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GAME_OBJECT, CMODEL)

#endif // !MODEL_LOADER_H