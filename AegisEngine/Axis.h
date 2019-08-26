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

		// �O�̃x�N�g���𐂒��ɂ���
		Right = XMVector3Cross(Up, Front);
		Right = XMVector3Normalize(Right);

		Up = XMVector3Cross(Front, Right);
		Up = XMVector3Normalize(Up);

		Axis = XMFLOAT3(0.f, 0.0f, 0.f);

	};
	virtual ~AXIS_COMPONENT() {};

	/**
	* @brief �O�̃x�N�g���𐂒��ɂ���֐�
	* @details �O�̃x�N�g���𐂒��ɂ���
	*/
	void Init() {
		// �O�̃x�N�g���𐂒��ɂ���
		Front = XMVector3Normalize(Front);

		Right = XMVector3Cross(Up, Front);
		Right = XMVector3Normalize(Right);

		Up = XMVector3Cross(Front, Right);
		Up = XMVector3Normalize(Up);
	};

	void Update() override {};
	void Uninit() override {};

	/**
	* @brief �t�����g�x�N�g����ݒ肷��֐�
	* @param[in] vector �ݒ肷��x�N�g��
	* @details �t�����g�x�N�g��������(vector)�ɐݒ肷��֐�
	*/
	void Set_Front(const XMVECTOR& vector) {
		Front = vector;
	};

	/**
	*@brief �t�����g�x�N�g�����擾����֐�
	* @details �t�����g�x�N�g�����擾����֐�
	*/
	const XMVECTOR& Get_Front() {
		return Front;
	};

	/**
	* @brief �A�b�v�x�N�g����ݒ肷��֐�
	* @param[in] vector �ݒ肷��x�N�g��
	* @details �A�b�v�x�N�g��������(vector)�ɐݒ肷��֐�
	*/
	void Set_Up(const XMVECTOR& vector) {
		Up = vector;
	};

	/**
	*@brief �A�b�v�x�N�g�����擾����֐�
	* @details �A�b�v�x�N�g�����擾����֐�
	*/
	const XMVECTOR& Get_Up() {
		return Up;
	};

	/**
	* @brief ���C�g�x�N�g����ݒ肷��֐�
	* @param[in] vector �ݒ肷��x�N�g��
	* @details ���C�g�x�N�g��������(vector)�ɐݒ肷��֐�
	*/
	void Set_Right(const XMVECTOR& vector) {
		Right = vector;
	};

	/**
	* @brief ���C�g�x�N�g�����擾����֐�
	* @details ���C�g�x�N�g�����擾����֐�
	*/
	const XMVECTOR& Get_Right() {
		return Right;
	};

	/**
	* @brief ���x�N�g�����擾����֐�
	* @return XMFLOAT3 Axis
	* @details ���x�N�g�����擾����֐�
	*/
	const XMFLOAT3& Get_Axis() {
		return Axis;
	}

	/**
	* @brief �t�����g�x�N�g������]������֐�
	* @param[in] angle ��]������p�x(�f�B�O���[)
	* @details �t�����g�x�N�g����(angle)�x�ŉ�]������֐�(�㉺)
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
	* @brief ���C�g�x�N�g������]������֐�
	* @param[in] angle ��]������p�x(�f�B�O���[)
	* @details ���C�g�x�N�g����(angle)�x�ŉ�]������֐�(���E)
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
	* @brief �A�b�v�x�N�g������]������֐�
	* @param[in] angle ��]������p�x(�f�B�O���[)
	* @details �A�b�v�x�N�g����(angle)�x�ŉ�]������֐�
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
