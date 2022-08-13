#include "TextureDX11.h"

#include "include\engine\core\AllocatorWrapper.h"

namespace aegis::render
{
	TextureDataDX11::TextureDataDX11()
	{
		ShaderResourceViewDX11* ptr = aegis::allocatorWrapper::allocate<ShaderResourceViewDX11>();

		this->ShaderResourceView.reset(static_cast<ShaderResourceViewDX11*>(ptr));
	}

	TextureDataDX11::~TextureDataDX11()
	{
		if (ShaderResourceView)
			ShaderResourceView->Release();
	}
}
