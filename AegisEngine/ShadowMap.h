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
	ComPtr<ID3D11DepthStencilView>		DepthStencilView;
	//! シェーダーリソースビュー
	ComPtr<ID3D11ShaderResourceView>	ShaderResourceView;
	ComPtr<ID3D11ShaderResourceView>	SRV;
	//! ラスタライザステート
	ComPtr<ID3D11RasterizerState>		RasterizerState;
	//! サンプラー
	ComPtr<ID3D11SamplerState>			Sampler;
	//! コンスタントバッファ
	ComPtr<ID3D11Buffer>				ShadowBuffer;
	CONSTANT_SHADOW_MAP					Shadow;
	ComPtr<ID3D11Buffer>				LightBuffer;
	//! 
	Aegis::Vector3						LightPos;
	//!
	CONSTANT_LIGHT						Light;

	XMMATRIX							ViewMatrix;
	XMMATRIX							PlojectionMatrix;

	RECT								Viewport;
	D3D11_VIEWPORT						DxViewport;

	bool Enable;

	ComPtr<ID3D11RenderTargetView> RenderTargetView;

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
		return ShaderResourceView.Get();
	}

	void Set_Target(const weak_ptr<GAME_OBJECT>& object);

	const bool Target_Enable();

	void Set();
};

#endif // !SHADOW_MAP_H