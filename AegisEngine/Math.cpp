#include	"Math.h"

const int Get_Random(int min, int max)
{
	static std::random_device rnd;							// 非決定的な乱数生成器を生成
	std::mt19937_64 mt(rnd());								//  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
	std::uniform_int_distribution<> rand(min, max);			// [min, max] 範囲の一様乱数

	return rand(mt);
}

bool Rand_Bool(const double probability) {
	static std::random_device rnd;				// 非決定的な乱数生成器を生成
	static std::mt19937_64 mt(rnd());			//  メルセンヌ・ツイスタの32ビット版、引数は初期シード値

	std::bernoulli_distribution uid(probability);
	return uid(mt);
}


float Lerp(const float x1, const float y1, const float x2, const float y2, const float tx)
{
	float dx, dy;

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