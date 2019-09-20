#pragma once

#ifndef MESH_H
#define MESH_H

#include	"main.h"
#include	"Renderer.h"

//
#define NUM_BONES 4
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

struct TEXTURE_S {
	string type;
	string path;
	ID3D11ShaderResourceView* Texture = nullptr;
};

struct Weight {
	UINT vertex_id;
	float value;
};

struct Bone {
	string name;
	XMMATRIX matrix;
	XMMATRIX animation;
	XMMATRIX offset;

	vector<Weight> weights;
};

//
struct BONEINFO {
	XMMATRIX BoneOffset;
	XMMATRIX FinalTransformation;
};

//
struct VERTEX_BONE_DATA
{
	UINT ID[NUM_BONES];
	float Weights[NUM_BONES];

	void AddBoneData(UINT BoneID, float Weight)
	{
		for (UINT i = 0; i < ARRAY_SIZE_IN_ELEMENTS(ID); i++) {
			if (Weights[i] == 0.0) {
				ID[i] = BoneID;
				Weights[i] = Weight;
				return;
			}
		}

		assert(0);
	}
};

// 描画サブセット構造体
struct SUBSET
{
	UINT StartVertex;
	UINT StartIndex;
	UINT IndexNum;
	DX11_MODEL_MATERIAL	Material;

	SUBSET() {
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
	std::string node_name;

	std::vector<VectorKey> translate;
	std::vector<VectorKey> scaling;
	std::vector<QuatKey>   rotation;
};

struct Anim {
	std::vector<NodeAnim> body;
};

class MESH {
private:
	vector<VERTEX_3D> Vertices;
	vector<UINT> Indices;
	vector<TEXTURE_S> Textures;

	SUBSET Subset;//

	vector<Anim> Animation;

	vector<Bone> Bones;

	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;

	string Name;

	XMMATRIX Matrix;

	map<string, MESH> ChildMeshes;

	void Draw_Mesh(XMMATRIX& parent_matrix) {
		XMMATRIX matrix;

		CRenderer::SetVertexBuffers(VertexBuffer);

		CRenderer::GetDeviceContext()->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &Textures[0].Texture);

		// 3Dマトリックス設定
		{	
			matrix = XMMatrixMultiply(Matrix, parent_matrix);

			CRenderer::SetWorldMatrix(&matrix);
		}

		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		CRenderer::GetDeviceContext()->DrawIndexed(Indices.size(), 0, 0);

		for (auto child : ChildMeshes)
		{
			child.second.Draw_Mesh(matrix);
		}
	}

	void Draw_Mesh_Animation(XMMATRIX& parent_matrix, vector<Anim> anime, DWORD frame) {
		XMMATRIX world;

		CRenderer::SetVertexBuffers(VertexBuffer);

		CRenderer::GetDeviceContext()->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &Textures[0].Texture);

		// 3Dマトリックス設定
		{
			for (auto i : anime.begin()->body)
			{
				if (i.node_name == Name)
				{
					unsigned int f = frame % i.translate.begin()->time;

					XMFLOAT3 pos = i.translate[f].value;
					XMMATRIX trans = XMMatrixTranslation(pos.x, pos.y, pos.z);

					f = frame % i.rotation.begin()->time;

					XMFLOAT4 rotation = i.rotation[f].value;
					XMVECTOR quat = XMLoadFloat4(&rotation);

					world = XMMatrixRotationQuaternion(quat);

					world = XMMatrixMultiply(world, trans);

					world = XMMatrixMultiply(world, parent_matrix);

					CRenderer::SetWorldMatrix(&world);

					break;
				}
			}
		}

		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		CRenderer::GetDeviceContext()->DrawIndexed(Indices.size(), 0, 0);

		for (auto child : ChildMeshes)
		{
			child.second.Draw_Mesh_Animation(world, anime, frame);
		}
	}

	//
	void Update_MAtrix(XMMATRIX& matrix, vector<Anim> anime, map<string, vector<Bone>>& bones, DWORD frame) {

		XMMATRIX world;

		for (auto i : anime.begin()->body)
		{
			if (i.node_name == Name)
			{
				unsigned int f = frame % i.translate.begin()->time;

				XMFLOAT3 pos = i.translate[f].value;
				XMMATRIX trans = XMMatrixTranslation(pos.x, pos.y, pos.z);

				f = frame % i.rotation.begin()->time;

				XMFLOAT4 rotation = i.rotation[f].value;
				XMVECTOR quat = XMLoadFloat4(&rotation);

				XMMATRIX qiat_mat = XMMatrixRotationQuaternion(quat);

				
				for (auto bone : bones[Name])
				{
					XMMATRIX mat = XMMatrixIdentity();

					mat = XMMatrixMultiply(mat, qiat_mat);

					mat = XMMatrixMultiply(mat, trans);

					bone.animation = mat;
				}

				break;
			}
		}

		for (auto child : ChildMeshes)
		{
			child.second.Update_MAtrix(world, anime, bones, frame);
		}
	}

	//
	void Update_Bone_Matrix(map<string, MESH> Childlen, map<string, vector<Bone>>& bones, XMMATRIX matrix = XMMatrixIdentity()) {
		XMMATRIX world = matrix;

		for (auto mesh : Childlen)
		{
			auto i = bones.find(Name);

			if (bones.end() != i)
			{
				for (auto j : i->second)
				{
					world *= j.animation;

					j.matrix = world;

					j.matrix *= j.offset;
				}
			}
		}

		for (auto child : ChildMeshes)
		{
			Update_Bone_Matrix(child.second.Get(), bones, world);
		}
	}

	bool SetupMesh() {
		HRESULT hr;

		{
			D3D11_BUFFER_DESC vbd;
			vbd.Usage = D3D11_USAGE_DEFAULT;
			vbd.ByteWidth = sizeof(VERTEX_3D) * Vertices.size();
			vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vbd.CPUAccessFlags = 0;
			vbd.MiscFlags = 0;

			// サブリソースの設定
			D3D11_SUBRESOURCE_DATA initData;
			ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));

			initData.pSysMem = &Vertices[0];
			initData.SysMemPitch = 0;
			initData.SysMemSlicePitch = 0;

			hr = CRenderer::GetDevice()->CreateBuffer(&vbd, &initData, &VertexBuffer);
			if (FAILED(hr))
				return false;
		}

		{
			D3D11_BUFFER_DESC ibd;
			ibd.Usage = D3D11_USAGE_DEFAULT;
			ibd.ByteWidth = sizeof(UINT) * Indices.size();
			ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			ibd.CPUAccessFlags = 0;
			ibd.MiscFlags = 0;

			// サブリソースの設定
			D3D11_SUBRESOURCE_DATA initData;
			ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));

			initData.pSysMem = &Indices[0];
			initData.SysMemPitch = 0;
			initData.SysMemSlicePitch = 0;

			hr = CRenderer::GetDevice()->CreateBuffer(&ibd, &initData, &IndexBuffer);
			if (FAILED(hr))
				return false;
		}
	}

public:

	MESH() {
		VertexBuffer = nullptr;
		IndexBuffer = nullptr;
	};

	MESH(vector<VERTEX_3D>& vertices, vector<UINT>& indices, vector<TEXTURE_S>& textures, XMMATRIX& matrix, string name) {
		VertexBuffer = nullptr;
		IndexBuffer = nullptr;

		Vertices = vertices;
		Indices = indices;
		Textures = textures;
		Matrix = matrix;

		Name = name;

		SetupMesh();
	}

	~MESH() {};

	void Draw(XMMATRIX& matrix) {
		Draw_Mesh(matrix);
	}

	void Draw_Animation(XMMATRIX& matrix, vector<Anim> anime, DWORD frame) {
		Draw_Mesh_Animation(matrix, anime, frame);
	}

	void Update() {
		
	}

	void Update_SkynMesh(map<string, MESH>& chiidren, XMMATRIX& matrix, vector<Anim> anime, map<string, vector<Bone>>& bones, DWORD frame) {
		Update_MAtrix(matrix, anime, bones, frame);

		Update_Bone_Matrix(chiidren, bones);
	}

	void Uninit() {
		SAFE_RELEASE(VertexBuffer);
		SAFE_RELEASE(IndexBuffer);

		Vertices.clear();

		Indices.clear();

		Bones.clear();

		for (auto tex : Textures)
		{
			SAFE_RELEASE(tex.Texture);
		}
		Textures.clear();

		for (auto child : ChildMeshes)
		{
			child.second.Uninit();
		}
		ChildMeshes.clear();
	}

	void SetAnimation(const vector<Anim>& animations) {
		Animation = animations;
	}

	void Add(const string name, const MESH& mesh) {
		ChildMeshes[name] = mesh;
	}

	map<string, MESH>& Get() {
		return ChildMeshes;
	}

	vector<Anim>& Get_Anime() {
		return Animation;
	}

	bool GetAnime() {
		if (0 == Animation.size())
		{
			return false;
		}

		return true;
	}

	vector<Bone>& Get_Bone() {
		return Bones;
	}
};

//class MESH {
//private:
//	vector<VERTEX_ANIME_3D> Vertices;
//	vector<UINT> Indices;
//	vector<TEXTURE_S> Textures;
//
//	vector<Anim> Animation;
//
//	ID3D11Buffer* VertexBuffer;
//	ID3D11Buffer* IndexBuffer;
//
//	string Name;
//
//	XMMATRIX Matrix;
//
//	map<string, MESH> ChildMeshes;
//
//	UINT NumBones;
//	vector<BONEINFO> m_BoneInfo;
//	map<string, UINT> m_BoneMapping; // maps a bone name to its index
//	vector<SUBSET> Subset;
//
//
//	void Draw_Mesh(XMMATRIX& parent_matrix) {
//		XMMATRIX matrix;
//
//		CRenderer::SetVertexBuffers(VertexBuffer);
//
//		CRenderer::GetDeviceContext()->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
//
//		CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &Textures[0].Texture);
//
//		// 3Dマトリックス設定
//		{
//			matrix = XMMatrixMultiply(Matrix, parent_matrix);
//
//			CRenderer::SetWorldMatrix(&matrix);
//		}
//
//		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//		CRenderer::GetDeviceContext()->DrawIndexed(Indices.size(), 0, 0);
//
//		for (auto child : ChildMeshes)
//		{
//			child.second.Draw_Mesh(matrix);
//		}
//	}
//
//	void Draw_Mesh_Animation(XMMATRIX& parent_matrix, vector<Anim> anime, DWORD frame) {
//		XMMATRIX world;
//
//		CRenderer::SetVertexBuffers(VertexBuffer);
//
//		CRenderer::GetDeviceContext()->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
//
//		CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &Textures[0].Texture);
//
//		// 3Dマトリックス設定
//		{
//			for (auto i : anime.begin()->body)
//			{
//				if (i.node_name == Name)
//				{
//					unsigned int f = frame % i.translate.begin()->time;
//
//					XMFLOAT3 pos = i.translate[f].value;
//					XMMATRIX trans = XMMatrixTranslation(pos.x, pos.y, pos.z);
//
//					f = frame % i.rotation.begin()->time;
//
//					XMFLOAT4 rotation = i.rotation[f].value;
//					XMVECTOR quat = XMLoadFloat4(&rotation);
//
//					world = XMMatrixRotationQuaternion(quat);
//
//					world = XMMatrixMultiply(world, trans);
//
//					world = XMMatrixMultiply(world, parent_matrix);
//
//					CRenderer::SetWorldMatrix(&world);
//
//					break;
//				}
//			}
//		}
//
//		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//		CRenderer::GetDeviceContext()->DrawIndexed(Indices.size(), 0, 0);
//
//		for (auto child : ChildMeshes)
//		{
//			child.second.Draw_Mesh_Animation(world, anime, frame);
//		}
//	}
//
//	//
//	void Update_MAtrix(XMMATRIX& matrix, vector<Anim> anime, map<string, vector<Bone>>& bones, DWORD frame) {
//
//		XMMATRIX world;
//
//		for (auto i : anime.begin()->body)
//		{
//			if (i.node_name == Name)
//			{
//				unsigned int f = frame % i.translate.begin()->time;
//
//				XMFLOAT3 pos = i.translate[f].value;
//				XMMATRIX trans = XMMatrixTranslation(pos.x, pos.y, pos.z);
//
//				f = frame % i.rotation.begin()->time;
//
//				XMFLOAT4 rotation = i.rotation[f].value;
//				XMVECTOR quat = XMLoadFloat4(&rotation);
//
//				XMMATRIX qiat_mat = XMMatrixRotationQuaternion(quat);
//
//
//				for (auto bone : bones[Name])
//				{
//					XMMATRIX mat = XMMatrixIdentity();
//
//					mat = XMMatrixMultiply(mat, qiat_mat);
//
//					mat = XMMatrixMultiply(mat, trans);
//
//					bone.animation = mat;
//				}
//
//				break;
//			}
//		}
//
//		for (auto child : ChildMeshes)
//		{
//			child.second.Update_MAtrix(world, anime, bones, frame);
//		}
//	}
//
//	//
//	void Update_Bone_Matrix(map<string, MESH> Childlen, map<string, vector<Bone>>& bones, XMMATRIX matrix = XMMatrixIdentity()) {
//		XMMATRIX world = matrix;
//
//		for (auto mesh : Childlen)
//		{
//			auto i = bones.find(Name);
//
//			if (bones.end() != i)
//			{
//				for (auto j : i->second)
//				{
//					world *= j.animation;
//
//					j.matrix = world;
//
//					j.matrix *= j.offset;
//				}
//			}
//		}
//
//		for (auto child : ChildMeshes)
//		{
//			Update_Bone_Matrix(child.second.Get(), bones, world);
//		}
//	}
//
//	bool SetupMesh() {
//		HRESULT hr;
//
//		{
//			D3D11_BUFFER_DESC vbd;
//			vbd.Usage = D3D11_USAGE_DEFAULT;
//			vbd.ByteWidth = sizeof(VERTEX_ANIME_3D) * Vertices.size();
//			vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//			vbd.CPUAccessFlags = 0;
//			vbd.MiscFlags = 0;
//
//			// サブリソースの設定
//			D3D11_SUBRESOURCE_DATA initData;
//			ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
//
//			initData.pSysMem = &Vertices[0];
//			initData.SysMemPitch = 0;
//			initData.SysMemSlicePitch = 0;
//
//			hr = CRenderer::GetDevice()->CreateBuffer(&vbd, &initData, &VertexBuffer);
//			if (FAILED(hr))
//				return false;
//		}
//
//		{
//			D3D11_BUFFER_DESC ibd;
//			ibd.Usage = D3D11_USAGE_DEFAULT;
//			ibd.ByteWidth = sizeof(UINT) * Indices.size();
//			ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
//			ibd.CPUAccessFlags = 0;
//			ibd.MiscFlags = 0;
//
//			// サブリソースの設定
//			D3D11_SUBRESOURCE_DATA initData;
//			ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
//
//			initData.pSysMem = &Indices[0];
//			initData.SysMemPitch = 0;
//			initData.SysMemSlicePitch = 0;
//
//			hr = CRenderer::GetDevice()->CreateBuffer(&ibd, &initData, &IndexBuffer);
//			if (FAILED(hr))
//				return false;
//		}
//	}
//
//public:
//
//	MESH() {
//		VertexBuffer = nullptr;
//		IndexBuffer = nullptr;
//	};
//
//	MESH(vector<VERTEX_ANIME_3D>& vertices, vector<UINT>& indices, vector<TEXTURE_S>& textures, XMMATRIX& matrix, string name) {
//		VertexBuffer = nullptr;
//		IndexBuffer = nullptr;
//
//		Vertices = vertices;
//		Indices = indices;
//		Textures = textures;
//		Matrix = matrix;
//
//		Name = name;
//
//		SetupMesh();
//	}
//
//	~MESH() {};
//
//	void Draw(XMMATRIX& matrix) {
//		Draw_Mesh(matrix);
//	}
//
//	void Draw_Animation(XMMATRIX& matrix, vector<Anim> anime, DWORD frame) {
//		Draw_Mesh_Animation(matrix, anime, frame);
//	}
//
//	void Update() {
//
//	}
//
//	void Update_SkynMesh(map<string, MESH>& chiidren, XMMATRIX& matrix, vector<Anim> anime, map<string, vector<Bone>>& bones, DWORD frame) {
//		Update_MAtrix(matrix, anime, bones, frame);
//
//		Update_Bone_Matrix(chiidren, bones);
//	}
//
//	void Uninit() {
//		SAFE_RELEASE(VertexBuffer);
//		SAFE_RELEASE(IndexBuffer);
//
//		Vertices.clear();
//
//		Indices.clear();
//
//		for (auto tex : Textures)
//		{
//			SAFE_RELEASE(tex.Texture);
//		}
//		Textures.clear();
//
//		for (auto child : ChildMeshes)
//		{
//			child.second.Uninit();
//		}
//		ChildMeshes.clear();
//	}
//
//	void SetAnimation(const vector<Anim>& animations) {
//		Animation = animations;
//	}
//
//	void Add(const string name, const MESH& mesh) {
//		ChildMeshes[name] = mesh;
//	}
//
//	map<string, MESH>& Get() {
//		return ChildMeshes;
//	}
//
//	vector<Anim>& Get_Anime() {
//		return Animation;
//	}
//
//	bool GetAnime() {
//		if (0 == Animation.size())
//		{
//			return false;
//		}
//
//		return true;
//	}
//};

#endif