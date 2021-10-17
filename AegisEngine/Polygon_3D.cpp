#include	"GameObject.h"
#include	"Polygon_3D.h"
#include	"texture.h"
#include	"Input.h"
#include	"camera.h"
#include	"Debug_Camera.h"
#include	"manager.h"
#include	"ShadowMap.h"
#include	"Scene.h"

using namespace Aegis;

static float roll = 0.0f;
static float pichi = 0.0f;
static float yaw = 0.0f;

POLYGON_3D::POLYGON_3D()
{
	pVertexBuffer = nullptr;
	Texture.reset(nullptr);

	XYZ = Vector3(0.5f, 0.5f, 0.5f);

	// テクスチャの設定
	//Texture = new TEXTURE(string("field004.png"));
	Texture = make_unique<TEXTURE>(string("field004.png"));
}

POLYGON_3D::POLYGON_3D(Vector3 position, Vector3 xyz)
{
	pVertexBuffer = nullptr;
	Texture = nullptr;

	Get_Transform().Set_Position(position);
	XYZ = xyz;

	// テクスチャの設定
	//Texture = new TEXTURE(string("field004.png"));
	Texture = make_unique<TEXTURE>(string("field004.png"));
}

POLYGON_3D::~POLYGON_3D()
{
	SAFE_RELEASE(pVertexBuffer);
	Texture.reset(nullptr);
}

void POLYGON_3D::Init(void)
{
	HRESULT hr;

	POLYGOM Polygon_3d;

	// 正面
	Polygon_3d.Vertex[0].Position = Vector3(-XYZ.x, XYZ.y, -XYZ.z);
	Polygon_3d.Vertex[0].Normal = Vector3(0.0f, 0.0f, -1.0f);
	Polygon_3d.Vertex[0].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[0].TexCoord = Vector2(0.0f, 0.0f);

	Polygon_3d.Vertex[1].Position = Vector3(XYZ.x, XYZ.y, -XYZ.z);
	Polygon_3d.Vertex[1].Normal = Vector3(0.0f, 0.0f, -1.0f);
	Polygon_3d.Vertex[1].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[1].TexCoord = Vector2(1.0f, 0.0f);

	Polygon_3d.Vertex[2].Position = Vector3(-XYZ.x, -XYZ.y, -XYZ.z);
	Polygon_3d.Vertex[2].Normal = Vector3(0.0f, 0.0f, -1.0f);
	Polygon_3d.Vertex[2].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[2].TexCoord = Vector2(0.0f, 1.0f);

	Polygon_3d.Vertex[3].Position = Vector3(XYZ.x, -XYZ.y, -XYZ.z);
	Polygon_3d.Vertex[3].Normal = Vector3(0.0f, 0.0f, -1.0f);
	Polygon_3d.Vertex[3].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[3].TexCoord = Vector2(1.0f, 1.0f);

	// 裏面
	Polygon_3d.Vertex[4].Position = Vector3(XYZ.x, XYZ.y, XYZ.z);
	Polygon_3d.Vertex[4].Normal = Vector3(0.0f, 0.0f, 1.0f);
	Polygon_3d.Vertex[4].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[4].TexCoord = Vector2(0.0f, 0.0f);

	Polygon_3d.Vertex[5].Position = Vector3(-XYZ.x, XYZ.y, XYZ.z);
	Polygon_3d.Vertex[5].Normal = Vector3(0.0f, 0.0f, 1.0f);
	Polygon_3d.Vertex[5].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[5].TexCoord = Vector2(1.0f, 0.0f);

	Polygon_3d.Vertex[6].Position = Vector3(XYZ.x, -XYZ.y, XYZ.z);
	Polygon_3d.Vertex[6].Normal = Vector3(0.0f, 0.0f, 1.0f);
	Polygon_3d.Vertex[6].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[6].TexCoord = Vector2(0.0f, 1.0f);

	Polygon_3d.Vertex[7].Position = Vector3(-XYZ.x, -XYZ.y, XYZ.z);
	Polygon_3d.Vertex[7].Normal = Vector3(0.0f, 0.0f, 1.0f);
	Polygon_3d.Vertex[7].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[7].TexCoord = Vector2(1.0f, 1.0f);

	// 右面
	Polygon_3d.Vertex[8].Position = Vector3(XYZ.x, XYZ.y, -XYZ.z);
	Polygon_3d.Vertex[8].Normal = Vector3(1.0f, 0.0f, 0.0f);
	Polygon_3d.Vertex[8].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[8].TexCoord = Vector2(0.0f, 0.0f);

	Polygon_3d.Vertex[9].Position = Vector3(XYZ.x, XYZ.y, XYZ.z);
	Polygon_3d.Vertex[9].Normal = Vector3(1.0f, 0.0f, 0.0f);
	Polygon_3d.Vertex[9].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[9].TexCoord = Vector2(1.0f, 0.0f);

	Polygon_3d.Vertex[10].Position = Vector3(XYZ.x, -XYZ.y, -XYZ.z);
	Polygon_3d.Vertex[10].Normal = Vector3(1.0f, 0.0f, 0.0f);
	Polygon_3d.Vertex[10].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[10].TexCoord = Vector2(0.0f, 1.0f);

	Polygon_3d.Vertex[11].Position = Vector3(XYZ.x, -XYZ.y, XYZ.z);
	Polygon_3d.Vertex[11].Normal = Vector3(1.0f, 0.0f, 0.0f);
	Polygon_3d.Vertex[11].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[11].TexCoord = Vector2(1.0f, 1.0f);

	// 左面
	Polygon_3d.Vertex[12].Position = Vector3(-XYZ.x, XYZ.y, XYZ.z);
	Polygon_3d.Vertex[12].Normal = Vector3(-1.0f, 0.0f, 0.0f);
	Polygon_3d.Vertex[12].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[12].TexCoord = Vector2(0.0f, 0.0f);

	Polygon_3d.Vertex[13].Position = Vector3(-XYZ.x, XYZ.y, -XYZ.z);
	Polygon_3d.Vertex[13].Normal = Vector3(-1.0f, 0.0f, 0.0f);
	Polygon_3d.Vertex[13].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[13].TexCoord = Vector2(1.0f, 0.0f);

	Polygon_3d.Vertex[14].Position = Vector3(-XYZ.x, -XYZ.y, XYZ.z);
	Polygon_3d.Vertex[14].Normal = Vector3(-1.0f, 0.0f, 0.0f);
	Polygon_3d.Vertex[14].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[14].TexCoord = Vector2(0.0f, 1.0f);

	Polygon_3d.Vertex[15].Position = Vector3(-XYZ.x, -XYZ.y, -XYZ.z);
	Polygon_3d.Vertex[15].Normal = Vector3(-1.0f, 0.0f, 0.0f);
	Polygon_3d.Vertex[15].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[15].TexCoord = Vector2(1.0f, 1.0f);

	// 上面
	Polygon_3d.Vertex[16].Position = Vector3(-XYZ.x, XYZ.y, XYZ.z);
	Polygon_3d.Vertex[16].Normal = Vector3(0.0f, 1.0f, 0.0f);
	Polygon_3d.Vertex[16].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[16].TexCoord = Vector2(0.0f, 0.0f);

	Polygon_3d.Vertex[17].Position = Vector3(XYZ.x, XYZ.y, XYZ.z);
	Polygon_3d.Vertex[17].Normal = Vector3(0.0f, 1.0f, 0.0f);
	Polygon_3d.Vertex[17].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[17].TexCoord = Vector2(1.0f, 0.0f);

	Polygon_3d.Vertex[18].Position = Vector3(-XYZ.x, XYZ.y, -XYZ.z);
	Polygon_3d.Vertex[18].Normal = Vector3(0.0f, 1.0f, 0.0f);
	Polygon_3d.Vertex[18].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[18].TexCoord = Vector2(0.0f, 1.0f);

	Polygon_3d.Vertex[19].Position = Vector3(XYZ.x, XYZ.y, -XYZ.z);
	Polygon_3d.Vertex[19].Normal = Vector3(0.0f, 1.0f, 0.0f);
	Polygon_3d.Vertex[19].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[19].TexCoord = Vector2(1.0f, 1.0f);

	// 下面
	Polygon_3d.Vertex[20].Position = Vector3(-XYZ.x, -XYZ.y, -XYZ.z);
	Polygon_3d.Vertex[20].Normal = Vector3(0.0f, -1.0f, 0.0f);
	Polygon_3d.Vertex[20].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[20].TexCoord = Vector2(0.0f, 0.0f);

	Polygon_3d.Vertex[21].Position = Vector3(XYZ.x, -XYZ.y, -XYZ.z);
	Polygon_3d.Vertex[21].Normal = Vector3(0.0f, -1.0f, 0.0f);
	Polygon_3d.Vertex[21].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[21].TexCoord = Vector2(1.0f, 0.0f);

	Polygon_3d.Vertex[22].Position = Vector3(-XYZ.x, -XYZ.y, XYZ.z);
	Polygon_3d.Vertex[22].Normal = Vector3(0.0f, -1.0f, 0.0f);
	Polygon_3d.Vertex[22].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[22].TexCoord = Vector2(0.0f, 1.0f);

	Polygon_3d.Vertex[23].Position = Vector3(XYZ.x, -XYZ.y, XYZ.z);
	Polygon_3d.Vertex[23].Normal = Vector3(0.0f, -1.0f, 0.0f);
	Polygon_3d.Vertex[23].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Polygon_3d.Vertex[23].TexCoord = Vector2(1.0f, 1.0f);

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
		XMMATRIX world(XMMatrixIdentity());

		Vector3 position = *Get_Transform().Get_Position();
		Vector3 rotate = *Get_Transform().Get_Rotation();
		Vector3 scale = *Get_Transform().Get_Scaling();

		XMMATRIX matrix = XMMatrixScaling(scale.x, scale.y, scale.z);
		matrix *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotate.x), XMConvertToRadians(rotate.y), XMConvertToRadians(rotate.z));
		matrix *= XMMatrixTranslation(position.x, position.y, position.z);

		const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
		const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

		if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
		{
			// シャドウマップ用の描画か?
			if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
			{
				XMMATRIX view = CManager::Get_Instance()->Get_ShadowMap()->Get_View();
				XMMATRIX proj = CManager::Get_Instance()->Get_ShadowMap()->Get_Plojection();

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
			if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
			{
				XMMATRIX view = CManager::Get_Instance()->Get_ShadowMap()->Get_View();
				XMMATRIX proj = CManager::Get_Instance()->Get_ShadowMap()->Get_Plojection();

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

	GAME_OBJECT::Draw();
}

void POLYGON_3D::Draw_DPP(void)
{
	// 3Dマトリックス設定
	{
		XMMATRIX world(XMMatrixIdentity());

		Vector3 position = *Get_Transform().Get_Position();
		Vector3 rotate = *Get_Transform().Get_Rotation();
		Vector3 scale = *Get_Transform().Get_Scaling();

		XMMATRIX matrix = XMMatrixScaling(scale.x, scale.y, scale.z);
		matrix *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotate.x), XMConvertToRadians(rotate.y), XMConvertToRadians(rotate.z));
		matrix *= XMMatrixTranslation(position.x, position.y, position.z);

		const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
		const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

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
	GAME_OBJECT::Update(delta_time);
}


void POLYGON_3D::Uninit(void)
{
	SAFE_RELEASE(pVertexBuffer);
	Texture.reset(nullptr);
}

//==============================
// ポジションの設定
//==============================
void POLYGON_3D::SetPosition(Vector3& position)
{
	Get_Transform().Set_Position(position);
}

//==============================
// 幅と高さの設定
//==============================
void POLYGON_3D::SetXYZ(const Vector3 xyz)
{
	XYZ = xyz;
}

//==============================
// 拡大縮小の値の設定
//==============================
void POLYGON_3D::SetScaling(Vector3& scaling)
{
	Get_Transform().Set_Scaling(scaling);
}

//==============================
// テクスチャの設定
//==============================
void POLYGON_3D::SetTexture(const string& const file_name)
{
	Texture->Set_Texture_Name(file_name);
}

Vector3* const POLYGON_3D::Get_Position()
{
	return Get_Transform().Get_Position();
}

Vector3* const POLYGON_3D::Get_Rotation()
{
	return Get_Transform().Get_Rotation();
}

Vector3* const POLYGON_3D::Get_Scaling()
{
	return Get_Transform().Get_Scaling();
}