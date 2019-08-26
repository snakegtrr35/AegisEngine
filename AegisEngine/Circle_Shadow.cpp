#include	"Circle_Shadow.h"


static const WORD g_Index[] = {
		0, 1, 2,
		1, 3, 2,
};

CIRCLE_SHADOW::CIRCLE_SHADOW()
{
	pVertexBuffer = nullptr;
	pIndexBuffer = nullptr;
	Texture = nullptr;

	Position = XMFLOAT3(0.0f, 0.0f, 0.0f);

	WH = XMFLOAT2(1.0f, 1.0f);

	Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);

	// �e�N�X�`���̐ݒ�
	Texture = new TEXTURE(string("shadow000.jpg"));

	// ���_�o�b�t�@�̐ݒ�
	{
		HRESULT hr;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
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
		HRESULT hr;

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
}

//==============================
// �R���X�g���N�^
// position : ���S���W
// wh : ���ƍ���
//==============================
CIRCLE_SHADOW::CIRCLE_SHADOW(XMFLOAT3 position, XMFLOAT2 wh)
{
	pVertexBuffer = nullptr;
	pIndexBuffer = nullptr;
	Texture = nullptr;

	Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);

	Position = position;
	WH = wh;

	// �e�N�X�`���̐ݒ�
	Texture = new TEXTURE(string("shadow000.jpg"));

	// ���_�o�b�t�@�̐ݒ�
	{
		HRESULT hr;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
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
		HRESULT hr;

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
}

CIRCLE_SHADOW::~CIRCLE_SHADOW()
{
	SAFE_RELEASE(pVertexBuffer);
	SAFE_RELEASE(pIndexBuffer);
	SAFE_DELETE(Texture);
}

void CIRCLE_SHADOW::Init(void)
{
}

void CIRCLE_SHADOW::Draw(void)
{
	Vertex[0].Position = XMFLOAT3(-WH.x, 0.f, WH.y);
	Vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	Vertex[1].Position = XMFLOAT3(WH.x, 0.f, WH.y);
	Vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	Vertex[2].Position = XMFLOAT3(-WH.x, 0.f, -WH.y);
	Vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	Vertex[3].Position = XMFLOAT3(WH.x, 0.f, -WH.y);
	Vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	Vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	// ���_�o�b�t�@�̏�������
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		CRenderer::GetDeviceContext()->Map(pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		memcpy(msr.pData, Vertex, sizeof(VERTEX_3D) * 4); // 4���_���R�s�[
		CRenderer::GetDeviceContext()->Unmap(pVertexBuffer, 0);
	}

	// ���̓A�Z���u���ɒ��_�o�b�t�@��ݒ�.
	CRenderer::SetVertexBuffers(pVertexBuffer);

	CRenderer::SetIndexBuffer(pIndexBuffer);

	// �e�N�X�`���̐ݒ�
	Texture->Set_Texture();

	D3D11_BLEND_DESC blend_state;
	CRenderer::GetSubtractBlendState(blend_state);

	CRenderer::SetBlendState(&blend_state, true);

	// �}�e���A��������
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = COLOR(1.0f, 1.0f, 1.0f, 0.1f);
	material.Ambient = COLOR(1.0f, 1.0f, 1.0f, 0.1f);
	CRenderer::SetMaterial(material);

	// 3D�}�g���b�N�X�ݒ�
	{
		XMMATRIX world = XMMatrixIdentity();

		world = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);				// �g��k��
		world *= XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);				// ��](���[���s�b�`���E)
		world *= XMMatrixTranslation(Position.x, Position.y, Position.z);		// �ړ�
		CRenderer::SetWorldMatrix(&world);
	}

	CRenderer::DrawIndexed(6, 0, 0);

	CRenderer::SetBlendState();

	ZeroMemory(&material, sizeof(material));
	material.Diffuse = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetMaterial(material);
}

void CIRCLE_SHADOW::Update(void)
{
}

void CIRCLE_SHADOW::Uninit(void)
{
	SAFE_RELEASE(pVertexBuffer);
	SAFE_DELETE(Texture);
}

//==============================
// ���ƍ����̐ݒ�
//==============================
void CIRCLE_SHADOW::SetWH(const XMFLOAT2 wh)
{
	WH = wh;
}

//==============================
// �e�N�X�`���̐ݒ�
//==============================
void CIRCLE_SHADOW::SetTexture(const string& const file_name)
{
	Texture->Set_Texture_Name(file_name);
}