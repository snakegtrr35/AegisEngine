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
	/**
	* @brief �m�����z
	* @param probability �m��( probability )
	* @return bool true ���� false
	* @details �m��( probability )��true����������A�m��( 1.0 - probability )��false�����������֐�
	*/
	const bool Random_Bool(const float probability);

	/**
	* @brief ���`���
	* @param y1 �ŏ��l�̎��̒l
	* @param y2 �ő�l�̎��̒l
	* @param tx ��Ԃ������l
	* @param x1 ���`�̍ŏ��l(�f�t�H���g��0.0f)
	* @param x2 ���`�̍ő�l(�f�t�H���g��1.0f)
	* @return float ��Ԃ����l
	* @details ���`��Ԃ��s���֐�
	*/
	float Lerp(const float y1, const float y2, const float tx, const float x1 = 0.f, const float x2 = 1.0f);

	// �ϐ��̃��[�v����
	short Loop_Plus(short tx, short max);
	short Loop_Minus(short tx, short max);
}

using namespace Math;

#endif // !MATH_H