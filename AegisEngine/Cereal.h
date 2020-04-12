#pragma once

#ifndef CEREAL_H
#define CEREAL_H

namespace DirectX
{
	// 非侵入型のシリアライズ定義

	template<class Archive>
	void serialize(Archive& archive, XMINT2& vector)
	{
		archive(vector.x, vector.y);
	}

	template<class Archive>
	void serialize(Archive& archive, XMINT3& vector)
	{
		archive(vector.x, vector.y, vector.z);
	}

	template<class Archive>
	void serialize(Archive& archive, XMINT4& vector)
	{
		archive(vector.x, vector.y, vector.z, vector.w);
	}
	


	template<class Archive>
	void serialize(Archive& archive, XMUINT2& vector)
	{
		archive(vector.x, vector.y);
	}

	template<class Archive>
	void serialize(Archive& archive, XMUINT3& vector)
	{
		archive(vector.x, vector.y, vector.z);
	}

	template<class Archive>
	void serialize(Archive& archive, XMUINT4& vector)
	{
		archive(vector.x, vector.y, vector.z, vector.w);
	}



	template<class Archive>
	void serialize(Archive& archive, XMFLOAT2& vector)
	{
		archive(vector.x, vector.y);
	}

	template<class Archive>
	void serialize(Archive& archive, XMFLOAT3& vector)
	{
		archive(vector.x, vector.y, vector.z);
	}

	template<class Archive>
	void serialize(Archive& archive, XMFLOAT4& vector)
	{
		archive(vector.x, vector.y, vector.z, vector.w);
	}

	template<class Archive>
	void serialize(Archive& archive, XMFLOAT4X4& matrix)
	{
		archive(matrix._11, matrix._12, matrix._13, matrix._14,
				matrix._21, matrix._22, matrix._23, matrix._24,
				matrix._31, matrix._32, matrix._33, matrix._34,
				matrix._41, matrix._42, matrix._43, matrix._44
			);
	}
}

template<class Archive>
void serialize(Archive& archive, RECT& rect)
{
	archive(rect.left, rect.top, rect.right, rect.bottom);
}

#endif // ! CEREAL_H