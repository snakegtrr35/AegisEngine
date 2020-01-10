#include	"Mesh.h"

#include	"manager.h"
#include	"Scene.h"
#include	"camera.h"

struct ANIME_VERTEX
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT4 Diffuse;
	XMFLOAT2 TexCoord;
	unsigned int bone_Index[4];
	float weight[4];
};

XMMATRIX aiMatrixToXMMATRIX(aiMatrix4x4 matrix);
aiMatrix4x4 XMMATRIXToaiMatrix(XMMATRIX matrix);

void MESH_ANIM::Load(const char *FileName)
{
	//モデルの読み込み
	m_Scene = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality);

	CreateBone(m_Scene->mRootNode);

	m_Meshes.reserve(m_Scene->mNumMeshes);

	for (int m = 0; m < m_Scene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_Scene->mMeshes[m];
		unsigned int vertex_Num = mesh->mNumVertices;
		//ANIME_VERTEX* temp_Vertex = new ANIME_VERTEX[mesh->mNumVertices];
		vector<ANIME_VERTEX*> temp_vertex;
		temp_vertex.reserve(mesh->mNumVertices);

		vector<DEFORM_VERTEX*> temp_DeformVertex;

		vector<ANIME_VERTEX*> vertex;
		vertex.reserve(mesh->mNumVertices);

		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			temp_DeformVertex.emplace_back(new DEFORM_VERTEX());

			temp_vertex.emplace_back(new ANIME_VERTEX());//

			vertex.emplace_back(new ANIME_VERTEX());//
		}

		ID3D11Buffer* vertex_Buffer;

		// 頂点バッファ生成
		//ANIME_VERTEX* vertex = new ANIME_VERTEX[mesh->mNumVertices];

		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			//vertex[i].Position = XMFLOAT3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			vertex[i]->Position = XMFLOAT3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

			//vertex[i].Normal = XMFLOAT3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			vertex[i]->Normal = XMFLOAT3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

			//vertex[i].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[i]->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			//vertex[i].TexCoord = XMFLOAT2(0.0f, 1.0f);
			// UV座標の設定
			if (mesh->mTextureCoords[0])
			{
				vertex[i]->TexCoord.x = (float)mesh->mTextureCoords[0][i].x;
				vertex[i]->TexCoord.y = (float)mesh->mTextureCoords[0][i].y;
			}

			temp_DeformVertex[i]->Position = mesh->mVertices[i];
			temp_DeformVertex[i]->Normal = mesh->mVertices[i];
			temp_DeformVertex[i]->DefromPosition = mesh->mVertices[i];
			temp_DeformVertex[i]->DefromNormal = mesh->mVertices[i];
			temp_DeformVertex[i]->BoneNum = 0;

			//temp_Vertex[i].Position = vertex[i].Position;
			//temp_Vertex[i].Normal = vertex[i].Normal;
			//temp_Vertex[i].Diffuse = vertex[i].Diffuse;
			//temp_Vertex[i].TexCoord = vertex[i].TexCoord;
			temp_vertex[i]->Position = vertex[i]->Position;//
			temp_vertex[i]->Normal = vertex[i]->Normal;//
			temp_vertex[i]->Diffuse = vertex[i]->Diffuse;//
			temp_vertex[i]->TexCoord = vertex[i]->TexCoord;//


			for (int b = 0; b < 4; b++)
			{
				//temp_Vertex[i].weight[b] = 0.0f;
				//temp_Vertex[i].bone_Index[b] = 511;
				temp_vertex[i]->weight[b] = 0.0f;//
				temp_vertex[i]->bone_Index[b] = 0;//

				temp_DeformVertex[i]->BoneName[b] = "";
				temp_DeformVertex[i]->BoneWeight[b] = 0.0f;
			}
		}



		for (unsigned int b = 0; b < mesh->mNumBones; b++)
		{

			aiBone* bone = mesh->mBones[b];
			m_Bone[m_BoneIndex[bone->mName.C_Str()]].OffsetMatrix = aiMatrixToXMMATRIX(bone->mOffsetMatrix);

			for (unsigned int w = 0; w < bone->mNumWeights; w++)
			{
				aiVertexWeight weight = bone->mWeights[w];
				//temp_Vertex[weight.mVertexId].bone_Index[temp_DeformVertex[weight.mVertexId]->BoneNum] = m_BoneIndex[bone->mName.C_Str()];
				//temp_Vertex[weight.mVertexId].weight[temp_DeformVertex[weight.mVertexId]->BoneNum] = weight.mWeight;
				temp_vertex[weight.mVertexId]->bone_Index[temp_DeformVertex[weight.mVertexId]->BoneNum] = m_BoneIndex[bone->mName.C_Str()];//
				temp_vertex[weight.mVertexId]->weight[temp_DeformVertex[weight.mVertexId]->BoneNum] = weight.mWeight;//
				temp_DeformVertex[weight.mVertexId]->BoneName[temp_DeformVertex[weight.mVertexId]->BoneNum] = bone->mName.C_Str();
				temp_DeformVertex[weight.mVertexId]->BoneWeight[temp_DeformVertex[weight.mVertexId]->BoneNum] = weight.mWeight;
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
			
			initData.pSysMem = temp_vertex.data();
			initData.SysMemPitch = 0;
			initData.SysMemSlicePitch = 0;
			
			CRenderer::GetDevice()->CreateBuffer(&bd, &initData, &vertex_Buffer);
			
			//D3D11_MAPPED_SUBRESOURCE ms;
			//CRenderer::GetDeviceContext()->Map(vertex_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
			////memcpy(ms.pData, temp_Vertex, sizeof(ANIME_VERTEX)* mesh->mNumVertices);
			//memcpy(ms.pData, temp_vertex.data(), sizeof(ANIME_VERTEX)* temp_vertex.size());//
			//CRenderer::GetDeviceContext()->Unmap(vertex_Buffer, 0);
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
					index.emplace_back(face->mIndices[i]);
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

		//delete[] temp_Vertex;
		for (auto v : temp_vertex)
		{
			delete v;
		}
		temp_vertex.clear();//

		//delete vertex;
		for (auto v : vertex)
		{
			delete v;
		}
		vertex.clear();//
	}

	{
		// 定数バッファ作成
		{
			ID3D11Buffer* buffer = nullptr;

			D3D11_BUFFER_DESC hBufferDesc;
			//hBufferDesc.ByteWidth = sizeof(MATRIX_BUFFER);
			hBufferDesc.ByteWidth = sizeof(XMFLOAT4X4) * BONE_MAX;
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
}

void MESH_ANIM::Uninit()
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

void MESH_ANIM::Draw(XMMATRIX& Matrix)
{
	//CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &Textures[0].Texture);

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

			bone->AnimationMatrix = aiMatrixToXMMATRIX(aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos));
		}
	}

	XMMATRIX Identity = XMMatrixIdentity();

	UpdateBoneMatrix(m_Scene->mRootNode, Identity);


	XMFLOAT4X4 bone[BONE_MAX];

	for (int b = 0; b < BONE_MAX; b++)
	{
		bone[b] = XMFLOAT4X4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	short i;

	//for (int b = 0; b < m_BoneNum; b++)
	for (auto m : m_Bone)
	{
		//XMStoreFloat4x4(&bonef[b], m_Bone[b].Matrix);
		XMStoreFloat4x4(&bone[i], m.second.Matrix);
	}

	// ボーンマトリックスの設定
	SetBoneMatrix(bone);

	DrawMesh(m_Scene->mRootNode, Matrix);

	CRenderer::Set_Shader();
}


void MESH_ANIM::DrawMesh(aiNode* Node, XMMATRIX& Matrix)
{
	XMMATRIX world;
	world = XMMatrixTranspose(aiMatrixToXMMATRIX(Node->mTransformation));
	world = XMMatrixMultiply(world, Matrix);

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

void MESH_ANIM::SetBoneMatrix(const XMFLOAT4X4* matrix)
{
	{
		//MATRIX_BUFFER matrix_buffer;
		//matrix_buffer.BoneMatrix = matrix;

		XMFLOAT4X4 matrix_buffer[BONE_MAX];

		for (WORD i = 0; i < BONE_MAX; i++)
		{
			matrix_buffer[i] = matrix[i];
		}


		CRenderer::GetDeviceContext()->UpdateSubresource(MatrixBuffer.get(), 0, NULL, &matrix_buffer, 0, 0);
	}

	auto buffer = MatrixBuffer.get();

	CRenderer::GetDeviceContext()->VSSetConstantBuffers(5, 1, &buffer);
}

void MESH_ANIM::Update(float delta_time)
{

}

void MESH_ANIM::CreateBone(aiNode* Node)
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

void MESH_ANIM::UpdateBoneMatrix(aiNode* Node, XMMATRIX Matrix)
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


XMMATRIX aiMatrixToXMMATRIX(aiMatrix4x4 matrix)
{
	XMMATRIX m;
	m.r[0] = XMLoadFloat4(&XMFLOAT4(matrix.a1, matrix.a2, matrix.a3, matrix.a4));
	m.r[1] = XMLoadFloat4(&XMFLOAT4(matrix.b1, matrix.b2, matrix.b3, matrix.b4));
	m.r[2] = XMLoadFloat4(&XMFLOAT4(matrix.c1, matrix.c2, matrix.c3, matrix.c4));
	m.r[3] = XMLoadFloat4(&XMFLOAT4(matrix.d1, matrix.d2, matrix.d3, matrix.d4));

	return m;
}

aiMatrix4x4 XMMATRIXToaiMatrix(XMMATRIX matrix)
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