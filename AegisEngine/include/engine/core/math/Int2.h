#pragma once

#ifndef _MATH_INT2_H_
#define _MATH_INT2_H_

namespace Aegis
{
	class Int2 {
	public:
		Int2() noexcept;
		Int2(int _x, int _y) noexcept;

		Int2(const Int2&) = default;

		Int2& operator=(const Int2&) = default;
		Int2(Int2&&) = default;
		Int2& operator=(Int2&&) = default;

		inline friend Int2 operator+(const Int2& a, const Int2& b)
		{
			return Int2(a.x + b.x, a.y + b.y);
		}

		inline friend Int2 operator+(const Int2& Vector, int scalar)
		{
			return Int2(Vector.x + scalar, Vector.y + scalar);
		}

		inline friend Int2 operator+(float scalar, const Int2& Vector)
		{
			return Int2(Vector.x + scalar, Vector.y + scalar);
		}

		inline friend Int2 operator-(const Int2& a, const Int2& b)
		{
			return Int2(a.x - b.x, a.y - b.y);
		}

		inline friend Int2 operator-(const Int2& Vector, int scalar)
		{
			return Int2(Vector.x - scalar, Vector.y - scalar);
		}

		inline friend Int2 operator-(float scalar, const Int2& Vector)
		{
			return Int2(Vector.x - scalar, Vector.y - scalar);
		}

		inline friend Int2 operator*(const Int2& a, const Int2& b)
		{
			return Int2(a.x * b.x, a.y * b.y);
		}

		inline friend Int2 operator*(const Int2& Vector, int scalar)
		{
			return Int2(Vector.x * scalar, Vector.y * scalar);
		}

		inline friend Int2 operator*(float scalar, const Int2& Vector)
		{
			return Int2(Vector.x * scalar, Vector.y * scalar);
		}

		inline Int2& operator+=(const Int2& vector)
		{
			x += vector.x;
			y += vector.y;
			return *this;
		}

		inline Int2& operator+=(const int scalar)
		{
			x += scalar;
			y += scalar;
			return *this;
		}

		inline Int2& operator-=(const Int2& vector)
		{
			x -= vector.x;
			y -= vector.y;
			return *this;
		}

		inline Int2& operator-=(const int scalar)
		{
			x -= scalar;
			y -= scalar;
			return *this;
		}

		inline Int2& operator*=(const Int2& vector)
		{
			x *= vector.x;
			y *= vector.y;
			return *this;
		}

		inline Int2& operator*=(const int scalar)
		{
			x *= scalar;
			y *= scalar;
			return *this;
		}

		inline Int2& operator/=(const Int2& vector)
		{
			x /= vector.x;
			y /= vector.y;
			return *this;
		}

		inline Int2& operator/=(const int scalar)
		{
			x /= scalar;
			y /= scalar;
			return *this;
		}

		inline float Length()
		{
			return sqrtf(x * x + y * y);
		}

		inline void Normalize()
		{
			float length = Length();
			x /= length;
			y /= length;
		}

		inline Int2 Normalize(const Int2& vector)
		{
			Int2 temp = vector;
			temp.Normalize();
			return temp;
		}

		inline float Dot(const Int2& a, const Int2& b)
		{
			return (a.x * b.x + a.y * b.y);
		}

		inline Int2 Lerp(const Int2& a, const Int2& b, float t)
		{
			return Int2(a + t * (b - a));
		}

		inline Int2 Reflect(const Int2& v, const Int2& n)
		{
			return v - 2.0f * Int2::Dot(v, n) * n;
		}
	public:
		int x;
		int y;
	};

}

#endif // !_MATH_INT2_H_