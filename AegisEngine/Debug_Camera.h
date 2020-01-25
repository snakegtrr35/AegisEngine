#pragma once

#ifndef DEBUG_CAMERA_H
#define DEBUG_CAMERA_H

#include	"Game_Object.h"

class DEBUG_CAMERA : public GAME_OBJECT {
private:
	static DEBUG_CAMERA* pDebugCamera;

	float HorzDist;			// 水平距離
	float VerDist;			// 垂直距離
	float TargetDist;		// ターゲット距離

	XMVECTOR Front;
	XMVECTOR Up;
	XMVECTOR Right;

	XMVECTOR Pos;
	XMVECTOR At;

	XMFLOAT3 F;
	XMFLOAT3 U;
	XMFLOAT3 R;

	XMFLOAT3 A;

	static float Lenght;

	float Rotate;

	XMFLOAT3 Velocity;

	RECT Viewport;

	float Viewing_Angle;

	static XMMATRIX m_ViewMatrix;
	static XMMATRIX m_ProjectionMatrix;

	bool MoveEnable;
	bool RotateEnable;

	//BoundingFrustum Frustum;

public:
	DEBUG_CAMERA();
	~DEBUG_CAMERA() {}

	void Init() override;
	void Uninit() override;
	void Update(float delta_time) override;
	void Draw() override;
	void Draw_DPP() override;

	static DEBUG_CAMERA* const Get_Camera() {
		return pDebugCamera;
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

	template<typename Archive>
	void serialize(Archive& ar)
	{
		XMStoreFloat3(&Position, Pos);
		XMStoreFloat3(&A, At);

		ar(cereal::base_class<GAME_OBJECT>(this));
		ar(A);

		ar(F);
		ar(U);
		ar(R);

		ar(Viewing_Angle);

		ar(Viewport);

		Pos = XMLoadFloat3(&Position);
		At = XMLoadFloat3(&A);

		Front = XMLoadFloat3(&F);
		Up = XMLoadFloat3(&U);
		Right = XMLoadFloat3(&R);
	}

	//const BoundingFrustum& Get_Frustum() {
	//	return Frustum;
	//}

	/*template<class Archive>
	void save(Archive& ar) const
	{
		ar(cereal::base_class<GAME_OBJECT>(this));
		ar(A);

		ar(F);
		ar(U);
		ar(R);

		ar(Viewing_Angle);

		ar(Viewport);
	}

	template<class Archive>
	void load(Archive& ar)
	{
		ar(cereal::base_class<GAME_OBJECT>(this));
		ar(A);
		ar(A);

		ar(F);
		ar(U);
		ar(R);

		ar(Viewing_Angle);

		ar(Viewport);

		Pos = XMLoadFloat3(&Position);
		At = XMLoadFloat3(&A);

		Front = XMLoadFloat3(&F);
		Up = XMLoadFloat3(&U);
		Right = XMLoadFloat3(&R);
	}*/

};

CEREAL_REGISTER_TYPE(DEBUG_CAMERA)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GAME_OBJECT, DEBUG_CAMERA)

#endif // !DEBUG_CAMERA_H
