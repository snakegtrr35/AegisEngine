#include "FBXmodel.h"

#include	"manager.h"
#include	"Scene.h"
#include	"ShadowMap.h"
#include	"camera.h"



XMMATRIX aiMatrixToMatrix(aiMatrix4x4 matrix)
{
	XMMATRIX m;
	m.r[0] = XMLoadFloat4(&XMFLOAT4(matrix.a1, matrix.a2, matrix.a3, matrix.a4));
	m.r[1] = XMLoadFloat4(&XMFLOAT4(matrix.b1, matrix.b2, matrix.b3, matrix.b4));
	m.r[2] = XMLoadFloat4(&XMFLOAT4(matrix.c1, matrix.c2, matrix.c3, matrix.c4));
	m.r[3] = XMLoadFloat4(&XMFLOAT4(matrix.d1, matrix.d2, matrix.d3, matrix.d4));

	return m;
}

aiMatrix4x4 MatrixToaiMatrix(XMMATRIX matrix)
{
	aiMatrix4x4 m;
	m.a1 = XMVectorGetX(matrix.r[0]);
	m.a2 = XMVectorGetY(matrix.r[0]);
	m.a3 = XMVectorGetZ(matrix.r[0]);
	m.a4 = XMVectorGetW(matrix.r[0]);

	m.b1 = XMVectorGetX(matrix.r[1]);
	m.b2 = XMVectorGetY(matrix.r[1]);
	m.b3 = XMVectorGetZ(matrix.r[1]);
	m.b4 = XMVectorGetW(matrix.r[1]);

	m.c1 = XMVectorGetX(matrix.r[2]);
	m.c2 = XMVectorGetY(matrix.r[2]);
	m.c3 = XMVectorGetZ(matrix.r[2]);
	m.c4 = XMVectorGetW(matrix.r[2]);
	
	m.d1 = XMVectorGetX(matrix.r[3]);
	m.d2 = XMVectorGetY(matrix.r[3]);
	m.d3 = XMVectorGetZ(matrix.r[3]);
	m.d4 = XMVectorGetW(matrix.r[3]);
	
	return m;
}



bool FBXmodel::Load(const string& FileName)
{
	HRESULT hr;

	//モデルの読み込み
	m_Scene = aiImportFile(FileName.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	
	if (nullptr == m_Scene)
	{
		return false;
	}

	CreateBone(m_Scene->mRootNode);
	
	for (int m = 0; m < m_Scene->mNumMeshes; m++)
	{		
		aiMesh* mesh = m_Scene->mMeshes[m];
		UINT vertex_Num = mesh->mNumVertices;

		vector<UINT> Bone_num;
		Bone_num.reserve(mesh->mNumVertices);

		vector<ANIME_VERTEX> vertex;
		vertex.reserve(mesh->mNumVertices);

		for (UINT i = 0; i < mesh->mNumVertices; i++)
		{
			Bone_num.emplace_back(0);
			vertex.emplace_back(ANIME_VERTEX());
		}

		for (UINT i = 0; i < mesh->mNumVertices; i++)
		{
			vertex[i].Position = XMFLOAT3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

			vertex[i].Normal = XMFLOAT3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

			vertex[i].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			if (mesh->mTextureCoords[0])
			{
				vertex[i].TexCoord.x = (float)mesh->mTextureCoords[0][i].x;
				vertex[i].TexCoord.y = (float)mesh->mTextureCoords[0][i].y;
			}

			vertex[i].Position;
			vertex[i].Normal;
			vertex[i].Diffuse;
			vertex[i].TexCoord;
		}

		
		for (UINT b = 0; b < mesh->mNumBones; b++)
		{

			aiBone* bone = mesh->mBones[b];
			m_Bone[m_BoneIndex[bone->mName.C_Str()]].OffsetMatrix =aiMatrixToMatrix(bone->mOffsetMatrix);

  			for (unsigned int w = 0; w < bone->mNumWeights; w++)
			{
				aiVertexWeight weight = bone->mWeights[w];
				vertex[weight.mVertexId].bone_Index[Bone_num[weight.mVertexId]] = m_BoneIndex[bone->mName.C_Str()];
				vertex[weight.mVertexId].weight[Bone_num[weight.mVertexId]] = weight.mWeight;
				Bone_num[weight.mVertexId]++;
			}
		}


		//頂点バッファ生成
		ID3D11Buffer* vertex_Buffer;
		{
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(ANIME_VERTEX) * vertex.size();
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA initData;
			ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));

			initData.pSysMem = vertex.data();
			initData.SysMemPitch = 0;
			initData.SysMemSlicePitch = 0;

			hr = CRenderer::GetDevice()->CreateBuffer(&bd, &initData, &vertex_Buffer);

			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}
		}
		

		// インデックスバッファ生成
		int index_Num;
		ID3D11Buffer* index_Beffer;
		{
			vector<WORD> index;
			for (UINT f = 0; f < mesh->mNumFaces; f++)
			{
				aiFace* face = &mesh->mFaces[f];
				for (UINT i = 0; i < face->mNumIndices; i++)
				{
					index.emplace_back(face->mIndices[i]);
				}
			}

			index_Num = index.size();
	
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(WORD) * index.size();
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA initData;
			ZeroMemory(&initData, sizeof(initData));
			initData.pSysMem = index.data();
			initData.SysMemPitch = 0;
			initData.SysMemSlicePitch = 0;

			hr = CRenderer::GetDevice()->CreateBuffer(&bd, &initData, &index_Beffer);
			
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}
		}

		MESH temp_mesh;
		temp_mesh.VertexBuffer= vertex_Buffer;
		temp_mesh.IndexBuffer = index_Beffer;
		temp_mesh.VertexNum = vertex_Num;
		temp_mesh.IndexNum = index_Num;

		m_Meshes.emplace_back(temp_mesh);

		//テクスチャ設定
		for (int m = 0; m < m_Scene->mNumMaterials; m++)
		{
			aiMaterial* material = m_Scene->mMaterials[m];
			aiString texture_file;
			material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texture_file);

			//テクスチャタイプの判定
			if (m_Scene->HasTextures()) {
				//埋め込みテクスチャ
				
			}
			else {
				//外部パステクスチャ
			}
		}

		Bone_num.clear();
		vertex.clear();
	}

	// 定数バッファ作成
	{
		ID3D11Buffer* buffer = nullptr;

		D3D11_BUFFER_DESC hBufferDesc;
		hBufferDesc.ByteWidth = sizeof(XMMATRIX) * m_BoneNum;
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		hBufferDesc.MiscFlags = 0;
		hBufferDesc.StructureByteStride = sizeof(float);

		{
			hr = CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, NULL, &buffer);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return false;
			}

			MatrixBuffer.reset(buffer);
		}
	}

	return true;
}


void FBXmodel::UnLoad()
{
	for (int m = 0; m < m_MeshNum; m++)
	{
		m_Meshes[m].VertexBuffer->Release();
		m_Meshes[m].IndexBuffer->Release();
	}
	m_Meshes.clear();
	
	aiReleaseImport(m_Scene);
}

void FBXmodel::Update(float delta_time)
{
	static float f = 0.0f;

	f += delta_time;

	if (ANIMETION_FRAME <= f)
	{
		frame++;

		f = 0.0f;
	}
	if (0 < m_Scene->mNumAnimations)
	{
		aiAnimation* animation = m_Scene->mAnimations[0];
		for (unsigned int c = 0; c < animation->mNumChannels; c++)
		{
			aiNodeAnim* nodeAnim = animation->mChannels[c];
			BONE* bone = &m_Bone[m_BoneIndex[nodeAnim->mNodeName.C_Str()]];

			int f = frame % nodeAnim->mNumRotationKeys;
			aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

			f = frame % nodeAnim->mNumPositionKeys;
			aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

			bone->AnimationMatrix = aiMatrixToMatrix(aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos));
		}
	}

	XMMATRIX Identity = XMMatrixIdentity();

	UpdateBoneMatrix(m_Scene->mRootNode, Identity);
}

void FBXmodel::Draw(XMMATRIX &Matrix)
{
	{
		vector<XMMATRIX> bone;
		bone.reserve(m_BoneNum);

		for (int b = 0; b < m_BoneNum; b++)
		{
			bone.emplace_back(m_Bone[b].Matrix);
		}

		SetBoneMatrix(bone);
	}

	CRenderer::Set_Shader(SHADER_INDEX_V::ANIMATION, SHADER_INDEX_P::DEFAULT);
	CRenderer::Set_InputLayout(INPUTLAYOUT::ANIMATION);

	DrawMesh(m_Scene->mRootNode, Matrix);

	CRenderer::Set_InputLayout();
	CRenderer::Set_Shader();
}

void FBXmodel::SetBoneMatrix(const vector<XMMATRIX>& matrix)
{
	CRenderer::GetDeviceContext()->UpdateSubresource(MatrixBuffer.get(), 0, NULL, matrix.data(), 0, 0);

	auto buffer = MatrixBuffer.get();

	CRenderer::GetDeviceContext()->VSSetConstantBuffers(5, 1, &buffer);
}

void FBXmodel::DrawMesh(aiNode * Node, XMMATRIX &Matrix)
{
	XMMATRIX world;
	world = XMMatrixTranspose(aiMatrixToMatrix(Node->mTransformation));
	world *= Matrix;

	XMFLOAT4X4 worldf;
	XMStoreFloat4x4(&worldf, world);

	for (int n = 0; n < Node->mNumMeshes; n++)
	{
		unsigned int m = Node->mMeshes[n];

		// 3Dマトリックス設定
		{
			auto camera01 = CManager::Get_Scene()->Get_Game_Object<CCamera>("camera");
			auto camera02 = CManager::Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

			// 普通のカメラかデバッグカメラか?
			if (nullptr != camera01)
			{
				// シャドウマップ用の描画か?
				if (CManager::Get_ShadowMap()->Get_Enable())
				{
					XMMATRIX view = CManager::Get_ShadowMap()->Get_View();
					XMMATRIX proj = CManager::Get_ShadowMap()->Get_Plojection();

					CRenderer::Set_MatrixBuffer(world, view, proj);
				}
				else
				{
					CRenderer::Set_MatrixBuffer(world, camera01->Get_Camera_View(), camera01->Get_Camera_Projection());

					CRenderer::Set_MatrixBuffer01(*camera01->Get_Pos());
				}
			}
			else
			{
				// シャドウマップ用の描画か?
				if (CManager::Get_ShadowMap()->Get_Enable())
				{
					XMMATRIX view = CManager::Get_ShadowMap()->Get_View();
					XMMATRIX proj = CManager::Get_ShadowMap()->Get_Plojection();

					CRenderer::Set_MatrixBuffer(world, view, proj);
				}
				else
				{
					CRenderer::Set_MatrixBuffer(world, camera02->Get_Camera_View(), camera02->Get_Camera_Projection());

					CRenderer::Set_MatrixBuffer01(*camera02->Get_Pos());
				}
			}
		}

		// 頂点バッファの設定
		{
			const UINT stride = sizeof(ANIME_VERTEX);
			const UINT offset = 0;
			ID3D11Buffer* vb[1] = { m_Meshes[m].VertexBuffer };
			CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, vb, &stride, &offset);
		}

		// インデックスバッファの設定
		{
			CRenderer::SetIndexBuffer(m_Meshes[m].IndexBuffer);
		}

		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		CRenderer::DrawIndexed(m_Meshes[m].IndexNum, 0, 0);
	}

	for (unsigned int i = 0; i < Node->mNumChildren; i++)
	{
		DrawMesh(Node->mChildren[i], world);
	}
}


void FBXmodel::CreateBone(aiNode * Node)
{
	m_BoneIndex[Node->mName.C_Str()] = m_BoneNum;

	m_Bone[m_BoneNum] = BONE();

	m_BoneNum++;

	for (unsigned int i = 0; i < Node->mNumChildren; i++)
	{
		CreateBone(Node->mChildren[i]);
	}
}


void FBXmodel::UpdateBoneMatrix(aiNode * Node, XMMATRIX Matrix)
{
	BONE* bone = &m_Bone[m_BoneIndex[Node->mName.C_Str()]];
	XMMATRIX world;
	XMMATRIX matrix, aMatrix;

	matrix = Matrix;
	aMatrix = bone->AnimationMatrix;
	
	world = matrix * aMatrix;
	
	bone->Matrix = world * bone->OffsetMatrix;

	for (unsigned int i = 0; i < Node->mNumChildren; i++)
	{
		UpdateBoneMatrix(Node->mChildren[i], world);
	}
}