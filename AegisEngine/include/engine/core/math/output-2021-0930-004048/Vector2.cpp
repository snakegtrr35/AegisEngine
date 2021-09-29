#include "Vector2.h"

namespace Aegis
{
	const Vector2 Vector2::Zero(0.f, 0.f);
	const Vector2 Vector2::One(1.0f, 1.0f);
	const Vector2 Vector2::Minus(-1.0f, -1.0f);

	inline Vector2::Vector2(const float _x, const float _y)
	{
		x = _x;
		y = _y;
	}

	inline Vector2 operator+(const Vector2& a, const Vector2& b)
	{
		return Vector2(a.x + b.x, a.y + b.y);
	}

	inline Vector2 operator+(const Vector2& Vector, float scalar)
	{
		return Vector2(Vector.x + scalar, Vector.y + scalar);
	}

	inline Vector2 operator+(float scalar, const Vector2& Vector)
	{
		return Vector2(Vector.x + scalar, Vector.y + scalar);
	}

	inline Vector2 operator-(const Vector2& a, const Vector2& b)
	{
		return Vector2(a.x - b.x, a.y - b.y);
	}

	inline Vector2 operator-(const Vector2& Vector, float scalar)
	{
		return Vector2(Vector.x - scalar, Vector.y - scalar);
	}

	inline Vector2 operator-(float scalar, const Vector2& Vector)
	{
		return Vector2(Vector.x - scalar, Vector.y - scalar);
	}

	inline Vector2 operator*(const Vector2& a, const Vector2& b)
	{
		return Vector2(a.x * b.x, a.y * b.y);
	}

	inline Vector2 operator*(const Vector2& Vector, float scalar)
	{
		return Vector2(Vector.x * scalar, Vector.y * scalar);
	}

	inline Vector2 operator*(float scalar, const Vector2& Vector)
	{
		return Vector2(Vector.x * scalar, Vector.y * scalar);
	}

	inline Vector2 operator/(const Vector2& a, const Vector2& b)
	{
		return Vector2(a.x / b.x, a.y / b.y);
	}

	inline Vector2 operator/(const Vector2& Vector, float scalar)
	{
		return Vector2(Vector.x / scalar, Vector.y / scalar);
	}

	inline Vector2 operator/(float scalar, const Vector2& Vector)
	{
		return Vector2(Vector.x / scalar, Vector.y / scalar);
	}
}