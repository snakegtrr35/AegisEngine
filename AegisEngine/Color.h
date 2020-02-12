#pragma once

#ifndef COLOR_H
#define COLOR_H

//! �F�\����
struct COLOR
{
	float r;
	float g;
	float b;
	float a;

	COLOR() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
	COLOR(float _r, float _g, float _b, float _a)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	// ������Z( = )
	COLOR& operator = (const COLOR& color)
	{
		r = color.r; g = color.g; b = color.b, a = color.a;
		return *this;
	}

	// �a( + )
	const COLOR operator + (const COLOR& color) const
	{
		return COLOR(r + color.r, g + color.g, b + color.b, a + color.a);
	}

	// ��( - )
	const COLOR operator - (const COLOR& color) const
	{
		return COLOR(r - color.r, g - color.g, b - color.b, a - color.a);
	}

	// ��( * )
	const COLOR operator * (const COLOR& color) const
	{
		return COLOR(r * color.r, g * color.g, b * color.b, a * color.a);
	}

	// ��( / )
	const COLOR operator / (const COLOR& color) const
	{
		return COLOR(r / color.r, g / color.g, b / color.b, a / color.a);
	}

	// ���g�ɑ΂���a( += ) 
	COLOR& operator += (const COLOR& color)
	{
		r += color.r; g += color.g;	b += color.b; a += color.a;
		return *this;
	}

	// ���g�ɑ΂��鍷( -= ) 
	COLOR& operator -= (const COLOR& color)
	{
		r -= color.r; g -= color.g;	b -= color.b; a -= color.a;
		return *this;
	}

	// ���g�ɑ΂����( *= ) 
	COLOR& operator *= (const COLOR& color)
	{
		r *= color.r; g *= color.g;	b *= color.b; a *= color.a;
		return *this;
	}

	// ���g�ɑ΂��鏤( /= ) 
	COLOR& operator /= (const COLOR& color)
	{
		r /= color.r; g /= color.g;	b /= color.b; a /= color.a;
		return *this;
	}

	bool operator !=(const COLOR& color)
	{
		if (r != color.r) return true;
		if (g != color.g) return true;
		if (b != color.b) return true;
		if (a != color.a) return true;
		return false;
	}

	bool operator ==(const COLOR& color)
	{
		if (r != color.r) return false;
		if (g != color.g) return false;
		if (b != color.b) return false;
		if (a != color.a) return false;
		return true;
	}
};

template<typename Archive>
void serialize(Archive& ar, COLOR& color)
{
	ar(color.r, color.g, color.b, color.a);
}

#endif // !COLOR_H