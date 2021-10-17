//=======================================
//
//		FBXmodel.h
//		name : Hiroto
//		data : 2019/12/03 15:23:47
//
//=======================================

#ifndef FBXmodel_H
#define FBXmodel_H

#undef min
#undef max

#include	<assimp/Importer.hpp>
#include	<assimp/cimport.h>
#include	<assimp/scene.h>
#include	<assimp/postprocess.h>
#include	<assimp/matrix4x4.h>

#include	"Mesh.h"

const constexpr BYTE BONE_NUM = 4;

struct ANIME_VERTEX
{
	Aegis::Vector3 Position;
	Aegis::Vector3 Normal;
	Aegis::Vector4 Diffuse;
	Aegis::Vector2 TexCoord;
	UINT bone_Index[BONE_NUM];
	float weight[BONE_NUM];

	ANIME_VERTEX() {
		for (char i = 0; i < BONE_NUM; i++)
		{
			bone_Index[i] = 0;
			weight[i] = 0.f;
		}
	}
};

struct BONE
{
	XMMATRIX Matrix;
	XMMATRIX AnimationMatrix;
	XMMATRIX OffsetMatrix;

	BONE() : Matrix(XMMatrixIdentity()), AnimationMatrix(XMMatrixIdentity()), OffsetMatrix(XMMatrixIdentity()) {}
};

const constexpr UINT BONE_MAX = 256;


/*** FBXmodelクラス ***/
class FBXmodel 
{
private:

	struct MESH
	{
		ID3D11Buffer* VertexBuffer;
		ID3D11Buffer* IndexBuffer;
		int VertexNum;
		int IndexNum;
	};

	vector<MESH> m_Meshes;
	UINT m_MeshNum;
	UINT frame = 0;

	UINT m_BoneNum = 0;

	const aiScene* m_Scene;

	vector<TEXTURE_S> Textures;


	//unordered_map<string, aiQuaternion> m_NodeRotation;
	//unordered_map<string, aiVector3D> m_NodePosition;

	unordered_map<string, UINT> m_BoneIndex;

	unordered_map<string, UINT> m_TextureIdMap;

	unordered_map<UINT, BONE> m_Bone;

	unique_ptr<ID3D11Buffer, Release> MatrixBuffer;

	void DrawMesh(const aiNode* Node, const XMMATRIX& Matrix);
	void DrawMesh_DPP(const aiNode* Node, const XMMATRIX& Matrix);
	void CreateBone(const aiNode* Node);
	void UpdateBoneMatrix(const aiNode* Node, const XMMATRIX& Matrix);
	void SetBoneMatrix(const vector<XMMATRIX>& matrix);

	vector<TEXTURE_S> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName, const aiScene* scene);
	string determineTextureType(const aiScene* scene, aiMaterial* mat);
	int getTextureIndex(aiString* str);
	ID3D11ShaderResourceView* getTextureFromModel(const aiScene* scene, int textureindex);

	void set_bone(const aiNode* Node, vector<XMMATRIX>& v);

public:
	FBXmodel() {}
	~FBXmodel() {}

	void Draw(XMMATRIX &Matrix);
	void Draw_DPP(XMMATRIX& Matrix);
	bool Load(const string& FileName);
	void UnLoad();
	void Update(float delta_time);
	
};

#endif // !FBXmodel_H
