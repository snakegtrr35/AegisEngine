#include	"Math.h"

static random_device rnd;							// 非決定的な乱数生成器

inline const int AeigisMath::Get_Random(int min, int max)
{
	mt19937_64 mt(rnd());								//  メルセンヌ・ツイスタの64ビット版、引数は初期シード値
	uniform_int_distribution<> rand(min, max);			// [min, max] 範囲の一様乱数

	return rand(mt);
}

inline const float AeigisMath::Get_Random(float min, float max)
{
	mt19937_64 mt(rnd());								//  メルセンヌ・ツイスタの64ビット版、引数は初期シード値
	uniform_real_distribution<> rand(min, max);			// [min, max] 範囲の一様乱数

	return rand(mt);
}

inline const double AeigisMath::Get_Random(double min, double max)
{
	mt19937_64 mt(rnd());								//  メルセンヌ・ツイスタの64ビット版、引数は初期シード値
	uniform_real_distribution<> rand(min, max);			// [min, max] 範囲の一様乱数

	return rand(mt);
}


const bool AeigisMath::Random_Bool(const float probability)
{
	mt19937_64 mt(rnd());							//  メルセンヌ・ツイスタの64ビット版、引数は初期シード値
	bernoulli_distribution uid(probability);

	return uid(mt);
}


float AeigisMath::LerpEx(const float y1, const float y2, const float tx, const float x1, const float x2)
{
	float dx, dy;

	dx = x2 - x1;
	dy = y2 - y1;

	return (y1 + (dy / dx) * (tx - x1));
}


short AeigisMath::Loop_Plus(short tx, short max)
{
	return (tx % max);
}

short AeigisMath::Loop_Minus(short tx, short max)
{
	return ((tx + max) % max);
}

// クォータニオンによるVector3の変換
AeigisMath::VECTOR3 AeigisMath::VECTOR3::Transform(const VECTOR3& v, const class Quaternion& q)
{
	// v + 2.0 * Cross(q.xyz, Cross(q.xyz,v) + q.w * v);
	VECTOR3 qv(q.x, q.y, q.z);
	VECTOR3 retVal = v;
	retVal += 2.0f * VECTOR3::Cross(qv, VECTOR3::Cross(qv, v) + q.w * v);

	return retVal;
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