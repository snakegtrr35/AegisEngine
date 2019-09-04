#include	"Math.h"

const int Get_Random(int min, int max)
{
	static std::random_device rnd;							// �񌈒�I�ȗ���������𐶐�
	std::mt19937_64 mt(rnd());								//  �����Z���k�E�c�C�X�^��32�r�b�g�ŁA�����͏����V�[�h�l
	std::uniform_int_distribution<> rand(min, max);			// [min, max] �͈͂̈�l����

	return rand(mt);
}

bool Rand_Bool(const double probability) {
	static std::random_device rnd;				// �񌈒�I�ȗ���������𐶐�
	static std::mt19937_64 mt(rnd());			//  �����Z���k�E�c�C�X�^��32�r�b�g�ŁA�����͏����V�[�h�l

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