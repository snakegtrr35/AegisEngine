#pragma once

#ifndef _MATH_DETAIL_QUATERNION_H_
#define _MATH_DETAIL_QUATERNION_H_

namespace aegis
{
	namespace quaternion
	{
		namespace detail
		{
			inline Quaternion toQuaternion(const DirectX::XMVECTOR& vec)
			{
				XMFLOAT4 quat;
				DirectX::XMStoreFloat4(&quat, vec);

				return Quaternion(quat);
			}

			inline DirectX::XMVECTOR toXMVECTOR(const DirectX::XMFLOAT3& vec)
			{
				return DirectX::XMLoadFloat3(&vec);
			}
		}

		inline static Quaternion Euler(const float32 x, const float32 y, const float32 z)
		{
			return detail::toQuaternion(DirectX::XMQuaternionRotationRollPitchYawFromVector(DirectX::XMVectorSet(x, y, z, 0.f)));
		}

		// 軸と角度からCQuaternionを構築します
		// 軸はすでに正規化されていると想定されますが、角度はディグリーです
		inline static Quaternion RotateAxis(const Vector3& axis, const float32 angle)
		{
			return detail::toQuaternion(DirectX::XMQuaternionRotationAxis(detail::toXMVECTOR(axis), angle));
		}
	}
}

#endif // !_MATH_DETAIL_QUATERNION_H_
