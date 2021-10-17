#pragma once

#ifndef _MATH_QUATERNION_H_
#define _MATH_QUATERNION_H_ 

//#include "Vector3.h"

#include <DirectXMath.h>
using namespace DirectX;

namespace Aegis
{
	class Quaternion : public DirectX::XMFLOAT4
	{
	public:

		friend class Vector3;

		Quaternion() noexcept = default;

		// これにより、クォータニオンコンポーネントが直接設定されます-
		// 軸 / 角度には使用しないでください
		Quaternion(float32 _x, float32 _y, float32 _z, float32 _w) noexcept;

		explicit Quaternion(DirectX::XMFLOAT4 quar) noexcept : DirectX::XMFLOAT4(quar)
		{
		}

		Quaternion(const Quaternion&) = default;

		Quaternion& operator=(const Quaternion&) = default;
		Quaternion(Quaternion&&) = default;
		Quaternion& operator=(Quaternion&&) = default;

		// 軸と角度からCQuaternionを構築します
		// 軸はすでに正規化されていると想定されますが、角度はディグリーです
		/*Quaternion RotateAxis(const Vector3& axis, const float32 angle)
		{
			float32 scalar = sin(Math::DegreeToRadian(angle) / 2.0f);
			x = axis.x * scalar;
			y = axis.y * scalar;
			z = axis.z * scalar;
			w = cos(angle / 2.0f);
		}*/

		// 内部コンポーネントを直接設定します
		void Set(float32 _x, float32 _y, float32 _z, float32 _w)
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

		// こクォータニオン正規化する
		void Normalize()
		{
			float32 length = sqrt(x * x + y * y + z * z + w * w);
			x /= length;
			y /= length;
			z /= length;
			w /= length;
		}

		// 提供されたクォータニオンを正規化する
		Quaternion Normalize(const Quaternion& q)
		{
			Quaternion retVal = q;
			retVal.Normalize();
			return retVal;
		}

		// 線形補間
		Quaternion Lerp(const Quaternion& a, const Quaternion& b, float32 t);

		// 2つのクォータニオン間のドット積（a Dot b）
		float32 Dot(const Quaternion& a, const Quaternion& b)
		{
			return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
		}

		// 球面線形補間
		Quaternion Slerp(const Quaternion& a, const Quaternion& b, float32 t)
		{
			//float32 rawCosm = Quaternion::Dot(a, b);
			//
			//float32 cosom = -rawCosm;
			//if (rawCosm >= 0.0f)
			//{
			//	cosom = rawCosm;
			//}
			//
			//float32 scale0, scale1;
			//
			//if (cosom < 0.9999f)
			//{
			//	const float32 omega = acos(cosom);
			//	const float32 invSin = 1.f / sin(omega);
			//	scale0 = sin((1.f - f) * omega) * invSin;
			//	scale1 = sin(f * omega) * invSin;
			//}
			//else
			//{
			//	// Use linear interpolation if the CQuaternions
			//	// are collinear
			//	scale0 = 1.0f - f;
			//	scale1 = f;
			//}
			//
			//if (rawCosm < 0.0f)
			//{
			//	scale1 = -scale1;
			//}
			//
			//Quaternion retVal;
			//retVal.x = scale0 * a.x + scale1 * b.x;
			//retVal.y = scale0 * a.y + scale1 * b.y;
			//retVal.z = scale0 * a.z + scale1 * b.z;
			//retVal.w = scale0 * a.w + scale1 * b.w;
			//retVal.Normalize();
			//return retVal;

			return toQuaternion( DirectX::XMQuaternionSlerpV(toXMVECTOR(a.Quat), toXMVECTOR(b.Quat), DirectX::XMVectorReplicate(t)) );
		}

		// 連結する
		// q回転、p回転
		Quaternion Concatenate(const Quaternion& q, const Quaternion& p);

		static Quaternion Euler(const float32 x, const float32 y, const float32 z)
		{
			//XMVECTOR Quaternion = DirectX::XMQuaternionIdentity();
			//
			//const XMVECTOR axisX = XMVectorSet(1.0f, 0.f, 0.f, 0.f);
			//const XMVECTOR axisY = XMVectorSet(0.0f, 1.0f, 0.f, 0.f);
			//const XMVECTOR axisZ = XMVectorSet(0.0f, 0.f, 1.0f, 0.f);
			//
			//XMVECTOR rotateX = XMQuaternionRotationAxis(axisX, XMConvertToRadians(x));
			//rotateX = XMVector4Normalize(rotateX);
			//XMVECTOR rotateY = XMQuaternionRotationAxis(axisY, XMConvertToRadians(y));
			//rotateY = XMVector4Normalize(rotateY);
			//XMVECTOR rotateZ = XMQuaternionRotationAxis(axisZ, XMConvertToRadians(z));
			//rotateZ = XMVector4Normalize(rotateZ);
			//
			//Quaternion = XMQuaternionMultiply(Quaternion, rotateX);
			//Quaternion = XMQuaternionMultiply(Quaternion, rotateY);
			//Quaternion = XMQuaternionMultiply(Quaternion, rotateZ);
			//Quaternion = XMQuaternionNormalize(Quaternion);
			//
			//XMFLOAT4 quat;
			//XMStoreFloat4(&quat, Quaternion);
			//
			//return Quaternion::Quaternion(quat.x, quat.y, quat.z, quat.w);

			return toQuaternion( DirectX::XMQuaternionRotationRollPitchYawFromVector(DirectX::XMVectorSet(z, y, z, 0.f)) );
		}

	public:
		union
		{
			float32 x;
			float32 y;
			float32 z;
			float32 w;

			DirectX::XMFLOAT4 Quat;
		};

		static const Quaternion Identity;

	private:
		//inline explicit Quaternion(const DirectX::XMFLOAT4& quat) noexcept : Quat(quat)
		//{
		//}


		inline static DirectX::XMVECTOR toXMVECTOR(const DirectX::XMFLOAT4& quat)
		{
			return DirectX::XMLoadFloat4(&quat);
		}

		inline static Quaternion toQuaternion(const DirectX::XMVECTOR& vec)
		{
			XMFLOAT4 quat;
			DirectX::XMStoreFloat4(&quat, vec);

			return Quaternion(quat);
		}
	};
}

#endif // !_MATH_QUATERNION_H_
