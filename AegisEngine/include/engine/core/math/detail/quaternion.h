#pragma once

#ifndef _MATH_DETAIL_QUATERNION_H_
#define _MATH_DETAIL_QUATERNION_H_

namespace Aegis
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
			return detail::toQuaternion(DirectX::XMQuaternionRotationRollPitchYawFromVector(DirectX::XMVectorSet(z, y, z, 0.f)));
		}

		// ���Ɗp�x����CQuaternion���\�z���܂�
		// ���͂��łɐ��K������Ă���Ƒz�肳��܂����A�p�x�̓f�B�O���[�ł�
		inline static Quaternion RotateAxis(const Vector3& axis, const float32 angle)
		{
			return detail::toQuaternion(DirectX::XMQuaternionRotationAxis(detail::toXMVECTOR(axis), angle));
		}
	}
}

#endif // !_MATH_DETAIL_QUATERNION_H_
