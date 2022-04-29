#pragma once

#ifndef DEBUG_CAMERA_H
#define DEBUG_CAMERA_H

#include	"GameObject.h"

class DEBUG_CAMERA : public GameObject {

	OBJECT_TYPE_INFO(DEBUG_CAMERA)

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

	aegis::Vector3 F;
	aegis::Vector3 U;
	aegis::Vector3 R;

	aegis::Vector3 A;

	static float Lenght;

	float Rotate;

	aegis::Vector3 Velocity;

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
	
	const XMMATRIX& Get_Camera_View() {
		return m_ViewMatrix;
	}

	const XMMATRIX& Get_Camera_Projection() {
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

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(cereal::make_nvp("GameObject", cereal::base_class<GameObject>(this)));
		archive(cereal::make_nvp("At", A));

		archive(cereal::make_nvp("Front", F));
		archive(cereal::make_nvp("Up", U));
		archive(cereal::make_nvp("Right", R));

		archive(cereal::make_nvp("FOV", Viewing_Angle));

		archive(cereal::make_nvp("Viewport", Viewport));
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(cereal::make_nvp("GameObject", cereal::base_class<GameObject>(this)));
		archive(cereal::make_nvp("At", A));

		archive(cereal::make_nvp("Front", F));
		archive(cereal::make_nvp("Up", U));
		archive(cereal::make_nvp("Right", R));

		archive(cereal::make_nvp("FOV", Viewing_Angle));

		archive(cereal::make_nvp("Viewport", Viewport));

		aegis::Vector3 pos = Get_Transform().Get_Position();
		Pos = XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(&pos));
		At = XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(&A));
		
		Front = XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(&F));
		Up = XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(&U));
		Right = XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(&R));
	}

};

CEREAL_REGISTER_TYPE(DEBUG_CAMERA)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GameObject, DEBUG_CAMERA)

#endif // !DEBUG_CAMERA_H
