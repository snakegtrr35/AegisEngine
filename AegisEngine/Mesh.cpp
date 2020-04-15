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
		tex.FileName.clear();
		SAFE_RELEASE(tex.Texture);
	}
	Textures.clear();

	for (auto& child : ChildMeshes)
	{
		child.second.Uninit();
	}
	ChildMeshes.clear();
}

void MESH::SetAnimation(const string& name, const Anim& animations)
{
	Animation[name] = animations;
}

//void MESH::Add(const string name, const MESH& mesh)
//{
//	ChildMeshes[name] = mesh;
//}

unordered_map<string, MESH>& MESH::Get()
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

		// サブリソースの設定
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

		// サブリソースの設定
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

	if (Indices.empty() || nullptr == Textures[0].Texture) FAILDE_ASSERT;

	if (!Indices.empty() && nullptr != Textures[0].Texture)
	{
		CRenderer::SetVertexBuffers(VertexBuffer);

		CRenderer::GetDeviceContext()->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &Textures[0].Texture);

		// 3Dマトリックス設定
		{
			matrix = XMMatrixMultiply(Matrix, parent_matrix);

			const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
			const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

			// 普通のカメラかデバッグカメラか?
			if (!camera01.expired() /*&& Empty_weak_ptr<CCamera>(camera01)*/)
			{
				// シャドウマップ用の描画か?
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
				// シャドウマップ用の描画か?
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
		// 3Dマトリックス設定
		{
			matrix = XMMatrixMultiply(Matrix, parent_matrix);

			const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
			const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

			// 普通のカメラかデバッグカメラ
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

	// 3Dマトリックス設定
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

					// 普通のカメラかデバッグカメラか?
					if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
					{
						// シャドウマップ用の描画か?
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
						// シャドウマップ用の描画か?
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

			// 普通のカメラかデバッグカメラか?
			if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
			{
				// シャドウマップ用の描画か?
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
				// シャドウマップ用の描画か?
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

	// 3Dマトリックス設定
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

					// 普通のカメラかデバッグカメラか?
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

			// 普通のカメラかデバッグカメラか?
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





////////////////////////////////////////////////////////////////////////////////






MESHS::MESHS() : VertexBuffer(nullptr), IndexBuffer(nullptr)
{
}

void MESHS::Set(const MESHS& meshs)
{
	this->Vertices = meshs.Vertices;
	this->Indices = meshs.Indices;
	this->Matrix = meshs.Matrix;
	this->Name = meshs.Name;
	this->TextureName = meshs.TextureName;
	this->Textures = meshs.Textures;

	this->ChildMeshes.resize(meshs.ChildMeshes.size());

	for (UINT i = 0; i < meshs.ChildMeshes.size(); i++)
	{
		this->ChildMeshes.at(i).Set(meshs.ChildMeshes.at(i));
	}
}

MESHS::MESHS(vector<VERTEX_3D>& vertices, vector<UINT>& indices, string& texture_name, XMMATRIX& matrix, string name) : Name(name), TextureName(texture_name), Matrix(XMMATRIXToXMFLOAT4X4(matrix)), Vertices(vertices), Indices(indices), VertexBuffer(nullptr), IndexBuffer(nullptr)
{
//#ifdef _DEBUG
//	if (false == SetupMesh(vertices))
//	{
//		FAILDE_ASSERT;
//	}
//#else
//	SetupMesh(vertices);
//#endif // _DEBUG
}

#include	"Library/DirectXTex/WICTextureLoader.h"

void MESHS::Init()
{
	{
		SetupMesh();
		{
			vector<VERTEX_3D> vertex;

			vertex.swap(Vertices);
		}

		{
			for (auto& tex : Textures)
			{
				const string directory = "./asset/model/";
				string path = directory + tex.FileName;
				wstring filenamews = wstring(path.begin(), path.end());

				{
					HRESULT hr = CreateWICTextureFromFile(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), filenamews.c_str(), nullptr, &tex.Texture, nullptr, nullptr);
					if (FAILED(hr))
						FAILDE_ASSERT
				}

			}
		}

		for (auto& child : ChildMeshes)
		{
			child.Init();
		}
	}
}

void MESHS::Draw(XMMATRIX& matrix, const vector<TEXTURE_S>& textures)
{
	Draw_Mesh(matrix, textures);
}

void MESHS::Draw_DPP(XMMATRIX& matrix)
{
	Draw_DPP_Mesh(matrix);
}

void MESHS::Update()
{
}

void MESHS::Uninit()
{
	SAFE_RELEASE(VertexBuffer);
	SAFE_RELEASE(IndexBuffer);

	Indices.clear();

	for (auto& tex : Textures)
	{
		tex.FileName.clear();
		SAFE_RELEASE(tex.Texture);
	}
	Textures.clear();

	for (auto& child : ChildMeshes)
	{
		child.Uninit();
	}
	ChildMeshes.clear();
}

vector<MESHS>& MESHS::Get_Meshs()
{
	return ChildMeshes;
}

vector<TEXTURE_S>& MESHS::Get_Textures()
{
	return Textures;
}

const string& MESHS::Get_Name()
{
	return Name;
}

void MESHS::Set_Name(const string& name)
{
	Name = name;
}

const string& MESHS::Get_Texture_Name()
{
	return TextureName;
}

void MESHS::Set_Texture_Name(const string & texture_name)
{
	TextureName = texture_name;
}

void MESHS::SetupMesh(/*vector<VERTEX_3D>& vertices*/)
{
	HRESULT hr;

	// 頂点バッファの生成
	if(nullptr == VertexBuffer && !Vertices.empty())
	{
		D3D11_BUFFER_DESC desc;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VERTEX_3D) * Vertices.size();
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		// サブリソースの設定
		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));

		initData.pSysMem = Vertices.data();

		hr = CRenderer::GetDevice()->CreateBuffer(&desc, &initData, &VertexBuffer);
		if (FAILED(hr))
			FAILDE_ASSERT;
	}

	// インデックスバッファの生成
	if (nullptr == IndexBuffer && !Indices.empty())
	{
		D3D11_BUFFER_DESC desc;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(UINT) * (UINT)Indices.size();
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		// サブリソースの設定
		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));

		initData.pSysMem = Indices.data();

		hr = CRenderer::GetDevice()->CreateBuffer(&desc, &initData, &IndexBuffer);
		if (FAILED(hr))
			FAILDE_ASSERT;
	}
}

void MESHS::Draw_Mesh(XMMATRIX& parent_matrix, const vector<TEXTURE_S>& textures)
{
	XMMATRIX matrix;

	if (!Indices.empty())
	{
		CRenderer::SetVertexBuffers(VertexBuffer);

		CRenderer::GetDeviceContext()->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		for (UINT i = 0; textures.size(); i++)
		{
			if (textures[i].FileName == TextureName)
			{
				CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &textures[i].Texture);
				break;
			}
		}

		//CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &Textures[0].Texture);

		// 3Dマトリックス設定
		{
			matrix = XMMatrixMultiply(XMFLOAT4X4ToXMMATRIX(Matrix), parent_matrix);

			const auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
			const auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

			// 普通のカメラかデバッグカメラか?
			if (!camera01.expired())
			{
				// シャドウマップ用の描画か?
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
				// シャドウマップ用の描画か?
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
		matrix = XMMatrixMultiply(XMFLOAT4X4ToXMMATRIX(Matrix), parent_matrix);
	}

	for (auto child : ChildMeshes)
	{
		child.Draw_Mesh(matrix, textures);
	}
}

void MESHS::Draw_DPP_Mesh(XMMATRIX& parent_matrix)
{
}