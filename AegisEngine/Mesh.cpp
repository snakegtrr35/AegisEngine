#include "Mesh.h"
#include "Renderer.h"

using namespace aegis;

#include "TextureImporter.h"

#pragma comment (lib, "assimp-vc141-mt.lib")

MESH::MESH() : VertexBuffer(nullptr), IndexBuffer(nullptr)
{
}

MESH::MESH(aegis::vector<VERTEX_3D>& vertices, aegis::vector<UINT>& indices, aegis::string& texture_name, XMMATRIX& matrix, aegis::string name) : Name(name), TextureName(texture_name), Matrix(XMMATRIXToXMFLOAT4X4(matrix)), Vertices(vertices), Indices(indices), VertexBuffer(nullptr), IndexBuffer(nullptr)
{
}

void MESH::Init()
{
	{
		SetupMesh();
		{
			vector<VERTEX_3D> vertex;

			vertex.swap(Vertices);
		}

		{
			for (auto& tex : Textures)
			{
				const aegis::string directory = "./asset/model/";
				aegis::string path = directory + tex.FileName;
				std::wstring filenamews = std::wstring(path.begin(), path.end());

				{
					TextureImporter* textureImporter = TextureImporter::getInstance();

					ShaderResourceView* texture;

					tex.Texture = textureImporter->GetShaderResourceView(filenamews.c_str());

				}
			}
		}

		for (auto& child : ChildMeshes)
		{
			child.Init();
		}
	}
}

void MESH::Draw(XMMATRIX& matrix, const aegis::vector<TEXTURE_S>& textures)
{
	Draw_Mesh(matrix, textures);
}

void MESH::Draw_DPP(XMMATRIX& matrix)
{
	Draw_DPP_Mesh(matrix);
}

void MESH::Update()
{
}

void MESH::Uninit()
{
	//SAFE_RELEASE(VertexBuffer);
	//SAFE_RELEASE(IndexBuffer);

	Indices.clear();

	for (auto& tex : Textures)
	{
		tex.FileName.clear();

		AllocatorDelete<aegis::ShaderResourceView> deleter;
		deleter(tex.Texture);
		tex.Texture = nullptr;
	}
	Textures.clear();

	for (auto& child : ChildMeshes)
	{
		child.Uninit();
	}
	ChildMeshes.clear();

	{
		AllocatorDelete<aegis::Buffer> deleter;
		deleter(VertexBuffer);
		VertexBuffer = nullptr;
		
		deleter(IndexBuffer);
		IndexBuffer = nullptr;
	}
}

aegis::vector<MESH>& MESH::Get_Meshs()
{
	return ChildMeshes;
}

aegis::vector<TEXTURE_S>& MESH::Get_Textures()
{
	return Textures;
}

const aegis::string& MESH::Get_Name()
{
	return Name;
}

void MESH::Set_Name(const aegis::string& name)
{
	Name = name;
}

const aegis::string& MESH::Get_Texture_Name()
{
	return TextureName;
}

void MESH::Set_Texture_Name(const aegis::string & texture_name)
{
	TextureName = texture_name;
}

void MESH::Set(const MESH& meshs)
{
	this->Vertices = meshs.Vertices;
	this->Indices = meshs.Indices;
	this->Matrix = meshs.Matrix;
	this->Name = meshs.Name;
	this->TextureName = meshs.TextureName;
	this->Textures = meshs.Textures;

	this->ChildMeshes.resize(meshs.ChildMeshes.size());

	for (UINT i = 0; i < meshs.ChildMeshes.size(); i++)
	{
		this->ChildMeshes.at(i).Set(meshs.ChildMeshes.at(i));
	}
}

void MESH::SetupMesh()
{
	CRenderer* render = CRenderer::getInstance();
	HRESULT hr;

	// 頂点バッファの生成
	if(nullptr == VertexBuffer && !Vertices.empty())
	{
		BufferDesc  bd{};
		bd.Usage = Usage::Default;
		bd.ByteWidth = sizeof(VERTEX_3D) * Vertices.size();
		bd.BindFlags = BindFlag::Vertexbuffer;
		bd.CPUAccessFlags = CpuAccessFlag::None;

		// サブリソースの設定
		SubresourceData sd{};
		sd.pSysMem = Vertices.data();

		///VertexBuffer.reset(render->CreateBuffer(bd, sd));
		VertexBuffer = render->CreateBuffer(bd, sd);
	}

	// インデックスバッファの生成
	if (nullptr == IndexBuffer && !Indices.empty())
	{
		BufferDesc  bd{};
		bd.Usage = Usage::Default;
		bd.ByteWidth = sizeof(uint32) * (UINT)Indices.size();
		bd.BindFlags = BindFlag::Indexbuffer;
		bd.CPUAccessFlags = CpuAccessFlag::None;

		// サブリソースの設定
		SubresourceData sd{};
		sd.pSysMem = Indices.data();

		///IndexBuffer.reset(render->CreateBuffer(bd, sd));
		IndexBuffer = render->CreateBuffer(bd, sd);
	}
}

void MESH::Draw_Mesh(XMMATRIX& parent_matrix, const aegis::vector<TEXTURE_S>& textures)
{
	CRenderer* render = CRenderer::getInstance();
	XMMATRIX matrix;

	if (!Indices.empty())
	{
		render->SetVertexBuffers(VertexBuffer);

		render->SetIndexBuffer(IndexBuffer, Format::R32Uint);

		for (UINT i = 0; textures.size(); i++)
		{
			if (textures[i].FileName == TextureName)
			{
				render->PSSetShaderResources(0, 1, &textures[i].Texture);
				break;
			}
		}

		// 3Dマトリックス設定
		{
			matrix = XMMatrixMultiply(XMFLOAT4X4ToXMMATRIX(Matrix), parent_matrix);

			const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
			const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

			// 普通のカメラかデバッグカメラか?
			if (!camera01.expired())
			{
				// シャドウマップ用の描画か?
				if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
				{
					XMMATRIX view = CManager::Get_Instance()->Get_ShadowMap()->Get_View();
					XMMATRIX proj = CManager::Get_Instance()->Get_ShadowMap()->Get_Plojection();

					render->Set_MatrixBuffer(matrix, view, proj);
				}
				else
				{
					render->Set_MatrixBuffer(matrix, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());

					render->Set_MatrixBuffer01(*camera01.lock()->Get_Pos());
				}
			}
			else
			{
				// シャドウマップ用の描画か?
				if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
				{
					XMMATRIX view = CManager::Get_Instance()->Get_ShadowMap()->Get_View();
					XMMATRIX proj = CManager::Get_Instance()->Get_ShadowMap()->Get_Plojection();

					render->Set_MatrixBuffer(matrix, view, proj);
				}
				else
				{
					render->Set_MatrixBuffer(matrix, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());

					render->Set_MatrixBuffer01(*camera02.lock()->Get_Pos());
				}
			}
		}

		render->SetPrimitiveTopology(PrimitiveTopology::TriangleList);

		render->DrawIndexed((UINT)Indices.size(), 0, 0);
	}
	else
	{
		matrix = XMMatrixMultiply(XMFLOAT4X4ToXMMATRIX(Matrix), parent_matrix);
	}

	for (auto& child : ChildMeshes)
	{
		child.Draw_Mesh(matrix, textures);
	}
}

void MESH::Draw_DPP_Mesh(XMMATRIX& parent_matrix)
{
}