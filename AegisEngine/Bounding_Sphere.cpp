#include	"Bounding_Sphere.h"
#include	"camera.h"
#include	"Debug_Camera.h"
#include	"manager.h"
#include	"Scene.h"
#include	"ShadowMap.h"

void BOUNDING_SHPERE::Init()
{
	{
		XMFLOAT3 pos = *Owner.lock()->Get_Position();

		Sphere = BoundingSphere(XMFLOAT3(0.f, 0.f, 0.f), Radius);

		XMMATRIX matrix;

		matrix = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
		matrix *= XMMatrixTranslation(Position.x + pos.x, Position.y + pos.y, Position.z + pos.z);

		Sphere.Transform(Sphere, matrix);
	}

	// 頂点バッファの設定
	if (nullptr == pVertexBuffer.get())
	{
		const UINT cnt = 10 * (UINT)Radius;

		vector<VERTEX_3D> vertex;
		vertex.reserve(cnt);

		for (int i = 0; i < cnt; i++)
		{
			vertex.emplace_back(VERTEX_3D());
		}

		const float angle = XM_2PI / cnt;

		for (int i = 0; i < cnt; i++)
		{
			vertex[i].Position = XMFLOAT3(cosf(angle * i) * Radius, sinf(angle * i) * Radius, 0.0f);
			vertex[i].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
			vertex[i].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
			vertex[i].TexCoord = XMFLOAT2(0.0f, 0.0f);
		}

		// 頂点バッファの設定
		{
			ID3D11Buffer* buffer;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));

			bd.ByteWidth = sizeof(VERTEX_3D) * cnt;
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd;
			sd.pSysMem = vertex.data();
			sd.SysMemPitch = 0;
			sd.SysMemSlicePitch = 0;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			pVertexBuffer.reset(buffer);
		}
	}

	// インデックスバッファの設定
	if (nullptr == pIndexBuffer.get())
	{
		const UINT cnt = 10 * (UINT)Radius;

		IndexNum = cnt * 2;
		vector<WORD> index_array;
		index_array.reserve(IndexNum);

		for (int i = 0; i < IndexNum; i++)
		{
			index_array.emplace_back(0);
		}

		for (int i = 0; i < cnt; i++)
		{
			index_array[i * 2] = (WORD)i;
			index_array[i * 2 + 1] = (WORD)((i + 1) % cnt);
		}

		// インデックスバッファの設定
		{
			ID3D11Buffer* buffer;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));

			bd.ByteWidth = sizeof(WORD) * IndexNum;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = index_array.data();
			sd.SysMemPitch = 0;
			sd.SysMemSlicePitch = 0;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			pIndexBuffer.reset(buffer);
		}
	}
}

void BOUNDING_SHPERE::Draw()
{
	//if (false == CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
	{
		// 入力アセンブラに頂点バッファを設定
		CRenderer::SetVertexBuffers(pVertexBuffer.get());

		// 入力アセンブラにインデックスバッファを設定
		CRenderer::SetIndexBuffer(pIndexBuffer.get());

		// トポロジの設定
		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		CRenderer::Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_TEXTURE);

		Draw_Ring(XMFLOAT3(0.f, 0.f, 0.f));

		Draw_Ring(XMFLOAT3(0.f, 90.0f, 0.f));

		Draw_Ring(XMFLOAT3(90.0f, 0.f, 0.f));

		CRenderer::Set_Shader();
	}
}

void BOUNDING_SHPERE::Update(float delta_time)
{
	OverWrite();

	{
		XMFLOAT3 pos = *Owner.lock()->Get_Position();

		Sphere = BoundingSphere(XMFLOAT3(0.f, 0.f, 0.f), Radius);

		XMMATRIX matrix;

		matrix = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
		matrix *= XMMatrixTranslation(Position.x + pos.x, Position.y + pos.y, Position.z + pos.z);

		Sphere.Transform(Sphere, matrix);
	}
}

void BOUNDING_SHPERE::Uninit()
{
	pVertexBuffer.reset(nullptr);
	pIndexBuffer.reset(nullptr);
}

void BOUNDING_SHPERE::Set_Radius(const float radius)
{
	Radius = max(radius, 0.01f);

	//Create_Buffer();
}

const float BOUNDING_SHPERE::Get_Radius()
{
	return Radius;
}

const BoundingSphere& BOUNDING_SHPERE::Get_Collition()
{
	return Sphere;
}

void BOUNDING_SHPERE::Draw_Ring(const XMFLOAT3& rotation)
{
	// 3Dマトリックス設定
	{
		XMFLOAT3 pos = *Owner.lock()->Get_Position();

		XMMATRIX world= XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.z));
		world *= XMMatrixTranslation(Position.x + pos.x, Position.y + pos.y, Position.z + pos.z);

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

	CRenderer::GetDeviceContext()->DrawIndexed(IndexNum, 0, 0);
}

void BOUNDING_SHPERE::OverWrite()
{
	Color = Default_Color;

	// 頂点バッファの設定
	{
		const UINT cnt = 10 * (UINT)Radius;

		vector<VERTEX_3D> vertex;
		vertex.reserve(cnt);

		for (int i = 0; i < cnt; i++)
		{
			vertex.emplace_back(VERTEX_3D());
		}

		const float angle = XM_2PI / cnt;

		for (int i = 0; i < cnt; i++)
		{
			vertex[i].Position = XMFLOAT3(cosf(angle * i) * Radius, sinf(angle * i) * Radius, 0.0f);
			vertex[i].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
			vertex[i].Diffuse = XMFLOAT4(Color.r, Color.g, Color.b, Color.a);
			vertex[i].TexCoord = XMFLOAT2(0.0f, 0.0f);
		}

		// 頂点バッファの設定
		{
			ID3D11Buffer* buffer;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));

			bd.ByteWidth = sizeof(VERTEX_3D) * cnt;
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd;
			sd.pSysMem = vertex.data();
			sd.SysMemPitch = 0;
			sd.SysMemSlicePitch = 0;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			pVertexBuffer.reset(buffer);
		}
	}

	// インデックスバッファの設定
	{
		const UINT cnt = 10 * (UINT)Radius;

		IndexNum = cnt * 2;
		vector<WORD> index_array;
		index_array.reserve(IndexNum);

		for (int i = 0; i < IndexNum; i++)
		{
			index_array.emplace_back(0);
		}

		for (int i = 0; i < cnt; i++)
		{
			index_array[i * 2] = (WORD)i;
			index_array[i * 2 + 1] = (WORD)((i + 1) % cnt);
		}

		// インデックスバッファの設定
		{
			ID3D11Buffer* buffer;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));

			bd.ByteWidth = sizeof(WORD) * IndexNum;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = index_array.data();
			sd.SysMemPitch = 0;
			sd.SysMemSlicePitch = 0;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			pIndexBuffer.reset(buffer);
		}
	}
}

#include	"imgui/imgui.h"

void BOUNDING_SHPERE::Draw_Inspector()
{
	ImGui::Text((char*)u8"コリジョン(球)");

	COMPONENT::Draw_Inspector();

	float position[3] = { Position.x, Position.y, Position.z };
	float scale[3] = { Scaling.x, Scaling.y, Scaling.z };

	ImGui::DragFloat3("Position##SPHERE", position, 0.01f);
	ImGui::DragFloat3("Scaling", scale, 0.01f);
	ImGui::DragFloat("Radius", &Radius, 0.01f, 0.01f, 1000.0f);

	Position = XMFLOAT3(position[0], position[1], position[2]);
	Scaling = XMFLOAT3(scale[0], scale[1], scale[2]);

	OverWrite();
}