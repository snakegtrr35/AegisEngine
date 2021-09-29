#ifndef _MATH_VECTOR2_H_
#define _MATH_VECTOR2_H_

namespace Aegis
{
	class Vector2 {
	public:
		Vector2() = default;
		explicit Vector2(const float _x, const float _y) noexcept;
		Vector2(const Vector2&) = default;

		Vector2& operator=(const Vector2&) = default;
		Vector2(Vector2&&) = default;
		Vector2& operator=(Vector2&&) = default;

		friend Vector2 operator+(const Vector2& a, const Vector2& b);

		friend Vector2 operator+(const Vector2& Vector, float scalar);

		friend Vector2 operator+(float scalar, const Vector2& Vector);

		friend Vector2 operator-(const Vector2& a, const Vector2& b);

		friend Vector2 operator-(const Vector2& Vector, float scalar);

		friend Vector2 operator-(float scalar, const Vector2& Vector);

		friend Vector2 operator*(const Vector2& a, const Vector2& b);

		friend Vector2 operator*(const Vector2& Vector, float scalar);

		friend Vector2 operator*(float scalar, const Vector2& Vector);

		friend Vector2 operator/(const Vector2& a, const Vector2& b);

		friend Vector2 operator/(const Vector2& Vector, float scalar);

		friend Vector2 operator/(float scalar, const Vector2& Vector);

		inline Vector2& operator+=(const Vector2& vector)
		{
			x += vector.x;
			y += vector.y;
			return *this;
		}

		inline Vector2& operator+=(const float scalar)
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

		inline Vector2& operator-=(float scalar)
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

		inline Vector2& operator*=(float scalar)
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

		inline Vector2& operator/=(float scalar)
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

		inline Vector2 Normalize(const Vector2& vector)
		{
			Vector2 temp = vector;
			temp.Normalize();
			return temp;
		}

		inline float Dot(const Vector2& a, const Vector2& b)
		{
			return (a.x * b.x + a.y * b.y);
		}

		inline Vector2 Lerp(const Vector2& a, const Vector2& b, float t)
		{
			return Vector2(a + t * (b - a));
		}

		inline Vector2 Reflect(const Vector2& v, const Vector2& n)
		{
			return v - 2.0f * Vector2::Dot(v, n) * n;
		}
	public:
		union
		{
			float x;
			float y;

			float f[2] = { 0.f };
		};

		static const Vector2 Zero;
		static const Vector2 One;
		static const Vector2 Minus;
	};

}

#endif // !_MATH_VECTOR2_H_