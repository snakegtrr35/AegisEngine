#include	"Mesh.h"

MESH::MESH()
{
	VertexBuffer = nullptr;
	IndexBuffer = nullptr;
}

MESH::MESH(vector<VERTEX_3D>& vertices, vector<UINT>& indices, vector<TEXTURE_S>& textures, XMMATRIX& matrix, string name)
{
	VertexBuffer = nullptr;
	IndexBuffer = nullptr;

	//Vertices = vertices;
	Indices = indices;
	Textures = textures;
	Matrix = matrix;

	Name = name;

	SetupMesh(vertices);
}

MESH::~MESH()
{
}

void MESH::Draw(XMMATRIX& matrix)
{
	Draw_Mesh(matrix);
}

void MESH::Draw_DPP(XMMATRIX& matrix)
{
	Draw_DPP_Mesh(matrix);
}

void MESH::Draw_Animation(XMMATRIX& matrix, unordered_map<string, Anim>& anime, DWORD frame, const string& name1, const string& name2, float blend)
{
	Draw_Mesh_Animation(matrix, anime, frame, name1, name2, blend);
}

void MESH::Draw_DPP_Animation(XMMATRIX& matrix, unordered_map<string, Anim>& anime, DWORD frame, const string& name1, const string& name2, float blend)
{
	Draw_DPP_Mesh_Animation(matrix, anime, frame, name1, name2, blend);
}

void MESH::Update()
{
}

void MESH::Uninit()
{
	SAFE_RELEASE(VertexBuffer);
	SAFE_RELEASE(IndexBuffer);

	Indices.clear();

	for (auto& tex : Textures)
	{
		SAFE_RELEASE(tex.Texture);
	}
	Textures.clear();

	for (auto child : ChildMeshes)
	{
		child.second.Uninit();
	}
	ChildMeshes.clear();
}

void MESH::SetAnimation(const string& name, const Anim& animations)
{
	Animation[name] = animations;
}

void MESH::Add(const string name, const MESH& mesh)
{
	ChildMeshes[name] = mesh;
}

map<string, MESH>& MESH::Get()
{
	return ChildMeshes;
}

unordered_map<string, Anim>& MESH::Get_Anime()
{
	return Animation;
}

bool MESH::GetAnime()
{
	if (0 == Animation.size())
	{
		return false;
	}

	return true;
}

bool MESH::SetupMesh(vector<VERTEX_3D>& vertices)
{
	HRESULT hr;

	{
		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_DEFAULT;
		vbd.ByteWidth = sizeof(VERTEX_3D) * vertices.size();
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;

		// �T�u���\�[�X�̐ݒ�
		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));

		initData.pSysMem = &vertices[0];
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		hr = CRenderer::GetDevice()->CreateBuffer(&vbd, &initData, &VertexBuffer);
		if (FAILED(hr))
			return false;
	}

	{
		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.ByteWidth = sizeof(UINT) * (UINT)Indices.size();
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;

		// �T�u���\�[�X�̐ݒ�
		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));

		initData.pSysMem = &Indices[0];
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		hr = CRenderer::GetDevice()->CreateBuffer(&ibd, &initData, &IndexBuffer);
		if (FAILED(hr))
			return false;
	}
}


void MESH::Draw_Mesh(XMMATRIX& parent_matrix)
{
	XMMATRIX matrix;

	if (!Indices.empty() && nullptr != Textures[0].Texture)
	{
		CRenderer::SetVertexBuffers(VertexBuffer);

		CRenderer::GetDeviceContext()->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &Textures[0].Texture);

		// 3D�}�g���b�N�X�ݒ�
		{
			matrix = XMMatrixMultiply(Matrix, parent_matrix);

			const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
			const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

			// ���ʂ̃J�������f�o�b�O�J������?
			if (!camera01.expired() /*&& Empty_weak_ptr<CCamera>(camera01)*/)
			{
				// �V���h�E�}�b�v�p�̕`�悩?
				if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
				{
					XMMATRIX view = CManager::Get_Instance()->Get_ShadowMap()->Get_View();
					XMMATRIX proj = CManager::Get_Instance()->Get_ShadowMap()->Get_Plojection();

					CRenderer::Set_MatrixBuffer(matrix, view, proj);
				}
				else
				{
					CRenderer::Set_MatrixBuffer(matrix, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());

					CRenderer::Set_MatrixBuffer01(*camera01.lock()->Get_Pos());
				}
			}
			else
			{
				// �V���h�E�}�b�v�p�̕`�悩?
				if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
				{
					XMMATRIX view = CManager::Get_Instance()->Get_ShadowMap()->Get_View();
					XMMATRIX proj = CManager::Get_Instance()->Get_ShadowMap()->Get_Plojection();

					CRenderer::Set_MatrixBuffer(matrix, view, proj);
				}
				else
				{
					CRenderer::Set_MatrixBuffer(matrix, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());

					CRenderer::Set_MatrixBuffer01(*camera02.lock()->Get_Pos());
				}
			}
		}

		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		CRenderer::GetDeviceContext()->DrawIndexed((UINT)Indices.size(), 0, 0);
	}
	else
	{
		matrix = XMMatrixMultiply(Matrix, parent_matrix);
	}

	for (auto child : ChildMeshes)
	{
		child.second.Draw_Mesh(matrix);
	}
}

void MESH::Draw_DPP_Mesh(XMMATRIX& parent_matrix)
{
	XMMATRIX matrix;

	if (!Indices.empty())
	{
		// 3D�}�g���b�N�X�ݒ�
		{
			matrix = XMMatrixMultiply(Matrix, parent_matrix);

			const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
			const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

			// ���ʂ̃J�������f�o�b�O�J����
			if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
			{
				CRenderer::Set_MatrixBuffer(matrix, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());
			}
			else
			{
				CRenderer::Set_MatrixBuffer(matrix, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());
			}
		}

		CRenderer::SetVertexBuffers(VertexBuffer);

		CRenderer::GetDeviceContext()->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		CRenderer::GetDeviceContext()->DrawIndexed((UINT)Indices.size(), 0, 0);
	}
	else
	{
		matrix = XMMatrixMultiply(Matrix, parent_matrix);
	}

	for (auto child : ChildMeshes)
	{
		child.second.Draw_DPP_Mesh(matrix);
	}
}

void MESH::Draw_Mesh_Animation(XMMATRIX& parent_matrix, unordered_map<string, Anim>& anime, DWORD frame, const string& name1, const string& name2, float blend)
{
	XMMATRIX world;

	CRenderer::SetVertexBuffers(VertexBuffer);

	CRenderer::GetDeviceContext()->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &Textures[0].Texture);

	// 3D�}�g���b�N�X�ݒ�
	{
		if ("none" == name2)
		{
			for (auto i : anime[name1].body)
			{
				if (i.node_name == Name)
				{
					unsigned int f = frame % i.translate.begin()->time;

					XMFLOAT3 pos = i.translate[f].value;
					XMMATRIX trans = XMMatrixTranslation(pos.x, pos.y, pos.z);

					f = frame % i.rotation.begin()->time;

					XMFLOAT4 rotation = i.rotation[f].value;
					XMVECTOR quat = XMLoadFloat4(&rotation);

					world = XMMatrixRotationQuaternion(quat);

					world = XMMatrixMultiply(world, trans);

					world = XMMatrixMultiply(world, parent_matrix);

					auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
					auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

					// ���ʂ̃J�������f�o�b�O�J������?
					if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
					{
						// �V���h�E�}�b�v�p�̕`�悩?
						if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
						{
							XMMATRIX view = CManager::Get_Instance()->Get_ShadowMap()->Get_View();
							XMMATRIX proj = CManager::Get_Instance()->Get_ShadowMap()->Get_Plojection();

							CRenderer::Set_MatrixBuffer(world, view, proj);
						}
						else
						{
							CRenderer::Set_MatrixBuffer(world, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());

							CRenderer::Set_MatrixBuffer01(*camera01.lock()->Get_Pos());
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
						}
						else
						{
							CRenderer::Set_MatrixBuffer(world, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());

							CRenderer::Set_MatrixBuffer01(*camera02.lock()->Get_Pos());
						}
					}

					break;
				}
			}
		}
		else
		{
			XMMATRIX trans1, trans2, trans;
			XMVECTOR quat1, quat2, puat;

			for (auto i : anime[name1].body)
			{
				if (i.node_name == Name)
				{
					unsigned int f = frame % i.translate.begin()->time;

					XMFLOAT3 pos = i.translate[f].value;
					trans1 = XMMatrixTranslation(pos.x, pos.y, pos.z);

					f = frame % i.rotation.begin()->time;

					XMFLOAT4 rotation = i.rotation[f].value;
					quat1 = XMLoadFloat4(&rotation);

					break;
				}
			}

			for (auto i : anime[name2].body)
			{
				if (i.node_name == Name)
				{
					unsigned int f = frame % i.translate.begin()->time;

					XMFLOAT3 pos = i.translate[f].value;
					trans2 = XMMatrixTranslation(pos.x, pos.y, pos.z);

					f = frame % i.rotation.begin()->time;

					XMFLOAT4 rotation = i.rotation[f].value;
					quat2 = XMLoadFloat4(&rotation);

					break;
				}
			}

			trans = trans1 * (1.0f - blend) + trans2 * blend;
			//puat = quat1 * (1.0f - blend) + quat2 *  blend;
			puat = XMQuaternionSlerp(quat1, quat2, blend);

			world = XMMatrixRotationQuaternion(puat);

			world = XMMatrixMultiply(world, trans);

			world = XMMatrixMultiply(world, parent_matrix);

			auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
			auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

			// ���ʂ̃J�������f�o�b�O�J������?
			if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
			{
				// �V���h�E�}�b�v�p�̕`�悩?
				if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
				{
					XMMATRIX view = CManager::Get_Instance()->Get_ShadowMap()->Get_View();
					XMMATRIX proj = CManager::Get_Instance()->Get_ShadowMap()->Get_Plojection();

					CRenderer::Set_MatrixBuffer(world, view, proj);
				}
				else
				{
					CRenderer::Set_MatrixBuffer(world, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());

					CRenderer::Set_MatrixBuffer01(*camera01.lock()->Get_Pos());
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
				}
				else
				{
					CRenderer::Set_MatrixBuffer(world, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());

					CRenderer::Set_MatrixBuffer01(*camera02.lock()->Get_Pos());
				}
			}
		}
	}

	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	CRenderer::GetDeviceContext()->DrawIndexed((UINT)Indices.size(), 0, 0);

	for (auto child : ChildMeshes)
	{
		child.second.Draw_Mesh_Animation(world, anime, frame, name1, name2, blend);
	}
}

void MESH::Draw_DPP_Mesh_Animation(XMMATRIX& parent_matrix, unordered_map<string, Anim>& anime, DWORD frame, const string& name1, const string& name2, float blend)
{
	XMMATRIX world;

	// 3D�}�g���b�N�X�ݒ�
	{
		if ("none" == name2)
		{
			for (auto i : anime[name1].body)
			{
				if (i.node_name == Name)
				{
					unsigned int f = frame % i.translate.begin()->time;

					XMFLOAT3 pos = i.translate[f].value;
					XMMATRIX trans = XMMatrixTranslation(pos.x, pos.y, pos.z);

					f = frame % i.rotation.begin()->time;

					XMFLOAT4 rotation = i.rotation[f].value;
					XMVECTOR quat = XMLoadFloat4(&rotation);

					world = XMMatrixRotationQuaternion(quat);

					world = XMMatrixMultiply(world, trans);

					world = XMMatrixMultiply(world, parent_matrix);

					auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
					auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

					// ���ʂ̃J�������f�o�b�O�J������?
					if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
					{
						CRenderer::Set_MatrixBuffer(world, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());
					}
					else
					{
						CRenderer::Set_MatrixBuffer(world, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());
					}

					break;
				}
			}
		}
		else
		{
			XMMATRIX trans1, trans2, trans;
			XMVECTOR quat1, quat2, puat;

			for (auto i : anime[name1].body)
			{
				if (i.node_name == Name)
				{
					unsigned int f = frame % i.translate.begin()->time;

					XMFLOAT3 pos = i.translate[f].value;
					trans1 = XMMatrixTranslation(pos.x, pos.y, pos.z);

					f = frame % i.rotation.begin()->time;

					XMFLOAT4 rotation = i.rotation[f].value;
					quat1 = XMLoadFloat4(&rotation);

					break;
				}
			}

			for (auto i : anime[name2].body)
			{
				if (i.node_name == Name)
				{
					unsigned int f = frame % i.translate.begin()->time;

					XMFLOAT3 pos = i.translate[f].value;
					trans2 = XMMatrixTranslation(pos.x, pos.y, pos.z);

					f = frame % i.rotation.begin()->time;

					XMFLOAT4 rotation = i.rotation[f].value;
					quat2 = XMLoadFloat4(&rotation);

					break;
				}
			}

			trans = trans1 * (1.0f - blend) + trans2 * blend;
			puat = XMQuaternionSlerp(quat1, quat2, blend);

			world = XMMatrixRotationQuaternion(puat);

			world = XMMatrixMultiply(world, trans);

			world = XMMatrixMultiply(world, parent_matrix);

			auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
			auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

			// ���ʂ̃J�������f�o�b�O�J������?
			if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
			{
				CRenderer::Set_MatrixBuffer(world, camera01.lock()->Get_Camera_View(), camera01.lock()->Get_Camera_Projection());
			}
			else
			{
				CRenderer::Set_MatrixBuffer(world, camera02.lock()->Get_Camera_View(), camera02.lock()->Get_Camera_Projection());
			}
		}
	}

	CRenderer::SetVertexBuffers(VertexBuffer);

	CRenderer::GetDeviceContext()->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	CRenderer::GetDeviceContext()->DrawIndexed((UINT)Indices.size(), 0, 0);

	for (auto child : ChildMeshes)
	{
		child.second.Draw_DPP_Mesh_Animation(world, anime, frame, name1, name2, blend);
	}
}