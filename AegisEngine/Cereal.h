#pragma once

#ifndef CEREAL_H
#define CEREAL_H

namespace DirectX
{
	// 非侵入型のシリアライズ定義

	// XMINT2
	template<class Archive>
	void save(Archive& archive, XMINT2 const& vector)
	{
		archive(cereal::make_nvp("x", vector.x),
				cereal::make_nvp("y", vector.y)
		);
	}

	template<class Archive>
	void load(Archive& archive, XMINT2& vector)
	{
		archive(cereal::make_nvp("x", vector.x),
				cereal::make_nvp("y", vector.y)
		);
	}

	// XMINT3
	template<class Archive>
	void save(Archive& archive, XMINT3 const& vector)
	{
		archive(cereal::make_nvp("x", vector.x),
				cereal::make_nvp("y", vector.y),
				cereal::make_nvp("z", vector.z)
		);
	}

	template<class Archive>
	void load(Archive& archive, XMINT3& vector)
	{
		archive(cereal::make_nvp("x", vector.x),
				cereal::make_nvp("y", vector.y),
				cereal::make_nvp("z", vector.z)
		);
	}

	// XMINT4
	template<class Archive>
	void save(Archive& archive, XMINT4 const& vector)
	{
		archive(cereal::make_nvp("x", vector.x),
				cereal::make_nvp("y", vector.y),
				cereal::make_nvp("z", vector.z),
				cereal::make_nvp("w", vector.w)
		);
	}

	template<class Archive>
	void load(Archive& archive, XMINT4& vector)
	{
		archive(cereal::make_nvp("x", vector.x),
				cereal::make_nvp("y", vector.y),
				cereal::make_nvp("z", vector.z),
				cereal::make_nvp("w", vector.w)
		);
	}

	// XMUINT2
	template<class Archive>
	void save(Archive& archive, XMUINT2 const& vector)
	{
		archive(cereal::make_nvp("x", vector.x),
				cereal::make_nvp("y", vector.y)
		);
	}

	template<class Archive>
	void load(Archive& archive, XMUINT2& vector)
	{
		archive(cereal::make_nvp("x", vector.x),
				cereal::make_nvp("y", vector.y)
		);
	}

	// XMUINT3
	template<class Archive>
	void save(Archive& archive, XMUINT3 const& vector)
	{
		archive(cereal::make_nvp("x", vector.x),
				cereal::make_nvp("y", vector.y),
				cereal::make_nvp("z", vector.z)
		);
	}

	template<class Archive>
	void load(Archive& archive, XMUINT3& vector)
	{
		archive(cereal::make_nvp("x", vector.x),
				cereal::make_nvp("y", vector.y),
				cereal::make_nvp("z", vector.z)
		);
	}

	// XMUINT4
	template<class Archive>
	void save(Archive& archive, XMUINT4 const& vector)
	{
		archive(cereal::make_nvp("x", vector.x),
				cereal::make_nvp("y", vector.y),
				cereal::make_nvp("z", vector.z),
				cereal::make_nvp("w", vector.w)
		);
	}

	template<class Archive>
	void load(Archive& archive, XMUINT4& vector)
	{
		archive(cereal::make_nvp("x", vector.x),
				cereal::make_nvp("y", vector.y),
				cereal::make_nvp("z", vector.z),
				cereal::make_nvp("w", vector.w)
		);
	}

	// XMFLOAT2
	template<class Archive>
	void save(Archive& archive, XMFLOAT2 const& vector)
	{
		archive(cereal::make_nvp("x", vector.x),
				cereal::make_nvp("y", vector.y)
		);
	}

	template<class Archive>
	void load(Archive& archive, XMFLOAT2& vector)
	{
		archive(cereal::make_nvp("x", vector.x),
				cereal::make_nvp("y", vector.y)
		);
	}

	// XMFLOAT3
	template<class Archive>
	void save(Archive& archive, XMFLOAT3 const& vector)
	{
		archive(cereal::make_nvp("x", vector.x),
				cereal::make_nvp("y", vector.y),
				cereal::make_nvp("z", vector.z)
		);
	}

	template<class Archive>
	void load(Archive& archive, XMFLOAT3& vector)
	{
		archive(cereal::make_nvp("x", vector.x),
				cereal::make_nvp("y", vector.y),
				cereal::make_nvp("z", vector.z)
		);
	}

	// XMFLOAT4
	template<class Archive>
	void save(Archive& archive, XMFLOAT4 const& vector)
	{
		archive(cereal::make_nvp("x", vector.x),
				cereal::make_nvp("y", vector.y),
				cereal::make_nvp("z", vector.z),
				cereal::make_nvp("w", vector.w)
		);
	}

	template<class Archive>
	void load(Archive& archive, XMFLOAT4& vector)
	{
		archive(cereal::make_nvp("x", vector.x),
				cereal::make_nvp("y", vector.y),
				cereal::make_nvp("z", vector.z),
				cereal::make_nvp("w", vector.w)
		);
	}

	// XMFLOAT4X4
	template<class Archive>
	void save(Archive& archive, XMFLOAT4X4 const& matrix)
	{
		archive(cereal::make_nvp("11", matrix._11), cereal::make_nvp("12", matrix._12), cereal::make_nvp("13", matrix._13), cereal::make_nvp("14", matrix._14),
				cereal::make_nvp("21", matrix._21), cereal::make_nvp("22", matrix._22), cereal::make_nvp("23", matrix._23), cereal::make_nvp("24", matrix._24),
				cereal::make_nvp("31", matrix._31), cereal::make_nvp("32", matrix._32), cereal::make_nvp("33", matrix._33), cereal::make_nvp("34", matrix._34),
				cereal::make_nvp("41", matrix._41), cereal::make_nvp("42", matrix._42), cereal::make_nvp("43", matrix._43), cereal::make_nvp("44", matrix._44)
		);
	}

	template<class Archive>
	void load(Archive& archive, XMFLOAT4X4& matrix)
	{
		archive(cereal::make_nvp("11", matrix._11), cereal::make_nvp("12", matrix._12), cereal::make_nvp("13", matrix._13), cereal::make_nvp("14", matrix._14),
				cereal::make_nvp("21", matrix._21), cereal::make_nvp("22", matrix._22), cereal::make_nvp("23", matrix._23), cereal::make_nvp("24", matrix._24),
				cereal::make_nvp("31", matrix._31), cereal::make_nvp("32", matrix._32), cereal::make_nvp("33", matrix._33), cereal::make_nvp("34", matrix._34),
				cereal::make_nvp("41", matrix._41), cereal::make_nvp("42", matrix._42), cereal::make_nvp("43", matrix._43), cereal::make_nvp("44", matrix._44)
		);
	}
}

// RECT
template<class Archive>
void save(Archive& archive, RECT const& rect)
{
	archive(cereal::make_nvp("left", rect.left),
			cereal::make_nvp("top", rect.top),
			cereal::make_nvp("right", rect.right),
			cereal::make_nvp("bottom", rect.bottom)
	);
}

template<class Archive>
void load(Archive& archive, RECT& rect)
{
	archive(cereal::make_nvp("left", rect.left),
			cereal::make_nvp("top", rect.top),
			cereal::make_nvp("right", rect.right),
			cereal::make_nvp("bottom", rect.bottom)
	);
}

#endif // ! CEREAL_H