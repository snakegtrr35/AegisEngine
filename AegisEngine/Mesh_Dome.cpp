//***********************************************************************************************
//
//  �X�J�C�h�[�� [skydome.cpp]
//
//***********************************************************************************************

//***********************************************************************************************
//  �C���N���[�h�t�@�C��
//***********************************************************************************************
#include	"main.h"
#include	"renderer.h"
#include	"texture.h"
#include	"Mesh_Dome.h"


//***********************************************************************************************
//  ������
//***********************************************************************************************
void MESH_DOOM::Init()
{
	int cornerNum = (int)Radius * 10;

	// ���_�o�b�t�@�̊m��
	VertexNum = (cornerNum + 1) * (cornerNum / 4 + 1);
	VERTEX_3D* vertexArray = new VERTEX_3D[VertexNum];

	// �C���f�b�N�X�o�b�t�@�̊m��
	IndexNum = (2 + (cornerNum * 2)) * cornerNum / 4 + (cornerNum / 4 - 1) * 2;
	unsigned short* indexArray = new unsigned short[IndexNum];

	// ���_�o�b�t�@�ւ̒��_���̏�������
	float theta = 0.0f, phi = 0.0f;
	float addAngle = 2.0f * XM_PI / (float)cornerNum;
	for (int y = 0; y < cornerNum / 4 + 1; y++)
	{
		phi = 0.0f;
		for (int x = 0; x < cornerNum + 1; x++)
		{
			vertexArray[x + (cornerNum + 1) * y].Position	= XMFLOAT3(Radius * sinf(theta) * cosf(phi), Radius * cosf(theta), Radius * sinf(theta) * sinf(phi));
			vertexArray[x + (cornerNum + 1) * y].Normal		= XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertexArray[x + (cornerNum + 1) * y].Diffuse	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertexArray[x + (cornerNum + 1) * y].TexCoord	= XMFLOAT2(-phi / (XM_2PI), -theta / (XM_PI * 0.25f) - 1.0f);

			phi -= addAngle;
		}

		theta -= addAngle;
	}

	// ���_�C���f�b�N�X�o�b�t�@�֒��_�C���f�b�N�X�̏�������
	int indexNum = 0;
	int indexFlapX = cornerNum - 1;
	int indexFlapY = cornerNum / 4 - 1;
	for (int y = 0; y < cornerNum / 4; y++)
	{
		for (int x = 0; x < cornerNum + 1; x++)
		{
			indexArray[indexNum++] = (cornerNum + 1) * (y + 1) + x;
			indexArray[indexNum++] = (cornerNum + 1) * y + x;

			// �k�ރ|���S���p
			if (x > indexFlapX && y < indexFlapY)
			{
				indexArray[indexNum++] = (cornerNum + 1) * y + (cornerNum + 1) - 1;
				indexArray[indexNum++] = (cornerNum + 1) * (y + 1 + 1) + 0;
			}
		}
	}
	

	// ���_�o�b�t�@����
	if(nullptr == VertexBuffer.get())
	{
		ID3D11Buffer* pVB = nullptr;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * VertexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = vertexArray;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &pVB);

		VertexBuffer.reset(pVB);
	}

	// �C���f�b�N�X�o�b�t�@����
	if (nullptr == IndexBuffer.get())
	{
		ID3D11Buffer* pIB = nullptr;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned short) * IndexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = indexArray;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &pIB);

		IndexBuffer.reset(pIB);
	}

	// ���������
	SAFE_DELETE_ARRAY(vertexArray);
	SAFE_DELETE_ARRAY(indexArray);

	// �g�����X�t�H�[��������
	Position	= XMFLOAT3(0.0f, 0.0f, 0.0f);
	Rotation	= XMFLOAT3(0.0f, 0.0f, 0.0f);
	Scaling		= XMFLOAT3(1.0f, 1.0f, 1.0f);

	// �e�N�X�`���̐ݒ�
	Texture.reset(new TEXTURE(string("sky.jpg")));
}

//***********************************************************************************************
//  ���
//***********************************************************************************************
void MESH_DOOM::Uninit()
{
	VertexBuffer.reset();
	IndexBuffer.reset();
	Texture.reset();
}

//***********************************************************************************************
//  �X�V
//***********************************************************************************************
void MESH_DOOM::Update()
{


}

//***********************************************************************************************
//  �`��
//***********************************************************************************************
void MESH_DOOM::Draw()
{
	{
		XMMATRIX world;

		world = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);																						// �g��k��
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));			// ��]
		world *= XMMatrixTranslation(Position.x, Position.y, Position.z);																				// �ړ�

		CRenderer::SetWorldMatrix(&world);
	}

	CRenderer::SetVertexBuffers(VertexBuffer.get());
	CRenderer::SetIndexBuffer(IndexBuffer.get());

	// �g�|���W�ݒ�
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);	

	Texture.get()->Set_Texture();

	// �|���S���`��
	CRenderer::GetDeviceContext()->DrawIndexed(IndexNum, 0, 0);

}
