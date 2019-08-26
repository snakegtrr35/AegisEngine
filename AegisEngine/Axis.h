#pragma once

#ifndef AXIS_H
#define AXIS_H

class COMPONENT;

class AXIS_COMPONENT : public COMPONENT {
private:

protected:

	XMVECTOR Front;
	XMVECTOR Up;
	XMVECTOR Right;

	XMFLOAT3 Axis;

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

		Axis = XMFLOAT3(0.f, 0.0f, 0.f);

	};
	virtual ~AXIS_COMPONENT() {};

	/**
	* @brief 三つのベクトルを垂直にする関数
	* @details 三つのベクトルを垂直にする
	*/
	void Init() {
		// 三つのベクトルを垂直にする
		Front = XMVector3Normalize(Front);

		Right = XMVector3Cross(Up, Front);
		Right = XMVector3Normalize(Right);

		Up = XMVector3Cross(Front, Right);
		Up = XMVector3Normalize(Up);
	};

	void Update() override {};
	void Uninit() override {};

	/**
	* @brief フロントベクトルを設定する関数
	* @param[in] vector 設定するベクトル
	* @details フロントベクトルを引数(vector)に設定する関数
	*/
	void Set_Front(const XMVECTOR& vector) {
		Front = vector;
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
		Up = vector;
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
		Right = vector;
	};

	/**
	* @brief ライトベクトルを取得する関数
	* @details ライトベクトルを取得する関数
	*/
	const XMVECTOR& Get_Right() {
		return Right;
	};

	/**
	* @brief 軸ベクトルを取得する関数
	* @return XMFLOAT3 Axis
	* @details 軸ベクトルを取得する関数
	*/
	const XMFLOAT3& Get_Axis() {
		return Axis;
	}

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

		Axis.x += angle;
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

		Axis.y += angle;
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

		Axis.z += angle;
	}

	void Rotation_X(const float angle) {
		XMMATRIX mtxRotation = XMMatrixRotationX(XMConvertToRadians(angle));

		Front = XMVector3TransformNormal(Front, mtxRotation);
		Front = XMVector3Normalize(Front);

		Up = XMVector3TransformNormal(Up, mtxRotation);
		Up = XMVector3Normalize(Up);

		Right = XMVector3TransformNormal(Right, mtxRotation);
		Right = XMVector3Normalize(Right);

		Axis.x += angle;
	}

	void Rotation_Y(const float angle) {
		XMMATRIX mtxRotation = XMMatrixRotationY(XMConvertToRadians(angle));

		Front = XMVector3TransformNormal(Front, mtxRotation);
		Front = XMVector3Normalize(Front);

		Up = XMVector3TransformNormal(Up, mtxRotation);
		Up = XMVector3Normalize(Up);

		Right = XMVector3TransformNormal(Right, mtxRotation);
		Right = XMVector3Normalize(Right);

		Axis.y += angle;
	}

	void Rotation_Z(const float angle) {
		XMMATRIX mtxRotation = XMMatrixRotationZ(XMConvertToRadians(angle));

		Front = XMVector3TransformNormal(Front, mtxRotation);
		Front = XMVector3Normalize(Front);

		Up = XMVector3TransformNormal(Up, mtxRotation);
		Up = XMVector3Normalize(Up);

		Right = XMVector3TransformNormal(Right, mtxRotation);
		Right = XMVector3Normalize(Right);

		Axis.z += angle;
	}
};

#endif // !AXIS_H
