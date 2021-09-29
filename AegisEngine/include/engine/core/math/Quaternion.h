#pragma once

#ifndef _MATH_QUATERNION_H_
#define _MATH_QUATERNION_H_ 

//#include "Vector3.h"

#include <DirectXMath.h>
using namespace DirectX;

namespace Aegis
{
	class Quaternion {
	public:

		friend class Vector3;

		Quaternion() noexcept = default;

		// ����ɂ��A�N�H�[�^�j�I���R���|�[�l���g�����ڐݒ肳��܂�-
		// �� / �p�x�ɂ͎g�p���Ȃ��ł�������
		Quaternion(float _x, float _y, float _z, float _w) noexcept;

		Quaternion(const Quaternion&) = default;

		Quaternion& operator=(const Quaternion&) = default;
		Quaternion(Quaternion&&) = default;
		Quaternion& operator=(Quaternion&&) = default;

		// ���Ɗp�x����CQuaternion���\�z���܂�
		// ���͂��łɐ��K������Ă���Ƒz�肳��܂����A�p�x�̓f�B�O���[�ł�
		/*Quaternion RotateAxis(const Vector3& axis, const float angle)
		{
			float scalar = sin(Math::DegreeToRadian(angle) / 2.0f);
			x = axis.x * scalar;
			y = axis.y * scalar;
			z = axis.z * scalar;
			w = cos(angle / 2.0f);
		}*/

		// �����R���|�[�l���g�𒼐ڐݒ肵�܂�
		void Set(float _x, float _y, float _z, float _w)
		{
			x = _x;
			y = _y;
			z = _z;
			w = _w;
		}

		void Conjugate()
		{
			x *= -1.0f;
			y *= -1.0f;
			z *= -1.0f;
		}

		// ���N�H�[�^�j�I�����K������
		void Normalize()
		{
			float length = sqrt(x * x + y * y + z * z + w * w);
			x /= length;
			y /= length;
			z /= length;
			w /= length;
		}

		// �񋟂��ꂽ�N�H�[�^�j�I���𐳋K������
		Quaternion Normalize(const Quaternion& q)
		{
			Quaternion retVal = q;
			retVal.Normalize();
			return retVal;
		}

		// ���`���
		Quaternion Lerp(const Quaternion& a, const Quaternion& b, float t);

		// 2�̃N�H�[�^�j�I���Ԃ̃h�b�g�ρia Dot b�j
		float Dot(const Quaternion& a, const Quaternion& b)
		{
			return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
		}

		// ���ʐ��`���
		Quaternion Slerp(const Quaternion& a, const Quaternion& b, float f)
		{
			float rawCosm = Quaternion::Dot(a, b);

			float cosom = -rawCosm;
			if (rawCosm >= 0.0f)
			{
				cosom = rawCosm;
			}

			float scale0, scale1;

			if (cosom < 0.9999f)
			{
				const float omega = acos(cosom);
				const float invSin = 1.f / sin(omega);
				scale0 = sin((1.f - f) * omega) * invSin;
				scale1 = sin(f * omega) * invSin;
			}
			else
			{
				// Use linear interpolation if the CQuaternions
				// are collinear
				scale0 = 1.0f - f;
				scale1 = f;
			}

			if (rawCosm < 0.0f)
			{
				scale1 = -scale1;
			}

			Quaternion retVal;
			retVal.x = scale0 * a.x + scale1 * b.x;
			retVal.y = scale0 * a.y + scale1 * b.y;
			retVal.z = scale0 * a.z + scale1 * b.z;
			retVal.w = scale0 * a.w + scale1 * b.w;
			retVal.Normalize();
			return retVal;
		}

		// �A������
		// q��]�Ap��]
		Quaternion Concatenate(const Quaternion& q, const Quaternion& p);

		static Quaternion Euler(const float x, const float y, const float z)
		{
			XMVECTOR Quaternion = DirectX::XMQuaternionIdentity();

			const XMVECTOR axisX = XMVectorSet(1.0f, 0.f, 0.f, 0.f);
			const XMVECTOR axisY = XMVectorSet(0.0f, 1.0f, 0.f, 0.f);
			const XMVECTOR axisZ = XMVectorSet(0.0f, 0.f, 1.0f, 0.f);

			XMVECTOR rotateX = XMQuaternionRotationAxis(axisX, XMConvertToRadians(x));
			rotateX = XMVector4Normalize(rotateX);
			XMVECTOR rotateY = XMQuaternionRotationAxis(axisY, XMConvertToRadians(y));
			rotateY = XMVector4Normalize(rotateY);
			XMVECTOR rotateZ = XMQuaternionRotationAxis(axisZ, XMConvertToRadians(z));
			rotateZ = XMVector4Normalize(rotateZ);

			Quaternion = XMQuaternionMultiply(Quaternion, rotateX);
			Quaternion = XMQuaternionMultiply(Quaternion, rotateY);
			Quaternion = XMQuaternionMultiply(Quaternion, rotateZ);
			Quaternion = XMQuaternionNormalize(Quaternion);

			XMFLOAT4 quat;
			XMStoreFloat4(&quat, Quaternion);

			return Quaternion::Quaternion(quat.x, quat.y, quat.z, quat.w);
		}

	public:
		float x;
		float y;
		float z;
		float w;

		static const Quaternion Identity;
	};
}

#endif // !_MATH_QUATERNION_H_
