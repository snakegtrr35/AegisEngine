#include	"Grid.h"

#include	"manager.h"
#include	"Scene.h"
#include	"ShadowMap.h"

#define COUNT (11 * 2 * 2)

GRID::GRID()
{
	const VERTEX_3D Vertexs[COUNT] = {

		// ��
		{ XMFLOAT3(-5.0f, 0.0f, -5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f, 0.0f, -5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },

		{ XMFLOAT3(-5.0f, 0.0f, -4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f, 0.0f, -4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },

		{ XMFLOAT3(-5.0f, 0.0f, -3.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f, 0.0f, -3.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },

		{ XMFLOAT3(-5.0f, 0.0f, -2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f, 0.0f, -2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },

		{ XMFLOAT3(-5.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },

		{ XMFLOAT3(-5.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },

		{ XMFLOAT3(-5.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },

		{ XMFLOAT3(-5.0f, 0.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f, 0.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },

		{ XMFLOAT3(-5.0f, 0.0f, 3.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f, 0.0f, 3.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },

		{ XMFLOAT3(-5.0f, 0.0f, 4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f, 0.0f, 4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },

		{ XMFLOAT3(-5.0f, 0.0f, 5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f, 0.0f, 5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },


		// �c
		{ XMFLOAT3(-5.0f, 0.0f, 5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-5.0f, 0.0f, -5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-4.0f, 0.0f, 5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-4.0f, 0.0f, -5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-3.0f, 0.0f, 5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-3.0f, 0.0f, -5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-2.0f, 0.0f, 5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-2.0f, 0.0f, -5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, 0.0f, 5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 0.0f, -5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(0.0f, 0.0f, 5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(0.0f, 0.0f, -5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(1.0f, 0.0f, 5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 0.0f,-5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(2.0f, 0.0f, 5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(2.0f, 0.0f, -5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(3.0f, 0.0f, 5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(3.0f, 0.0f, -5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(4.0f, 0.0f, 5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(4.0f, 0.0f, -5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(5.0f, 0.0f, 5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(5.0f, 0.0f, -5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
	};

	// ���_�o�b�t�@�̐ݒ�
	{
		HRESULT hr;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

		bd.ByteWidth = sizeof(VERTEX_3D) * COUNT;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		// �T�u���\�[�X�̐ݒ�
		D3D11_SUBRESOURCE_DATA srd;
		ZeroMemory(&srd, sizeof(D3D11_SUBRESOURCE_DATA));

		srd.pSysMem = Vertexs;
		srd.SysMemPitch = 0;
		srd.SysMemSlicePitch = 0;

		// ���_�o�b�t�@�̐���
		hr = CRenderer::GetDevice()->CreateBuffer(&bd, &srd, &pVertexBuffer);

		if (FAILED(hr))
		{
			assert(false);
			return;
		}
	}
}

GRID::~GRID()
{
	Uninit();
}

void GRID::Init()
{
}

void GRID::Draw()
{
	if (false == CManager::Get_ShadowMap()->Get_Enable())
	{
		// ���̓A�Z���u���ɒ��_�o�b�t�@��ݒ�.
		CRenderer::SetVertexBuffers(pVertexBuffer);

		// 3D�}�g���b�N�X�ݒ�
		{
			XMMATRIX world = XMMatrixIdentity();

			world = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);																						// �g��k��
			world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));			// ��](���[���s�b�`���E)
			world *= XMMatrixTranslation(Position.x, Position.y, Position.z);																				// �ړ�

			const auto camera01 = CManager::Get_Scene()->Get_Game_Object<CCamera>("camera");
			const auto camera02 = CManager::Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

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

		// �g�|���W�̐ݒ�
		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		CRenderer::Set_Shader(SHADER_INDEX_V::DEFAULT, SHADER_INDEX_P::NO_TEXTURE);

		CRenderer::GetDeviceContext()->Draw(COUNT, 0);

		// �C���X�^���V���O
		//CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//CRenderer::GetDeviceContext()->DrawInstanced(COUNT, 200000, 0, 0);

		CRenderer::Set_Shader();
	}
}

void GRID::Update(float delta_time)
{

}

void GRID::Uninit()
{
	SAFE_RELEASE(pVertexBuffer);
}