#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H

#include	"main.h"
#include	"Renderer.h"

class SHADOW_MAP {
private:

	//! デプスステンシルビュー
	unique_ptr <ID3D11DepthStencilView, Release>		DepthStencilView;
	//! シェーダーリソースビュー
	unique_ptr <ID3D11ShaderResourceView, Release >		ShaderResourceView;
	//! サンプラー
	unique_ptr < ID3D11SamplerState, Release>			Sampler;
	//! コンスタントバッファ
	unique_ptr < ID3D11Buffer, Release>					ShadowBuffer;
	CONSTANT											Shadow;
	//! 
	XMFLOAT3											LightPos;

	XMMATRIX											ViewMatrix;
	XMMATRIX											PlojectionMatrix;

	bool Enable;

public:

    SHADOW_MAP();
	virtual ~SHADOW_MAP() { Uninit(); }

    bool Init(const float width, const float height );
	void Update();
	void Uninit();
    void Begin();
	void End();

	void Set_ShaderResourceView();
	void Set_SamplerState();

	void Set_Buffer(const XMMATRIX& world);

	void Set_LightPos(const XMFLOAT3& pos);

	XMMATRIX& Get_View() {
		return ViewMatrix;
	}

	XMMATRIX& Get_Plojection() {
		return PlojectionMatrix;
	}

	bool Get_Enable() {
		return Enable;
	}
};

#endif // !SHADOW_MAP_H