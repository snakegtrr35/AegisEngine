//=======================================
//
//		FBXmodel.h
//		name : Hiroto
//		data : 2019/12/03 15:23:47
//
//=======================================

#ifndef _FBXmodel_H__
#define _FBXmodel_H__

#include	<assimp/Importer.hpp>
#include	<assimp/cimport.h>
#include	<assimp/scene.h>
#include	<assimp/postprocess.h>
#include	<assimp/matrix4x4.h>

/*** ç\ë¢ëÃ ***/

struct ANIME_VERTEX
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT4 Diffuse;
	XMFLOAT2 TexCoord;
	UINT bone_Index[4];
	float weight[4];
};

struct DEFORM_VERTEX
{
	aiVector3D Position;
	aiVector3D DefromPosition;
	aiVector3D Normal;
	aiVector3D DefromNormal;

	std::string BoneName[4];
	float BoneWeight[4];

	int BoneNum = 0;

};

struct BONE
{
	XMMATRIX Matrix = XMMatrixIdentity();
	XMMATRIX AnimationMatrix = XMMatrixIdentity();
	XMMATRIX OffsetMatrix = XMMatrixIdentity();
};

/*** FBXmodelÉNÉâÉX ***/
class FBXmodel 
{
private:

	struct MESH
	{
		ID3D11Buffer* VertexBuffer;
		ID3D11Buffer* IndexBuffer;
		int VertexNum;
		int IndexNum;

		vector<DEFORM_VERTEX*> deform_Vertex;
	};

	struct MATRIX_BUFFER {
		XMFLOAT4X4 Bone_Matrix[256];
	};

	vector<MESH> m_Meshes;
	UINT m_MeshNum;
	UINT frame = 0;

	int m_BoneNum = 0;

	const aiScene* m_Scene;


	unordered_map<string, aiQuaternion> m_NodeRotation;
	unordered_map<string, aiVector3D> m_NodePosition;
	unordered_map<string, UINT> m_BoneIndex;

	unordered_map<string, UINT> m_TextureIdMap;

	unordered_map<UINT, BONE> m_Bone;
	vector<DEFORM_VERTEX*> m_DeformVertex;

	unique_ptr<ID3D11Buffer, Release> MatrixBuffer;

	XMMATRIX aiMatrixToMatrix(aiMatrix4x4 matrix);
	aiMatrix4x4 MatrixToaiMatrix(XMMATRIX matrix);

public:
	void Draw(XMMATRIX &Matrix);
	void Load(const string& FileName);
	void UnLoad();
	//void Update();
	void DrawMesh(aiNode * Node, XMMATRIX &Matrix);
	void CreateBone(aiNode * Node);
	void UpdateBoneMatrix(aiNode* Node, XMMATRIX Matrix);

	void SetBoneMatrix(const XMFLOAT4X4* matrix);

	FBXmodel();
	
};

#endif // !_FBXmodel_H__
