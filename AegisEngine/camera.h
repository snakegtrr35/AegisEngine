#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include	"Game_Object.h"

class CCamera : public GAME_OBJECT{
private:
	static CCamera* pCamera;

	float HorzDist;			// ��������
	float VerDist;			// ��������
	float TargetDist;		// �^�[�Q�b�g����

	XMVECTOR Pos;

	XMVECTOR Front;
	XMVECTOR Up;
	XMVECTOR Right;

	XMVECTOR At;

	static float Lenght_Z;
	static float Lenght_Y;

	XMFLOAT3 Velocity;

	RECT Viewport;

	float Viewing_Angle;

	static XMMATRIX m_ViewMatrix;
	static XMMATRIX m_ProjectionMatrix;

	bool MoveEnable;
	bool RotateEnable;

public:
	void Init();
	void Uninit();
	void Update(float delta_time);
	void Draw();
	void Draw_DPP();

	static CCamera* const Get_Camera() {
		return pCamera;
	}

	static const XMMATRIX& Get_Camera_View() {
		return m_ViewMatrix;
	}

	static const XMMATRIX& Get_Camera_Projection() {
		return m_ProjectionMatrix;
	}

	XMVECTOR* const  Get_Front() {
		return &Front;
	};

	XMVECTOR* Get_Pos() {
		return &Pos;
	}

	XMVECTOR* const  Get_Up() {
		return &Up;
	};

	XMVECTOR* const  Get_Right() {
		return &Right;
	};

	XMVECTOR* const  Get_At() {
		return &At;
	};

	void  Set_Viewing_Angle(const float viewing_angle) {
		Viewing_Angle = viewing_angle;
	};

	const float Get_Viewing_Angle() {
		return Viewing_Angle;
	};

	bool Get_Visibility(const XMFLOAT3& position);

	void Set_Move_Enable(const bool flag) {
		MoveEnable = flag;
	}

	void Set_Rotate_Enable(const bool flag) {
		RotateEnable = flag;
	}

};

#endif // !CAMERA_H