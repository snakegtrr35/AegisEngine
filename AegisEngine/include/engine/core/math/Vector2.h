﻿#ifndef _MATH_VECTOR2_H_
#define _MATH_VECTOR2_H_

namespace aegis
{
	class Vector2 : public DirectX::XMFLOAT2 {
	public:
		Vector2() = default;
		explicit Vector2(const float32 _x, const float32 _y) noexcept
		{
			x = _x;
			y = _y;
		}

		explicit Vector2(const DirectX::XMFLOAT2 vector) noexcept : DirectX::XMFLOAT2(vector)
		{
		}


		Vector2(const Vector2&) = default;

		Vector2& operator=(const Vector2&) = default;
		Vector2(Vector2&&) = default;
		Vector2& operator=(Vector2&&) = default;

		Vector2& operator=(const DirectX::XMFLOAT2& vector)
		{
			this->x = vector.x;
			this->y = vector.y;

			return *this;
		}

		inline friend Vector2 operator+(const Vector2& a, const Vector2& b)
		{
			return Vector2(a.x + b.x, a.y + b.y);
		}

		inline friend Vector2 operator+(const Vector2& Vector, float32 scalar)
		{
			return Vector2(Vector.x + scalar, Vector.y + scalar);
		}

		inline friend Vector2 operator+(float32 scalar, const Vector2& Vector)
		{
			return Vector2(Vector.x + scalar, Vector.y + scalar);
		}

		inline friend Vector2 operator-(const Vector2& a, const Vector2& b)
		{
			return Vector2(a.x - b.x, a.y - b.y);
		}

		inline friend Vector2 operator-(const Vector2& Vector, float32 scalar)
		{
			return Vector2(Vector.x - scalar, Vector.y - scalar);
		}

		inline friend Vector2 operator-(float32 scalar, const Vector2& Vector)
		{
			return Vector2(Vector.x - scalar, Vector.y - scalar);
		}

		inline friend Vector2 operator*(const Vector2& a, const Vector2& b)
		{
			return Vector2(a.x * b.x, a.y * b.y);
		}

		inline friend Vector2 operator*(const Vector2& Vector, float32 scalar)
		{
			return Vector2(Vector.x * scalar, Vector.y * scalar);
		}

		inline friend Vector2 operator*(float32 scalar, const Vector2& Vector)
		{
			return Vector2(Vector.x * scalar, Vector.y * scalar);
		}

		inline friend Vector2 operator/(const Vector2& a, const Vector2& b)
		{
			return Vector2(a.x / b.x, a.y / b.y);
		}

		inline friend Vector2 operator/(const Vector2& Vector, float32 scalar)
		{
			return Vector2(Vector.x / scalar, Vector.y / scalar);
		}

		inline friend Vector2 operator/(float32 scalar, const Vector2& Vector)
		{
			return Vector2(scalar / Vector.x, scalar / Vector.y);
		}

		inline Vector2& operator+=(const Vector2& vector)
		{
			x += vector.x;
			y += vector.y;
			return *this;
		}

		inline Vector2& operator+=(const float32 scalar)
		{
			x += scalar;
			y += scalar;
			return *this;
		}

		inline Vector2& operator-=(const Vector2& vector)
		{
			x -= vector.x;
			y -= vector.y;
			return *this;
		}

		inline Vector2& operator-=(float32 scalar)
		{
			x -= scalar;
			y -= scalar;
			return *this;
		}

		inline Vector2& operator*=(const Vector2& vector)
		{
			x *= vector.x;
			y *= vector.y;
			return *this;
		}

		inline Vector2& operator*=(float32 scalar)
		{
			x *= scalar;
			y *= scalar;
			return *this;
		}

		inline Vector2& operator/=(const Vector2& vector)
		{
			x /= vector.x;
			y /= vector.y;
			return *this;
		}

		inline Vector2& operator/=(float32 scalar)
		{
			x /= scalar;
			y /= scalar;
			return *this;
		}

		inline float32 Length()
		{
			return sqrtf(x * x + y * y);
		}

		inline void Normalize()
		{
			float32 length = Length();
			x /= length;
			y /= length;
		}

		inline Vector2 Normalize(const Vector2& vector)
		{
			Vector2 temp = vector;
			temp.Normalize();
			return temp;
		}

		inline float32 Dot(const Vector2& a, const Vector2& b)
		{
			return (a.x * b.x + a.y * b.y);
		}

		inline Vector2 Lerp(const Vector2& a, const Vector2& b, float32 t)
		{
			return Vector2(a + t * (b - a));
		}

		inline Vector2 Reflect(const Vector2& v, const Vector2& n)
		{
			return v - 2.0f * Vector2::Dot(v, n) * n;
		}
	public:
		//union
		//{
		//	float32 x;
		//	float32 y;
		//
		//	float32 f[2] = { 0.f };
		//};

		static const Vector2 Zero;
		static const Vector2 One;
		static const Vector2 Minus;
	};

}

#endif // !_MATH_VECTOR2_H_