#pragma once

#ifndef COLOR_H
#define COLOR_H

namespace aegis
{

	//! 色構造体
	struct COLOR
	{
		float32 r;
		float32 g;
		float32 b;
		float32 a;

		COLOR() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
		COLOR(float32 _r, float32 _g, float32 _b, float32 _a = 1.0f)
		{
			r = _r;
			g = _g;
			b = _b;
			a = _a;
		}

		// 代入演算( = )
		COLOR& operator=(const COLOR& color)
		{
			r = color.r; g = color.g; b = color.b, a = color.a;
			return *this;
		}

		// 和( + )
		const COLOR operator+(const COLOR& color) const
		{
			return COLOR(r + color.r, g + color.g, b + color.b, a + color.a);
		}

		// 差( - )
		const COLOR operator-(const COLOR& color) const
		{
			return COLOR(r - color.r, g - color.g, b - color.b, a - color.a);
		}

		// 積( * )
		const COLOR operator*(const COLOR& color) const
		{
			return COLOR(r * color.r, g * color.g, b * color.b, a * color.a);
		}

		// 商( / )
		const COLOR operator/(const COLOR& color) const
		{
			return COLOR(r / color.r, g / color.g, b / color.b, a / color.a);
		}

		// 自身に対する和( += ) 
		COLOR& operator+=(const COLOR& color)
		{
			r += color.r;
			g += color.g;
			b += color.b;
			a += color.a;
			return *this;
		}

		// 自身に対する差( -= ) 
		COLOR& operator-=(const COLOR& color)
		{
			r -= color.r;
			g -= color.g;
			b -= color.b;
			a -= color.a;
			return *this;
		}

		// 自身に対する積( *= ) 
		COLOR& operator*=(const COLOR& color)
		{
			r *= color.r;
			g *= color.g;
			b *= color.b;
			a *= color.a;
			return *this;
		}

		// 自身に対する商( /= ) 
		COLOR& operator/=(const COLOR& color)
		{
			r /= color.r;
			g /= color.g;
			b /= color.b;
			a /= color.a;
			return *this;
		}

		bool operator !=(const COLOR& color)
		{
			return (color.r != r) || (color.g != g) || (color.b != b) || (color.a != a);
		}

		bool operator ==(const COLOR& color)
		{
			return (color.r == r) && (color.g == g) && (color.b == b) && (color.a == a);
		}

		template<class Archive>
		void save(Archive& archive) const
		{
			archive(CEREAL_NVP(r),
					CEREAL_NVP(g),
					CEREAL_NVP(b),
					CEREAL_NVP(a)
			);
		}

		template<class Archive>
		void load(Archive& archive)
		{
			archive(CEREAL_NVP(r),
					CEREAL_NVP(g),
					CEREAL_NVP(b),
					CEREAL_NVP(a)
			);
		}
	};
}

#endif // !COLOR_H