#pragma once

#ifndef _MATH_MATHDEF_H_
#define _MATH_MATHDEF_H_

#include <cmath>
#include <limits>

namespace aegis
{
	namespace Math
	{
		static constexpr float32 NaN = std::numeric_limits<float>::quiet_NaN();
		static constexpr float32 Inf = std::numeric_limits<float>::infinity();
		static constexpr float32 Epsilon = std::numeric_limits<float>::epsilon();
		static constexpr float32 PI = 3.1415926535897931f;
		static constexpr float32 PI2 = PI * 2.0f;
		static constexpr float32 INV_PI = 1.0f / PI;
		static constexpr float32 PI_DIV2 = PI * 0.5f;

		static constexpr float32 Float32Max = 3.402823466e+38f;

		inline constexpr float32 DegreeToRadian(float degree) { return degree * (PI / 180.0f); }
		inline constexpr float32 RadianToDegree(float radian) { return radian * (180.0f / PI); }

		inline constexpr float32 Lerp(const float a, const float b, float t)
		{
			return a + t * (b - a);
		}

		inline constexpr float64 Lerp(const float64 a, const float64 b, float64 t)
		{
			return a + t * (b - a);
		}
	}
}

#endif // !_MATH_MATHDEF_H_