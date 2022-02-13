#pragma once

#ifndef _MATH_MATHDEF_H_
#define _MATH_MATHDEF_H_

#include <cmath>
#include <limits>

namespace aegis
{
	namespace Math
	{
		static constexpr float NaN = std::numeric_limits<float>::quiet_NaN();
		static constexpr float Inf = std::numeric_limits<float>::infinity();
		static constexpr float Epsilon = std::numeric_limits<float>::epsilon();
		static constexpr float PI = 3.1415926535897931f;
		static constexpr float PI2 = PI * 2.0f;
		static constexpr float INV_PI = 1.0f / PI;
		static constexpr float PI_DIV2 = PI * 0.5f;

		inline constexpr float DegreeToRadian(float degree) { return degree * (PI / 180.0f); }
		inline constexpr float RadianToDegree(float radian) { return radian * (180.0f / PI); }

		inline constexpr float Lerp(const float a, const float b, float t)
		{
			return a + t * (b - a);
		}

		inline constexpr double Lerp(const double a, const double b, double t)
		{
			return a + t * (b - a);
		}
	}
}

#endif // !_MATH_MATHDEF_H_