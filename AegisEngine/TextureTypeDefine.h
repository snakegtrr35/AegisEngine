#pragma once

#include "Renderer.h"

namespace aegis::render
{
	struct TEXTURE_FILE {
		aegis::string Path;		//! テクスチャファイルのファイルパス

		template<class Archive>
		void save(Archive& archive) const
		{
			archive(cereal::make_nvp("Path", Path));
		}

		template<class Archive>
		void load(Archive& archive)
		{
			archive(cereal::make_nvp("Path", Path));
		}
	};

	struct TextureData {
		aegis::uniquePtr<aegis::ShaderResourceView> ShaderResourceView;	//! リソース
		aegis::Int2 WH;							//!	テクスチャの幅と高さ
		UINT Cnt = 0;							//! 参照回数

		TextureData() = default;

		virtual ~TextureData()
		{
			//if (ShaderResourceView)
			//	ShaderResourceView->Release();
				//aegis::allocatorWrapper::deallocate/*<aegis::ShaderResourceView>*/(ShaderResourceView);
		}

		void Release() {}
	};
}