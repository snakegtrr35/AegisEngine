#include "FBXmodel.h"

#include	"manager.h"
#include	"Scene.h"
#include	"ShadowMap.h"
#include	"camera.h"



FBXmodel::FBXmodel()
{
}

XMMATRIX FBXmodel::aiMatrixToMatrix(aiMatrix4x4 matrix)
{
	XMMATRIX m;
	m.r[0] = XMLoadFloat4(&XMFLOAT4(matrix.a1, matrix.a2, matrix.a3, matrix.a4));
	m.r[1] = XMLoadFloat4(&XMFLOAT4(matrix.b1, matrix.b2, matrix.b3, matrix.b4));
	m.r[2] = XMLoadFloat4(&XMFLOAT4(matrix.c1, matrix.c2, matrix.c3, matrix.c4));
	m.r[3] = XMLoadFloat4(&XMFLOAT4(matrix.d1, matrix.d2, matrix.d3, matrix.d4));

	return m;
}

aiMatrix4x4 FBXmodel::MatrixToaiMatrix(XMMATRIX matrix)
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



void FBXmodel::Load(const string& FileName)
{
	//モデルの読み込み
	m_Scene = aiImportFile(FileName.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	
	CreateBone(m_Scene->mRootNode);
	
	for (int m = 0; m < m_Scene->mNumMeshes; m++)
	{		
		aiMesh* mesh = m_Scene->mMeshes[m];
		unsigned int vertex_Num = mesh->mNumVertices;
		ANIME_VERTEX* temp_Vertex = new ANIME_VERTEX[mesh->mNumVertices];
		//DEFORM_VERTEX* temp_DeformVertex = new DEFORM_VERTEX[mesh->mNumVertices];

		std::vector<DEFORM_VERTEX*> temp_DeformVertex;
		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			temp_DeformVertex.emplace_back(new DEFORM_VERTEX());
		}

		ID3D11Buffer* vertex_Buffer;

		

		// 頂点バッファ生成
		ANIME_VERTEX* vertex = new ANIME_VERTEX[mesh->mNumVertices];

		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			vertex[i].Position = XMFLOAT3(mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z);

			vertex[i].Normal = XMFLOAT3(mesh->mNormals[i].x, 
				mesh->mNormals[i].y, 
				mesh->mNormals[i].z);
			vertex[i].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[i].TexCoord = XMFLOAT2(0.0f, 1.0f);

			
			temp_DeformVertex[i]->Position = mesh->mVertices[i];
			temp_DeformVertex[i]->Normal = mesh->mVertices[i];
			temp_DeformVertex[i]->DefromPosition = mesh->mVertices[i];
			temp_DeformVertex[i]->DefromNormal = mesh->mVertices[i];
			temp_DeformVertex[i]->BoneNum = 0;

			temp_Vertex[i].Position = vertex[i].Position;
			temp_Vertex[i].Normal = vertex[i].Normal;
			temp_Vertex[i].Diffuse = vertex[i].Diffuse;
			temp_Vertex[i].TexCoord = vertex[i].TexCoord;

			for (int b = 0; b < 4; b++)
			{
				temp_Vertex[i].weight[b] = 0.0f;
				temp_Vertex[i].bone_Index[b] = 511;

				temp_DeformVertex[i]->BoneName[b] = "";
				temp_DeformVertex[i]->BoneWeight[b] = 0.0f;
			}	
		}

		

		for (unsigned int b = 0; b < mesh->mNumBones; b++)
		{

			aiBone* bone = mesh->mBones[b];
			m_Bone[m_BoneIndex[bone->mName.C_Str()]].OffsetMatrix =aiMatrixToMatrix(bone->mOffsetMatrix);

  			for (unsigned int w = 0; w < bone->mNumWeights; w++)
			{
				aiVertexWeight weight = bone->mWeights[w];
				temp_Vertex[weight.mVertexId].bone_Index[temp_DeformVertex[weight.mVertexId]->BoneNum] = m_BoneIndex[bone->mName.C_Str()];
				temp_DeformVertex[weight.mVertexId]->BoneName[temp_DeformVertex[weight.mVertexId]->BoneNum] = bone->mName.C_Str();
				temp_DeformVertex[weight.mVertexId]->BoneWeight[temp_DeformVertex[weight.mVertexId]->BoneNum] = weight.mWeight;
				temp_Vertex[weight.mVertexId].weight[temp_DeformVertex[weight.mVertexId]->BoneNum] = weight.mWeight;
 				temp_DeformVertex[weight.mVertexId]->BoneNum++;
			}
		}


		m_DeformVertex = temp_DeformVertex;

		{
			//頂点バッファ生成
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(ANIME_VERTEX) * mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bd.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA initData;
			ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));

			initData.pSysMem = temp_Vertex;
			initData.SysMemPitch = 0;
			initData.SysMemSlicePitch = 0;

			//CRenderer::GetDevice()->CreateBuffer(&bd, nullptr, &vertex_Buffer);
			CRenderer::GetDevice()->CreateBuffer(&bd, &initData, &vertex_Buffer);

			/*D3D11_MAPPED_SUBRESOURCE ms;
			CRenderer::GetDeviceContext()->Map(vertex_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
			memcpy(ms.pData, temp_Vertex, sizeof(ANIME_VERTEX)* mesh->mNumVertices);
			CRenderer::GetDeviceContext()->Unmap(vertex_Buffer, 0);*/
		}
		

		// インデックスバッファ生成
		int index_Num;
		ID3D11Buffer* index_Beffer;
		{
			std::vector<int> index;
			for (int f = 0; f < mesh->mNumFaces; f++)
			{
				aiFace* face = &mesh->mFaces[f];
				for (unsigned int i = 0; i < face->mNumIndices; i++)
				{
					index.push_back(face->mIndices[i]);
				}
			}

			index_Num = index.size();
			unsigned short* temp_index = new unsigned short[index_Num];
			for (int i = 0; i < index_Num; i++)
			{
				temp_index[i] = (unsigned short)index[i];
			}

			
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(unsigned short) * index_Num;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = temp_index;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &index_Beffer);
			
			delete[] temp_index;
		}

		MESH temp_mesh;
		temp_mesh.VertexBuffer = vertex_Buffer;
		temp_mesh.IndexBuffer = index_Beffer;
		temp_mesh.VertexNum = vertex_Num;
		temp_mesh.IndexNum = index_Num;
		temp_mesh.deform_Vertex = temp_DeformVertex;

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
		
		delete[] temp_Vertex;

		delete vertex;
	}

	// 定数バッファ作成
	{
		ID3D11Buffer* buffer = nullptr;

		D3D11_BUFFER_DESC hBufferDesc;
		hBufferDesc.ByteWidth = sizeof(MATRIX_BUFFER);
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		hBufferDesc.MiscFlags = 0;
		hBufferDesc.StructureByteStride = sizeof(float);

		{
			HRESULT hr = CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, NULL, &buffer);
			if (FAILED(hr))
			{
				FAILDE_ASSERT;
				return;
			}

			MatrixBuffer.reset(buffer);
		}
	}
}


void FBXmodel::UnLoad()
{
	for (auto vertex : m_DeformVertex)
	{
		delete vertex;
	}
	m_DeformVertex.clear();


	for (int m = 0; m < m_MeshNum; m++)
	{
		m_Meshes[m].VertexBuffer->Release();
		m_Meshes[m].IndexBuffer->Release();
	}
	m_Meshes.clear();
	
	aiReleaseImport(m_Scene);
}

//void FBXmodel::Update()
//{
//	
//}

void FBXmodel::Draw(XMMATRIX &Matrix)
{
	frame++;
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

	
	XMFLOAT4X4 bonef[256];

	for (int bf = 0; bf < 256; bf++)
	{
		bonef[bf] = XMFLOAT4X4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	for (int b = 0; b < m_BoneNum; b++)
	{
		XMStoreFloat4x4(&bonef[b], m_Bone[b].Matrix);
	}

	SetBoneMatrix(bonef);

	CRenderer::Set_Shader(SHADER_INDEX_V::ANIMATION, SHADER_INDEX_P::DEFAULT);
	CRenderer::Set_InputLayout(INPUTLAYOUT::ANIMATION);

	DrawMesh(m_Scene->mRootNode, Matrix);

	CRenderer::Set_InputLayout();
	CRenderer::Set_Shader();
}

void FBXmodel::SetBoneMatrix(const XMFLOAT4X4* matrix)
{
	MATRIX_BUFFER bone_matrix;

	for (int b = 0; b < 256; b++)
	{
		bone_matrix.Bone_Matrix[b] = matrix[b];
	}

	CRenderer::GetDeviceContext()->UpdateSubresource(MatrixBuffer.get(), 0, NULL, &bone_matrix, 0, 0);

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
	BONE bone;
	bone.Matrix = XMMatrixIdentity();
	bone.AnimationMatrix = XMMatrixIdentity();
	bone.OffsetMatrix = XMMatrixIdentity();

	m_BoneIndex[Node->mName.C_Str()] = m_BoneNum;

	m_Bone[m_BoneNum] = bone;

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