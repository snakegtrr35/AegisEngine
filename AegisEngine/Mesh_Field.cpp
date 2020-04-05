#include	"Game_Object.h"
#include	"Mesh_Field.h"
#include	"camera.h"
#include	"Debug_Camera.h"
#include	"renderer.h"
#include	"texture.h"
#include	"manager.h"
#include	"Scene.h"
#include	"ShadowMap.h"

MESH_FIELD::MESH_FIELD() : GridSize(XMFLOAT3(1.0f, 0.0f, 1.0f)), GridNum(XMINT2(10, 10))
{
	VertexBuffer.reset(nullptr);
	IndexBuffer.reset(nullptr);

	// �e�N�X�`���̐���
	Texture = make_unique<TEXTURE>("UVCheckerMap01-512.png");
};

void MESH_FIELD::Init()
{
	const WORD VertexNum = (GridNum.x + 1) * (GridNum.y + 1);

	// ���_�o�b�t�@�̍쐬
	{
		VertexArray.reserve(VertexNum);
		VertexArray.assign(VertexNum, VERTEX_3D());

		// ���_�o�b�t�@�ւ̏�������
		for (int z = 0; z < GridNum.y + 1; z++)
		{
			for (int x = 0; x < GridNum.x + 1; x++)
			{
				XMFLOAT3 position;

				position.x = -0.5f * (float)GridNum.x * GridSize.x + (float)x * GridSize.x;
				//position.y = sinf(z * 0.8) * 0.3f;
				position.y = 0.0f;
				position.z = 0.5f * (float)GridNum.y * GridSize.z - (float)z * GridSize.z;

				VertexArray[x + (GridNum.x + 1) * z].Position = position;
				VertexArray[x + (GridNum.y + 1) * z].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
				VertexArray[x + (GridNum.x + 1) * z].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				VertexArray[x + (GridNum.x + 1) * z].TexCoord = XMFLOAT2((float)x, (float)z);
			}
		}

		// ���_�o�b�t�@�ւ̏�������(Normal)
		for (int z = 1; z < GridNum.y; z++)
		{
			for (int x = 1; x < GridNum.x; x++)
			{
				XMFLOAT3 va, vb;

				va.x = VertexArray[(z - 1) * GridNum.y + x].Position.x - VertexArray[(z + 1) * GridNum.x + x].Position.x;
				va.y = VertexArray[(z - 1) * GridNum.y + x].Position.y - VertexArray[(z + 1) * GridNum.x + x].Position.y;
				va.z = VertexArray[(z - 1) * GridNum.y + x].Position.z - VertexArray[(z + 1) * GridNum.x + x].Position.z;

				vb.x = VertexArray[(x + 1) + (GridNum.x + 1) * z].Position.x - VertexArray[(x - 1) + (GridNum.x + 1) * z].Position.x;
				vb.y = VertexArray[(x + 1) + (GridNum.x + 1) * z].Position.y - VertexArray[(x - 1) + (GridNum.x + 1) * z].Position.y;
				vb.z = VertexArray[(x + 1) + (GridNum.x + 1) * z].Position.z - VertexArray[(x - 1) + (GridNum.x + 1) * z].Position.z;

				XMFLOAT3 vn;

				// �O��
				vn.x = va.y * vb.z - va.z * vb.y;
				vn.y = va.z * vb.x - va.x * vb.z;
				vn.z = va.x * vb.y - va.y * vb.x;

				float legth = sqrtf(vn.x * vn.x + vn.y * vn.y + vn.z * vn.z);

				vn.x = vn.x / legth;
				vn.y = vn.y / legth;
				vn.z = vn.z / legth;

				VertexArray[x + (GridNum.y + 1) * z].Normal = vn;
			}
		}

		// ���_�o�b�t�@�̐���
		{
			ID3D11Buffer* buffer = nullptr;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(VERTEX_3D) * VertexNum;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = VertexArray.data();

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			VertexBuffer.reset(buffer);
		}
	}

	// �C���f�b�N�X�o�b�t�@�̍쐬
	{
		IndexNum = (2 + (GridNum.x * 2)) * GridNum.y + (GridNum.y - 1) * 2;

		const UINT size = IndexNum;

		vector<WORD> indexArray;
		indexArray.reserve(IndexNum);
		indexArray.assign(IndexNum, 0);

		// ���_�C���f�b�N�X�o�b�t�@�ւ̏�������
		int indexNum = 0;
		int indexFlapX = GridNum.x - 1;
		int indexFlapZ = GridNum.y - 1;
		for (int z = 0; z < GridNum.y; z++)
		{
			for (int x = 0; x < GridNum.x + 1; x++)
			{
				indexArray[indexNum++] = WORD((GridNum.x + 1) * (z + 1) + x);
				indexArray[indexNum++] = WORD((GridNum.x + 1) * z + x);

				// �k�ރ|���S���p�ɃC���f�b�N�X�̒ǉ�
				if (x > indexFlapX && z < indexFlapZ)
				{
					indexArray[indexNum++] = WORD((GridNum.x + 1) * z + (GridNum.x + 1) - 1);
					indexArray[indexNum++] = WORD((GridNum.x + 1) * (z + 1 + 1));
				}
			}
		}

		// �C���f�b�N�X�o�b�t�@�̐���
		{
			ID3D11Buffer* buffer = nullptr;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(WORD) * IndexNum;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = indexArray.data();

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

			IndexBuffer.reset(buffer);
		}
	}

	GAME_OBJECT::Init();
}

void MESH_FIELD::Uninit()
{
	VertexBuffer.reset(nullptr);
	IndexBuffer.reset(nullptr);

	Texture.reset(nullptr);
}

void MESH_FIELD::Update(float delta_time)
{
	GAME_OBJECT::Update(delta_time);
}

void MESH_FIELD::Draw()
{
	// 3D�}�g���b�N�X�ݒ�
	{
		XMMATRIX world;

		float angle = 0.0f;
		if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
		{
			//angle = 180.0f;
		}

		world = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);																						// �g��k��
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z + angle));			// ��]
		world *= XMMatrixTranslation(Position.x, Position.y, Position.z);																				// �ړ�

		const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
		const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

		if (!camera01.expired() /*&& Empty_weak_ptr<CCamera>(camera01)*/)
		{
			// �V���h�E�}�b�v�p�̕`�悩?
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
			// �V���h�E�}�b�v�p�̕`�悩?
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

	// ���_�o�b�t�@�ݒ�
	CRenderer::SetVertexBuffers(VertexBuffer.get());

	// �C���f�b�N�X�o�b�t�@�ݒ�
	CRenderer::SetIndexBuffer(IndexBuffer.get());

	// �e�N�X�`���̐ݒ�
	Texture->Set_Texture();

	// �g�|���W�ݒ�
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	CRenderer::GetDeviceContext()->DrawIndexed(IndexNum, 0, 0);

	GAME_OBJECT::Draw();
}

void MESH_FIELD::Draw_DPP()
{
	// 3D�}�g���b�N�X�ݒ�
	{
		XMMATRIX world = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));
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

	// ���_�o�b�t�@�ݒ�
	CRenderer::SetVertexBuffers(VertexBuffer.get());

	// �C���f�b�N�X�o�b�t�@�ݒ�
	CRenderer::SetIndexBuffer(IndexBuffer.get());

	// �g�|���W�ݒ�
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	CRenderer::GetDeviceContext()->DrawIndexed(IndexNum, 0, 0);
}

const float MESH_FIELD::Get_Height(const XMFLOAT3& position)
{
	int x, z;

	XMFLOAT3 p0, p1, p2, v01, v02, n, v, hp, va = XMFLOAT3(0.f, 0.f, 0.f), vb;

	float dp0n, dvn, dpn, t;

	v = XMFLOAT3(0.0f, -10.0f, 0.0f);

	x = (position.x +  (GridSize.x * GridNum.x) * 0.5f) / GridSize.x;
	z = (-position.z + (GridSize.z * GridNum.y) * 0.5f) / GridSize.z;
	va.x = VertexArray[(z + 1) * (GridNum.x + 1) + (x + 1)].Position.x - VertexArray[z * (GridNum.x + 1) + x].Position.x;
	va.y = VertexArray[(z + 1) * (GridNum.x + 1) + (x + 1)].Position.y - VertexArray[z * (GridNum.x + 1) + x].Position.y;
	va.z = VertexArray[(z + 1) * (GridNum.x + 1) + (x + 1)].Position.z - VertexArray[z * (GridNum.x + 1) + x].Position.z;

	vb.x = position.x - VertexArray[z * (GridNum.x + 1) + x].Position.x;
	vb.y = position.y - VertexArray[z * (GridNum.x + 1) + x].Position.y;
	vb.z = position.z - VertexArray[z * (GridNum.x + 1) + x].Position.z;

	if ((va.z * vb.x - va.x * vb.z) > 0.0f)
	{
		p0 = VertexArray[(z + 1) * (GridNum.x + 1) + x].Position;
		p1 = VertexArray[z * (GridNum.x + 1) + x].Position;
		p2 = VertexArray[(z + 1) * (GridNum.y + 1) + (x + 1)].Position;
	}
	else
	{
		p0 = VertexArray[z * (GridNum.x + 1) + (x + 1)].Position;
		p1 = VertexArray[(z + 1) * (GridNum.x + 1) + (x + 1)].Position;
		p2 = VertexArray[z * (GridNum.x + 1) + x].Position;
	}

	v01.x = p1.x - p2.x;
	v01.y = p1.y - p2.y;
	v01.z = p1.z - p2.z;

	v02.x = p2.x - p0.x;
	v02.y = p2.y - p0.y;
	v02.z = p2.z - p0.z;

	// �O��
	n.x = v01.y * v02.z - v01.z * v02.y;
	n.y = v01.z * v02.x - v01.x * v02.z;
	n.z = v01.x * v02.y - v01.y * v02.x;

	// ����
	dvn = v.x * n.x + v.y * n.y + v.z * n.z;

	// ����
	dp0n = p0.x * n.x + p0.y * n.y + p0.z * n.z;

	// ����
	dpn = position.x * n.x + position.y * n.y + position.z * n.z;

	t = (dp0n - dpn) / dvn;

	hp.x = position.x + v.x * t;
	hp.y = position.y + v.y * t;
	hp.z = position.z + v.z * t;

	return hp.y;
}

//==============================
// �e�N�X�`���̐ݒ�
//==============================
void MESH_FIELD::SetTexture(const string& const file_name)
{
	Texture->Set_Texture_Name(file_name);
}





MESH_WALL::MESH_WALL()
{
	VertexArray = nullptr;
	IndexBuffer = VertexBuffer = nullptr;
	Texture = nullptr;

	GridSize = XMFLOAT3(1.0f, 0.0f, 1.0f);
	GridNum = XMINT2(10, 5);
};

void MESH_WALL::Init()
{
	unsigned int VertexNum = (GridNum.x + 1) * (GridNum.y + 1);
	VertexArray = new VERTEX_3D[VertexNum];

	IndexNum = (2 + (GridNum.x * 2)) * GridNum.y + (GridNum.y - 1) * 2;
	unsigned short* indexArray = new unsigned short[IndexNum];

	// ���_�o�b�t�@�ւ̏�������
	for (int z = 0; z < GridNum.y + 1; z++)
	{
		for (int x = 0; x < GridNum.x + 1; x++)
		{
			XMFLOAT3 position;

			position.x = -0.5f * (float)GridNum.x * GridSize.x + (float)x * GridSize.x;
			position.y = 0.0f;
			position.z = 0.5f * (float)GridNum.y * GridSize.z - (float)z * GridSize.z;

			VertexArray[x + (GridNum.x + 1) * z].Position = position;

			VertexArray[x + (GridNum.y + 1) * z].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

			VertexArray[x + (GridNum.x + 1) * z].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			VertexArray[x + (GridNum.x + 1) * z].TexCoord = XMFLOAT2((float)x, (float)z);
		}
	}

	// ���_�o�b�t�@�ւ̏�������(Normal)
	for (int z = 1; z < GridNum.y; z++)
	{
		for (int x = 1; x < GridNum.x; x++)
		{
			XMFLOAT3 va, vb;

			va.x = VertexArray[(z - 1) * GridNum.y + x].Position.x - VertexArray[(z + 1) * GridNum.x + x].Position.x;
			va.y = VertexArray[(z - 1) * GridNum.y + x].Position.y - VertexArray[(z + 1) * GridNum.x + x].Position.y;
			va.z = VertexArray[(z - 1) * GridNum.y + x].Position.z - VertexArray[(z + 1) * GridNum.x + x].Position.z;

			vb.x = VertexArray[(x + 1) + (GridNum.x + 1) * z].Position.x - VertexArray[(x - 1) + (GridNum.x + 1) * z].Position.x;
			vb.y = VertexArray[(x + 1) + (GridNum.x + 1) * z].Position.y - VertexArray[(x - 1) + (GridNum.x + 1) * z].Position.y;
			vb.z = VertexArray[(x + 1) + (GridNum.x + 1) * z].Position.z - VertexArray[(x - 1) + (GridNum.x + 1) * z].Position.z;

			XMFLOAT3 vn;

			// �O��
			vn.x = va.y * vb.z - va.z * vb.y;
			vn.y = va.z * vb.x - va.x * vb.z;
			vn.z = va.x * vb.y - va.y * vb.x;

			float legth = sqrtf(vn.x * vn.x + vn.y * vn.y + vn.z * vn.z);

			vn.x = vn.x / legth;
			vn.y = vn.y / legth;
			vn.z = vn.z / legth;

			VertexArray[x + (GridNum.y + 1) * z].Normal = vn;
		}
	}

	// ���_�C���f�b�N�X�o�b�t�@�ւ̏�������
	int indexNum = 0;
	int indexFlapX = GridNum.x - 1;
	int indexFlapZ = GridNum.y - 1;
	for (int z = 0; z < GridNum.y; z++)
	{
		for (int x = 0; x < GridNum.x + 1; x++)
		{
			indexArray[indexNum++] = WORD((GridNum.x + 1) * (z + 1) + x);
			indexArray[indexNum++] = WORD((GridNum.x + 1) * z + x);

			// �k�ރ|���S���p�ɃC���f�b�N�X�̒ǉ�
			if (x > indexFlapX && z < indexFlapZ)
			{
				indexArray[indexNum++] = WORD((GridNum.x + 1) * z + (GridNum.x + 1) - 1);
				indexArray[indexNum++] = WORD((GridNum.x + 1) * (z + 1 + 1));
			}
		}
	}


	// ���_�o�b�t�@�̐���
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * VertexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = VertexArray;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &VertexBuffer);
	}

	// �C���f�b�N�X�o�b�t�@�̐���
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned short) * IndexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = indexArray;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &IndexBuffer);
	}

	delete[] indexArray;

	Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);

	// �e�N�X�`���̐ݒ�
	Texture = new TEXTURE(string("field004.png"));
}

void MESH_WALL::Uninit()
{
	SAFE_DELETE(VertexArray);
	SAFE_RELEASE(VertexBuffer);
	SAFE_RELEASE(IndexBuffer);
	SAFE_DELETE(Texture);
}

void MESH_WALL::Update(float delta_time)
{
	GAME_OBJECT::Update(delta_time);
}

void MESH_WALL::Draw()
{
	{
		// 3D�}�g���b�N�X�ݒ�
		{
			XMMATRIX world;

			world = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);																						// �g��k��
			world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));			// ��]
			world *= XMMatrixTranslation(Position.x, Position.y, Position.z);																				// �ړ�

			const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
			const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

			if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
			{
				// �V���h�E�}�b�v�p�̕`�悩?
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

					CRenderer::Set_MatrixBuffer01(*camera02.lock()->Get_Pos());

					CRenderer::Set_Shader();
				}
			}
			else
			{
				// �V���h�E�}�b�v�p�̕`�悩?
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

		// ���_�o�b�t�@�ݒ�
		CRenderer::SetVertexBuffers(VertexBuffer);

		// �C���f�b�N�X�o�b�t�@�ݒ�
		CRenderer::SetIndexBuffer(IndexBuffer);

		// �e�N�X�`���̐ݒ�
		Texture->Set_Texture();

		// �g�|���W�ݒ�
		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		CRenderer::GetDeviceContext()->DrawIndexed(IndexNum, 0, 0);

		if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
		{
			CRenderer::Set_Shader();
		}
	}
	GAME_OBJECT::Draw();
}

void MESH_WALL::Draw_DPP()
{
	// 3D�}�g���b�N�X�ݒ�
	{
		XMMATRIX world = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
		world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));
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

	// ���_�o�b�t�@�ݒ�
	CRenderer::SetVertexBuffers(VertexBuffer);

	// �C���f�b�N�X�o�b�t�@�ݒ�
	CRenderer::SetIndexBuffer(IndexBuffer);

	// �g�|���W�ݒ�
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	CRenderer::GetDeviceContext()->DrawIndexed(IndexNum, 0, 0);
}

const float MESH_WALL::Get_Height(const XMFLOAT3& position)
{
	int x, z;

	XMFLOAT3 p0, p1, p2, v01, v02, n, v, hp, va = XMFLOAT3(0.f, 0.f, 0.f), vb;

	float dp0n, dvn, dpn, t;

	v = XMFLOAT3(0.0f, -10.0f, 0.0f);

	x = (position.x + (GridSize.x * GridNum.x) * 0.5f) / GridSize.x;
	z = (-position.z + (GridSize.z * GridNum.y) * 0.5f) / GridSize.z;

	va.x = VertexArray[(z + 1) * (GridNum.x + 1) + (x + 1)].Position.x - VertexArray[z * (GridNum.x + 1) + x].Position.x;
	va.y = VertexArray[(z + 1) * (GridNum.x + 1) + (x + 1)].Position.y - VertexArray[z * (GridNum.x + 1) + x].Position.y;
	va.z = VertexArray[(z + 1) * (GridNum.x + 1) + (x + 1)].Position.z - VertexArray[z * (GridNum.x + 1) + x].Position.z;

	vb.x = position.x - VertexArray[z * (GridNum.x + 1) + x].Position.x;
	vb.y = position.y - VertexArray[z * (GridNum.x + 1) + x].Position.y;
	vb.z = position.z - VertexArray[z * (GridNum.x + 1) + x].Position.z;

	if ((va.z * vb.x - va.x * vb.z) > 0.0f)
	{
		p0 = VertexArray[(z + 1) * (GridNum.x + 1) + x].Position;
		p1 = VertexArray[z * (GridNum.x + 1) + x].Position;
		p2 = VertexArray[(z + 1) * (GridNum.y + 1) + (x + 1)].Position;
	}
	else
	{
		p0 = VertexArray[z * (GridNum.x + 1) + (x + 1)].Position;
		p1 = VertexArray[(z + 1) * (GridNum.x + 1) + (x + 1)].Position;
		p2 = VertexArray[z * (GridNum.x + 1) + x].Position;
	}

	v01.x = p1.x - p2.x;
	v01.y = p1.y - p2.y;
	v01.z = p1.z - p2.z;

	v02.x = p2.x - p0.x;
	v02.y = p2.y - p0.y;
	v02.z = p2.z - p0.z;

	// �O��
	n.x = v01.y * v02.z - v01.z * v02.y;
	n.y = v01.z * v02.x - v01.x * v02.z;
	n.z = v01.x * v02.y - v01.y * v02.x;

	// ����
	dvn = v.x * n.x + v.y * n.y + v.z * n.z;

	// ����
	dp0n = p0.x * n.x + p0.y * n.y + p0.z * n.z;

	// ����
	dpn = position.x * n.x + position.y * n.y + position.z * n.z;

	t = (dp0n - dpn) / dvn;

	hp.x = position.x + v.x * t;
	hp.y = position.y + v.y * t;
	hp.z = position.z + v.z * t;

	return hp.y;
}