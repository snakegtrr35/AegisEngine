/**
* @file Timer.h
* @brief ���Ԋ֌W�̃N���X������w�b�_�[�t�@�C��
*/

#pragma once

#ifndef TIMER_H
#define TIMER_H

/**
* @brief �^�C�}�[�N���X
* @details �Q�[����FPS�Ɉˑ����Ȃ�(�ق�)�^�C�}�[
*/
class TIMER {
private:
	static LARGE_INTEGER start;		//! �J�n���Ԃ������Ă���ϐ�
	static LARGE_INTEGER end;		//! ���ݎ��Ԃ������Ă���ϐ�
	static LARGE_INTEGER frep;		//! �^�C���֌W�̒l�������Ă���ϐ�

protected:

public:

	/**
	* @brief �������̊֐�
	*/
	static void Init() {

		memset(&start, 0, sizeof(start));
		memset(&end, 0, sizeof(end));
		memset(&frep, 0, sizeof(frep));

		QueryPerformanceFrequency(&frep);

		QueryPerformanceCounter(&start);
	};

	/**
	* @brief ���Ԃ̎擾�֐�
	* @return DWORD �Q�[���J�n������̌o�ߎ���
	*/
	static DWORD Get_Time() {
		QueryPerformanceCounter(&end);
		return (end.QuadPart - start.QuadPart);
	};

	/**
	* @brief ���Ԃ̎擾�֐�(�P�� �b)
	* @return DWORD �Q�[���J�n������̌o�ߎ���(�P�� �b)
	*/
	static DWORD Get_Time_Sec() {
		QueryPerformanceCounter(&end);
		return (end.QuadPart - start.QuadPart) / frep.QuadPart;
	};

	/**
	* @brief ���Ԃ̎擾�֐�((�P�� �~���b))
	* @param[in] digit �����_�ȉ��̌���
	* @return DWORD �Q�[���J�n������̌o�ߎ���(�P�� �~���b)
	*/
	static DWORD Get_Time_Mili(const unsigned char digit = 1) {
		QueryPerformanceCounter(&end);

		return (end.QuadPart - start.QuadPart) * std::pow(10, digit) / frep.QuadPart;
	};
};



/**
* @brief �N���b�N�^�C�}�[�N���X
* @details �Q�[����FPS�Ɉˑ�����^�C�}�[
*/
class CLOCK_TIMER {
private:
	static DWORD Time;		//! �^�C��������ϐ�

protected:

public:

	/**
	* @brief �������̊֐�
	*/
	static void Init() {
		Time = 0;
	};

	/**
	* @brief �^�C�}�[�̍X�V�֐�
	*/
	static void Update() {
		Time++;
	}

	/**
	* @brief ���Ԃ̎擾�֐�
	*/
	static const DWORD Get_Time() {
		return Time;
	};
};

#endif // !TIMER_H