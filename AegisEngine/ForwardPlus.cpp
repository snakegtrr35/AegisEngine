#include	"ForwardPlus.h"
#include	"Renderer.h"

bool FORWARDLUS::Init()
{
	return true;
}

void FORWARDLUS::Update()
{

}

void FORWARDLUS::Draw()
{
	ID3D11RenderTargetView* pNULLRTV = nullptr;
	ID3D11DepthStencilView* pNULLDSV = nullptr;
	ID3D11ShaderResourceView* pNULLSRV = nullptr;
	ID3D11UnorderedAccessView* pNULLUAV = nullptr;
	ID3D11SamplerState* pNULLSampler = nullptr;

	// Depth pre-pass
	{
		// Depth pre-pass (to eliminate pixel overdraw during forward rendering)
		CRenderer::GetDeviceContext()->OMSetRenderTargets(1, &pNULLRTV, m_pDepthStencilView.get());  // null color buffer
		CRenderer::GetDeviceContext()->OMSetDepthStencilState(m_DepthStateEnable.get(), NULL);  // we are using inverted 32-bit float depth for better precision
		//CRenderer::GetDeviceContext()->IASetInputLayout(m_pLayoutPositionOnly11);
		CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader.get(), nullptr, 0);
		CRenderer::GetDeviceContext()->PSSetShader(nullptr, nullptr, 0);  // null pixel shader
		CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &pNULLSRV);
		CRenderer::GetDeviceContext()->PSSetShaderResources(1, 1, &pNULLSRV);
		CRenderer::GetDeviceContext()->PSSetSamplers(0, 1, &pNULLSampler);

		// Draw the main scene
		//Scene.m_pSceneMesh->Render(pd3dImmediateContext);

		//// Draw the alpha test geometry
		//ID3D11BlendState* pBlendStateForAlphaTest = m_pBlendStateOpaqueDepthOnly;
		//pd3dImmediateContext->RSSetState(CommonUtil.GetRasterizerState(RASTERIZER_STATE_DISABLE_CULLING));
		//pd3dImmediateContext->OMSetBlendState(pBlendStateForAlphaTest, BlendFactor, 0xffffffff);
		//pd3dImmediateContext->OMSetRenderTargets(1, &pRTV, DepthStencilBuffer.m_pDepthStencilView);  // bind color buffer to prevent D3D warning
		//pd3dImmediateContext->IASetInputLayout(m_pLayoutPositionAndTex11);
		//pd3dImmediateContext->VSSetShader(m_pScenePositionAndTexVS, NULL, 0);
		//pd3dImmediateContext->PSSetShader(m_pSceneAlphaTestOnlyPS, NULL, 0);
		//pd3dImmediateContext->PSSetSamplers(0, 1, CommonUtil.GetSamplerStateParam(SAMPLER_STATE_ANISO));
		//Scene.m_pAlphaMesh->Render(pd3dImmediateContext, 0);

		// Restore to default
		CRenderer::GetDeviceContext()->RSSetState(nullptr);
		CRenderer::GetDeviceContext()->OMSetBlendState(m_pBlendStateOpaque, BlendFactor, 0xffffffff);
	}

	// Light culling
	{
		CRenderer::GetDeviceContext()->OMSetRenderTargets(1, &pNULLRTV, pNULLDSV);  // null color buffer and depth-stencil
		CRenderer::GetDeviceContext()->OMSetDepthStencilState(CommonUtil.GetDepthStencilState(DEPTH_STENCIL_STATE_DISABLE_DEPTH_TEST), 0x00);
		CRenderer::GetDeviceContext()->VSSetShader(nullptr, nullptr, 0);  // null vertex shader
		CRenderer::GetDeviceContext()->PSSetShader(nullptr, nullptr, 0);  // null pixel shader
		CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &pNULLSRV);
		CRenderer::GetDeviceContext()->PSSetShaderResources(1, 1, &pNULLSRV);
		CRenderer::GetDeviceContext()->PSSetSamplers(0, 1, &pNULLSampler);

		// Calculate per-tile depth bounds on the GPU, using a Compute Shader
		CRenderer::GetDeviceContext()->CSSetShader(pDepthBoundsCS, nullptr, 0);
		CRenderer::GetDeviceContext()->CSSetShaderResources(0, 1, &pDepthSRV);
		CRenderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, CommonUtil.GetDepthBoundsUAVParam(), nullptr);
		CRenderer::GetDeviceContext()->Dispatch(CommonUtil.GetNumTilesX(), CommonUtil.GetNumTilesY(), 1);
		CRenderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &pNULLUAV, nullptr);

		// Cull lights on the GPU, using a Compute Shader
		CRenderer::GetDeviceContext()->CSSetShader(CommonUtil.GetLightCullCS(), nullptr, 0);
		CRenderer::GetDeviceContext()->CSSetShaderResources(0, 1, LightUtil.GetPointLightBufferCenterAndRadiusSRVParam());
		CRenderer::GetDeviceContext()->CSSetShaderResources(1, 1, LightUtil.GetSpotLightBufferCenterAndRadiusSRVParam());
		CRenderer::GetDeviceContext()->CSSetShaderResources(2, 1, CommonUtil.GetDepthBoundsSRVParam());
		CRenderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, CommonUtil.GetLightIndexBufferUAVParam(), nullptr);
		CRenderer::GetDeviceContext()->CSSetUnorderedAccessViews(1, 1, CommonUtil.GetSpotIndexBufferUAVParam(), nullptr);
		CRenderer::GetDeviceContext()->Dispatch(CommonUtil.GetNumTilesX(), CommonUtil.GetNumTilesY(), 1);

		CRenderer::GetDeviceContext()->CSSetShader(nullptr, nullptr, 0);
		CRenderer::GetDeviceContext()->CSSetShaderResources(0, 1, &pNULLSRV);
		CRenderer::GetDeviceContext()->CSSetShaderResources(1, 1, &pNULLSRV);
		CRenderer::GetDeviceContext()->CSSetShaderResources(2, 1, &pNULLSRV);
		CRenderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &pNULLUAV, nullptr);
		CRenderer::GetDeviceContext()->CSSetUnorderedAccessViews(1, 1, &pNULLUAV, nullptr);
	}
}

void FORWARDLUS::Uninit()
{

}