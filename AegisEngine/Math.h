/**
 * @file Math.h
 * @brief �Z�p�֌W�̃w�b�_�[�t�@�C��
 */

#pragma once

#ifndef MATH_H
#define MATH_H

namespace Math
{
	// �����֌W�̊֐�

	// ��l����
	const int Get_Random(const int min, const int max);
	const float Get_Random(const float min, const float max);
	const double Get_Random(const double min, const double max);
	// true��false�Ƃ���2�̒l���m�����z������֐�
	// �m��( probability )��true����������A�m��( 1.0 - probability )��false�����������
	const bool Random_Bool(const float probability);

	// ���`���
	// x1, y1 : �ŏ��l��x,y x2, y2 : �ő�l��x,y tx : ��Ԃ������l
	float Lerp(const float x1, const float y1, const float x2, const float y2, const float tx);

	// �ϐ��̃��[�v����
	short Loop_Plus(short tx, short max);
	short Loop_Minus(short tx, short max);
}

using namespace Math;

#endif // !MATH_H