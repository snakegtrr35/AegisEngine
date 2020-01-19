#pragma once

#ifndef MESH_H
#define MESH_H

#include	<assimp/Importer.hpp>
#include	<assimp/cimport.h>
#include	<assimp/scene.h>
#include	<assimp/postprocess.h>
#include	<assimp/matrix4x4.h>

#include	"Renderer.h"

#include	"manager.h"
#include	"ShadowMap.h"
#include	"Scene.h"

//
#define NUM_BONES 4
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

struct TEXTURE_S {
	string path;
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

	map<string, MESH> ChildMeshes;

	void Draw_Mesh(XMMATRIX& parent_matrix) {
		XMMATRIX matrix;

		if (!Indices.empty() && nullptr != Textures[0].Texture)
		{
			CRenderer::SetVertexBuffers(VertexBuffer);

			CRenderer::GetDeviceContext()->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

			CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &Textures[0].Texture);

			// 3Dマトリックス設定
			{
				matrix = XMMatrixMultiply(Matrix, parent_matrix);

				auto camera01 = CManager::Get_Scene()->Get_Game_Object<CCamera>("camera");
				auto camera02 = CManager::Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");
				
				// 普通のカメラかデバッグカメラか?
				if (nullptr != camera01)
				{
					// シャドウマップ用の描画か?
					if (CManager::Get_ShadowMap()->Get_Enable())
					{
						XMMATRIX view = CManager::Get_ShadowMap()->Get_View();
						XMMATRIX proj = CManager::Get_ShadowMap()->Get_Plojection();

						CRenderer::Set_MatrixBuffer(matrix, view, proj);
					}
					else
					{
						CRenderer::Set_MatrixBuffer(matrix, camera01->Get_Camera_View(), camera01->Get_Camera_Projection());

						CRenderer::Set_MatrixBuffer01(*camera01->Get_Pos());
					}
				}
				else
				{
					// シャドウマップ用の描画か?
					if (CManager::Get_ShadowMap()->Get_Enable())
					{
						XMMATRIX view = CManager::Get_ShadowMap()->Get_View();
						XMMATRIX proj = CManager::Get_ShadowMap()->Get_Plojection();

						CRenderer::Set_MatrixBuffer(matrix, view, proj);
					}
					else
					{
						CRenderer::Set_MatrixBuffer(matrix, camera02->Get_Camera_View(), camera02->Get_Camera_Projection());

						CRenderer::Set_MatrixBuffer01(*camera02->Get_Pos());
					}
				}
			}

			CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			CRenderer::GetDeviceContext()->DrawIndexed(Indices.size(), 0, 0);
		}
		else
		{
			matrix = XMMatrixMultiply(Matrix, parent_matrix);
		}

		for (auto child : ChildMeshes)
		{
			child.second.Draw_Mesh(matrix);
		}
	}

	void Draw_DPP_Mesh(XMMATRIX& parent_matrix) {
		XMMATRIX matrix;

		if (!Indices.empty())
		{
			// 3Dマトリックス設定
			{
				matrix = XMMatrixMultiply(Matrix, parent_matrix);

				auto camera01 = CManager::Get_Scene()->Get_Game_Object<CCamera>("camera");
				auto camera02 = CManager::Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

				// 普通のカメラかデバッグカメラか?
				if (nullptr != camera01)
				{
					CRenderer::Set_MatrixBuffer(matrix, camera01->Get_Camera_View(), camera01->Get_Camera_Projection());
				}
				else
				{
					CRenderer::Set_MatrixBuffer(matrix, camera02->Get_Camera_View(), camera02->Get_Camera_Projection());
				}
			}

			CRenderer::SetVertexBuffers(VertexBuffer);

			CRenderer::GetDeviceContext()->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

			CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			CRenderer::GetDeviceContext()->DrawIndexed(Indices.size(), 0, 0);
		}
		else
		{
			matrix = XMMatrixMultiply(Matrix, parent_matrix);
		}

		for (auto child : ChildMeshes)
		{
			child.second.Draw_DPP_Mesh(matrix);
		}
	}

	void Draw_Mesh_Animation(XMMATRIX& parent_matrix, unordered_map<string, Anim>& anime, DWORD frame, const string& name1, const string& name2 = string("none"), float blend = 0.0f) {
		XMMATRIX world;

		CRenderer::SetVertexBuffers(VertexBuffer);

		CRenderer::GetDeviceContext()->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &Textures[0].Texture);

		// 3Dマトリックス設定
		{
			if ("none" == name2)
			{
				for (auto i : anime[name1].body)
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

						auto camera01 = CManager::Get_Scene()->Get_Game_Object<CCamera>("camera");
						auto camera02 = CManager::Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

						// 普通のカメラかデバッグカメラか?
						if (nullptr != camera01)
						{
							// シャドウマップ用の描画か?
							if (CManager::Get_ShadowMap()->Get_Enable())
							{
								XMMATRIX view = CManager::Get_ShadowMap()->Get_View();
								XMMATRIX proj = CManager::Get_ShadowMap()->Get_Plojection();

								CRenderer::Set_MatrixBuffer(world, view, proj);
							}
							else
							{
								CRenderer::Set_MatrixBuffer(world, camera01->Get_Camera_View(), camera01->Get_Camera_Projection());

								CRenderer::Set_MatrixBuffer01(*camera01->Get_Pos());
							}
						}
						else
						{
							// シャドウマップ用の描画か?
							if (CManager::Get_ShadowMap()->Get_Enable())
							{
								XMMATRIX view = CManager::Get_ShadowMap()->Get_View();
								XMMATRIX proj = CManager::Get_ShadowMap()->Get_Plojection();

								CRenderer::Set_MatrixBuffer(world, view, proj);
							}
							else
							{
								CRenderer::Set_MatrixBuffer(world, camera02->Get_Camera_View(), camera02->Get_Camera_Projection());

								CRenderer::Set_MatrixBuffer01(*camera02->Get_Pos());
							}
						}

						break;
					}
				}
			}
			else
			{
				XMMATRIX trans1, trans2, trans;
				XMVECTOR quat1, quat2, puat;

				for (auto i : anime[name1].body)
				{
					if (i.node_name == Name)
					{
						unsigned int f = frame % i.translate.begin()->time;

						XMFLOAT3 pos = i.translate[f].value;
						trans1 = XMMatrixTranslation(pos.x, pos.y, pos.z);

						f = frame % i.rotation.begin()->time;

						XMFLOAT4 rotation = i.rotation[f].value;
						quat1 = XMLoadFloat4(&rotation);

						break;
					}
				}

				for (auto i : anime[name2].body)
				{
					if (i.node_name == Name)
					{
						unsigned int f = frame % i.translate.begin()->time;

						XMFLOAT3 pos = i.translate[f].value;
						trans2 = XMMatrixTranslation(pos.x, pos.y, pos.z);

						f = frame % i.rotation.begin()->time;

						XMFLOAT4 rotation = i.rotation[f].value;
						quat2 = XMLoadFloat4(&rotation);

						break;
					}
				}

				trans = trans1 * (1.0f - blend) + trans2 * blend;
				//puat = quat1 * (1.0f - blend) + quat2 *  blend;
				puat = XMQuaternionSlerp(quat1, quat2, blend);

				world = XMMatrixRotationQuaternion(puat);

				world = XMMatrixMultiply(world, trans);

				world = XMMatrixMultiply(world, parent_matrix);

				auto camera01 = CManager::Get_Scene()->Get_Game_Object<CCamera>("camera");
				auto camera02 = CManager::Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

				// 普通のカメラかデバッグカメラか?
				if (nullptr != camera01)
				{
					// シャドウマップ用の描画か?
					if (CManager::Get_ShadowMap()->Get_Enable())
					{
						XMMATRIX view = CManager::Get_ShadowMap()->Get_View();
						XMMATRIX proj = CManager::Get_ShadowMap()->Get_Plojection();

						CRenderer::Set_MatrixBuffer(world, view, proj);
					}
					else
					{
						CRenderer::Set_MatrixBuffer(world, camera01->Get_Camera_View(), camera01->Get_Camera_Projection());

						CRenderer::Set_MatrixBuffer01(*camera01->Get_Pos());
					}
				}
				else
				{
					// シャドウマップ用の描画か?
					if (CManager::Get_ShadowMap()->Get_Enable())
					{
						XMMATRIX view = CManager::Get_ShadowMap()->Get_View();
						XMMATRIX proj = CManager::Get_ShadowMap()->Get_Plojection();

						CRenderer::Set_MatrixBuffer(world, view, proj);
					}
					else
					{
						CRenderer::Set_MatrixBuffer(world, camera02->Get_Camera_View(), camera02->Get_Camera_Projection());

						CRenderer::Set_MatrixBuffer01(*camera02->Get_Pos());
					}
				}
			}
		}

		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		CRenderer::GetDeviceContext()->DrawIndexed(Indices.size(), 0, 0);

		for (auto child : ChildMeshes)
		{
			child.second.Draw_Mesh_Animation(world, anime, frame, name1, name2, blend);
		}
	}

	void Draw_DPP_Mesh_Animation(XMMATRIX& parent_matrix, unordered_map<string, Anim>& anime, DWORD frame, const string& name1, const string& name2 = string("none"), float blend = 0.0f) {
		XMMATRIX world;

		// 3Dマトリックス設定
		{
			if ("none" == name2)
			{
				for (auto i : anime[name1].body)
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

						auto camera01 = CManager::Get_Scene()->Get_Game_Object<CCamera>("camera");
						auto camera02 = CManager::Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

						// 普通のカメラかデバッグカメラか?
						if (nullptr != camera01)
						{
								CRenderer::Set_MatrixBuffer(world, camera01->Get_Camera_View(), camera01->Get_Camera_Projection());
						}
						else
						{
							CRenderer::Set_MatrixBuffer(world, camera02->Get_Camera_View(), camera02->Get_Camera_Projection());
						}

						break;
					}
				}
			}
			else
			{
				XMMATRIX trans1, trans2, trans;
				XMVECTOR quat1, quat2, puat;

				for (auto i : anime[name1].body)
				{
					if (i.node_name == Name)
					{
						unsigned int f = frame % i.translate.begin()->time;

						XMFLOAT3 pos = i.translate[f].value;
						trans1 = XMMatrixTranslation(pos.x, pos.y, pos.z);

						f = frame % i.rotation.begin()->time;

						XMFLOAT4 rotation = i.rotation[f].value;
						quat1 = XMLoadFloat4(&rotation);

						break;
					}
				}

				for (auto i : anime[name2].body)
				{
					if (i.node_name == Name)
					{
						unsigned int f = frame % i.translate.begin()->time;

						XMFLOAT3 pos = i.translate[f].value;
						trans2 = XMMatrixTranslation(pos.x, pos.y, pos.z);

						f = frame % i.rotation.begin()->time;

						XMFLOAT4 rotation = i.rotation[f].value;
						quat2 = XMLoadFloat4(&rotation);

						break;
					}
				}

				trans = trans1 * (1.0f - blend) + trans2 * blend;
				puat = XMQuaternionSlerp(quat1, quat2, blend);

				world = XMMatrixRotationQuaternion(puat);

				world = XMMatrixMultiply(world, trans);

				world = XMMatrixMultiply(world, parent_matrix);

				auto camera01 = CManager::Get_Scene()->Get_Game_Object<CCamera>("camera");
				auto camera02 = CManager::Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

				// 普通のカメラかデバッグカメラか?
				if (nullptr != camera01)
				{
					CRenderer::Set_MatrixBuffer(world, camera01->Get_Camera_View(), camera01->Get_Camera_Projection());
				}
				else
				{
					CRenderer::Set_MatrixBuffer(world, camera02->Get_Camera_View(), camera02->Get_Camera_Projection());
				}
			}
		}

		CRenderer::SetVertexBuffers(VertexBuffer);

		CRenderer::GetDeviceContext()->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		CRenderer::GetDeviceContext()->DrawIndexed(Indices.size(), 0, 0);

		for (auto child : ChildMeshes)
		{
			child.second.Draw_DPP_Mesh_Animation(world, anime, frame, name1, name2, blend);
		}
	}

	bool SetupMesh(vector<VERTEX_3D>& vertices) {
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

		//Vertices = vertices;
		Indices = indices;
		Textures = textures;
		Matrix = matrix;

		Name = name;

		SetupMesh(vertices);
	}

	~MESH() {};

	void Draw(XMMATRIX& matrix) {
		Draw_Mesh(matrix);
	}

	void Draw_DPP(XMMATRIX& matrix) {
		Draw_DPP_Mesh(matrix);
	}

	void Draw_Animation(XMMATRIX& matrix, unordered_map<string, Anim>& anime, DWORD frame, const string& name1, const string& name2 = string("none"), float blend = 0.0f) {
		Draw_Mesh_Animation(matrix, anime, frame, name1, name2, blend);
	}

	void Draw_DPP_Animation(XMMATRIX& matrix, unordered_map<string, Anim>& anime, DWORD frame, const string& name1, const string& name2 = string("none"), float blend = 0.0f) {
		Draw_DPP_Mesh_Animation(matrix, anime, frame, name1, name2, blend);
	}

	void Update() {}

	void Uninit() {
		SAFE_RELEASE(VertexBuffer);
		SAFE_RELEASE(IndexBuffer);

		Indices.clear();

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

	void SetAnimation(const string& name, const Anim& animations) {
		Animation[name] = animations;
	}

	void Add(const string name, const MESH& mesh) {
		ChildMeshes[name] = mesh;
	}

	map<string, MESH>& Get() {
		return ChildMeshes;
	}

	unordered_map<string, Anim>& Get_Anime() {
		return Animation;
	}

	bool GetAnime() {
		if (0 == Animation.size())
		{
			return false;
		}

		return true;
	}
};

#endif