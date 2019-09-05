#pragma once

#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H


#include	<assimp/Importer.hpp>
#include	<assimp/cimport.h>
#include	<assimp/scene.h>
#include	<assimp/postprocess.h>
#include	<assimp/matrix4x4.h>

#pragma comment (lib, "assimp-vc141-mt.lib")

#include	"main.h"
#include	"Renderer.h"
#include	"Mesh.h"
#include	"Game_Object.h"

class CMODEL : public GAME_OBJECT {
private:
	vector<Mesh> meshes;					// 親
	string directory;
	vector<TEXTURE_S> textures_loaded;

	void processNode(aiNode* parent_node, aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene, aiNode* parent_node, aiNode* node);
	vector<TEXTURE_S> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName, const aiScene* scene);
	string determineTextureType(const aiScene* scene, aiMaterial* mat);
	int getTextureIndex(aiString* str);
	ID3D11ShaderResourceView* getTextureFromModel(const aiScene* scene, int textureindex);

	bool Enable;

	typedef struct {
		CMODEL* model;
		string file_name;
	}CHILD_DATE;

private:
	map<string, CHILD_DATE> children;	// 子


public:
	CMODEL();

	CMODEL(XMFLOAT3& position, XMFLOAT3& rotation, XMFLOAT3& scaling);

	~CMODEL();

	bool Load(string& filename);
	bool Reload(string& filename);

	void Init() override {};
	void Draw() override;
	void Update() override {};
	void Uninit() override;

	void SetPosition(const XMFLOAT3& position);
	void SetRotation(const XMFLOAT3& rotation);
	void SetScaling(const XMFLOAT3& scaling);

	void Set_Enable(const bool flag);
	const bool Get_Enable();


	CHILD_DATE& const Add_Child(const string& child_name, const string& file_name);
	CHILD_DATE& const Get_Child(const string& child_name);

	void Set_Chile_Position(string& const child_name, const XMFLOAT3& position);
	void Set_Chile_Rotation(string& const child_name, const XMFLOAT3& rotation);
	void Set_Chile_Scaling(string& const child_name, const XMFLOAT3& scaling);

	void Add_Chile_Position(string child_name, const XMFLOAT3& position);
	void Add_Chile_Rotation(string child_name, const XMFLOAT3& rotation);
	void Add_Chile_Scaling(string child_name, const XMFLOAT3& scaling);

	XMFLOAT3& const Get_Chile_Position(const string& child_name);
	XMFLOAT3& const Get_Chile_Rotation(const string& child_name);
	XMFLOAT3& const Get_Chile_Scaling(const string& child_name);

	void Set_Child_Enable(string& const child_name, const bool flag);
	const bool Get_Child_Enable(string& const child_name);
};


//class MODEL_MANEGER {
//private:
//	static map<string, CMODEL*> ModelResource;
//
//	static void Load();
//
//protected:
//
//public:
//
//	static void Init();
//	static void Uninit();
//
//	static void Add(const string& const name);
//	static void Unload(const string& const name);				// モデルの解放
//};
//
//class MODEL : public GAME_OBJECT {
//private:
//	string Name;
//
//	MODEL(const string& const name);
//	~MODEL() {};
//protected:
//
//public:
//	void Set_Model_Name(const string& const name);			// モデル名の設定
//	const string& Get_Model_Name();								// モデル名の取得
//
//	void Draw();
//};

#endif // !MODEL_LOADER_H