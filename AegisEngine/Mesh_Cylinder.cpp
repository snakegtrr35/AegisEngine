#include	"Game_Object.h"
#include	"Mesh_Cylinder.h"
#include	"camera.h"
#include	"Debug_Camera.h"
#include	"renderer.h"
#include	"manager.h"
#include	"Scene.h"
#include	"ShadowMap.h"

void MESH_CYlLINDER::Init()
{
	int cornerNum = (int)Radius * CylinderLength * 10;

	// ���_�o�b�t�@�̊m��
	VertexNum = (cornerNum + 1) * (CylinderLength + 1);
	VERTEX_3D* vertexArray = new VERTEX_3D[VertexNum];

	// �C���f�b�N�X�o�b�t�@�̊m��
	IndexNum = (2 + (cornerNum * 2)) * CylinderLength + (CylinderLength - 1) * 2;
	unsigned short* indexArray = new unsigned short[IndexNum];

	// ���_�o�b�t�@�ւ̒��_���̏�������
	float theta = 0.0f;
	float addAngle = 2.0f * XM_PI / ((float)CylinderLength * 60.0f);
	for (int y = 0; y < CylinderLength + 1; y++)
	{
		theta = 0.0f;

		for (int x = 0; x < cornerNum + 1; x++)
		{
			vertexArray[x + (cornerNum + 1) * y].Position	= XMFLOAT3(Radius * cosf(theta), (float)CylinderLength - (float)y, Radius * sinf(theta));
			vertexArray[x + (cornerNum + 1) * y].Normal		= XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertexArray[x + (cornerNum + 1) * y].Diffuse	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertexArray[x + (cornerNum + 1) * y].TexCoord	= XMFLOAT2(-theta / (2.0f * XM_PI / (float)CylinderLength), (float)y / CylinderLength);

			theta -= addAngle;
		}
	}

	// ���_�C���f�b�N�X�o�b�t�@�֒��_�C���f�b�N�X�̏�������
	int indexNum = 0;
	int indexFlapX = cornerNum - 1;
	int indexFlapY = CylinderLength - 1;
	for (int y = 0; y < CylinderLength; y++)
	{
		for (int x = 0; x < cornerNum + 1; x++)
		{
			indexArray[indexNum++] = (cornerNum + 1) * (y + 1) + x;
			indexArray[indexNum++] = (cornerNum + 1) * y + x;

			// �k�ރ|���S���p�ɃC���f�b�N�X�̒ǉ�
			if (x > indexFlapX && y < indexFlapY)
			{
				indexArray[indexNum++] = (cornerNum + 1) * y + (cornerNum + 1) - 1;
				indexArray[indexNum++] = (cornerNum + 1) * (y + 1 + 1) + 0;
			}
		}
	}


	// ���_�o�b�t�@����
	if (nullptr == VertexBuffer.get())
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
	Texture.reset(new TEXTURE(string("field004.png")));
}

void MESH_CYlLINDER::Uninit()
{
	VertexBuffer.reset();
	IndexBuffer.reset();
	Texture.reset();
}

void MESH_CYlLINDER::Draw()
{
	if (false == CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
	{
		{
			XMMATRIX world;

			world = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
			world *= XMMatrixRotationRollPitchYaw(Rotation.x, Rotation.y, Rotation.z);
			world *= XMMatrixTranslation(Position.x, Position.y, Position.z);

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

		CRenderer::SetVertexBuffers(VertexBuffer.get());	// ���_�o�b�t�@�ݒ�
		CRenderer::SetIndexBuffer(IndexBuffer.get());		// �C���f�b�N�X�o�b�t�@�ݒ�

		Texture.get()->Set_Texture();

		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);	// �g�|���W�ݒ�

		// �|���S���`��
		CRenderer::GetDeviceContext()->DrawIndexed(IndexNum, 0, 0);
	}

	GAME_OBJECT::Draw();
}

void MESH_CYlLINDER::Draw_DPP()
{
	{
		XMMATRIX world;

		world = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
		world *= XMMatrixRotationRollPitchYaw(Rotation.x, Rotation.y, Rotation.z);
		world *= XMMatrixTranslation(Position.x, Position.y, Position.z);

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

	CRenderer::SetVertexBuffers(VertexBuffer.get());	// ���_�o�b�t�@�ݒ�
	CRenderer::SetIndexBuffer(IndexBuffer.get());		// �C���f�b�N�X�o�b�t�@�ݒ�

	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);	// �g�|���W�ݒ�

	// �|���S���`��
	CRenderer::GetDeviceContext()->DrawIndexed(IndexNum, 0, 0);
}

void MESH_CYlLINDER::Update(float delta_time)
{
	GAME_OBJECT::Update(delta_time);
}
