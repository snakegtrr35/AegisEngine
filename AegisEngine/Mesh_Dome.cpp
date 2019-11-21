#include	"renderer.h"
#include	"texture.h"
#include	"Mesh_Dome.h"

#include	"manager.h"
#include	"Scene.h"

void MESH_DOOM::Init()
{
	int cornerNum = 256;

	// ���_�o�b�t�@�̊m��
	VertexNum = (cornerNum + 1) * (cornerNum / 2 + 1);
	VERTEX_3D* vertexArray = new VERTEX_3D[VertexNum];

	// �C���f�b�N�X�o�b�t�@�̊m��
	IndexNum = (2 + (cornerNum * 2)) * cornerNum / 2 + (cornerNum / 2 - 1) * 2;
	unsigned short* indexArray = new unsigned short[IndexNum];

	// ���_�o�b�t�@�ւ̒��_���̏�������
	float theta = 0.0f, phi = 0.0f;
	float addAngle = XM_2PI / (float)cornerNum;
	for (int y = 0; y < cornerNum / 2 + 1; y++)
	{
		phi = 0.0f;
		for (int x = 0; x < cornerNum + 1; x++)
		{
			vertexArray[x + (cornerNum + 1) * y].Position	= XMFLOAT3(Radius * sinf(theta) * cosf(phi), Radius * cosf(theta), Radius * sinf(theta) * sinf(phi));
			vertexArray[x + (cornerNum + 1) * y].Normal		= XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertexArray[x + (cornerNum + 1) * y].Diffuse	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertexArray[x + (cornerNum + 1) * y].TexCoord	= XMFLOAT2(-phi / XM_2PI, -theta / XM_PI);

			phi -= addAngle;
		}

		theta -= addAngle;
	}

	// ���_�C���f�b�N�X�o�b�t�@�֒��_�C���f�b�N�X�̏�������
	int indexNum = 0;
	int indexFlapX = cornerNum - 1;
	int indexFlapY = cornerNum / 2 - 1;
	for (int y = 0; y < cornerNum / 2; y++)
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
	Texture.reset(new TEXTURE(string("sky.png")));
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
void MESH_DOOM::Update(float delta_time)
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

		auto camera01 = CManager::Get_Scene()->Get_Game_Object<CCamera>();
		auto camera02 = CManager::Get_Scene()->Get_Game_Object<DEBUG_CAMERA>();

		if (nullptr != camera01)

		{
			CRenderer::Set_MatrixBuffer(world, camera01->Get_Camera_View(), camera01->Get_Camera_Projection());
		}
		else
		{
			CRenderer::Set_MatrixBuffer(world, camera02->Get_Camera_View(), camera02->Get_Camera_Projection());
		}
	}

	CRenderer::SetVertexBuffers(VertexBuffer.get());
	CRenderer::SetIndexBuffer(IndexBuffer.get());

	// �g�|���W�ݒ�
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	Texture.get()->Set_Texture();

	CRenderer::Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_LIGHT);

	// �|���S���`��
	CRenderer::GetDeviceContext()->DrawIndexed(IndexNum, 0, 0);

	CRenderer::Set_Shader();

}
