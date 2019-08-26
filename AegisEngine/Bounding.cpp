#include	"Bounding.h"
#include	"texture.h"

BOUNDING_SHPERE::BOUNDING_SHPERE()
{
	Radius = 0.f;

	Cnt = 0;
}

BOUNDING_SHPERE::~BOUNDING_SHPERE()
{
	Uninit();
}

void BOUNDING_SHPERE::Init(void)
{
}

void BOUNDING_SHPERE::Draw(void)
{
	VERTEX_3D* vertex = new VERTEX_3D[Cnt];

	// 横の円
	int i;
	float angle = XM_2PI / Cnt;

	for (i = 0; i < Cnt; i++)
	{
		vertex[i].Position = XMFLOAT3((cosf(angle * i) * Radius), 0.0f, (sinf(angle * i) * Radius));
		vertex[i].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[i].TexCoord = XMFLOAT2(0.0f, 0.0f);
		vertex[i].Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	}

	// 頂点バッファの設定
	{
		HRESULT hr;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

		bd.ByteWidth = sizeof(VERTEX_3D) * Cnt;
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
		hr = CRenderer::GetDevice()->CreateBuffer(&bd, &srd, &pVertexBuffer[0]);

		if (FAILED(hr))
		{
			return;
		}
	}

	delete vertex;

	vertex = new VERTEX_3D[Cnt];

	// 縦の円
	angle = XM_2PI / Cnt;

	for (i = 0; i < Cnt; i++)
	{
		vertex[i].Position = XMFLOAT3(0.0f, (sinf(angle * i) * Radius), (cosf(angle * i) * Radius));
		vertex[i].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[i].TexCoord = XMFLOAT2(0.0f, 0.0f);
		vertex[i].Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	}

	// 頂点バッファの設定
	{
		HRESULT hr;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

		bd.ByteWidth = sizeof(VERTEX_3D) * Cnt;
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
		hr = CRenderer::GetDevice()->CreateBuffer(&bd, &srd, &pVertexBuffer[1]);

		if (FAILED(hr))
		{
			return;
		}
	}

	delete vertex;

	// 3Dマトリックス設定
	{
		XMMATRIX world = XMMatrixIdentity();

		world = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);																						// 拡大縮小
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));			// 回転
		world *= XMMatrixTranslation(Position.x, Position.y, Position.z);																				// 移動

		CRenderer::SetWorldMatrix(&world);
	}

	// トポロジの設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// 
	CRenderer::Set_Shader(SHADER_INDEX_V::NO_LIGHT, SHADER_INDEX_P::NO_TEXTURE);

	for (char i = 0; i < 2; i++)
	{
		// 入力アセンブラに頂点バッファを設定.
		CRenderer::SetVertexBuffers(pVertexBuffer[i]);

		CRenderer::GetDeviceContext()->Draw(Cnt, 0);
	}

	// トポロジの設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	CRenderer::Set_Shader();
}

void BOUNDING_SHPERE::Update(void)
{
}

void BOUNDING_SHPERE::Uninit(void)
{
}

void BOUNDING_SHPERE::Set_Radius(const float radius)
{
	Radius = radius;
}

const float BOUNDING_SHPERE::Get_Radius()
{
	return Radius;
}

void BOUNDING_SHPERE::Set_Cnt(const unsigned char& cnt)
{
	Cnt = cnt;
}






BOUNDING_AABB::BOUNDING_AABB()
{
	Radius = XMFLOAT3(1.0f, 1.0f, 1.0f);

	Vertex = new VERTEX_3D[4];
}

BOUNDING_AABB::~BOUNDING_AABB()
{
	Uninit();
}

void BOUNDING_AABB::Init(void)
{
}

void BOUNDING_AABB::Draw(void)
{
	//{
	//	Vertex[0].Position = XMFLOAT3(-Radius.x, Radius.y, -Radius.z);
	//	Vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	//	Vertex[0].Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	//	Vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	//	Vertex[1].Position = XMFLOAT3(Radius.x, Radius.y, -Radius.z);
	//	Vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	//	Vertex[1].Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	//	Vertex[1].TexCoord = XMFLOAT2(0.0f, 0.0f);

	//	Vertex[3].Position = XMFLOAT3(Radius.x, -Radius.y, -Radius.z);
	//	Vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//	Vertex[3].Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	//	Vertex[3].TexCoord = XMFLOAT2(0.0f, 0.0f);

	//	Vertex[2].Position = XMFLOAT3(-Radius.x, -Radius.y, -Radius.z);
	//	Vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//	Vertex[2].Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	//	Vertex[2].TexCoord = XMFLOAT2(0.0f, 0.0f);


	//	/*Vertex[4].Position = XMFLOAT3(-Radius.x, Radius.y, Radius.z);
	//	Vertex[4].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	//	Vertex[4].Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	//	Vertex[4].TexCoord = XMFLOAT2(0.0f, 0.0f);

	//	Vertex[5].Position = XMFLOAT3(Radius.x, Radius.y, Radius.z);
	//	Vertex[5].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	//	Vertex[5].Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	//	Vertex[5].TexCoord = XMFLOAT2(0.0f, 0.0f);

	//	Vertex[7].Position = XMFLOAT3(Radius.x, -Radius.y, Radius.z);
	//	Vertex[7].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	//	Vertex[7].Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	//	Vertex[7].TexCoord = XMFLOAT2(0.0f, 0.0f);

	//	Vertex[6].Position = XMFLOAT3(-Radius.x, -Radius.y, Radius.z);
	//	Vertex[6].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	//	Vertex[6].Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	//	Vertex[6].TexCoord = XMFLOAT2(0.0f, 0.0f);*/

	//	// 頂点バッファの設定
	//	{
	//		HRESULT hr;

	//		D3D11_BUFFER_DESC bd;
	//		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

	//		bd.ByteWidth = sizeof(VERTEX_3D) * 8;
	//		bd.Usage = D3D11_USAGE_DEFAULT;
	//		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//		bd.CPUAccessFlags = 0;
	//		bd.MiscFlags = 0;
	//		bd.StructureByteStride = 0;

	//		// サブリソースの設定
	//		D3D11_SUBRESOURCE_DATA srd;
	//		ZeroMemory(&srd, sizeof(D3D11_SUBRESOURCE_DATA));

	//		srd.pSysMem = Vertex;
	//		srd.SysMemPitch = 0;
	//		srd.SysMemSlicePitch = 0;

	//		// 頂点バッファの生成
	//		hr = CRenderer::GetDevice()->CreateBuffer(&bd, &srd, &pVertexBuffer);

	//		if (FAILED(hr))
	//		{
	//			assert(false);
	//			return;
	//		}
	//	}
	//}

	//// インデックスバッファの設定
	//{
	//	HRESULT hr;

	//	const WORD index[] = {
	//		0, 1,
	//		1, 2,
	//		2, 3,
	//		3, 0,
	//		/*4, 5,
	//		5, 6,
	//		6, 7,
	//		7, 4,
	//		0, 4,
	//		1, 5,
	//		2, 6,
	//		3, 7*/
	//	};

	//	D3D11_BUFFER_DESC ibDesc;
	//	ibDesc.ByteWidth = sizeof(WORD) * 8;
	//	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	//	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//	ibDesc.CPUAccessFlags = 0;
	//	ibDesc.MiscFlags = 0;
	//	ibDesc.StructureByteStride = 0;

	//	D3D11_SUBRESOURCE_DATA irData;
	//	irData.pSysMem = index;
	//	irData.SysMemPitch = 0;
	//	irData.SysMemSlicePitch = 0;

	//	hr = CRenderer::GetDevice()->CreateBuffer(&ibDesc, &irData, &pIndexBuffer);
	//	if (FAILED(hr))
	//	{
	//		FAILDE_ASSERT
	//	}
	//}

	//// 入力アセンブラに頂点バッファを設定.
	//CRenderer::SetVertexBuffers(pVertexBuffer);

	//CRenderer::SetIndexBuffer(pIndexBuffer);


	//// 3Dマトリックス設定
	//{
	//	XMMATRIX world = XMMatrixIdentity();

	//	world = XMMatrixScaling(Radius.x, Radius.y, Radius.z);																						// 拡大縮小
	//	world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));			// 回転
	//	world *= XMMatrixTranslation(Position.x, Position.y, Position.z);																				// 移動

	//	CRenderer::SetWorldMatrix(&world);
	//}

	//// トポロジの設定
	//CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//CRenderer::Set_Shader(SHADER_INDEX_V::NO_LIGHT, SHADER_INDEX_P::NO_TEXTURE);

	//CRenderer::DrawIndexed(8, 0, 0);

	//CRenderer::Set_Shader();
}

void BOUNDING_AABB::Update(void)
{
}

void BOUNDING_AABB::Uninit(void)
{
}

void BOUNDING_AABB::Set_Radius(const XMFLOAT3& radius)
{
	Radius = radius;
}

XMFLOAT3& const BOUNDING_AABB::Get_Radius()
{
	return Radius;
}