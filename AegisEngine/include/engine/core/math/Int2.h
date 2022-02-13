#pragma once

#ifndef _MATH_INT2_H_
#define _MATH_INT2_H_

namespace aegis
{
	class Int2 : public DirectX::XMINT2 {
	public:
		Int2() noexcept : DirectX::XMINT2(0, 0)
		{}

		Int2(int32 _x, int32 _y) noexcept : DirectX::XMINT2(_x, _y)
		{}

		Int2(const Int2&) = default;

		Int2& operator=(const Int2&) = default;
		Int2(Int2&&) = default;
		Int2& operator=(Int2&&) = default;

		inline friend Int2 operator+(const Int2& a, const Int2& b)
		{
			return Int2(a.x + b.x, a.y + b.y);
		}

		inline friend Int2 operator+(const Int2& Vector, int32 scalar)
		{
			return Int2(Vector.x + scalar, Vector.y + scalar);
		}

		inline friend Int2 operator+(int32 scalar, const Int2& Vector)
		{
			return Int2(Vector.x + scalar, Vector.y + scalar);
		}

		inline friend Int2 operator-(const Int2& a, const Int2& b)
		{
			return Int2(a.x - b.x, a.y - b.y);
		}

		inline friend Int2 operator-(const Int2& Vector, int32 scalar)
		{
			return Int2(Vector.x - scalar, Vector.y - scalar);
		}

		inline friend Int2 operator-(int32 scalar, const Int2& Vector)
		{
			return Int2(Vector.x - scalar, Vector.y - scalar);
		}

		inline friend Int2 operator*(const Int2& a, const Int2& b)
		{
			return Int2(a.x * b.x, a.y * b.y);
		}

		inline friend Int2 operator*(const Int2& Vector, int32 scalar)
		{
			return Int2(Vector.x * scalar, Vector.y * scalar);
		}

		inline friend Int2 operator*(int32 scalar, const Int2& Vector)
		{
			return Int2(Vector.x * scalar, Vector.y * scalar);
		}

		inline Int2& operator+=(const Int2& vector)
		{
			x += vector.x;
			y += vector.y;
			return *this;
		}

		inline Int2& operator+=(const int32 scalar)
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

		inline Int2& operator-=(const int32 scalar)
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

		inline Int2& operator*=(const int32 scalar)
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

		inline Int2& operator/=(const int32 scalar)
		{
			x /= scalar;
			y /= scalar;
			return *this;
		}

		inline float32 Length()
		{
			return std::sqrtf(static_cast<float32>(x * x + y * y));
		}

		inline float32 LengthSq()
		{
			return (x * x + y * y);
		}

		inline void Normalize()
		{
			float32 length = Length();
			x /= length;
			y /= length;
		}

		inline Int2 Normalize(const Int2& vector)
		{
			Int2 temp = vector;
			temp.Normalize();
			return temp;
		}

		inline float32 Dot(const Int2& a, const Int2& b)
		{
			return (a.x * b.x + a.y * b.y);
		}

		inline Int2 Lerp(const Int2& a, const Int2& b, int32 t)
		{
			return Int2(a + t * (b - a));
		}

		inline Int2 Reflect(const Int2& v, const Int2& n)
		{
			return v - 2.0f * Int2::Dot(v, n) * n;
		}
	public:
		//int32 x;
		//int32 y;
	};

}

#endif // !_MATH_INT2_H_