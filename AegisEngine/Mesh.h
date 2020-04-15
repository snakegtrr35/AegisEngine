#pragma once

#ifndef MESH_H
#define MESH_H

#include	<assimp/Importer.hpp>
#include	<assimp/cimport.h>
#include	<assimp/scene.h>
#include	<assimp/postprocess.h>
#include	<assimp/matrix4x4.h>

#include	"Renderer.h"

#include	"camera.h"
#include	"Debug_Camera.h"
#include	"manager.h"
#include	"ShadowMap.h"
#include	"Scene.h"

//
#define NUM_BONES 4
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

struct TEXTURE_S {
	string FileName;
	ID3D11ShaderResourceView* Texture = nullptr;
};

//
struct BONEINFO {
	XMMATRIX Offset;
	XMMATRIX Animation;
	XMMATRIX Matrix;
};

// 描画サブセット構造体
struct MESH_SUBSET
{
	UINT StartVertex;
	UINT StartIndex;
	UINT IndexNum;
	vector<UINT> Indeces;

	MESH_SUBSET() {
		StartVertex = 0;
		StartIndex = 0;
		IndexNum = 0;
	}
};

struct VectorKey {
	unsigned int time;
	XMFLOAT3 value;
};

struct QuatKey {
	unsigned int time;
	XMFLOAT4 value;
};

struct NodeAnim {
	string node_name;

	vector<VectorKey> translate;
	vector<VectorKey> scaling;
	vector<QuatKey>   rotation;
};

struct Anim {
	vector<NodeAnim> body;
};

class MESH {
private:
	//vector<VERTEX_3D> Vertices;
	vector<UINT> Indices;
	vector<TEXTURE_S> Textures;

	unordered_map<string, Anim> Animation;

	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;

	string Name;

	XMMATRIX Matrix;

	unordered_map<string, MESH> ChildMeshes;

	void Draw_Mesh(XMMATRIX& parent_matrix);

	void Draw_DPP_Mesh(XMMATRIX& parent_matrix);

	void Draw_Mesh_Animation(XMMATRIX& parent_matrix, unordered_map<string, Anim>& anime, DWORD frame, const string& name1, const string& name2 = string("none"), float blend = 0.0f);

	void Draw_DPP_Mesh_Animation(XMMATRIX& parent_matrix, unordered_map<string, Anim>& anime, DWORD frame, const string& name1, const string& name2 = string("none"), float blend = 0.0f);

	bool SetupMesh(vector<VERTEX_3D>& vertices);

public:

	MESH();

	MESH(vector<VERTEX_3D>& vertices, vector<UINT>& indices, vector<TEXTURE_S>& textures, XMMATRIX& matrix, string name);

	~MESH();

	void Draw(XMMATRIX& matrix);

	void Draw_DPP(XMMATRIX& matrix);

	void Draw_Animation(XMMATRIX& matrix, unordered_map<string, Anim>& anime, DWORD frame, const string& name1, const string& name2 = string("none"), float blend = 0.0f);

	void Draw_DPP_Animation(XMMATRIX& matrix, unordered_map<string, Anim>& anime, DWORD frame, const string& name1, const string& name2 = string("none"), float blend = 0.0f);

	void Update();

	void Uninit();

	void SetAnimation(const string& name, const Anim& animations);

	//void Add(const string name, const MESH& mesh);

	unordered_map<string, MESH>& Get();

	unordered_map<string, Anim>& Get_Anime();

	bool GetAnime();
};





////////////////////////////////////////////////////////////////////////////////






class MESHS {
protected:
	vector<VERTEX_3D> Vertices;
	vector<UINT> Indices;

	vector<TEXTURE_S> Textures;

	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;

	string Name; //! メッシュの名前

	string TextureName;	//! テクスチャ名

	XMFLOAT4X4 Matrix;

	vector<MESHS> ChildMeshes;

	void Draw_Mesh(XMMATRIX& parent_matrix, const vector<TEXTURE_S>& textures);

	void Draw_DPP_Mesh(XMMATRIX& parent_matrix);

	void SetupMesh(/*vector<VERTEX_3D>& vertices*/);

public:

	MESHS();


	MESHS(vector<VERTEX_3D>& vertices, vector<UINT>& indices, string& texture_name, XMMATRIX& matrix, string name);

	~MESHS() { Uninit(); }

	void Init();

	void Draw(XMMATRIX& matrix, const vector<TEXTURE_S>& textures);

	void Draw_DPP(XMMATRIX& matrix);

	void Update();

	void Uninit();

	vector<MESHS>& Get_Meshs();

	vector<TEXTURE_S>& Get_Textures();

	const string& Get_Name();

	void Set_Name(const string& name);

	const string& Get_Texture_Name();

	void Set_Texture_Name(const string& texture_name);

	void Set(const MESHS& meshs);

	template<class Archive>
	void serialize(Archive& ar) {
		//ar(Vertices);
		ar(Indices);
		ar(Name);
		ar(Matrix);
		ar(ChildMeshes);
	}
};


inline XMMATRIX Covert_Matrix(const aiMatrix4x4* matrix)
{
	aiMatrix4x4 mtr = *matrix;

	aiTransposeMatrix4(&mtr);		// 転置行列 DirectX用にする

	return XMLoadFloat4x4((XMFLOAT4X4*)& mtr);
}

#endif