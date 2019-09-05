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
	XMMATRIX offset;
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



class Mesh{
public:

	Mesh(vector<VERTEX_3D> vertices, vector<UINT> indices, vector<TEXTURE_S> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		//World = XMMatrixIdentity();

		setupMesh();
	}

	Mesh(vector<VERTEX_3D> vertices, vector<UINT> indices, vector<TEXTURE_S> textures, vector<Bone> bones, XMMATRIX matrix, XMMATRIX parent_matrix)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		this->bones = bones;

		World = XMMatrixIdentity();

		Matrix = matrix;
		ParentMatrix = parent_matrix;

		setupMesh();
	}

	void Draw(XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 scaling)
	{
		CRenderer::SetVertexBuffers(VertexBuffer);

		CRenderer::GetDeviceContext()->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &textures[0].Texture);

		// 3Dマトリックス設定
		{
			Matrix = XMMatrixMultiply(Matrix, ParentMatrix);

			//World *= Matrix;
			//World = XMMatrixMultiply(World, Scaling);
			//World = XMMatrixMultiply(World, Rotation);
			//World = XMMatrixMultiply(World, Translation);

			//World = XMMatrixScaling(scaling.x, scaling.y, scaling.z);																						// 拡大縮小
			//World *= XMMatrixRotationRollPitchYaw( XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.z) );		// 回転(ロールピッチヨウ)
			//World *= XMMatrixTranslation(position.x, position.y, position.z);																				// 移動
			CRenderer::SetWorldMatrix(&Matrix);
		}
		
		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		CRenderer::GetDeviceContext()->DrawIndexed(indices.size(), 0, 0);
	}

	void Uninit()
	{
		SAFE_RELEASE(VertexBuffer);
		SAFE_RELEASE(IndexBuffer);

		vertices.clear();

		indices.clear();

		for (auto bone : bones)
		{
			bone.weights.clear();
		}
		bones.clear();

		for (auto tex : textures)
		{
			SAFE_RELEASE(tex.Texture);
		}
		textures.clear();


	}

	void SetAnimation(const vector<Anim>& animations)
	{
		this->animation = animations;
	}

private:

	vector<VERTEX_3D> vertices;
	vector<UINT> indices;
	vector<TEXTURE_S> textures;

	vector<Anim> animation;

	vector<Bone> bones;

	/*  Render data  */
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;

	XMMATRIX World;

	XMMATRIX Matrix, ParentMatrix;

	/*  Functions    */
	// Initializes all the buffer objects/arrays
	bool setupMesh()
	{
		HRESULT hr;

		{
			D3D11_BUFFER_DESC vbd;
			vbd.Usage = D3D11_USAGE_DEFAULT;
			vbd.ByteWidth = sizeof(VERTEX_3D) * vertices.size();
			vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vbd.CPUAccessFlags = 0;
			vbd.MiscFlags = 0;

			// サブリソースの設定
			D3D11_SUBRESOURCE_DATA initData;
			ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));

			initData.pSysMem = &vertices[0];
			initData.SysMemPitch = 0;
			initData.SysMemSlicePitch = 0;

			hr = CRenderer::GetDevice()->CreateBuffer(&vbd, &initData, &VertexBuffer);
			if (FAILED(hr))
				return false;
		}

		{
			D3D11_BUFFER_DESC ibd;
			ibd.Usage = D3D11_USAGE_DEFAULT;
			ibd.ByteWidth = sizeof(UINT) * indices.size();
			ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			ibd.CPUAccessFlags = 0;
			ibd.MiscFlags = 0;

			// サブリソースの設定
			D3D11_SUBRESOURCE_DATA initData;
			ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));

			initData.pSysMem = &indices[0];
			initData.SysMemPitch = 0;
			initData.SysMemSlicePitch = 0;

			hr = CRenderer::GetDevice()->CreateBuffer(&ibd, &initData, &IndexBuffer);
			if (FAILED(hr))
				return false;
		}
	}
};

#endif