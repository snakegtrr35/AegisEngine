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
		Aegis::Vector4 Direction;
	};

	//! デプスステンシルビュー
	unique_ptr <ID3D11DepthStencilView, Release>			DepthStencilView;

	//! シェーダーリソースビュー
	unique_ptr <ID3D11ShaderResourceView, Release>		ShaderResourceView;
	unique_ptr <ID3D11ShaderResourceView, Release>		SRV;

	//! ラスタライザステート
	unique_ptr <ID3D11RasterizerState, Release>		RasterizerState;
	//! サンプラー
	unique_ptr < ID3D11SamplerState, Release>			Sampler;
	//! コンスタントバッファ
	unique_ptr <ID3D11Buffer, Release>					ShadowBuffer;
	CONSTANT_SHADOW_MAP									Shadow;

	unique_ptr <ID3D11Buffer, Release>					LightBuffer;
	//! 
	Aegis::Vector3											LightPos;
	//!
	CONSTANT_LIGHT										Light;

	XMMATRIX											ViewMatrix;
	XMMATRIX											PlojectionMatrix;

	RECT												Viewport;
	D3D11_VIEWPORT										DxViewport;

	bool Enable;

	unique_ptr<ID3D11RenderTargetView, Release> RenderTargetView;

	weak_ptr<GAME_OBJECT> Target;

	static UINT WIDTH;
	static UINT HEIGHT;

	void Set_LightPos(const Aegis::Vector3& pos);

public:

    SHADOW_MAP();
	virtual ~SHADOW_MAP() { Uninit(); }

    bool Init();
	void Update();
	void Uninit();
    void Begin();
	void End();

	void Set_SamplerState();

	void Set_Light(const Aegis::Vector3& pos);

	XMMATRIX& Get_View() {
		return ViewMatrix;
	}

	XMMATRIX& Get_Plojection() {
		return PlojectionMatrix;
	}

	bool Get_Enable() {
		return Enable;
	}

	Aegis::Vector3 Get_LightPos() {
		return LightPos;
	}

	ID3D11ShaderResourceView* Get() {
		return ShaderResourceView.get();
	}

	void Set_Target(const weak_ptr<GAME_OBJECT>& object);

	const bool Target_Enable();

	void Set();
};

#endif // !SHADOW_MAP_H