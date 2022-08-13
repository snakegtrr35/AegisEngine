#include "Math.h"
#include <random>



namespace aegis
{
	namespace Math
	{
		float32 LerpEx(const float32 y1, const float32 y2, const float32 tx, const float32 x1, const float32 x2)
		{
			float32 dx, dy;

			dx = x2 - x1;
			dy = y2 - y1;

			return (y1 + (dy / dx) * (tx - x1));
		}


		short Loop_Plus(short tx, short max)
		{
			return (tx % max);
		}

		short Loop_Minus(short tx, short max)
		{
			return ((tx + max) % max);
		}
	}
}

XMFLOAT3 operator+(const XMFLOAT3& left, const XMFLOAT3 right)
{
	return XMFLOAT3(left.x + right.x, left.y + right.y, left.z + right.z);
}

XMFLOAT3 operator-(const XMFLOAT3& left, const XMFLOAT3 right)
{
	return XMFLOAT3(left.x - right.x, left.y - right.y, left.z - right.z);
}

XMFLOAT3 operator*(const XMFLOAT3& left, const float s)
{
	return XMFLOAT3(left.x * s, left.y * s, left.z * s);
}

XMFLOAT3 operator*(const XMFLOAT3& left, const XMFLOAT3 right)
{
	return XMFLOAT3(left.x * right.x, left.y * right.y, left.z * right.z);
}

XMFLOAT3 operator/(const XMFLOAT3& left, const XMFLOAT3 right)
{
	return XMFLOAT3(left.x / right.x, left.y / right.y, left.z / -right.z);
}

XMFLOAT3 operator/(const float s, const XMFLOAT3 right)
{
	return XMFLOAT3(s / right.x, s / right.y, s / -right.z);
}



XMFLOAT4 operator*(const XMFLOAT4& left, const float s)
{
	return XMFLOAT4(left.x * s, left.y * s, left.z * s, left.w * s);
}