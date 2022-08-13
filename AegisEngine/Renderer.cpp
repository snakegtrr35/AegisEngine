#include "Renderer.h"
#include "RenderDX11.h"

namespace aegis
{
	CRenderer* CRenderer::getInstance()
	{
		static std::unique_ptr<CRenderer> Instance = nullptr;

		if (nullptr == Instance)
		{
#ifdef DX11
			Instance = std::make_unique<RenderDX11>();
#elif DX12
			// 未実装
#elif VULKAN
			// 未実装
#endif
		}

		return Instance.get();
	}

	void* Cast(aegis::ShaderResourceView* shaderResourceView)
	{
		assert(shaderResourceView != nullptr);

#ifdef DX11
		return static_cast<aegis::ShaderResourceViewDX11*>(shaderResourceView)->ShaderResourceView.Get();
#elif DX12
		// 未実装
		return nullptr;
#elif VULKAN
		// 未実装
		return nullptr;
#endif
	}
}
