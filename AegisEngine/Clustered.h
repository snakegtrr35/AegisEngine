#pragma once

#ifndef FORWARDLUS_H
#define FORWARDLUS_H

#include	"common.h"


static constexpr UINT CLUSTER_X = 16;
static constexpr UINT CLUSTER_Y = 16;
static constexpr UINT CLUSTER_Z = 32;

class CLUSTERED {
private:

	unique_ptr <ID3D11Texture3D, Release>				ClusteredTexture;
	unique_ptr <ID3D11ShaderResourceView, Release>		ClusteredSRV;

	unique_ptr <ID3D11Texture1D, Release>				Light_Index_List;
	unique_ptr <ID3D11ShaderResourceView, Release>		Light_Index_ListSRV;

	BoundingBox Aabb;
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer_BOX;			//! AABBの頂点バッファ

	BoundingFrustum Frustum;
	unique_ptr<ID3D11Buffer, Release> pVertexBuffer_Frustum;		// 頂点バッファ

	unique_ptr<ID3D11Buffer, Release> pIndexBuffer;					//! インデックスバッファ

	XMFLOAT3 Position;
	XMFLOAT3 Rotation;

	void Init_Aabb();
	void Init_Frustum();

	void Draw_Aabb();
	void Draw_Frustum();

	void OverWrite_Aabb();
	void OverWrite_Frustum();

public:

	CLUSTERED();
	~CLUSTERED() { Uninit(); };

	bool Init();
	void Update();
	void Draw();
	void Uninit();
};

#endif // !FORWARDLUS_H