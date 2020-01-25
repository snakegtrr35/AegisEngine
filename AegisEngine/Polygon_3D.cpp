#include	"Polygon_3D.h"
#include	"texture.h"
#include	"Input.h"
#include	"My_imgui.h"

#include	"manager.h"
#include	"ShadowMap.h"
#include	"Scene.h"

static float roll = 0.0f;
static float pichi = 0.0f;
static float yaw = 0.0f;

POLYGON_3D::POLYGON_3D()
{
	pVertexBuffer = nullptr;
	Texture = nullptr;

	XYZ = XMFLOAT3(0.5f, 0.5f, 0.5f);

	// テクスチャの設定
	Texture = new TEXTURE(string("field004.png"));
}

POLYGON_3D::POLYGON_3D(XMFLOAT3 position, XMFLOAT3 xyz)
{
	pVertexBuffer = nullptr;
	Texture = nullptr;

	Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);

	Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);

	Position = position;
	XYZ = xyz;

	// テクスチャの設定
	Texture = new TEXTURE(string("field004.png"));
}

POLYGON_3D::~POLYGON_3D()
{
	SAFE_RELEASE(pVertexBuffer);
	SAFE_DELETE(Texture);
}

void POLYGON_3D::Init(void)
{
	HRESULT hr;

	POLYGOM Polygon_3d;

	// 正面
	Polygon_3d.Vertex[0].Position = XMFLOAT3(-XYZ.x, XYZ.y, -XYZ.z);
	Polygon_3d.Vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	Polygon_3d.Vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	Polygon_3d.Vertex[1].Position = XMFLOAT3(XYZ.x, XYZ.y, -XYZ.z);
	Polygon_3d.Vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	Polygon_3d.Vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	Polygon_3d.Vertex[2].Position = XMFLOAT3(-XYZ.x, -XYZ.y, -XYZ.z);
	Polygon_3d.Vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	Polygon_3d.Vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	Polygon_3d.Vertex[3].Position = XMFLOAT3(XYZ.x, -XYZ.y, -XYZ.z);
	Polygon_3d.Vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	Polygon_3d.Vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	// 裏面
	Polygon_3d.Vertex[4].Position = XMFLOAT3(XYZ.x, XYZ.y, XYZ.z);
	Polygon_3d.Vertex[4].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	Polygon_3d.Vertex[4].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[4].TexCoord = XMFLOAT2(0.0f, 0.0f);

	Polygon_3d.Vertex[5].Position = XMFLOAT3(-XYZ.x, XYZ.y, XYZ.z);
	Polygon_3d.Vertex[5].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	Polygon_3d.Vertex[5].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[5].TexCoord = XMFLOAT2(1.0f, 0.0f);

	Polygon_3d.Vertex[6].Position = XMFLOAT3(XYZ.x, -XYZ.y, XYZ.z);
	Polygon_3d.Vertex[6].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	Polygon_3d.Vertex[6].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[6].TexCoord = XMFLOAT2(0.0f, 1.0f);

	Polygon_3d.Vertex[7].Position = XMFLOAT3(-XYZ.x, -XYZ.y, XYZ.z);
	Polygon_3d.Vertex[7].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	Polygon_3d.Vertex[7].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[7].TexCoord = XMFLOAT2(1.0f, 1.0f);

	// 右面
	Polygon_3d.Vertex[8].Position = XMFLOAT3(XYZ.x, XYZ.y, -XYZ.z);
	Polygon_3d.Vertex[8].Normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
	Polygon_3d.Vertex[8].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[8].TexCoord = XMFLOAT2(0.0f, 0.0f);

	Polygon_3d.Vertex[9].Position = XMFLOAT3(XYZ.x, XYZ.y, XYZ.z);
	Polygon_3d.Vertex[9].Normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
	Polygon_3d.Vertex[9].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[9].TexCoord = XMFLOAT2(1.0f, 0.0f);

	Polygon_3d.Vertex[10].Position = XMFLOAT3(XYZ.x, -XYZ.y, -XYZ.z);
	Polygon_3d.Vertex[10].Normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
	Polygon_3d.Vertex[10].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[10].TexCoord = XMFLOAT2(0.0f, 1.0f);

	Polygon_3d.Vertex[11].Position = XMFLOAT3(XYZ.x, -XYZ.y, XYZ.z);
	Polygon_3d.Vertex[11].Normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
	Polygon_3d.Vertex[11].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[11].TexCoord = XMFLOAT2(1.0f, 1.0f);

	// 左面
	Polygon_3d.Vertex[12].Position = XMFLOAT3(-XYZ.x, XYZ.y, XYZ.z);
	Polygon_3d.Vertex[12].Normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
	Polygon_3d.Vertex[12].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[12].TexCoord = XMFLOAT2(0.0f, 0.0f);

	Polygon_3d.Vertex[13].Position = XMFLOAT3(-XYZ.x, XYZ.y, -XYZ.z);
	Polygon_3d.Vertex[13].Normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
	Polygon_3d.Vertex[13].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[13].TexCoord = XMFLOAT2(1.0f, 0.0f);

	Polygon_3d.Vertex[14].Position = XMFLOAT3(-XYZ.x, -XYZ.y, XYZ.z);
	Polygon_3d.Vertex[14].Normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
	Polygon_3d.Vertex[14].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[14].TexCoord = XMFLOAT2(0.0f, 1.0f);

	Polygon_3d.Vertex[15].Position = XMFLOAT3(-XYZ.x, -XYZ.y, -XYZ.z);
	Polygon_3d.Vertex[15].Normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
	Polygon_3d.Vertex[15].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[15].TexCoord = XMFLOAT2(1.0f, 1.0f);

	// 上面
	Polygon_3d.Vertex[16].Position = XMFLOAT3(-XYZ.x, XYZ.y, XYZ.z);
	Polygon_3d.Vertex[16].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Polygon_3d.Vertex[16].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[16].TexCoord = XMFLOAT2(0.0f, 0.0f);

	Polygon_3d.Vertex[17].Position = XMFLOAT3(XYZ.x, XYZ.y, XYZ.z);
	Polygon_3d.Vertex[17].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Polygon_3d.Vertex[17].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[17].TexCoord = XMFLOAT2(1.0f, 0.0f);

	Polygon_3d.Vertex[18].Position = XMFLOAT3(-XYZ.x, XYZ.y, -XYZ.z);
	Polygon_3d.Vertex[18].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Polygon_3d.Vertex[18].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[18].TexCoord = XMFLOAT2(0.0f, 1.0f);

	Polygon_3d.Vertex[19].Position = XMFLOAT3(XYZ.x, XYZ.y, -XYZ.z);
	Polygon_3d.Vertex[19].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Polygon_3d.Vertex[19].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[19].TexCoord = XMFLOAT2(1.0f, 1.0f);

	// 下面
	Polygon_3d.Vertex[20].Position = XMFLOAT3(-XYZ.x, -XYZ.y, -XYZ.z);
	Polygon_3d.Vertex[20].Normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	Polygon_3d.Vertex[20].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[20].TexCoord = XMFLOAT2(0.0f, 0.0f);

	Polygon_3d.Vertex[21].Position = XMFLOAT3(XYZ.x, -XYZ.y, -XYZ.z);
	Polygon_3d.Vertex[21].Normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	Polygon_3d.Vertex[21].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[21].TexCoord = XMFLOAT2(1.0f, 0.0f);

	Polygon_3d.Vertex[22].Position = XMFLOAT3(-XYZ.x, -XYZ.y, XYZ.z);
	Polygon_3d.Vertex[22].Normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	Polygon_3d.Vertex[22].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[22].TexCoord = XMFLOAT2(0.0f, 1.0f);

	Polygon_3d.Vertex[23].Position = XMFLOAT3(XYZ.x, -XYZ.y, XYZ.z);
	Polygon_3d.Vertex[23].Normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	Polygon_3d.Vertex[23].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[23].TexCoord = XMFLOAT2(1.0f, 1.0f);

	// 頂点バッファの設定
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

		bd.ByteWidth = sizeof(VERTEX_3D) * 4 * 6;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		// サブリソースの設定
		D3D11_SUBRESOURCE_DATA srd;
		ZeroMemory(&srd, sizeof(D3D11_SUBRESOURCE_DATA));

		srd.pSysMem = Polygon_3d.Vertex;
		srd.SysMemPitch = 0;
		srd.SysMemSlicePitch = 0;

		// 頂点バッファの生成
		hr = CRenderer::GetDevice()->CreateBuffer(&bd, &srd, &pVertexBuffer);

		if (FAILED(hr))
		{
			assert(false);
			return;
		}
	}
}

void POLYGON_3D::Draw(void)
{
	// 3Dマトリックス設定
	{
		XMMATRIX world;

		world = XMMatrixScaling(Scaling.x , Scaling.y, Scaling.z);
		world *= XMMatrixRotationRollPitchYaw( XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z) );
		world *= XMMatrixTranslation(Position.x, Position.y, Position.z);

		const auto camera01 = CManager::Get_Scene()->Get_Game_Object<CCamera>("camera");
		const auto camera02 = CManager::Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

		if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
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
				CRenderer::Set_MatrixBuffer(world, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());

				CRenderer::Set_MatrixBuffer01(*camera01.lock()->Get_Pos());

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
				CRenderer::Set_MatrixBuffer(world, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());

				CRenderer::Set_MatrixBuffer01(*camera02.lock()->Get_Pos());

				CRenderer::Set_Shader();
			}
		}
	}

	// 入力アセンブラに頂点バッファを設定.
	CRenderer::SetVertexBuffers(pVertexBuffer);

	// テクスチャの設定
	Texture->Set_Texture();

	// トポロジの設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	CRenderer::GetDeviceContext()->Draw(4 * 6, 0);
}

void POLYGON_3D::Draw_DPP(void)
{
	// 3Dマトリックス設定
	{
		XMMATRIX world;

		world = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));
		world *= XMMatrixTranslation(Position.x, Position.y, Position.z);

		const auto camera01 = CManager::Get_Scene()->Get_Game_Object<CCamera>("camera");
		const auto camera02 = CManager::Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

		if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
		{
			CRenderer::Set_MatrixBuffer(world, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());
		}
		else
		{
			CRenderer::Set_MatrixBuffer(world, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());
		}
	}

	// 入力アセンブラに頂点バッファを設定.
	CRenderer::SetVertexBuffers(pVertexBuffer);

	// トポロジの設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	CRenderer::GetDeviceContext()->Draw(4 * 6, 0);
}

void POLYGON_3D::Update(float delta_time)
{
}


void POLYGON_3D::Uninit(void)
{
	SAFE_RELEASE(pVertexBuffer);
	SAFE_DELETE(Texture);
}

//==============================
// ポジションの設定
//==============================
void POLYGON_3D::SetPosition(const XMFLOAT3 position)
{
	Position = position;
}

//==============================
// 幅と高さの設定
//==============================
void POLYGON_3D::SetXYZ(const XMFLOAT3 xyz)
{
	XYZ = xyz;
}

//==============================
// 拡大縮小の値の設定
//==============================
void POLYGON_3D::SetScaling(XMFLOAT3 scaling)
{
	Scaling = scaling;
}

//==============================
// テクスチャの設定
//==============================
void POLYGON_3D::SetTexture(const string& const file_name)
{
	Texture->Set_Texture_Name(file_name);
}

XMFLOAT3* const POLYGON_3D::Get_Position()
{
	return &Position;
}

XMFLOAT3* const POLYGON_3D::Get_Rotation()
{
	return &Rotation;
}

XMFLOAT3* const POLYGON_3D::Get_Scaling()
{
	return &Scaling;
}