#pragma once

#ifndef _MATH_VECTOR3_H_
#define _MATH_VECTOR3_H_

//class Vector3;
#include"Quaternion.h"

namespace Aegis
{
	class Vector3 {
	public:

		friend class Quaternion;

		Vector3() = default;
		Vector3(const float _x, const float _y, const float _z) noexcept
		{
			x = _x;
			y = _y;
			z = _z;
		}

		Vector3(const Vector3&) = default;

		Vector3& operator=(const Vector3&) = default;
		Vector3(Vector3&&) = default;
		Vector3& operator=(Vector3&&) = default;

		inline friend Vector3 operator+(const Vector3& a, const Vector3& b)
		{
			return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
		}

		inline friend Vector3 operator+(const Vector3& Vector, float scalar)
		{
			return Vector3(Vector.x + scalar, Vector.y + scalar, Vector.z + scalar);
		}

		inline friend Vector3 operator+(float scalar, const Vector3& Vector)
		{
			return Vector3(Vector.x + scalar, Vector.y + scalar, Vector.z + scalar);
		}

		inline friend Vector3 operator-(const Vector3& a, const Vector3& b)
		{
			return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
		}

		inline friend Vector3 operator-(const Vector3& Vector, float scalar)
		{
			return Vector3(Vector.x - scalar, Vector.y - scalar, Vector.z - scalar);
		}

		inline friend Vector3 operator-(float scalar, const Vector3& Vector)
		{
			return Vector3(Vector.x - scalar, Vector.y - scalar, Vector.z - scalar);
		}

		inline friend Vector3 operator*(const Vector3& left, const Vector3& right)
		{
			return Vector3(left.x * right.x, left.y * right.y, left.z * right.z);
		}

		inline friend Vector3 operator*(const Vector3& Vector, float scalar)
		{
			return Vector3(Vector.x * scalar, Vector.y * scalar, Vector.z * scalar);
		}

		inline friend Vector3 operator*(float scalar, const Vector3& Vector)
		{
			return Vector3(Vector.x * scalar, Vector.y * scalar, Vector.z * scalar);
		}

		inline float& operator[](const int index)
		{
			return f[index];
		}

		inline float operator[](const int index) const
		{
			return f[index];
		}

		inline Vector3& operator+=(const Vector3& vector)
		{
			x += vector.x;
			y += vector.y;
			z += vector.z;
			return *this;
		}

		inline Vector3& operator+=(const float scalar)
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

		inline Vector3& operator-=(const float scalar)
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

		inline Vector3& operator*=(const float scalar)
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

		inline Vector3& operator/=(float scalar)
		{
			x /= scalar;
			y /= scalar;
			z /= scalar;
			return *this;
		}

		inline float Length()
		{
			return (sqrt(x * x + y * y + z * z));
		}

		inline void Normalize()
		{
			float length = Length();
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

		inline static float Dot(const Vector3& a, const Vector3& b)
		{
			return (a.x * b.x + a.y * b.y + a.z * b.z);
		}

		inline static Vector3 Cross(const Vector3& a, const Vector3& b)
		{
			Vector3 temp;
			temp.x = a.y * b.z - a.z * b.y;
			temp.y = a.z * b.x - a.x * b.z;
			temp.z = a.x * b.y - a.y * b.x;
			return temp;
		}

		inline Vector3 Lerp(const Vector3& a, const Vector3& b, float t)
		{
			return Vector3(a + t * (b - a));
		}

		inline Vector3 Reflect(const Vector3& v, const Vector3& n)
		{
			return v - 2.0f * Vector3::Dot(v, n) * n;
		}

		// クォータニオンによるVector3の変換
		static Vector3 Transform(const Vector3& v, const Quaternion& q)
		{
			// v + 2.0 * Cross(q.xyz, Cross(q.xyz,v) + q.w * v);
			Vector3 qv(q.x, q.y, q.z);
			Vector3 retVal = v;
			retVal += 2.0f * Vector3::Cross(qv, Vector3::Cross(qv, v) + q.w * v);

			return retVal;
		}

		public:
			union
			{
				float x;
				float y;
				float z;

				float f[3] = { 0.f };
			};

		static const Vector3 Front;
		static const Vector3 Right;
		static const Vector3 Back;
		static const Vector3 Left;
		static const Vector3 Up;
		static const Vector3 Down;
		static const Vector3 Zero;
		static const Vector3 One;
		static const Vector3 Minus;
	};
}

#endif // !_MATH_VECTOR3_H_