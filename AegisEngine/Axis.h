#pragma once

#ifndef AXIS_H
#define AXIS_H

class COMPONENT;

class AXIS_COMPONENT : public COMPONENT {
private:

	XMVECTOR Front;
	XMVECTOR Up;
	XMVECTOR Right;

	XMFLOAT4 F;
	XMFLOAT4 U;
	XMFLOAT4 R;

	void Set(const XMVECTOR& f, const XMVECTOR& u, const XMVECTOR& r) {
		XMStoreFloat4(&F, f);
		XMStoreFloat4(&U, u);
		XMStoreFloat4(&R, r);
	}

public:

	AXIS_COMPONENT() {
		Front = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

		Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		Right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

		// 三つのベクトルを垂直にする
		Right = XMVector3Cross(Up, Front);
		Right = XMVector3Normalize(Right);

		Up = XMVector3Cross(Front, Right);
		Up = XMVector3Normalize(Up);

		XMStoreFloat4(&F, Front);
		XMStoreFloat4(&U, Up);
		XMStoreFloat4(&R, Right);

	};
	virtual ~AXIS_COMPONENT() { Set(Front, Up, Right); };

	/**
	* @brief 三つのベクトルを垂直にする関数
	* @details 三つのベクトルを垂直にする
	*/
	void Init() {
		/*// 三つのベクトルを垂直にする
		Front = XMVector3Normalize(Front);

		Right = XMVector3Cross(Up, Front);
		Right = XMVector3Normalize(Right);

		Up = XMVector3Cross(Front, Right);
		Up = XMVector3Normalize(Up);*/

		Front = XMLoadFloat4(&F);
		Up = XMLoadFloat4(&U);
		Right = XMLoadFloat4(&R);
	};

	void Update(float delta_time) override {};
	void Uninit() override {};

	/**
	* @brief フロントベクトルを設定する関数
	* @param[in] vector 設定するベクトル
	* @details フロントベクトルを引数(vector)に設定する関数
	*/
	void Set_Front(const XMVECTOR& vector) {
		Front = XMQuaternionNormalize(vector);

		//
		Right = XMVector3Cross(Up, Front);
		Right = XMVector3Normalize(Right);

		Up = XMVector3Cross(Front, Right);
		Up = XMVector3Normalize(Up);

		Set(Front, Up, Right);
	};

	/**
	*@brief フロントベクトルを取得する関数
	* @details フロントベクトルを取得する関数
	*/
	const XMVECTOR& Get_Front() {
		return Front;
	};

	/**
	* @brief アップベクトルを設定する関数
	* @param[in] vector 設定するベクトル
	* @details アップベクトルを引数(vector)に設定する関数
	*/
	void Set_Up(const XMVECTOR& vector) {
		Up = XMQuaternionNormalize(vector);
	};

	/**
	*@brief アップベクトルを取得する関数
	* @details アップベクトルを取得する関数
	*/
	const XMVECTOR& Get_Up() {
		return Up;
	};

	/**
	* @brief ライトベクトルを設定する関数
	* @param[in] vector 設定するベクトル
	* @details ライトベクトルを引数(vector)に設定する関数
	*/
	void Set_Right(const XMVECTOR& vector) {
		Right = XMQuaternionNormalize(vector);
	};

	/**
	* @brief ライトベクトルを取得する関数
	* @details ライトベクトルを取得する関数
	*/
	const XMVECTOR& Get_Right() {
		return Right;
	};

	/**
	* @brief フロントベクトルを回転させる関数
	* @param[in] angle 回転させる角度(ディグリー)
	* @details フロントベクトルを(angle)度で回転させる関数(上下)
	*/
	void Rotation_Front(const float angle) {
		XMMATRIX mtxRotation = XMMatrixRotationAxis(Right, XMConvertToRadians(angle));

		Front = XMVector3TransformNormal(Front, mtxRotation);
		Front = XMVector3Normalize(Front);

		Up = XMVector3TransformNormal(Up, mtxRotation);
		Up = XMVector3Normalize(Up);

		//Right = XMVector3TransformNormal(Right, mtxRotation);
		//Right = XMVector3Normalize(Right);

		Right = XMVector3Cross(Up, Front);//
		Right = XMVector3Normalize(Right);//

		Set(Front, Up, Right);
	}

	/**
	* @brief ライトベクトルを回転させる関数
	* @param[in] angle 回転させる角度(ディグリー)
	* @details ライトベクトルを(angle)度で回転させる関数(左右)
	*/
	void Rotation_Right(const float angle) {
		XMMATRIX mtxRotation = XMMatrixRotationAxis(Up, angle);

		Front = XMVector3TransformNormal(Front, mtxRotation);
		Front = XMVector3Normalize(Front);

		//Up = XMVector3TransformNormal(Up, mtxRotation);
		//Up = XMVector3Normalize(Up);

		Right = XMVector3TransformNormal(Right, mtxRotation);
		Right = XMVector3Normalize(Right);

		Up = XMVector3Cross(Front, Right);//
		Up = XMVector3Normalize(Up);//

		Set(Front, Up, Right);
	}

	/**
	* @brief アップベクトルを回転させる関数
	* @param[in] angle 回転させる角度(ディグリー)
	* @details アップベクトルを(angle)度で回転させる関数
	*/
	void Rotation_Up(const float angle) {
		XMMATRIX mtxRotation = XMMatrixRotationAxis(Front, XMConvertToRadians(angle));

		//Front = XMVector3TransformNormal(Front, mtxRotation);
		//Front = XMVector3Normalize(Front);

		Up = XMVector3TransformNormal(Up, mtxRotation);
		Up = XMVector3Normalize(Up);

		Right = XMVector3TransformNormal(Right, mtxRotation);
		Right = XMVector3Normalize(Right);

		Front = XMVector3Cross(Up, Right);//
		Front = XMVector3Normalize(Up);//

		Set(Front, Up, Right);
	}

	void Rotation_X(const float angle) {
		XMMATRIX mtxRotation = XMMatrixRotationX(XMConvertToRadians(angle));

		Front = XMVector3TransformNormal(Front, mtxRotation);
		Front = XMVector3Normalize(Front);

		Up = XMVector3TransformNormal(Up, mtxRotation);
		Up = XMVector3Normalize(Up);

		Right = XMVector3TransformNormal(Right, mtxRotation);
		Right = XMVector3Normalize(Right);

		Set(Front, Up, Right);
	}

	void Rotation_Y(const float angle) {
		XMMATRIX mtxRotation = XMMatrixRotationY(XMConvertToRadians(angle));

		Front = XMVector3TransformNormal(Front, mtxRotation);
		Front = XMVector3Normalize(Front);

		Up = XMVector3TransformNormal(Up, mtxRotation);
		Up = XMVector3Normalize(Up);

		Right = XMVector3TransformNormal(Right, mtxRotation);
		Right = XMVector3Normalize(Right);

		Set(Front, Up, Right);
	}

	void Rotation_Z(const float angle) {
		XMMATRIX mtxRotation = XMMatrixRotationZ(XMConvertToRadians(angle));

		Front = XMVector3TransformNormal(Front, mtxRotation);
		Front = XMVector3Normalize(Front);

		Up = XMVector3TransformNormal(Up, mtxRotation);
		Up = XMVector3Normalize(Up);

		Right = XMVector3TransformNormal(Right, mtxRotation);
		Right = XMVector3Normalize(Right);

		Set(Front, Up, Right);
	}

	void Rotation(const XMVECTOR& axis, const float angle) {
		XMMATRIX mtxRotation = XMMatrixRotationAxis(axis, XMConvertToRadians(angle));

		Front = XMVector3TransformNormal(Front, mtxRotation);
		Front = XMVector3Normalize(Front);

		Up = XMVector3TransformNormal(Up, mtxRotation);
		Up = XMVector3Normalize(Up);

		Right = XMVector3TransformNormal(Right, mtxRotation);
		Right = XMVector3Normalize(Right);

		Set(Front, Up, Right);
	}

	template<class Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<COMPONENT>(this));

		ar(F);
		ar(U);
		ar(R);
	}

	/*template<class Archive>
	void save(Archive& ar) const
	{
		ar(cereal::base_class<COMPONENT>(this));

		ar(F);
		ar(U);
		ar(R);
	}

	template<class Archive>
	void load(Archive& ar)
	{
		ar(cereal::base_class<COMPONENT>(this));

		ar(F);
		ar(U);
		ar(R);
	}*/
};

CEREAL_REGISTER_TYPE(AXIS_COMPONENT)
CEREAL_REGISTER_POLYMORPHIC_RELATION(COMPONENT, AXIS_COMPONENT)

#endif // !AXIS_H