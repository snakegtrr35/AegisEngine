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

		void SetBlendState(BlendDesc* blendState = nullptr, bool flag = false) override;

		void GetAddBlendState(BlendDesc& blendState) override;

		void GetSubtractBlendState(BlendDesc& blendState) override;

		void Get2DBlendState(BlendDesc& blendState) override;

		void SetDepthEnable(bool Enable) override;

		void Change_Window_Mode();

		void SetWorldViewProjection2D(const XMFLOAT3& scaling = XMFLOAT3(1.0f, 1.0f, 1.0f), const XMFLOAT3& rotation = XMFLOAT3(0.0f, 0.0f, 0.0f)) override;

		void Set_MatrixBuffer(const XMMATRIX world, const XMMATRIX view, const XMMATRIX projection) override;

		void Set_MatrixBuffer01(const XMVECTOR camera_pos) override;

		void SetMaterial(aegis::MATERIAL Material) override;

		void SetLight(aegis::LIGHT* Light) override;

		void Light_Identity() override;

		void Set_RasterizerState() override;

		DepthStencilView* CreateDepthStencilView(Texture2D* texture, DepthStencilViewDesc depthStencilViewDesc) const override;

		void SetRenderTargets(uint32 numViews, RenderTargetView* const* renderTargetViews, DepthStencilView* depthStencilView) const override;

		void ClearRenderTargetView(RenderTargetView* renderTargetView, float32 color[4]) const override;

		void ClearDepthStencilView(DepthStencilView* depthStencilView, ClearFlag clearFlags, float32 depth, uint8 stencil) const override;

		void SetViewports(uint32 numViewports, ViewPort* const* viewports) const override;

		RasterizerState* CreateRasterizeState(RasterizeState rasterizeState) const override;

		void SetRasterizeState(RasterizerState* rasterizerState) const override;

		Buffer* CreateBuffer(const BufferDesc& desc, const SubresourceData initData) override;

		Texture1D* CreateTexture1D(Texture1DDesc desc, SubresourceData* initialData) const override;

		Texture2D* CreateTexture2D(Texture2DDesc desc, SubresourceData* initialData) const override;

		Texture3D* CreateTexture3D(Texture3DDesc desc, SubresourceData* initialData) const override;

		ShaderResourceView* CreateShaderResourceView(Texture* texture, ShaderResourceViewDesc srvDesc) const override;

		UnorderedAccessView* CreateUnorderedAccessView(Texture* texture, UnorderedAccessViewDesc* unorderedAccessViewDesc) const override;

		void ClearUnorderedAccessView(UnorderedAccessView* unorderedAccessView, float32 clearValues[4]) const override;
		void ClearUnorderedAccessView(UnorderedAccessView* unorderedAccessView, uint32 clearValues[4]) const override;

		void CSSetUnorderedAccessViews(uint32 startSlot, uint32 numBuffers, UnorderedAccessView* const* unorderedAccessViews) const override;

		void SetVertexBuffers(Buffer* vertexBuffer, uint32 stride = sizeof(VERTEX_3D)) override;

		void SetVertexBuffers(Buffer* indexBuffer, Buffer* instancingBuffer, UINT size) override;

		void SetIndexBuffer(Buffer* indexBuffer, Format format = Format::R16Uint) const override;

		PixelShader* CreatePixelShader(void* shaderData, uint32 size) const override;
		VertexShader* CreateVertexShader(void* shaderData, uint32 size) const override;
		ComputeShader* CreateComputeShader(void* shaderData, uint32 size) const override;

		void Draw(uint32 indexCount, uint32 startVertexLocation) override;
		void DrawIndexed(uint32 iIndexCount, uint32 startIndexLocation, int32 baseVertexLocation) override;
		void DrawIndexedInstanced(int32 indexCountPerInstance, uint32 instanceCount, uint32 startIndexLocation, int32 baseVertexLocation, uint32 startInstanceLocation) override;

		void Dispatch(uint32 threadGroupCountX, uint32 threadGroupCountY, uint32 threadGroupCountZ) const override;

		void CSSetShader(ComputeShader* pShader) const override;

		void VSSetConstantBuffers(uint32 startSlot, uint32 numBuffers, Buffer* const* pBuffers) override;

		void PSSetConstantBuffers(uint32 startSlot, uint32 numBuffers, Buffer* const* pBuffers) override;

		void CSSetConstantBuffers(uint32 startSlot, uint32 numBuffers, Buffer* const* pBuffers) override;

		void PSSetShaderResources(uint32 startSlot, uint32 numBuffers, ShaderResourceView* const* shaderResourceViews) const override;

		void CSSetShaderResources(uint32 startSlot, uint32 numBuffers, ShaderResourceView* const* shaderResourceViews) const override;

		SamplerState* CreateSampler(const SamplerDesc desc) const override;

		void PSSetSamplers(UINT startSlot, UINT numSamplers, SamplerState* const* ppSamplers) override;

		void CSSetSamplers(UINT startSlot, UINT numSamplers, SamplerState* const* ppSamplers) override;

		void SetPrimitiveTopology(PrimitiveTopology primitiveTopology) override;

		void UpdateSubresource(Buffer* buffer, void* pData) override;

		void Map(Buffer* buffer, void* pData, uint32 dataSize) override;

		MappedSubresource Map(Buffer* buffer) override;

		void Unmap(Buffer* buffer) override;

		void Set_Shader(const aegis::SHADER_INDEX_V v_index = aegis::SHADER_INDEX_V::DEFAULT, const aegis::SHADER_INDEX_P p_index = aegis::SHADER_INDEX_P::DEFAULT) override;

		void Set_InputLayout(const INPUTLAYOUT InputLayout = aegis::INPUTLAYOUT::DEFAULT) override;

		void SetPassRendring() override;
		void SetPassGeometry() override;

		void End_Draw() override;

		HRESULT CreateTextFormat(const TEXT_FOMAT& fomat);

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
		D3D_FEATURE_LEVEL											m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;
		ComPtr<ID3D11Device>										m_D3DDevice;
		ComPtr<ID3D11DeviceContext>									m_ImmediateContext;
		ComPtr<IDXGISwapChain1>										m_SwapChain;
		ComPtr<ID3D11RenderTargetView>								m_RenderTargetView;

		ComPtr<ID3D11RenderTargetView>								RenderTargetView_16bit;
		ComPtr<ID3D11ShaderResourceView>							ShaderResourceView_16bit;

		ComPtr<ID3D11RenderTargetView>								RenderTargetView[3];
		ComPtr<ID3D11ShaderResourceView>							ShaderResourceView[3];
		ComPtr<ID3D11DepthStencilView>								m_DepthStencilView;

		ComPtr<ID2D1Device>											m_D2DDevice;
		ComPtr<ID2D1DeviceContext>									m_D2DDeviceContext;
		ComPtr<ID2D1Bitmap1>										m_D2DTargetBitmap;
		ComPtr<IDXGIDevice1>										m_dxgiDev;

		ComPtr<IDWriteTextFormat>									m_DwriteTextFormat;
		ComPtr<IDWriteTextLayout>									m_TextLayout;
		ComPtr<IDWriteFactory>										m_DwriteFactory;

		unordered_map<aegis::SHADER_INDEX_V, ComPtr<ID3D11VertexShader>>	m_VertexShader;
		unordered_map<aegis::SHADER_INDEX_P, ComPtr<ID3D11PixelShader>>	m_PixelShader;

		ComPtr<ID3D11DepthStencilState>								m_DepthStateEnable;
		ComPtr<ID3D11DepthStencilState>								m_DepthStateDisable;
		ComPtr<ID3D11RasterizerState>								m_RasterizerState;

		ComPtr<ID3D11SamplerState>									m_SamplerState;

		ComPtr<ID3D11InputLayout>									m_VertexLayout[3];
		ComPtr<ID3D11Buffer>										m_MaterialBuffer;
		ComPtr<ID3D11Buffer>										m_LightBuffer;
		ComPtr<ID3D11Buffer>										m_Bone_Matrix_Buffer;
		ComPtr<ID3D11Buffer>										m_ConstantBuffer;
		ComPtr<ID3D11Buffer>										m_ConstantBuffer_02;

		bool														Stand_By_Enable = false;
		std::once_flag												mOnceFlag;

		/**
		* @brief Direct3Dの初期化
		* @return bool 成功なら true 失敗なら false
		* @details Direct3Dの初期化する
		*/
		bool Init3D() override;

		/**
		* @brief Direct2Dの初期化
		* @return bool 成功なら true 失敗なら false
		* @details Direct2Dの初期化する
		*/
		bool Init2D() override;
	};
}