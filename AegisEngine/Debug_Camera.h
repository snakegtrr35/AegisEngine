#pragma once

#ifndef DEBUG_CAMERA_H
#define DEBUG_CAMERA_H

#include	"Game_Object.h"

class DEBUG_CAMERA : public GAME_OBJECT {
private:
	static DEBUG_CAMERA* pDebugCamera;

	float HorzDist;			// ��������
	float VerDist;			// ��������
	float TargetDist;		// �^�[�Q�b�g����

	XMVECTOR Pos;

	XMVECTOR Front;
	XMVECTOR Up;
	XMVECTOR Right;

	XMVECTOR At;

	static float Lenght;

	float Rotate;

	XMFLOAT3 Velocity;

	RECT Viewport;

	float Viewing_Angle;

	static XMMATRIX m_ViewMatrix;
	static XMMATRIX m_ProjectionMatrix;

	bool MoveEnable;
	bool RotateEnable;

public:
	void Init() override;
	void Uninit() override;
	void Update(float delta_time) override;
	void Draw() override;

	static DEBUG_CAMERA* const Get_Camera();
	static const XMMATRIX Get_Camera_View();
	static const XMMATRIX Get_Camera_Projection();

	XMVECTOR* const  Get_Front() {
		return &Front;
	};

	XMVECTOR* const  Get_Up() {
		return &Up;
	};

	XMVECTOR* const  Get_Right() {
		return &Right;
	};

	XMVECTOR* const  Get_At() {
		return &At;
	};

	const float Get() {
		return Rotate;
	};

	void  Set_Lenght(const float lenght) {
		Lenght = lenght;

		Pos = At - Front * Lenght;
	};

	const float Get_Lenght() {
		return Lenght;
	};

	void  Set_Viewing_Angle(const float viewing_angle) {
		Viewing_Angle = viewing_angle;
	};

	const float Get_Viewing_Angle() {
		return Viewing_Angle;
	};

	void Set_Move_Enable(const bool flag) {
		MoveEnable = flag;
	}

	void Set_Rotate_Enable(const bool flag) {
		RotateEnable = flag;
	}

};

#endif // !DEBUG_CAMERA_H
