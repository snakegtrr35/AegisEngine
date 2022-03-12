#pragma once

#ifndef MESH_H
#define MESH_H

#undef min
#undef max

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
	std::string FileName;
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
	aegis::vector<UINT> Indeces;

	MESH_SUBSET() {
		StartVertex = 0;
		StartIndex = 0;
		IndexNum = 0;
	}
};

struct VectorKey {
	unsigned int time;
	aegis::Vector3 value;
};

struct QuatKey {
	unsigned int time;
	aegis::Vector4 value;
};

struct NodeAnim {
	std::string node_name;

	aegis::vector<VectorKey> translate;
	aegis::vector<VectorKey> scaling;
	aegis::vector<QuatKey>   rotation;
};

struct Anim {
	aegis::vector<NodeAnim> body;
};


class MESHS {
protected:
	aegis::vector<VERTEX_3D> Vertices;
	aegis::vector<UINT> Indices;

	aegis::vector<TEXTURE_S> Textures;

	ComPtr<ID3D11Buffer> VertexBuffer;
	ComPtr<ID3D11Buffer> IndexBuffer;

	std::string Name; //! メッシュの名前

	std::string TextureName;	//! テクスチャ名

	XMFLOAT4X4 Matrix;

	aegis::vector<MESHS> ChildMeshes;

	void Draw_Mesh(XMMATRIX& parent_matrix, const aegis::vector<TEXTURE_S>& textures);

	void Draw_DPP_Mesh(XMMATRIX& parent_matrix);

	void SetupMesh();

public:

	MESHS();


	MESHS(aegis::vector<VERTEX_3D>& vertices, aegis::vector<UINT>& indices, std::string& texture_name, XMMATRIX& matrix, std::string name);

	~MESHS() { Uninit(); }

	void Init();

	void Draw(XMMATRIX& matrix, const aegis::vector<TEXTURE_S>& textures);

	void Draw_DPP(XMMATRIX& matrix);

	void Update();

	void Uninit();

	aegis::vector<MESHS>& Get_Meshs();

	aegis::vector<TEXTURE_S>& Get_Textures();

	const std::string& Get_Name();

	void Set_Name(const std::string& name);

	const std::string& Get_Texture_Name();

	void Set_Texture_Name(const std::string& texture_name);

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