#pragma once

#ifndef _INCLUDE_MATH_VECTOR4_H_
#define _INCLUDE_MATH_VECTOR4_H_

namespace aegis
{
	class Vector4 : public DirectX::XMFLOAT4 {
	public:

		friend class Quaternion;

		Vector4() = default;
		Vector4(const float32 _x, const float32 _y, const float32 _z, const float32 _w) noexcept
		{
			x = _x;
			y = _y;
			z = _z;
			w = _w;
		}

		explicit Vector4(const DirectX::XMFLOAT4 vector) noexcept : DirectX::XMFLOAT4(vector)
		{
		}

		Vector4(const Vector4&) = default;

		Vector4& operator=(const Vector4&) = default;
		Vector4(Vector4&&) = default;
		Vector4& operator=(Vector4&&) = default;

		Vector4& operator=(const DirectX::XMFLOAT4& vector)
		{
			this->x = vector.x;
			this->y = vector.y;
			this->z = vector.z;

			return *this;
		}

		inline friend Vector4 operator+(const Vector4& a, const Vector4& b)
		{
			return Vector4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
		}

		inline friend Vector4 operator+(const Vector4& Vector, float32 scalar)
		{
			return Vector4(Vector.x + scalar, Vector.y + scalar, Vector.z + scalar, Vector.w + scalar);
		}

		inline friend Vector4 operator+(float32 scalar, const Vector4& Vector)
		{
			return Vector4(Vector.x + scalar, Vector.y + scalar, Vector.z + scalar, Vector.w + scalar);
		}

		inline friend Vector4 operator-(const Vector4& a, const Vector4& b)
		{
			return Vector4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
		}

		inline friend Vector4 operator-(const Vector4& Vector, float32 scalar)
		{
			return Vector4(Vector.x - scalar, Vector.y - scalar, Vector.z - scalar, Vector.w - scalar);
		}

		inline friend Vector4 operator-(float32 scalar, const Vector4& Vector)
		{
			return Vector4(Vector.x - scalar, Vector.y - scalar, Vector.z - scalar, Vector.w - scalar);
		}

		inline friend Vector4 operator*(const Vector4& left, const Vector4& right)
		{
			return Vector4(left.x * right.x, left.y * right.y, left.z * right.z, left.w * right.w);
		}

		inline friend Vector4 operator*(const Vector4& Vector, float32 scalar)
		{
			return Vector4(Vector.x * scalar, Vector.y * scalar, Vector.z * scalar, Vector.w * scalar);
		}

		inline friend Vector4 operator*(float32 scalar, const Vector4& Vector)
		{
			return Vector4(Vector.x * scalar, Vector.y * scalar, Vector.z * scalar, Vector.w * scalar);
		}

		inline friend Vector4 operator/(const Vector4& left, const Vector4& right)
		{
			return Vector4(left.x / right.x, left.y / right.y, left.z / right.z, left.w / right.w);
		}

		inline friend Vector4 operator/(const Vector4& Vector, float32 scalar)
		{
			return Vector4(Vector.x / scalar, Vector.y / scalar, Vector.z / scalar, Vector.w / scalar);
		}

		inline friend Vector4 operator/(float32 scalar, const Vector4& Vector)
		{
			return Vector4(scalar / Vector.x, scalar / Vector.y, scalar / Vector.z, scalar / Vector.w);
		}

		inline float32& operator[](const int32 index)
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

		inline Vector4& operator+=(const Vector4& vector)
		{
			x += vector.x;
			y += vector.y;
			z += vector.z;
			return *this;
		}

		inline Vector4& operator+=(const float32 scalar)
		{
			x += scalar;
			y += scalar;
			z += scalar;
			return *this;
		}

		inline Vector4& operator-=(const Vector4& vector)
		{
			x -= vector.x;
			y -= vector.y;
			z -= vector.z;
			return *this;
		}

		inline Vector4& operator-=(const float32 scalar)
		{
			x -= scalar;
			y -= scalar;
			z -= scalar;
			return *this;
		}

		inline Vector4& operator*=(const Vector4& vector)
		{
			x *= vector.x;
			y *= vector.y;
			z *= vector.z;
			return *this;
		}

		inline Vector4& operator*=(const float32 scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}

		inline Vector4& operator/=(const Vector4& vector)
		{
			x /= vector.x;
			y /= vector.y;
			z /= vector.z;
			return *this;
		}

		inline Vector4& operator/=(float32 scalar)
		{
			x /= scalar;
			y /= scalar;
			z /= scalar;
			return *this;
		}

		inline float32 Length()
		{
			return (std::sqrt(x * x + y * y + z * z + w * w));
		}

		inline float32 LengthSq()
		{
			return (x * x + y * y + z * z + w * w);
		}

		inline void Normalize()
		{
			float32 length = Length();
			x /= length;
			y /= length;
			z /= length;
			w /= length;
		}

		inline Vector4 Normalize(const Vector4& vector)
		{
			Vector4 temp = vector;
			temp.Normalize();
			return temp;
		}

		inline static float32 Dot(const Vector4& a, const Vector4& b)
		{
			return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
		}

		inline static Vector4 Cross(const Vector4& a, const Vector4& b, const Vector4& c)
		{
			//Vector4 temp;
			//temp.x = a.y * b.z - a.z * b.y;
			//temp.y = a.z * b.x - a.x * b.z;
			//temp.z = a.x * b.y - a.y * b.x;
			//return temp;

			return toVector4( DirectX::XMVector4Cross(toXMVECTOR(a), toXMVECTOR(b), toXMVECTOR(c)) );
		}

		inline Vector4 Lerp(const Vector4& a, const Vector4& b, float32 t)
		{
			return Vector4(a + t * (b - a));
		}

		inline Vector4 Reflect(const Vector4& v, const Vector4& n)
		{
			//return v - 2.0f * Vector4::Dot(v, n) * n;
			return toVector4( DirectX::XMVector4Reflect(toXMVECTOR(v), toXMVECTOR(n)) );
		}

	public:
		static const Vector4 Zero;
		static const Vector4 One;
		static const Vector4 Minus;

	private:

		inline static DirectX::XMVECTOR toXMVECTOR(const DirectX::XMFLOAT4& vec)
		{
			return DirectX::XMLoadFloat4(&vec);
		}

		inline static Vector4 toVector4(const DirectX::XMVECTOR& vec)
		{
			XMFLOAT4 f;
			DirectX::XMStoreFloat4(&f, vec);

			return Vector4(f);
		}
	};
}

#endif // !_INCLUDE_MATH_VECTOR4_H_
