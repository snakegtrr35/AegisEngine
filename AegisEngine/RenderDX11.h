#pragma once

#include "Renderer.h"
#include "TextureDX11.h"

#include <d3d11.h>
#include <dxgi1_2.h>
#include <dxgi1_4.h>
#include <d2d1_1.h>
#include <dwrite.h>

namespace aegis
{
	struct RenderTargetViewDX11 : RenderTargetView
	{
		ComPtr<ID3D11RenderTargetView> RenderTargetView;

		bool IsEmpty() const override;
		void Release() override;
	};

	struct DepthStencilViewDX11 : DepthStencilView
	{
		ComPtr<ID3D11DepthStencilView> DepthStencilView;

		bool IsEmpty() const override;
		void Release() override;
	};

	struct RasterizerStateDX11 : RasterizerState
	{
		ComPtr<ID3D11RasterizerState> RasterizerState;

		bool IsEmpty() const override;
		void Release() override;
	};

	struct BufferDX11 : Buffer
	{
		ComPtr<ID3D11Buffer> Buffer;

		bool IsEmpty() const override;
		void Release() override;
	};

	struct Texture1DDX11 : Texture2D
	{
		ComPtr<ID3D11Texture1D> Texture;

		bool IsEmpty() const override;
		void Release() override;
	};

	struct Texture2DDX11 : Texture2D
	{
		ComPtr<ID3D11Texture2D> Texture;

		bool IsEmpty() const override;
		void Release() override;
	};

	struct Texture3DDX11 : Texture3D
	{
		ComPtr<ID3D11Texture3D> Texture;

		bool IsEmpty() const override;
		void Release() override;
	};

	struct ShaderResourceViewDX11 : ShaderResourceView
	{
		ComPtr<ID3D11ShaderResourceView> ShaderResourceView;

		bool IsEmpty() const override;
		void Release() override;
	};

	struct UnorderedAccessViewDX11 : UnorderedAccessView
	{
		ComPtr<ID3D11UnorderedAccessView> UnorderedAccessView;

		bool IsEmpty() const override;
		void Release() override;
	};

	struct SamplerStateDX11 : SamplerState
	{
		ComPtr<ID3D11SamplerState> Sampler;

		bool IsEmpty() const override;
		void Release() override;
	};

	struct PixelShaderDX11 : PixelShader
	{
		ComPtr<ID3D11PixelShader> Shader;

		bool IsEmpty() const override;
		void Release() override;
	};

	struct VertexShaderDX11 : VertexShader
	{
		ComPtr<ID3D11VertexShader> Shader;

		bool IsEmpty() const override;
		void Release() override;
	};

	struct ComputeShaderDX11 : ComputeShader
	{

		ComPtr<ID3D11ComputeShader> Shader;

		bool IsEmpty() const override;
		void Release() override;
	};

	struct TEXT_FOMAT {
		aegis::string FontName;
		float FontSize = 32.0f;
		DWRITE_FONT_WEIGHT Weight = DWRITE_FONT_WEIGHT_NORMAL;
		DWRITE_FONT_STYLE Style = DWRITE_FONT_STYLE_NORMAL;
		DWRITE_FONT_STRETCH Stretch = DWRITE_FONT_STRETCH_NORMAL;
	};

	struct TEXT_LAYOUT {
		aegis::string Text;
		float Width;
		float Height;
	};

	class RenderDX11 : public aegis::CRenderer
	{
	public:

		RenderDX11();
		~RenderDX11();

		bool Init() override;
		void Uninit() override;
		void Begin() override;
		void End() override;

		ID3D11Device* GetDevice() const { return m_D3DDevice.Get(); }
		ID3D11DeviceContext* GetDeviceContext() const { return m_ImmediateContext.Get(); }

		// 閾ｪ蜑・
		/**
		* @brief 繝悶Ξ繝ｳ繝峨せ繝・・繝郁ｨｭ螳・
		*/
		void SetBlendState(BlendDesc* blendState = nullptr, bool flag = false) override;
		/**
		* @brief 蜉邂怜粋謌千畑繝悶Ξ繝ｳ繝・繧ｹ繝・・繝医・縺溘ａ縺ｮ繧定ｨｭ螳壹ｒ蜿門ｾ・
		*/
		void GetAddBlendState(BlendDesc& blendState) override;
		/**
		* @brief 貂帷ｮ怜粋謌千畑繝悶Ξ繝ｳ繝・繧ｹ繝・・繝医・縺溘ａ縺ｮ繧定ｨｭ螳壹ｒ蜿門ｾ・
		*/
		void GetSubtractBlendState(BlendDesc& blendState) override;
		/**
		* @brief 2D逕ｨ縺ｮ繝悶Ξ繝ｳ繝・繧ｹ繝・・繝医・縺溘ａ縺ｮ繧定ｨｭ螳壹ｒ蜿門ｾ励☆繧・
		*/
		void Get2DBlendState(BlendDesc& blendState) override;
		/**
		* @brief 豺ｱ蠎ｦ繧ｹ繝・・繝医・譛牙柑辟｡蜉ｹ縺ｮ蛻・ｊ譖ｿ縺・
		*/
		void SetDepthEnable(bool Enable) override;
		/**
		* @brief 繧ｦ繧､繝ｳ繝峨え縺ｮ蛻・ｊ譖ｿ縺・
		*/
		void Change_Window_Mode();

		/**
		* @brief 2D逕ｨ縺ｮ繝ｯ繝ｼ繝ｫ繝峨・繝医Μ繝・け繧ｹ險ｭ螳・
		* @param[in] scaling縲諡｡螟ｧ邵ｮ蟆上・繝槭ヨ繝ｪ繝・け繧ｹ
		* @param[in] rotation縲蝗櫁ｻ｢縺ｮ繝槭ヨ繝ｪ繝・け繧ｹ
		* @details 2D逕ｨ縺ｮ繝ｯ繝ｼ繝ｫ繝峨・繝医Μ繝・け繧ｹ險ｭ螳壹ｒ險ｭ螳壹☆繧・
		*/
		void SetWorldViewProjection2D(const XMFLOAT3& scaling = XMFLOAT3(1.0f, 1.0f, 1.0f), const XMFLOAT3& rotation = XMFLOAT3(0.0f, 0.0f, 0.0f)) override;

		/**
		* @brief 繧ｳ繝ｳ繧ｹ繧ｿ繝ｳ繝医ヰ繝・ヵ繧｡險ｭ螳・
		* @param[in] world縲繝ｯ繝ｼ繝ｫ繝峨・繝医Μ繝・け繧ｹ
		* @param[in] view縲繝薙Η繝ｼ繝槭ヨ繝ｪ繝・け繧ｹ
		* @param[in] projection縲繝励Ο繧ｸ繧ｧ繧ｯ繧ｷ繝ｧ繝ｳ繝槭ヨ繝ｪ繝・け繧ｹ
		* @details 繧ｳ繝ｳ繧ｹ繧ｿ繝ｳ繝医ヰ繝・ヵ繧｡繧定ｨｭ螳壹☆繧・
		*/
		void Set_MatrixBuffer(const XMMATRIX world, const XMMATRIX view, const XMMATRIX projection) override;

		void Set_MatrixBuffer01(const XMVECTOR camera_pos) override;

		/**
		* @brief 繝槭ユ繝ｪ繧｢繝ｫ險ｭ螳・
		* @param[in] Material縲繝槭ユ繝ｪ繧｢繝ｫ
		* @details 繝槭ユ繝ｪ繧｢繝ｫ險ｭ螳壹ｒ險ｭ螳壹☆繧・
		*/
		void SetMaterial(aegis::MATERIAL Material) override;

		/**
		* @brief 繝・ぅ繝ｬ繧ｯ繧ｷ繝ｧ繝翫Ν繝ｩ繧､繝郁ｨｭ螳・
		* @param[in] Light縲繝・ぅ繝ｬ繧ｯ繧ｷ繝ｧ繝翫Ν繝ｩ繧､繝・
		* @details 繝・ぅ繝ｬ繧ｯ繧ｷ繝ｧ繝翫Ν繝ｩ繧､繝郁ｨｭ螳壹ｒ險ｭ螳壹☆繧・
		*/
		void SetLight(aegis::LIGHT* Light) override;

		/**
		* @brief 繝・ぅ繝ｬ繧ｯ繧ｷ繝ｧ繝翫Ν繝ｩ繧､繝医・蛻晄悄蛹・
		* @details 繝・ぅ繝ｬ繧ｯ繧ｷ繝ｧ繝翫Ν繝ｩ繧､繝郁ｨｭ螳壹ｒ蛻晄悄蛹悶☆繧・
		*/
		void Light_Identity() override;

		/**
		* @brief 繝ｩ繧ｹ繧ｿ繝ｩ繧､繧ｺ繧ｹ繝・・繝医・險ｭ螳・
		* @details
		*/
		void Set_RasterizerState() override;

		DepthStencilView* CreateDepthStencilView(Texture2D* texture, DepthStencilViewDesc depthStencilViewDesc) const override;

		void SetRenderTargets(uint32 numViews, RenderTargetView* const* renderTargetViews, DepthStencilView* depthStencilView) const override;

		void ClearRenderTargetView(RenderTargetView* renderTargetView, float32 color[4]) const override;

		void ClearDepthStencilView(DepthStencilView* depthStencilView, ClearFlag clearFlags, float32 depth, uint8 stencil) const override;

		void SetViewports(uint32 numViewports, ViewPort* const* viewports) const override;

		RasterizerState* CreateRasterizeState(RasterizeState rasterizeState) const override;

		void SetRasterizeState(RasterizerState* rasterizerState) const override;

		//**
		/* @brief 繝舌ャ繝輔ぃ菴懈・
		/* @param[in] desc縲繝舌ャ繝輔ぃ繝ｪ繧ｽ繝ｼ繧ｹ螳夂ｾｩ
		/* @param[in] initData縲蛻晄悄蛹悶ョ繝ｼ繧ｿ
		/* @details 繝舌ャ繝輔ぃ繝ｪ繧ｽ繝ｼ繧ｹ螳夂ｾｩ縺ｫ蝓ｺ縺･縺・◆繝舌ャ繝輔ぃ繧剃ｽ懈・縺吶ｋ
		/*/
		Buffer* CreateBuffer(const BufferDesc& desc, const SubresourceData initData) override;

		//**
		/* @brief 1D繝・け繧ｹ繝√Ε縺ｮ驟榊・繧剃ｽ懈・
		/* @param[in] desc縲1D繝・け繧ｹ繝√Ε繝ｪ繧ｽ繝ｼ繧ｹ螳夂ｾｩ
		/* @param[in] initData縲蛻晄悄蛹悶ョ繝ｼ繧ｿ
		/* @return Texture1D 菴懈・縺輔ｌ縺・D繝・け繧ｹ繝√Ε繝ｪ繧ｽ繝ｼ繧ｹ
		/* @details 1D繝・け繧ｹ繝√Ε縺ｮ驟榊・繧剃ｽ懈・縺励∪縺・
		/*/
		virtual Texture1D* CreateTexture1D(Texture1DDesc desc, SubresourceData* initialData) const override;

		//**
		/* @brief 2D繝・け繧ｹ繝√Ε縺ｮ驟榊・繧剃ｽ懈・
		/* @param[in] desc縲2D繝・け繧ｹ繝√Ε繝ｪ繧ｽ繝ｼ繧ｹ螳夂ｾｩ
		/* @param[in] initData縲蛻晄悄蛹悶ョ繝ｼ繧ｿ
		/* @return Texture2D 菴懈・縺輔ｌ縺・D繝・け繧ｹ繝√Ε繝ｪ繧ｽ繝ｼ繧ｹ
		/* @details 2D繝・け繧ｹ繝√Ε縺ｮ驟榊・繧剃ｽ懈・縺励∪縺・
		/*/
		Texture2D* CreateTexture2D(Texture2DDesc desc, SubresourceData* initialData) const override;

		//**
		/* @brief 3D繝・け繧ｹ繝√Ε縺ｮ驟榊・繧剃ｽ懈・
		/* @param[in] desc縲3D繝・け繧ｹ繝√Ε繝ｪ繧ｽ繝ｼ繧ｹ螳夂ｾｩ
		/* @param[in] initData縲蛻晄悄蛹悶ョ繝ｼ繧ｿ
		/* @return Texture2D 菴懈・縺輔ｌ縺・D繝・け繧ｹ繝√Ε繝ｪ繧ｽ繝ｼ繧ｹ
		/* @details 3D繝・け繧ｹ繝√Ε縺ｮ驟榊・繧剃ｽ懈・縺励∪縺・
		/*/
		virtual Texture3D* CreateTexture3D(Texture3DDesc desc, SubresourceData* initialData) const override;

		ShaderResourceView* CreateShaderResourceView(Texture* texture, ShaderResourceViewDesc srvDesc) const override;

		UnorderedAccessView* CreateUnorderedAccessView(Texture* texture, UnorderedAccessViewDesc* unorderedAccessViewDesc) const override;

		void ClearUnorderedAccessView(UnorderedAccessView* unorderedAccessView, float32 clearValues[4]) const override;
		void ClearUnorderedAccessView(UnorderedAccessView* unorderedAccessView, uint32 clearValues[4]) const override;

		void CSSetUnorderedAccessViews(uint32 startSlot, uint32 numBuffers, UnorderedAccessView* const* unorderedAccessViews) const override;

		/**
		* @brief 鬆らせ繝舌ャ繝輔ぃ縺ｮ險ｭ螳・
		* @details 鬆らせ繝舌ャ繝輔ぃ險ｭ螳壹ｒ險ｭ螳壹☆繧・
		*/
		void SetVertexBuffers(Buffer* vertexBuffer, uint32 stride = sizeof(VERTEX_3D)) override;

		/**
		* @brief 鬆らせ繝舌ャ繝輔ぃ縺ｮ險ｭ螳・繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｷ繝ｳ繧ｰ逕ｨ)
		* @details 繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｷ繝ｳ繧ｰ逕ｨ縺ｮ鬆らせ繝舌ャ繝輔ぃ險ｭ螳壹ｒ險ｭ螳壹☆繧・
		*/
		void SetVertexBuffers(Buffer* indexBuffer, Buffer* instancingBuffer, UINT size) override;

		/**
		* @brief 繧､繝ｳ繝・ャ繧ｯ繧ｹ繝舌ャ繝輔ぃ縺ｮ險ｭ螳・
		* @details 繧､繝ｳ繝・ャ繧ｯ繧ｹ繝舌ャ繝輔ぃ險ｭ螳壹ｒ險ｭ螳壹☆繧・
		*/
		void SetIndexBuffer(Buffer* indexBuffer, Format format = Format::R16Uint) const override;

		PixelShader* CreatePixelShader(void* shaderData, uint32 size) const override;
		VertexShader* CreateVertexShader(void* shaderData, uint32 size) const override;
		ComputeShader* CreateComputeShader(void* shaderData, uint32 size) const override;

		void Draw(uint32 indexCount, uint32 startVertexLocation) override;
		void DrawIndexed(uint32 iIndexCount, uint32 startIndexLocation, int32 baseVertexLocation) override;
		void DrawIndexedInstanced(int32 indexCountPerInstance, uint32 instanceCount, uint32 startIndexLocation, int32 baseVertexLocation, uint32 startInstanceLocation) override;

		void Dispatch(uint32 threadGroupCountX, uint32 threadGroupCountY, uint32 threadGroupCountZ) const override;

		void CSSetShader(ComputeShader* pShader) const override;

		//**
		/* @brief 鬆らせ繧ｷ繧ｧ繝ｼ繝繝ｼ縺ｧ菴ｿ逕ｨ縺輔ｌ繧句ｮ壽焚繝舌ャ繝輔ぃ繝ｼ繧定ｨｭ螳・
		/* @param[in] startSlot 0縺九ｉ蟋九∪繧矩・蛻励↓繧､繝ｳ繝・ャ繧ｯ繧ｹ繧剃ｻ倥￠縺ｦ縲∝ｮ壽焚繝舌ャ繝輔ぃ繝ｼ縺ｮ險ｭ螳壹＠縺ｾ縺・
		/* @param[in] numBuffers 險ｭ螳壹☆繧九ヰ繝・ヵ繧｡繝ｼ縺ｮ謨ｰ
		/* @param[in] pBuffers 螳壽焚繝舌ャ繝輔ぃ繝ｼ繝ｪ繧ｽ繝ｼ繧ｹ縺ｮ驟榊・
		/* @details 鬆らせ繧ｷ繧ｧ繝ｼ繝繝ｼ繝代う繝励Λ繧､繝ｳ繧ｹ繝・・繧ｸ縺ｧ菴ｿ逕ｨ縺輔ｌ繧句ｮ壽焚繝舌ャ繝輔ぃ繝ｼ繧定ｨｭ螳壹☆繧・
		/*/
		void VSSetConstantBuffers(uint32 startSlot, uint32 numBuffers, Buffer* const* pBuffers) override;

		//**
		/* @brief 繝斐け繧ｻ繝ｫ繧ｷ繧ｧ繝ｼ繝繝ｼ縺ｧ菴ｿ逕ｨ縺輔ｌ繧句ｮ壽焚繝舌ャ繝輔ぃ繝ｼ繧定ｨｭ螳・
		/* @param[in] startSlot 0縺九ｉ蟋九∪繧矩・蛻励↓繧､繝ｳ繝・ャ繧ｯ繧ｹ繧剃ｻ倥￠縺ｦ縲∝ｮ壽焚繝舌ャ繝輔ぃ繝ｼ縺ｮ險ｭ螳壹＠縺ｾ縺・
		/* @param[in] numBuffers 險ｭ螳壹☆繧九ヰ繝・ヵ繧｡繝ｼ縺ｮ謨ｰ
		/* @param[in] pBuffers 螳壽焚繝舌ャ繝輔ぃ繝ｼ繝ｪ繧ｽ繝ｼ繧ｹ縺ｮ驟榊・
		/* @details 繝斐け繧ｻ繝ｫ繧ｷ繧ｧ繝ｼ繝繝ｼ繝代う繝励Λ繧､繝ｳ繧ｹ繝・・繧ｸ縺ｧ菴ｿ逕ｨ縺輔ｌ繧句ｮ壽焚繝舌ャ繝輔ぃ繝ｼ繧定ｨｭ螳壹☆繧・
		/*/
		void PSSetConstantBuffers(uint32 startSlot, uint32 numBuffers, Buffer* const* pBuffers) override;

		//**
		/* @brief 繧ｳ繝ｳ繝斐Η繝ｼ繝医す繧ｧ繝ｼ繝繝ｼ縺ｧ菴ｿ逕ｨ縺輔ｌ繧句ｮ壽焚繝舌ャ繝輔ぃ繝ｼ繧定ｨｭ螳・
		/* @param[in] startSlot 0縺九ｉ蟋九∪繧矩・蛻励↓繧､繝ｳ繝・ャ繧ｯ繧ｹ繧剃ｻ倥￠縺ｦ縲∝ｮ壽焚繝舌ャ繝輔ぃ繝ｼ縺ｮ險ｭ螳壹＠縺ｾ縺・
		/* @param[in] numBuffers 險ｭ螳壹☆繧九ヰ繝・ヵ繧｡繝ｼ縺ｮ謨ｰ
		/* @param[in] pBuffers 螳壽焚繝舌ャ繝輔ぃ繝ｼ繝ｪ繧ｽ繝ｼ繧ｹ縺ｮ驟榊・
		/* @details 繧ｳ繝ｳ繝斐Η繝ｼ繝医す繧ｧ繝ｼ繝繝ｼ繝代う繝励Λ繧､繝ｳ繧ｹ繝・・繧ｸ縺ｧ菴ｿ逕ｨ縺輔ｌ繧句ｮ壽焚繝舌ャ繝輔ぃ繝ｼ繧定ｨｭ螳壹☆繧・
		/*/
		void CSSetConstantBuffers(uint32 startSlot, uint32 numBuffers, Buffer* const* pBuffers) override;

		void PSSetShaderResources(uint32 startSlot, uint32 numBuffers, ShaderResourceView* const* shaderResourceViews) const override;

		void CSSetShaderResources(uint32 startSlot, uint32 numBuffers, ShaderResourceView* const* shaderResourceViews) const override;

		SamplerState* CreateSampler(const SamplerDesc desc) const override;

		//**
		/* @brief 繧ｵ繝ｳ繝励Λ繝ｼ迥ｶ諷九・驟榊・繧偵ヴ繧ｯ繧ｻ繝ｫ繧ｷ繧ｧ繝ｼ繝繝ｼ縺ｫ險ｭ螳壹＠縺ｾ縺・
		/* @param[in] startSlot 0縺九ｉ蟋九∪繧矩・蛻励↓繧､繝ｳ繝・ャ繧ｯ繧ｹ繧剃ｻ倥￠縺ｦ縲√し繝ｳ繝励Λ繝ｼ縺ｮ險ｭ螳壹＠縺ｾ縺・
		/* @param[in] numBuffers 險ｭ螳壹☆繧九し繝ｳ繝励Λ繝ｼ縺ｮ謨ｰ
		/* @param[in] pBuffers 繧ｵ繝ｳ繝励Λ繝ｼ縺ｮ驟榊・
		/* @details 繧ｵ繝ｳ繝励Λ繝ｼ迥ｶ諷九・驟榊・繧偵ヴ繧ｯ繧ｻ繝ｫ繧ｷ繧ｧ繝ｼ繝繝ｼ繝代う繝励Λ繧､繝ｳ繧ｹ繝・・繧ｸ縺ｫ險ｭ螳壹＠縺ｾ縺・
		/*/
		void PSSetSamplers(UINT startSlot, UINT numSamplers, SamplerState* const* ppSamplers) override;

		//**
		/* @brief 繧ｵ繝ｳ繝励Λ繝ｼ迥ｶ諷九・驟榊・繧偵さ繝ｳ繝斐Η繝ｼ繝医す繧ｧ繝ｼ繝繝ｼ縺ｫ險ｭ螳壹＠縺ｾ縺・
		/* @param[in] startSlot 0縺九ｉ蟋九∪繧矩・蛻励↓繧､繝ｳ繝・ャ繧ｯ繧ｹ繧剃ｻ倥￠縺ｦ縲√し繝ｳ繝励Λ繝ｼ縺ｮ險ｭ螳壹＠縺ｾ縺・
		/* @param[in] numBuffers 險ｭ螳壹☆繧九し繝ｳ繝励Λ繝ｼ縺ｮ謨ｰ
		/* @param[in] pBuffers 繧ｵ繝ｳ繝励Λ繝ｼ縺ｮ驟榊・
		/* @details 繧ｵ繝ｳ繝励Λ繝ｼ迥ｶ諷九・驟榊・繧偵さ繝ｳ繝斐Η繝ｼ繝医す繧ｧ繝ｼ繝繝ｼ繝代う繝励Λ繧､繝ｳ繧ｹ繝・・繧ｸ縺ｫ險ｭ螳壹＠縺ｾ縺・
		/*/
		void CSSetSamplers(UINT startSlot, UINT numSamplers, SamplerState* const* ppSamplers) override;

		//**
		/* @brief 繝励Μ繝溘ユ繧｣繝悶ヨ繝昴Ο繧ｸ繝ｼ縺ｮ險ｭ螳・
		/* @param[in] primitiveTopology縲繝励Μ繝溘ユ繧｣繝悶ヨ繝昴Ο繧ｸ繝ｼ
		/* @details 繝励Μ繝溘ユ繧｣繝門梛縺ｫ髢｢縺吶ｋ諠・ｱ縺ｮ險ｭ螳・
		/*/
		void SetPrimitiveTopology(PrimitiveTopology primitiveTopology) override;

		//**
		/* @brief 繝ｪ繧ｽ繝ｼ繧ｹ縺ｫ蟇ｾ縺吶ｋCPU繝・・繧ｿ縺ｧ縺ｮ譖ｴ譁ｰ繧定｡後≧
		/* @param[in] buffer繝ｪ繧ｽ繝ｼ繧ｹ
		/* @param[in] pData 譖ｴ譁ｰ・井ｸ頑嶌縺搾ｼ峨☆繧気PU繝・・繧ｿ繝昴う繝ｳ繧ｿ
		/* @details 繝ｪ繧ｽ繝ｼ繧ｹ縺ｫ蟇ｾ縺吶ｋCPU繝・・繧ｿ縺ｧ縺ｮ譖ｴ譁ｰ・井ｸ頑嶌縺搾ｼ峨ｒ陦後≧
		/*/
		void UpdateSubresource(Buffer* buffer, void* pData) override;

		//**
		/* @brief 繝ｪ繧ｽ繝ｼ繧ｹ縺ｫ蟇ｾ縺吶ｋCPU繝・・繧ｿ縺ｧ縺ｮ荳頑嶌縺阪ｒ陦後≧
		/* @param[in] buffer縲繝ｪ繧ｽ繝ｼ繧ｹ
		/* @param[in] pData縲荳頑嶌縺阪☆繧気PU繝・・繧ｿ繝昴う繝ｳ繧ｿ
		/* @param[in] dataSize縲荳頑嶌縺阪☆繧気PU繝・・繧ｿ繧ｵ繧､繧ｺ
		/* @details 繝ｪ繧ｽ繝ｼ繧ｹ縺ｫ蟇ｾ縺吶ｋ縲√Ο繝・け繧偵＠縺ｦCPU繝・・繧ｿ縺ｧ縺ｮ荳頑嶌縺阪ｒ陦後≧
		/*/
		void Map(Buffer* buffer, void* pData, uint32 dataSize) override;

		//**
		/* @brief 繝ｪ繧ｽ繝ｼ繧ｹ縺ｫ蟇ｾ縺吶ｋ荳頑嶌縺阪・縺溘ａ縺ｮ繝ｭ繝・け繧偵☆繧・
		/* @param[in] buffer 繝ｪ繧ｽ繝ｼ繧ｹ
		/* @return MappedSubresource 繝・ヰ繧､繧ｹ繧ｳ繝ｳ繝・く繧ｹ繝医・繝昴う繝ｳ繧ｿ
		/* @details 繝ｪ繧ｽ繝ｼ繧ｹ縺ｫ蟇ｾ縺吶ｋ縲，PU繝・・繧ｿ縺ｧ縺ｮ荳頑嶌縺阪・縺溘ａ縺ｮ繝ｭ繝・け繧定｡後≧
		/*/
		MappedSubresource Map(Buffer* buffer) override;

		//**
		/* @brief 繝ｪ繧ｽ繝ｼ繧ｹ縺ｫ蟇ｾ縺吶ｋ譖ｸ縺崎ｾｼ縺ｿ繝ｻ隱ｭ縺ｿ霎ｼ縺ｿ繝ｭ繝・け繧定ｧ｣髯､縺吶ｋ
		/* @param[in] buffer縲繝ｭ繝・け繧定ｧ｣髯､縺吶ｋ繝ｪ繧ｽ繝ｼ繧ｹ
		/* @details 繝ｪ繧ｽ繝ｼ繧ｹ縺ｫ蟇ｾ縺吶ｋ譖ｸ縺崎ｾｼ縺ｿ繝ｻ隱ｭ縺ｿ霎ｼ縺ｿ繝ｭ繝・け繧定ｧ｣髯､縺吶ｋ
		/*/
		void Unmap(Buffer* buffer) override;

		void Set_Shader(const aegis::SHADER_INDEX_V v_index = aegis::SHADER_INDEX_V::DEFAULT, const aegis::SHADER_INDEX_P p_index = aegis::SHADER_INDEX_P::DEFAULT) override;

		void Set_InputLayout(const INPUTLAYOUT InputLayout = aegis::INPUTLAYOUT::DEFAULT) override;

		void SetPassRendring() override;
		void SetPassGeometry() override;

		void End_Draw() override;

		/**
		* @brief 繝・く繧ｹ繝医ヵ繧ｩ繝ｼ繝槭ャ繝医・菴懈・
		* @param[in] TEXT_FOMAT 繝・く繧ｹ繝医ヵ繧ｩ繝ｼ繝槭ャ繝・
		* @return HRESULT 謌ｻ繧雁､
		* @details 繝・く繧ｹ繝医ヵ繧ｩ繝ｼ繝槭ャ繝医・菴懈・縺吶ｋ
		*/
		HRESULT CreateTextFormat(const TEXT_FOMAT& fomat);

		/**
		* @brief 繝・く繧ｹ繝医Ξ繧､繧｢繧ｦ繝医ｒ菴懈・
		* @param TEXT_LAYOUT 繝・く繧ｹ繝医Ξ繧､繧｢繧ｦ繝・
		* @return HRESULT 謌ｻ繧雁､
		* @details 繝・く繧ｹ繝医Ξ繧､繧｢繧ｦ繝医ｒ菴懈・縺吶ｋ
		*/
		HRESULT CreateTextLayout(const TEXT_LAYOUT& layout);

		ID3D11ShaderResourceView* Get() {
			return ShaderResourceView[0].Get();
		}

		ID3D11ShaderResourceView* Get2() {
			return ShaderResourceView[1].Get();
		}

		ID3D11ShaderResourceView* Get3() {
			return ShaderResourceView[2].Get();
		}

		ID3D11ShaderResourceView* Get_Texture() {
			return ShaderResourceView_16bit.Get();
		}

	private:
		//! DirectX縺ｮ繝輔Η繝ｼ繝√Ε繝ｼ繝ｬ繝吶Ν
		D3D_FEATURE_LEVEL											m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;
		//! DirectX11縺ｮ繝・ヰ繧､繧ｹ
		ComPtr<ID3D11Device>										m_D3DDevice;
		//! DirectX11縺ｮ繝・ヰ繧､繧ｹ繧ｳ繝ｳ繝・く繧ｹ繝・
		ComPtr<ID3D11DeviceContext>									m_ImmediateContext;
		//! 繧ｹ繝ｯ繝・・繝√ぉ繧､繝ｳ
		ComPtr<IDXGISwapChain1>										m_SwapChain;
		//! 繝ｬ繝ｳ繝繝ｼ繧ｿ繝ｼ繧ｲ繝・ヨ繝薙Η繝ｼ
		ComPtr<ID3D11RenderTargetView>								m_RenderTargetView;

		ComPtr<ID3D11RenderTargetView>								RenderTargetView_16bit;
		ComPtr<ID3D11ShaderResourceView>							ShaderResourceView_16bit;

		ComPtr<ID3D11RenderTargetView>								RenderTargetView[3];
		ComPtr<ID3D11ShaderResourceView>							ShaderResourceView[3];

		//! 繝・・繧ｹ繧ｹ繝・Φ繧ｷ繝ｫ
		ComPtr<ID3D11DepthStencilView>								m_DepthStencilView;
		//! Direct2D縺ｮ繝・ヰ繧､繧ｹ
		ComPtr<ID2D1Device>											m_D2DDevice;
		//! Direct2D縺ｮ繝・ヰ繧､繧ｹ繧ｳ繝ｳ繝・く繧ｹ繝・
		ComPtr<ID2D1DeviceContext>									m_D2DDeviceContext;
		//! Direct2D縺ｮ繧ｿ繝ｼ繧ｲ繝・ヨ繝薙ャ繝医・繝・・
		ComPtr<ID2D1Bitmap1>										m_D2DTargetBitmap;
		//! Dxgi
		ComPtr<IDXGIDevice1>										m_dxgiDev;

		//! 繝・く繧ｹ繝医ヵ繧ｩ繝ｼ繝槭ャ繝・
		ComPtr<IDWriteTextFormat>									m_DwriteTextFormat;
		//! 繝・く繧ｹ繝医Ξ繧､繧｢繧ｦ繝・
		ComPtr<IDWriteTextLayout>									m_TextLayout;
		//! Direct2D縺ｮ繝ｩ繧､繝医ヵ繧｡繧ｯ繝医Μ繝ｼ
		ComPtr<IDWriteFactory>										m_DwriteFactory;

		unordered_map<aegis::SHADER_INDEX_V, ComPtr<ID3D11VertexShader>>	m_VertexShader;
		unordered_map<aegis::SHADER_INDEX_P, ComPtr<ID3D11PixelShader>>	m_PixelShader;

		ComPtr<ID3D11DepthStencilState>								m_DepthStateEnable;
		ComPtr<ID3D11DepthStencilState>								m_DepthStateDisable;
		ComPtr<ID3D11RasterizerState>								m_RasterizerState;

		ComPtr<ID3D11SamplerState>									m_SamplerState;

		//! 鬆らせ繝ｬ繧､繧｢繧ｦ繝・
		ComPtr<ID3D11InputLayout>									m_VertexLayout[3];
		//! 繝槭ユ繝ｪ繧｢繝ｫ繝舌ャ繝輔ぃ
		ComPtr<ID3D11Buffer>										m_MaterialBuffer;
		//! 繝ｩ繧､繝医ヰ繝・ヵ繧｡
		ComPtr<ID3D11Buffer>										m_LightBuffer;
		//! 繝懊・繝ｳ諠・ｱ繝舌ャ繝輔ぃ
		ComPtr<ID3D11Buffer>										m_Bone_Matrix_Buffer;
		//! 繧ｳ繝ｳ繧ｹ繧ｿ繝ｳ繝医ヰ繝・ヵ繧｡
		ComPtr<ID3D11Buffer>										m_ConstantBuffer;

		ComPtr<ID3D11Buffer>										m_ConstantBuffer_02;

		//! 繧ｹ繧ｿ繝ｳ繝舌う繝｢繝ｼ繝峨ヵ繝ｩ繧ｰ
		bool														Stand_By_Enable = false;

		std::once_flag												mOnceFlag;

		/**
		* @brief Direct3D縺ｮ蛻晄悄蛹・
		* @return bool 謌仙粥縺ｪ繧・true 螟ｱ謨励↑繧・false
		* @details Direct3D縺ｮ蛻晄悄蛹悶☆繧・
		*/
		bool Init3D() override;

		/**
		* @brief Direct2D縺ｮ蛻晄悄蛹・
		* @return bool 謌仙粥縺ｪ繧・true 螟ｱ謨励↑繧・false
		* @details Direct2D縺ｮ蛻晄悄蛹悶☆繧・
		*/
		bool Init2D() override;
	};
}