#pragma once

#ifndef CEREAL_H
#define CEREAL_H

namespace DirectX
{
	// ��N���^�̃V���A���C�Y��`

	template<class Archive>
	void serialize(Archive& archive, XMINT2& vector)
	{
		archive(vector.x, vector.y);
	}

	template<class Archive>
	void serializeXMFLOAT2(Archive& archive, XMFLOAT2& vector)
	{
		archive(vector.x, vector.y);
	}

	template<class Archive>
	void serializeXMFLOAT3(Archive& archive, XMFLOAT3& vector)
	{
		archive(vector.x, vector.y, vector.z);
	}

	template<class Archive>
	void serializeXMFLOAT4(Archive& archive, XMFLOAT4& vector)
	{
		archive(vector.x, vector.y, vector.z, vector.w);
	}
}

#endif // ! CEREAL_H