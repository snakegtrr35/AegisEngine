#include	"Math.h"

static random_device rnd;							// 非決定的な乱数生成器

const int Math::Get_Random(int min, int max)
{
	mt19937_64 mt(rnd());								//  メルセンヌ・ツイスタの64ビット版、引数は初期シード値
	uniform_int_distribution<> rand(min, max);			// [min, max] 範囲の一様乱数

	return rand(mt);
}

const float Math::Get_Random(float min, float max)
{
	mt19937_64 mt(rnd());								//  メルセンヌ・ツイスタの64ビット版、引数は初期シード値
	uniform_real_distribution<> rand(min, max);			// [min, max] 範囲の一様乱数

	return rand(mt);
}

const double Math::Get_Random(double min, double max)
{
	mt19937_64 mt(rnd());								//  メルセンヌ・ツイスタの64ビット版、引数は初期シード値
	uniform_real_distribution<> rand(min, max);			// [min, max] 範囲の一様乱数

	return rand(mt);
}


const bool Math::Random_Bool(const float probability) {
	mt19937_64 mt(rnd());							//  メルセンヌ・ツイスタの64ビット版、引数は初期シード値
	bernoulli_distribution uid(probability);

	return uid(mt);
}


float Math::Lerp(const float y1, const float y2, const float tx, const float x1, const float x2)
{
	float dx, dy;

	dx = x2 - x1;
	dy = y2 - y1;

	return (y1 + (dy / dx) * (tx - x1));
}


short Math::Loop_Plus(short tx, short max)
{
	return (tx % max);
}

short Math::Loop_Minus(short tx, short max)
{
	return ((tx + max) % max);
}