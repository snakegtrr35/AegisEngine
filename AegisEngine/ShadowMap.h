#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H

#include	"main.h"
#include	"Renderer.h"

class GAME_OBJECT;

class SHADOW_MAP {
private:

	struct CONSTANT_SHADOW_MAP
	{
		XMMATRIX ViewMatrix;
		XMMATRIX ProjectionMatrix;
	};

	struct CONSTANT_LIGHT {
		XMFLOAT4 Direction;
	};

	//! デプスステンシルビュー
	unique_ptr <ID3D11DepthStencilView, Release>			DepthStencilView;

	//! シェーダーリソースビュー
	unique_ptr <ID3D11ShaderResourceView, Release>		ShaderResourceView;
	unique_ptr <ID3D11ShaderResourceView, Release>		SRV;

	unique_ptr <ID3D11BlendState, Release>				BlendState;

	//! ラスタライザステート
	unique_ptr <ID3D11RasterizerState, Release>		RasterizerState;
	//! サンプラー
	unique_ptr < ID3D11SamplerState, Release>			Sampler;
	//! コンスタントバッファ
	unique_ptr <ID3D11Buffer, Release>					ShadowBuffer;
	CONSTANT_SHADOW_MAP									Shadow;

	unique_ptr <ID3D11Buffer, Release>					LightBuffer;
	//! 
	XMFLOAT3											LightPos;
	//!
	CONSTANT_LIGHT										Light;

	XMMATRIX											ViewMatrix;
	XMMATRIX											PlojectionMatrix;

	RECT												Viewport;
	D3D11_VIEWPORT										DxViewport;

	bool Enable;

	unique_ptr<ID3D11RenderTargetView, Release> RenderTargetView;

	GAME_OBJECT* Target;

	static float WIDTH;
	static float HEIGHT;

	void Set_LightPos(const XMFLOAT3& pos);

public:

    SHADOW_MAP();
	virtual ~SHADOW_MAP() { Uninit(); }

    bool Init();
	void Update();
	void Uninit();
    void Begin();
	void End();

	void Set_SamplerState();

	void Set_Light(const XMFLOAT3& pos);

	XMMATRIX& Get_View() {
		return ViewMatrix;
	}

	XMMATRIX& Get_Plojection() {
		return PlojectionMatrix;
	}

	bool Get_Enable() {
		return Enable;
	}

	XMFLOAT3 Get_LightPos() {
		return LightPos;
	}

	ID3D11ShaderResourceView* Get() {
		return ShaderResourceView.get();
	}

	void Set_Target(GAME_OBJECT* object);

	void Set();
};

#endif // !SHADOW_MAP_H