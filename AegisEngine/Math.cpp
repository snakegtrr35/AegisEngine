#include	"Math.h"

static random_device rnd;							// �񌈒�I�ȗ���������

inline const int Math::Get_Random(int min, int max)
{
	mt19937_64 mt(rnd());								//  �����Z���k�E�c�C�X�^��64�r�b�g�ŁA�����͏����V�[�h�l
	uniform_int_distribution<> rand(min, max);			// [min, max] �͈͂̈�l����

	return rand(mt);
}

inline const float Math::Get_Random(float min, float max)
{
	mt19937_64 mt(rnd());								//  �����Z���k�E�c�C�X�^��64�r�b�g�ŁA�����͏����V�[�h�l
	uniform_real_distribution<> rand(min, max);			// [min, max] �͈͂̈�l����

	return rand(mt);
}

inline const double Math::Get_Random(double min, double max)
{
	mt19937_64 mt(rnd());								//  �����Z���k�E�c�C�X�^��64�r�b�g�ŁA�����͏����V�[�h�l
	uniform_real_distribution<> rand(min, max);			// [min, max] �͈͂̈�l����

	return rand(mt);
}


const bool Math::Random_Bool(const float probability)
{
	mt19937_64 mt(rnd());							//  �����Z���k�E�c�C�X�^��64�r�b�g�ŁA�����͏����V�[�h�l
	bernoulli_distribution uid(probability);

	return uid(mt);
}


float Math::LerpEx(const float y1, const float y2, const float tx, const float x1, const float x2)
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

// �N�H�[�^�j�I���ɂ��Vector3�̕ϊ�
VECTOR3 Math::VECTOR3::Transform(const VECTOR3& v, const class Quaternion& q)
{
	// v + 2.0 * Cross(q.xyz, Cross(q.xyz,v) + q.w * v);
	VECTOR3 qv(q.x, q.y, q.z);
	VECTOR3 retVal = v;
	retVal += 2.0f * VECTOR3::Cross(qv, VECTOR3::Cross(qv, v) + q.w * v);

	return retVal;
}