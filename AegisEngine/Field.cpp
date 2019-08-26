#include	"Field.h"
#include	"Renderer.h"

#include	"main.h"

static const WORD g_Index[] = {
		0, 1, 2,
		1, 3, 2,
};

FIELD::FIELD()
{
	pVertexBuffer = nullptr;
}

FIELD::FIELD(XMFLOAT3 position, XMFLOAT2 wh)
{
	HRESULT hr;

	pVertexBuffer = nullptr;

	Position = position;
	WH = wh;

	Vertex[0].Position = XMFLOAT3(Position.x - WH.x, Position.y, Position.y + WH.y);
	Vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	Vertex[1].Position = XMFLOAT3(Position.x + WH.x, Position.y, Position.y + WH.y);
	Vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	Vertex[2].Position = XMFLOAT3(Position.x - WH.x, Position.y, Position.y - WH.y);
	Vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	Vertex[3].Position = XMFLOAT3(Position.x + WH.x, Position.y, Position.y - WH.y);
	Vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	// ���_�o�b�t�@�̐ݒ�
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		// �T�u���\�[�X�̐ݒ�
		D3D11_SUBRESOURCE_DATA srd;
		ZeroMemory(&srd, sizeof(D3D11_SUBRESOURCE_DATA));

		srd.pSysMem = Vertex;
		srd.SysMemPitch = 0;
		srd.SysMemSlicePitch = 0;

		// ���_�o�b�t�@�̐���
		hr = CRenderer::GetDevice()->CreateBuffer(&bd, &srd, &pVertexBuffer);

		if (FAILED(hr))
		{
			return;
		}
	}

	// �C���f�b�N�X�o�b�t�@�̐ݒ�
	{
		D3D11_BUFFER_DESC ibDesc;
		ibDesc.ByteWidth = sizeof(WORD) * 6;
		ibDesc.Usage = D3D11_USAGE_DEFAULT;
		ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibDesc.CPUAccessFlags = 0;
		ibDesc.MiscFlags = 0;
		ibDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA irData;
		irData.pSysMem = g_Index;
		irData.SysMemPitch = 0;
		irData.SysMemSlicePitch = 0;

		hr = CRenderer::GetDevice()->CreateBuffer(&ibDesc, &irData, &pIndexBuffer);
		if (FAILED(hr))
		{
			return;
		}
	}

	// �e�N�X�`���̐ݒ�
	Texture = new TEXTURE(string("field004.png"));
}

FIELD::~FIELD()
{
	Uninit();
}

void FIELD::Init(void)
{
	HRESULT hr;

	Vertex[0].Position = XMFLOAT3(Position.x - WH.x, Position.y, Position.y + WH.y);
	Vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	Vertex[1].Position = XMFLOAT3(Position.x + WH.x, Position.y, Position.y + WH.y);
	Vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	Vertex[2].Position = XMFLOAT3(Position.x - WH.x, Position.y, Position.y - WH.y);
	Vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	Vertex[3].Position = XMFLOAT3(Position.x + WH.x, Position.y, Position.y - WH.y);
	Vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);


	// ���_�o�b�t�@�̐ݒ�
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		// �T�u���\�[�X�̐ݒ�
		D3D11_SUBRESOURCE_DATA srd;
		ZeroMemory(&srd, sizeof(D3D11_SUBRESOURCE_DATA));

		srd.pSysMem = Vertex;
		srd.SysMemPitch = 0;
		srd.SysMemSlicePitch = 0;

		// ���_�o�b�t�@�̐���
		hr = CRenderer::GetDevice()->CreateBuffer(&bd, &srd, &pVertexBuffer);

		if (FAILED(hr))
		{
			return;
		}
	}

	// �C���f�b�N�X�o�b�t�@�̐ݒ�
	{
		D3D11_BUFFER_DESC ibDesc;
		ibDesc.ByteWidth = sizeof(WORD) * 6;
		ibDesc.Usage = D3D11_USAGE_DEFAULT;
		ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibDesc.CPUAccessFlags = 0;
		ibDesc.MiscFlags = 0;
		ibDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA irData;
		irData.pSysMem = g_Index;
		irData.SysMemPitch = 0;
		irData.SysMemSlicePitch = 0;

		hr = CRenderer::GetDevice()->CreateBuffer(&ibDesc, &irData, &pIndexBuffer);
		if (FAILED(hr))
		{
			return;
		}
	}

	// �e�N�X�`���̐ݒ�
	Texture = new TEXTURE(string("field004.png"));

}

void FIELD::Draw(void)
{
	HRESULT hr;

	Vertex[0].Position = XMFLOAT3(Position.x - WH.x, Position.y, Position.y + WH.y);
	Vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	Vertex[1].Position = XMFLOAT3(Position.x + WH.x, Position.y, Position.y + WH.y);
	Vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	Vertex[2].Position = XMFLOAT3(Position.x - WH.x, Position.y, Position.y - WH.y);
	Vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	Vertex[3].Position = XMFLOAT3(Position.x + WH.x, Position.y, Position.y - WH.y);
	Vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	// ���_�o�b�t�@�̐ݒ�
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		// �T�u���\�[�X�̐ݒ�
		D3D11_SUBRESOURCE_DATA srd;
		ZeroMemory(&srd, sizeof(D3D11_SUBRESOURCE_DATA));

		srd.pSysMem = Vertex;
		srd.SysMemPitch = 0;
		srd.SysMemSlicePitch = 0;

		// ���_�o�b�t�@�̐���
		hr = CRenderer::GetDevice()->CreateBuffer(&bd, &srd, &pVertexBuffer);

		if (FAILED(hr))
		{
			return;
		}
	}

	// �C���f�b�N�X�o�b�t�@�̐ݒ�
	{
		D3D11_BUFFER_DESC ibDesc;
		ibDesc.ByteWidth = sizeof(WORD) * 6;
		ibDesc.Usage = D3D11_USAGE_DEFAULT;
		ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibDesc.CPUAccessFlags = 0;
		ibDesc.MiscFlags = 0;
		ibDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA irData;
		irData.pSysMem = g_Index;
		irData.SysMemPitch = 0;
		irData.SysMemSlicePitch = 0;

		hr = CRenderer::GetDevice()->CreateBuffer(&ibDesc, &irData, &pIndexBuffer);
		if (FAILED(hr))
		{
			return;
		}
	}

	// ���̓A�Z���u���ɒ��_�o�b�t�@��ݒ�.
	CRenderer::SetVertexBuffers(pVertexBuffer);

	CRenderer::SetIndexBuffer(pIndexBuffer);

	Texture->Set_Texture();//

	// 3D�}�g���b�N�X�ݒ�
	{
		XMMATRIX world;

		world = XMMatrixScaling(1.0f, 1.0f, 1.0f);						// �g��k��
		world *= XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);		// ��](���[���s�b�`���E)
		world *= XMMatrixTranslation(0.0f, 0.0f, 0.0f);					// �ړ�
		CRenderer::SetWorldMatrix(&world);
	}

	CRenderer::DrawIndexed(6, 0, 0);
}


void FIELD::Update(void)
{

}


void FIELD::Uninit(void)
{
	SAFE_RELEASE(pVertexBuffer);

	SAFE_DELETE(Texture);
}

//==============================
// �|�W�V�����̐ݒ�
//==============================
void FIELD::SetPosition(const XMFLOAT3 position)
{
	Position = position;
}

//==============================
// ���ƍ����̐ݒ�
//==============================
void FIELD::SetWH(const XMFLOAT2 wh)
{
	WH = wh;
}

//==============================
// �e�N�X�`���̐ݒ�
//==============================
void FIELD::SetTexture(const string& const file_name)
{
	Texture->Set_Texture_Name(file_name);
}