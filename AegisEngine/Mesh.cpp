#include	"Mesh.h"

using namespace aegis;

#include	"external/DirectXTex/WICTextureLoader.h"

#pragma comment (lib, "assimp-vc141-mt.lib")

MESH::MESH() : VertexBuffer(nullptr), IndexBuffer(nullptr)
{
}

MESH::MESH(aegis::vector<VERTEX_3D>& vertices, aegis::vector<UINT>& indices, std::string& texture_name, XMMATRIX& matrix, std::string name) : Name(name), TextureName(texture_name), Matrix(XMMATRIXToXMFLOAT4X4(matrix)), Vertices(vertices), Indices(indices), VertexBuffer(nullptr), IndexBuffer(nullptr)
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
				const std::string directory = "./asset/model/";
				std::string path = directory + tex.FileName;
				std::wstring filenamews = std::wstring(path.begin(), path.end());

				{
					CRenderer* render = CRenderer::getInstance();

					HRESULT hr = CreateWICTextureFromFile(render->GetDevice(), render->GetDeviceContext(), filenamews.c_str(), nullptr, &tex.Texture, nullptr, nullptr);
					if (FAILED(hr))
						FAILDE_ASSERT
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
		SAFE_RELEASE(tex.Texture);
	}
	Textures.clear();

	for (auto& child : ChildMeshes)
	{
		child.Uninit();
	}
	ChildMeshes.clear();
}

aegis::vector<MESH>& MESH::Get_Meshs()
{
	return ChildMeshes;
}

aegis::vector<TEXTURE_S>& MESH::Get_Textures()
{
	return Textures;
}

const std::string& MESH::Get_Name()
{
	return Name;
}

void MESH::Set_Name(const std::string& name)
{
	Name = name;
}

const std::string& MESH::Get_Texture_Name()
{
	return TextureName;
}

void MESH::Set_Texture_Name(const std::string & texture_name)
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
		D3D11_BUFFER_DESC desc;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VERTEX_3D) * Vertices.size();
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		// サブリソースの設定
		D3D11_SUBRESOURCE_DATA initData{};

		initData.pSysMem = Vertices.data();

		hr = render->GetDevice()->CreateBuffer(&desc, &initData, &VertexBuffer);
		if (FAILED(hr))
			FAILDE_ASSERT;
	}

	// インデックスバッファの生成
	if (nullptr == IndexBuffer && !Indices.empty())
	{
		D3D11_BUFFER_DESC desc;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(UINT) * (UINT)Indices.size();
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		// サブリソースの設定
		D3D11_SUBRESOURCE_DATA initData{};

		initData.pSysMem = Indices.data();

		hr = render->GetDevice()->CreateBuffer(&desc, &initData, &IndexBuffer);
		if (FAILED(hr))
			FAILDE_ASSERT;
	}
}

void MESH::Draw_Mesh(XMMATRIX& parent_matrix, const aegis::vector<TEXTURE_S>& textures)
{
	CRenderer* render = CRenderer::getInstance();
	XMMATRIX matrix;

	if (!Indices.empty())
	{
		render->SetVertexBuffers(VertexBuffer.Get());

		render->GetDeviceContext()->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		for (UINT i = 0; textures.size(); i++)
		{
			if (textures[i].FileName == TextureName)
			{
				render->GetDeviceContext()->PSSetShaderResources(0, 1, &textures[i].Texture);
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

		render->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		render->DrawIndexed((UINT)Indices.size(), 0, 0);
	}
	else
	{
		matrix = XMMatrixMultiply(XMFLOAT4X4ToXMMATRIX(Matrix), parent_matrix);
	}

	for (auto child : ChildMeshes)
	{
		child.Draw_Mesh(matrix, textures);
	}
}

void MESH::Draw_DPP_Mesh(XMMATRIX& parent_matrix)
{
}