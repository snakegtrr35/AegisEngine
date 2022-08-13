#pragma once

#ifndef MESH_H
#define MESH_H

#undef min
#undef max

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/matrix4x4.h>

//#include "Renderer.h"

#include "camera.h"
#include "Debug_Camera.h"
#include "manager.h"
#include "ShadowMap.h"
#include "Scene.h"

namespace aegis
{
	class Buffer;
}

//
#define NUM_BONES 4
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

struct TEXTURE_S {
	aegis::string FileName;
	aegis::ShaderResourceView* Texture = nullptr;
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
	aegis::string node_name;

	aegis::vector<VectorKey> translate;
	aegis::vector<VectorKey> scaling;
	aegis::vector<QuatKey>   rotation;
};

struct Anim {
	aegis::vector<NodeAnim> body;
};


class MESH {
protected:
	aegis::vector<aegis::VERTEX_3D> Vertices;
	aegis::vector<UINT> Indices;

	aegis::vector<TEXTURE_S> Textures;

	///std::shared_ptr<aegis::Buffer> VertexBuffer;
	///std::shared_ptr<aegis::Buffer> IndexBuffer;
	aegis::Buffer* VertexBuffer;
	aegis::Buffer* IndexBuffer;

	aegis::string Name; //! メッシュの名前

	aegis::string TextureName;	//! テクスチャ名

	XMFLOAT4X4 Matrix;

	aegis::vector<MESH> ChildMeshes;

	void Draw_Mesh(XMMATRIX& parent_matrix, const aegis::vector<TEXTURE_S>& textures);

	void Draw_DPP_Mesh(XMMATRIX& parent_matrix);

	void SetupMesh();

public:

	MESH();
	MESH(const MESH&) = default;


	MESH(aegis::vector<aegis::VERTEX_3D>& vertices, aegis::vector<UINT>& indices, aegis::string& texture_name, XMMATRIX& matrix, aegis::string name);

	~MESH() { Uninit(); }

	void Init();

	void Draw(XMMATRIX& matrix, const aegis::vector<TEXTURE_S>& textures);

	void Draw_DPP(XMMATRIX& matrix);

	void Update();

	void Uninit();

	aegis::vector<MESH>& Get_Meshs();

	aegis::vector<TEXTURE_S>& Get_Textures();

	const aegis::string& Get_Name();

	void Set_Name(const aegis::string& name);

	const aegis::string& Get_Texture_Name();

	void Set_Texture_Name(const aegis::string& texture_name);

	void Set(const MESH& meshs);

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(cereal::make_nvp("Vertices", Vertices));
		archive(cereal::make_nvp("Indices", Indices));
		archive(cereal::make_nvp("Name", aegis::string(Name)));
		archive(cereal::make_nvp("Matrix", Matrix));
		archive(cereal::make_nvp("ChildMeshes", ChildMeshes));
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(cereal::make_nvp("Vertices", Vertices));
		archive(cereal::make_nvp("Indices", Indices));

		aegis::string s;
		archive(cereal::make_nvp("Name", s));
		Name.reserve(s.size());
		Name = s;

		archive(cereal::make_nvp("Matrix", Matrix));
		archive(cereal::make_nvp("ChildMeshes", ChildMeshes));
	}
};


inline XMMATRIX Covert_Matrix(const aiMatrix4x4* matrix)
{
	aiMatrix4x4 mtr = *matrix;

	aiTransposeMatrix4(&mtr);		// 転置行列 DirectX用にする

	return XMLoadFloat4x4((XMFLOAT4X4*)& mtr);
}

#endif