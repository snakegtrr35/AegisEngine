#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H

#include "GameObject.h"
#include "RendererTypeDefine.h"

namespace aegis
{
	class DepthStencilView;
	class ShaderResourceView;
	class RasterizerState;
	class SamplerState;
	class Buffer;
	class RenderTargetView;
}

class SHADOW_MAP {
private:

	struct CONSTANT_SHADOW_MAP
	{
		XMMATRIX ViewMatrix;
		XMMATRIX ProjectionMatrix;
	};

	struct CONSTANT_LIGHT {
		aegis::Vector4 Direction;
	};

	//! デプスステンシルビュー
	aegis::uniquePtr<aegis::DepthStencilView>	DepthStencilView;
	//! シェーダーリソースビュー
	aegis::uniquePtr<aegis::ShaderResourceView>	ShaderResourceView;
	aegis::uniquePtr<aegis::ShaderResourceView>	SRV;
	//! ラスタライザステート
	aegis::uniquePtr<aegis::RasterizerState>		RasterizerState;
	//! サンプラー
	aegis::uniquePtr<aegis::SamplerState>		Sampler;
	//! コンスタントバッファ
	aegis::uniquePtr<aegis::Buffer>				ShadowBuffer;
	CONSTANT_SHADOW_MAP							Shadow;
	aegis::uniquePtr<aegis::Buffer>					LightBuffer;
	//! 
	aegis::Vector3								LightPos;
	//!
	CONSTANT_LIGHT								Light;

	XMMATRIX									ViewMatrix;
	XMMATRIX									PlojectionMatrix;

	RECT										Viewport;
	aegis::ViewPort								DxViewport;

	bool										Enable;

	aegis::uniquePtr<aegis::RenderTargetView>	RenderTargetView;

	std::weak_ptr<GameObject>					Target;

	static UINT WIDTH;
	static UINT HEIGHT;

	void Set_LightPos(const aegis::Vector3& pos);

public:

    SHADOW_MAP();
	virtual ~SHADOW_MAP() { Uninit(); }

    bool Init();
	void Update();
	void Uninit();
    void Begin();
	void End();

	void Set_SamplerState();

	void Set_Light(const aegis::Vector3& pos);

	XMMATRIX& Get_View() {
		return ViewMatrix;
	}

	XMMATRIX& Get_Plojection() {
		return PlojectionMatrix;
	}

	bool Get_Enable() {
		return Enable;
	}

	aegis::Vector3 Get_LightPos() {
		return LightPos;
	}

	aegis::ShaderResourceView* Get() {
		return ShaderResourceView.get();
	}

	void Set_Target(const std::weak_ptr<GameObject>& object);

	const bool Target_Enable();

	void Set();

	aegis::SamplerState* getSampler() { return Sampler.get(); }
};

#endif // !SHADOW_MAP_H