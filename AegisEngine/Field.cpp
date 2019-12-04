
#include	"Field.h"
#include	"Renderer.h"

#include	"manager.h"
#include	"Scene.h"
#include	"ShadowMap.h"

FIELD::FIELD()
{
	pVertexBuffer = nullptr;


}

FIELD::FIELD(XMFLOAT3 position, XMFLOAT2 wh)
{
	HRESULT hr;

	pVertexBuffer = nullptr;

	//Position = position;
	WH = wh;

	Vertex[0].Position = XMFLOAT3(-5.0f, 0.0f, -5.0f);
	Vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	Vertex[1].Position = XMFLOAT3(5.0f, 0.0f, -5.0f);
	Vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	Vertex[2].Position = XMFLOAT3(-5.0f, 0.0f, 5.0f);
	Vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	Vertex[3].Position = XMFLOAT3(5.0f, 0.0f, 5.0f);
	Vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	// 頂点バッファの設定
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		// サブリソースの設定
		D3D11_SUBRESOURCE_DATA srd;
		ZeroMemory(&srd, sizeof(D3D11_SUBRESOURCE_DATA));

		srd.pSysMem = Vertex;
		srd.SysMemPitch = 0;
		srd.SysMemSlicePitch = 0;

		// 頂点バッファの生成
		hr = CRenderer::GetDevice()->CreateBuffer(&bd, &srd, &pVertexBuffer);

		if (FAILED(hr))
		{
			return;
		}
	}

	// インデックスバッファの設定
	{
		const WORD index[] = {
		0, 1, 2,
		1, 3, 2,
		};

		D3D11_BUFFER_DESC ibDesc;
		ibDesc.ByteWidth = sizeof(WORD) * 6;
		ibDesc.Usage = D3D11_USAGE_DEFAULT;
		ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibDesc.CPUAccessFlags = 0;
		ibDesc.MiscFlags = 0;
		ibDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA irData;
		irData.pSysMem = index;
		irData.SysMemPitch = 0;
		irData.SysMemSlicePitch = 0;

		hr = CRenderer::GetDevice()->CreateBuffer(&ibDesc, &irData, &pIndexBuffer);
		if (FAILED(hr))
		{
			return;
		}
	}

	// テクスチャの設定
	Texture = new TEXTURE(string("field004.png"));
}

FIELD::~FIELD()
{
	Uninit();
}

void FIELD::Init()
{
	HRESULT hr;

	Vertex[0].Position = XMFLOAT3(-5.0f, 0.0f, 5.0f);
	Vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	Vertex[1].Position = XMFLOAT3(5.0f, 0.0f, 5.0f);
	Vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	Vertex[2].Position = XMFLOAT3(-5.0f, 0.0f, -5.0f);
	Vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	Vertex[3].Position = XMFLOAT3(5.0f, 0.0f, -5.0f);
	Vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);


	// 頂点バッファの設定
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		// サブリソースの設定
		D3D11_SUBRESOURCE_DATA srd;
		ZeroMemory(&srd, sizeof(D3D11_SUBRESOURCE_DATA));

		srd.pSysMem = Vertex;
		srd.SysMemPitch = 0;
		srd.SysMemSlicePitch = 0;

		// 頂点バッファの生成
		hr = CRenderer::GetDevice()->CreateBuffer(&bd, &srd, &pVertexBuffer);

		if (FAILED(hr))
		{
			return;
		}
	}

	// インデックスバッファの設定
	{
		const WORD index[] = {
		0, 1, 2,
		1, 3, 2,
		};

		D3D11_BUFFER_DESC ibDesc;
		ibDesc.ByteWidth = sizeof(WORD) * 6;
		ibDesc.Usage = D3D11_USAGE_DEFAULT;
		ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibDesc.CPUAccessFlags = 0;
		ibDesc.MiscFlags = 0;
		ibDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA irData;
		irData.pSysMem = index;
		irData.SysMemPitch = 0;
		irData.SysMemSlicePitch = 0;

		hr = CRenderer::GetDevice()->CreateBuffer(&ibDesc, &irData, &pIndexBuffer);
		if (FAILED(hr))
		{
			return;
		}
	}

	// テクスチャの設定
	Texture = new TEXTURE("field004.png");

}

void FIELD::Draw()
{
	// 入力アセンブラに頂点バッファを設定.
	CRenderer::SetVertexBuffers(pVertexBuffer);

	CRenderer::SetIndexBuffer(pIndexBuffer);

	Texture->Set_Texture();//

	// 3Dマトリックス設定
	{
		XMMATRIX world;

		world = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));
		world *= XMMatrixTranslation(Position.x, Position.y, Position.z);

		auto camera01 = CManager::Get_Scene()->Get_Game_Object<CCamera>();
		auto camera02 = CManager::Get_Scene()->Get_Game_Object<DEBUG_CAMERA>();

		if (nullptr != camera01)
		{
			// シャドウマップ用の描画か?
			if (CManager::Get_ShadowMap()->Get_Enable())
			{
				XMMATRIX view = CManager::Get_ShadowMap()->Get_View();
				XMMATRIX proj = CManager::Get_ShadowMap()->Get_Plojection();

				CRenderer::Set_MatrixBuffer(world, view, proj);

				CRenderer::Set_Shader(SHADER_INDEX_V::SHADOW_MAP, SHADER_INDEX_P::SHADOW_MAP);
			}
			else
			{
				CRenderer::Set_MatrixBuffer(world, camera01->Get_Camera_View(), camera01->Get_Camera_Projection());

				CRenderer::Set_MatrixBuffer01(*camera02->Get_Pos());

				CRenderer::Set_Shader();
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

				CRenderer::Set_Shader(SHADER_INDEX_V::SHADOW_MAP, SHADER_INDEX_P::SHADOW_MAP);
			}
			else
			{
				CRenderer::Set_MatrixBuffer(world, camera02->Get_Camera_View(), camera02->Get_Camera_Projection());

				CRenderer::Set_MatrixBuffer01(*camera02->Get_Pos());

				CRenderer::Set_Shader();
			}
		}
	}

	CRenderer::DrawIndexed(6, 0, 0);

	CRenderer::Set_Shader();
}


void FIELD::Update(float delta_time)
{

}


void FIELD::Uninit()
{
	SAFE_RELEASE(pVertexBuffer);

	SAFE_DELETE(Texture);
}

//==============================
// ポジションの設定
//==============================
void FIELD::SetPosition(const XMFLOAT3 position)
{
	Position = position;
}

//==============================
// 幅と高さの設定
//==============================
void FIELD::SetWH(const XMFLOAT2 wh)
{
	WH = wh;
}

//==============================
// テクスチャの設定
//==============================
void FIELD::SetTexture(const string& const file_name)
{
	Texture->Set_Texture_Name(file_name);
}