#pragma once

#ifndef INCLUDE_TEXTUREIMPORTER_H_
#define INCLUDE_TEXTUREIMPORTER_H_

#include "Renderer.h"

namespace aegis
{
	class TextureImporter
	{
	public:
		static TextureImporter* getInstance();

		~TextureImporter();

		ShaderResourceView* GetShaderResourceView(aegis::wstring fileName) const;

		ShaderResourceView* GetShaderResourceView(void* textureDara, uint32 size) const;

		void GetSize(aegis::wstring fileName, int32* width, int32* hieght) const;

	private:
		TextureImporter();
	};
}

#endif // !INCLUDE_TEXTUREIMPORTER_H_
