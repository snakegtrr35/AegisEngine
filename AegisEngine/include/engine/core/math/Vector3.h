#pragma once

#ifndef _MATH_VECTOR3_H_
#define _MATH_VECTOR3_H_

//class Vector3;
#include "Quaternion.h"

#include <DirectXMath.h>
using namespace DirectX;

namespace Aegis
{
	class Vector3 : public DirectX::XMFLOAT3 {
	public:

		Vector3() = default;
		Vector3(const float32 _x, const float32 _y, const float32 _z) noexcept
		{
			x = _x;
			y = _y;
			z = _z;
		}

		explicit Vector3(const DirectX::XMFLOAT3 vector) noexcept : DirectX::XMFLOAT3(vector)
		{
		}

		Vector3(const Vector3&) = default;

		Vector3& operator=(const Vector3&) = default;
		Vector3(Vector3&&) = default;
		Vector3& operator=(Vector3&&) = default;

		Vector3& operator=(const DirectX::XMFLOAT3& vector)
		{
			this->x = vector.x;
			this->y = vector.y;
			this->z = vector.z;

			return *this;
		}

		inline friend Vector3 operator+(const Vector3& a, const Vector3& b)
		{
			return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
		}

		inline friend Vector3 operator+(const Vector3& Vector, float32 scalar)
		{
			return Vector3(Vector.x + scalar, Vector.y + scalar, Vector.z + scalar);
		}

		inline friend Vector3 operator+(float32 scalar, const Vector3& Vector)
		{
			return Vector3(Vector.x + scalar, Vector.y + scalar, Vector.z + scalar);
		}

		inline friend Vector3 operator-(const Vector3& a, const Vector3& b)
		{
			return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
		}

		inline friend Vector3 operator-(const Vector3& Vector, float32 scalar)
		{
			return Vector3(Vector.x - scalar, Vector.y - scalar, Vector.z - scalar);
		}

		inline friend Vector3 operator-(float32 scalar, const Vector3& Vector)
		{
			return Vector3(Vector.x - scalar, Vector.y - scalar, Vector.z - scalar);
		}

		inline friend Vector3 operator*(const Vector3& left, const Vector3& right)
		{
			return Vector3(left.x * right.x, left.y * right.y, left.z * right.z);
		}

		inline friend Vector3 operator*(const Vector3& Vector, float32 scalar)
		{
			return Vector3(Vector.x * scalar, Vector.y * scalar, Vector.z * scalar);
		}

		inline friend Vector3 operator*(float32 scalar, const Vector3& Vector)
		{
			return Vector3(Vector.x * scalar, Vector.y * scalar, Vector.z * scalar);
		}

		inline friend Vector3 operator/(const Vector3& left, const Vector3& right)
		{
			return Vector3(left.x / right.x, left.y / right.y, left.z / right.z);
		}

		inline friend Vector3 operator/(const Vector3& Vector, float32 scalar)
		{
			return Vector3(Vector.x / scalar, Vector.y / scalar, Vector.z / scalar);
		}

		inline friend Vector3 operator/(float32 scalar, const Vector3& Vector)
		{
			return Vector3(scalar / Vector.x, scalar / Vector.y, scalar / Vector.z);
		}

		inline float32& operator[](const int32 index)
		{
			switch (index)
			{
				case 0 :
					return x;
				case 1:
					return y;
				case 2:
					return z;

				default:
					assert("index is out of range");
			}
		}

		inline float32 operator[](const int32 index) const
		{
			switch (index)
			{
				case 0:
					return x;
				case 1:
					return y;
				case 2:
					return z;

				default:
					assert("index is out of range");
			}
		}

		inline Vector3& operator+=(const Vector3& vector)
		{
			x += vector.x;
			y += vector.y;
			z += vector.z;
			return *this;
		}

		inline Vector3& operator+=(const float32 scalar)
		{
			x += scalar;
			y += scalar;
			z += scalar;
			return *this;
		}

		inline Vector3& operator-=(const Vector3& vector)
		{
			x -= vector.x;
			y -= vector.y;
			z -= vector.z;
			return *this;
		}

		inline Vector3& operator-=(const float32 scalar)
		{
			x -= scalar;
			y -= scalar;
			z -= scalar;
			return *this;
		}

		inline Vector3& operator*=(const Vector3& vector)
		{
			x *= vector.x;
			y *= vector.y;
			z *= vector.z;
			return *this;
		}

		inline Vector3& operator*=(const float32 scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}

		inline Vector3& operator/=(const Vector3& vector)
		{
			x /= vector.x;
			y /= vector.y;
			z /= vector.z;
			return *this;
		}

		inline Vector3& operator/=(float32 scalar)
		{
			x /= scalar;
			y /= scalar;
			z /= scalar;
			return *this;
		}

		inline float32 Length()
		{
			return (std::sqrt(x * x + y * y + z * z));
		}

		inline float32 LengthSq()
		{
			return (x * x + y * y + z * z);
		}

		inline void Normalize()
		{
			float32 length = Length();
			x /= length;
			y /= length;
			z /= length;
		}

		inline Vector3 Normalize(const Vector3& vector)
		{
			Vector3 temp = vector;
			temp.Normalize();
			return temp;
		}

		inline static float32 Dot(const Vector3& a, const Vector3& b)
		{
			return (a.x * b.x + a.y * b.y + a.z * b.z);
		}

		inline static Vector3 Cross(const Vector3& a, const Vector3& b)
		{
			//Vector3 temp;
			//temp.x = a.y * b.z - a.z * b.y;
			//temp.y = a.z * b.x - a.x * b.z;
			//temp.z = a.x * b.y - a.y * b.x;
			//return temp;

			return toVector3( DirectX::XMVector3Cross(toXMVECTOR(a), toXMVECTOR(b)) );
		}

		inline Vector3 Lerp(const Vector3& a, const Vector3& b, float32 t)
		{
			return Vector3(a + t * (b - a));
		}

		inline Vector3 Reflect(const Vector3& v, const Vector3& n)
		{
			//return v - 2.0f * Vector3::Dot(v, n) * n;
			return toVector3( DirectX::XMVector3Reflect(toXMVECTOR(v), toXMVECTOR(n)) );
		}

		// クォータニオンによるVector3の変換
		inline static Vector3 Transform(const Vector3& v, const Quaternion& q)
		{
			return toVector3(DirectX::XMVector3Rotate(toXMVECTOR(v), DirectX::XMLoadFloat4(&q.Quat)));
		}

		template<typename Archive>
		void serialize(Archive& ar, Vector3& vector)
		{
			ar(vector.x, vector.y, vector.z);
		}

	public:
		//union
		//{
		//	float32 x;
		//	float32 y;
		//	float32 z;
		//
		//	DirectX::XMFLOAT3 f;
		//};

		static const Vector3 Front;
		static const Vector3 Right;
		static const Vector3 Back;
		static const Vector3 Left;
		static const Vector3 Up;
		static const Vector3 Down;
		static const Vector3 Zero;
		static const Vector3 One;
		static const Vector3 Minus;

	private:
		//inline explicit Vector3(const DirectX::XMFLOAT3& vector) noexcept : XMFLOAT3(vector)
		//{
		//}


		inline static DirectX::XMVECTOR toXMVECTOR(const DirectX::XMFLOAT3& vec)
		{
			return DirectX::XMLoadFloat3(&vec);
		}

		inline static Vector3 toVector3(const DirectX::XMVECTOR& vec)
		{
			XMFLOAT3 f;
			DirectX::XMStoreFloat3(&f, vec);

			return Vector3(f);
		}
	};
}

#endif // !_MATH_VECTOR3_H_