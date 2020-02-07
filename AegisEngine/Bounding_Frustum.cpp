#include	"Bounding_Frustum.h"
#include	"camera.h"
#include	"Debug_Camera.h"
#include	"manager.h"
#include	"Scene.h"
#include	"ShadowMap.h"

void BOUNDING_FRUSTUM::Init()
{
	{
		BoundingFrustum::CreateFromMatrix(Frustum, XMMatrixPerspectiveFovLH(XMConvertToRadians(80.0f + 35.0f), float(SCREEN_WIDTH / SCREEN_HEIGHT), 0.1f, 1000.0f));
		Frustum.Origin.z = 0.0f;
		{
			XMMATRIX matrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));
			matrix *= XMMatrixTranslation(Position.x, Position.y, Position.z);

			Frustum.Transform(Frustum, matrix);
		}
	}

	Color = COLOR(1.f, 0.f, 0.f, 1.f);

	if (nullptr != pVertexBuffer.get())
		pVertexBuffer.reset(nullptr);

	if (nullptr != pIndexBuffer.get())
		pIndexBuffer.reset(nullptr);

	// 頂点バッファの設定
	if (nullptr == pVertexBuffer.get())
	{
		const char VertexNum = 8;

		VERTEX_3D Vertex[VertexNum];

		XMFLOAT3 corners[BoundingFrustum::CORNER_COUNT];
		Frustum.GetCorners(corners);

		Vertex[0].Position = corners[1];
		Vertex[1].Position = corners[0];
		Vertex[2].Position = corners[2];
		Vertex[3].Position = corners[3];
		Vertex[4].Position = corners[5];
		Vertex[5].Position = corners[4];
		Vertex[6].Position = corners[6];
		Vertex[7].Position = corners[7];

		for (char i = 0; i < VertexNum; i++)
		{
			Vertex[i].Normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
			Vertex[i].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
			Vertex[i].TexCoord = XMFLOAT2(0.0f, 0.0f);
		}

		// 頂点バッファの設定
		{
			ID3D11Buffer* buffer;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));

			bd.ByteWidth = sizeof(VERTEX_3D) * VertexNum;
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd;
			sd.pSysMem = Vertex;
			sd.SysMemPitch = 0;
			sd.SysMemSlicePitch = 0;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			pVertexBuffer.reset(buffer);
		}
	}

	// インデックスバッファの設定
	if (nullptr == pIndexBuffer.get())
	{
		ID3D11Buffer* buffer;

		const WORD index[24] = {
		0, 1,
		1, 3,
		3, 2,
		2, 0,

		0, 4,

		4, 5,
		5, 7,
		7, 6,
		6, 4,

		5, 1,
		3, 7,
		6, 2
		};

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.ByteWidth = sizeof(WORD) * 24;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

		pIndexBuffer.reset(buffer);
	}
}

void BOUNDING_FRUSTUM::Draw()
{
	if (false == CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
	{
		// 入力アセンブラに頂点バッファを設定
		CRenderer::SetVertexBuffers(pVertexBuffer.get());

		// 入力アセンブラにインデックスバッファを設定
		CRenderer::SetIndexBuffer(pIndexBuffer.get());

		// トポロジの設定
		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		CRenderer::Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_TEXTURE);

		{
			// 3Dマトリックス設定
			{
				XMMATRIX world = XMMatrixIdentity();

				world = XMMatrixIdentity();
				//world *= XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
				//world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));
				//world *= XMMatrixTranslation(Position.x, Position.y, Position.z);

				const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
				const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

				if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
				{
					CRenderer::Set_MatrixBuffer(world, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());

					CRenderer::Set_MatrixBuffer01(*camera01.lock()->Get_Pos());
				}
				else
				{
					CRenderer::Set_MatrixBuffer(world, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());

					CRenderer::Set_MatrixBuffer01(*camera02.lock()->Get_Pos());
				}
			}

			CRenderer::GetDeviceContext()->DrawIndexed(24, 0, 0);
		}

		CRenderer::Set_Shader();
	}
}

void BOUNDING_FRUSTUM::Update(float delta_time)
{
	BoundingFrustum::CreateFromMatrix(Frustum, XMMatrixPerspectiveFovLH(XMConvertToRadians(80.0f + 35.0f), float(SCREEN_WIDTH / SCREEN_HEIGHT), 0.01f, 1000.0f));
	Frustum.Origin.z = 0.0f;

	XMFLOAT3 r;

	const auto camera = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

	//if (nullptr != camera)
	{
		//XMStoreFloat3(&r, XMLoadFloat3(camera->Get_Rotation()) );

		Position.x = camera.lock()->Get_Position()->x;
		Position.y = camera.lock()->Get_Position()->y;
		Position.z = camera.lock()->Get_Position()->z;

		XMFLOAT3 vec;
		XMStoreFloat3(&vec, *camera.lock()->Get_Front());

		/*if (vec.z >= 0.0f)
		{
			r.x = -atan(vec.y / vec.z) - XM_PI;
		}
		else
		{
			r.x = -atan(vec.y / vec.z);
		}*/

		/*if (vec.z >= 0.0f)
		{
			r.y = atan(vec.x / vec.z);
		}
		else
		{
			r.y = atan(vec.x / vec.z) + XM_PI;
		}*/

		Rotation = *camera.lock()->Get_Rotation();
	}

	XMMATRIX matrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));
	//XMMATRIX matrix = XMMatrixRotationRollPitchYaw(Rotation.x, Rotation.y, Rotation.z);
	matrix *= XMMatrixTranslation(Position.x, Position.y, Position.z);

	Frustum.Transform(Frustum, matrix);

	OverWrite();
}

void BOUNDING_FRUSTUM::Uninit()
{
	pVertexBuffer.reset(nullptr);
	pIndexBuffer.reset(nullptr);
}

void BOUNDING_FRUSTUM::OverWrite()
{
	// 頂点バッファの設定
	{
		const char VertexNum = 8;

		VERTEX_3D Vertex[VertexNum];

		XMFLOAT3 corners[BoundingFrustum::CORNER_COUNT];
		Frustum.GetCorners(corners);

		Vertex[0].Position = corners[1];
		Vertex[1].Position = corners[0];
		Vertex[2].Position = corners[2];
		Vertex[3].Position = corners[3];
		Vertex[4].Position = corners[5];
		Vertex[5].Position = corners[4];
		Vertex[6].Position = corners[6];
		Vertex[7].Position = corners[7];

		for (char i = 0; i < VertexNum; i++)
		{
			Vertex[i].Normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
			Vertex[i].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
			Vertex[i].TexCoord = XMFLOAT2(0.0f, 0.0f);
		}

		// 頂点バッファの設定
		{
			ID3D11Buffer* buffer;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));

			bd.ByteWidth = sizeof(VERTEX_3D) * VertexNum;
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd;
			sd.pSysMem = Vertex;
			sd.SysMemPitch = 0;
			sd.SysMemSlicePitch = 0;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			pVertexBuffer.reset(buffer);
		}
	}

	// インデックスバッファの設定
	{
		ID3D11Buffer* buffer;

		const WORD index[24] = {
		0, 1,
		1, 3,
		3, 2,
		2, 0,

		0, 4,

		4, 5,
		5, 7,
		7, 6,
		6, 4,

		5, 1,
		3, 7,
		6, 2
		};

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.ByteWidth = sizeof(WORD) * 24;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

		pIndexBuffer.reset(buffer);
	}
}