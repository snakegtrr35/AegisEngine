#pragma once

#ifndef MESH_H
#define MESH_H

#include	"main.h"
#include	"Renderer.h"

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
	XMMATRIX animation;
	XMMATRIX offset;

	//XMFLOAT4X4 offset;
	//XMFLOAT4X4 offset;

	vector<Weight> weights;
};

struct VectorKey {
	double time;
	XMFLOAT3 value;
};

struct QuatKey {
	double time;
	XMFLOAT4 value;
};

struct NodeAnim {
	std::string node_name;

	std::vector<VectorKey> translate;
	std::vector<VectorKey> scaling;
	std::vector<QuatKey>   rotation;
};

struct Anim {
	double duration;
	std::vector<NodeAnim> body;
};



//class Mesh{
//public:
//
//	Mesh(vector<VERTEX_3D> vertices, vector<UINT> indices, vector<TEXTURE_S> textures, vector<Bone> bones, XMMATRIX& matrix, XMMATRIX& parent_matrix/*, string name, string p_name*/)
//	{
//		this->vertices = vertices;
//		this->indices = indices;
//		this->textures = textures;
//		this->bones = bones;
//
//		Matrix = matrix;
//		ParentMatrix = parent_matrix;
//
//		//Name = name;
//		//ParentName = p_name;
//
//		setupMesh();
//	}
//
//	void Draw(XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 scaling)
//	{
//		CRenderer::SetVertexBuffers(VertexBuffer);
//
//		CRenderer::GetDeviceContext()->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
//
//		CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &textures[0].Texture);
//
//		// 3Dマトリックス設定
//		{
//			Matrix = XMMatrixMultiply(Matrix, ParentMatrix);
//
//			CRenderer::SetWorldMatrix(&Matrix);
//		}
//		
//		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//		CRenderer::GetDeviceContext()->DrawIndexed(indices.size(), 0, 0);
//	}
//
//	void Uninit()
//	{
//		SAFE_RELEASE(VertexBuffer);
//		SAFE_RELEASE(IndexBuffer);
//
//		vertices.clear();
//
//		indices.clear();
//
//		for (auto bone : bones)
//		{
//			bone.weights.clear();
//		}
//		bones.clear();
//
//		for (auto tex : textures)
//		{
//			SAFE_RELEASE(tex.Texture);
//		}
//		textures.clear();
//	}
//
//	void SetAnimation(const vector<Anim>& animations)
//	{
//		this->animation = animations;
//	}
//
//private:
//
//	vector<VERTEX_3D> vertices;
//	vector<UINT> indices;
//	vector<TEXTURE_S> textures;
//
//	vector<Anim> animation;
//
//	vector<Bone> bones;
//
//	/*  Render data  */
//	ID3D11Buffer* VertexBuffer;
//	ID3D11Buffer* IndexBuffer;
//
//	XMMATRIX Matrix, ParentMatrix;
//
//	string Name, ParentName;
//
//	/*  Functions    */
//	// Initializes all the buffer objects/arrays
//	bool setupMesh()
//	{
//		HRESULT hr;
//
//		{
//			D3D11_BUFFER_DESC vbd;
//			vbd.Usage = D3D11_USAGE_DEFAULT;
//			vbd.ByteWidth = sizeof(VERTEX_3D) * vertices.size();
//			vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//			vbd.CPUAccessFlags = 0;
//			vbd.MiscFlags = 0;
//
//			// サブリソースの設定
//			D3D11_SUBRESOURCE_DATA initData;
//			ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
//
//			initData.pSysMem = &vertices[0];
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
//			ibd.ByteWidth = sizeof(UINT) * indices.size();
//			ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
//			ibd.CPUAccessFlags = 0;
//			ibd.MiscFlags = 0;
//
//			// サブリソースの設定
//			D3D11_SUBRESOURCE_DATA initData;
//			ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
//
//			initData.pSysMem = &indices[0];
//			initData.SysMemPitch = 0;
//			initData.SysMemSlicePitch = 0;
//
//			hr = CRenderer::GetDevice()->CreateBuffer(&ibd, &initData, &IndexBuffer);
//			if (FAILED(hr))
//				return false;
//		}
//	}
//};



class MESH {
private:
	vector<VERTEX_3D> Vertices;
	vector<UINT> Indices;
	vector<TEXTURE_S> Textures;

	vector<Anim> Animation;

	vector<Bone> Bones;

	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;

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

	MESH(vector<VERTEX_3D> vertices, vector<UINT> indices, vector<TEXTURE_S> textures, vector<Bone> bones, XMMATRIX& matrix) {
		VertexBuffer = nullptr;
		IndexBuffer = nullptr;

		Vertices = vertices;
		Indices = indices;
		Textures = textures;
		Bones = bones;
		Matrix = matrix;

		SetupMesh();
	}

	~MESH() {};

	void Draw(XMMATRIX& matrix) {
		Draw_Mesh(matrix);
	}

	void Uninit() {
		SAFE_RELEASE(VertexBuffer);
		SAFE_RELEASE(IndexBuffer);

		Vertices.clear();

		Indices.clear();

		for (auto bone : Bones)
		{
			bone.weights.clear();
		}
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
};

#endif