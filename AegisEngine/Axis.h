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

		// �O�̃x�N�g���𐂒��ɂ���
		Right = XMVector3Cross(Up, Front);
		Right = XMVector3Normalize(Right);

		Up = XMVector3Cross(Front, Right);
		Up = XMVector3Normalize(Up);

		XMStoreFloat4(&F, Front);
		XMStoreFloat4(&U, Up);
		XMStoreFloat4(&R, Right);

	};
	virtual ~AXIS_COMPONENT() {};

	/**
	* @brief �O�̃x�N�g���𐂒��ɂ���֐�
	* @details �O�̃x�N�g���𐂒��ɂ���
	*/
	void Init() {
		/*// �O�̃x�N�g���𐂒��ɂ���
		Front = XMVector3Normalize(Front);

		Right = XMVector3Cross(Up, Front);
		Right = XMVector3Normalize(Right);

		Up = XMVector3Cross(Front, Right);
		Up = XMVector3Normalize(Up);*/

		//Front = XMLoadFloat4(&F);
		//Up = XMLoadFloat4(&U);
		//Right = XMLoadFloat4(&R);
	};


	void Draw() override {}
	void Draw_DPP() override {}
	void Update(float delta_time) override {}
	void Uninit() override {};

	void Draw_Inspector() override {}

	/**
	* @brief �t�����g�x�N�g����ݒ肷��֐�
	* @param[in] vector �ݒ肷��x�N�g��
	* @details �t�����g�x�N�g��������(vector)�ɐݒ肷��֐�
	*/
	void Set_Front(const XMVECTOR& vector) {
		Front = XMQuaternionNormalize(vector);

		// �O�̃x�N�g���𐂒��ɂ���
		Right = XMVector3Cross(Up, Front);
		Right = XMVector3Normalize(Right);

		Up = XMVector3Cross(Front, Right);
		Up = XMVector3Normalize(Up);
	};

	/**
	* @brief �t�����g�x�N�g����ݒ肷��֐�
	* @param[in] vector �ݒ肷��XMFLOAT4
	* @details �t�����g�x�N�g��������(XMFLOAT4)�ɐݒ肷��֐�
	*/
	void Set_Front(const XMFLOAT4& vector) {
		Front = XMLoadFloat4(&vector);
		Front = XMQuaternionNormalize(Front);

		// �O�̃x�N�g���𐂒��ɂ���
		Right = XMVector3Cross(Up, Front);
		Right = XMVector3Normalize(Right);

		Up = XMVector3Cross(Front, Right);
		Up = XMVector3Normalize(Up);
	};

	/**
	*@brief �t�����g�x�N�g�����擾����֐�
	* @details �t�����g�x�N�g�����擾����֐�
	*/
	const XMVECTOR Get_Front() {
		return Front;
	};

	/**
	* @brief �A�b�v�x�N�g����ݒ肷��֐�
	* @param[in] vector �ݒ肷��x�N�g��
	* @details �A�b�v�x�N�g��������(vector)�ɐݒ肷��֐�
	*/
	void Set_Up(const XMVECTOR& vector) {
		Up = XMQuaternionNormalize(vector);

		// �O�̃x�N�g���𐂒��ɂ���
		Front = XMVector3Cross(Right, Up);
		Front = XMVector3Normalize(Front);

		Right = XMVector3Cross(Front, Up);
		Right = XMVector3Normalize(Right);
	};

	/**
	* @brief �A�b�v�x�N�g����ݒ肷��֐�
	* @param[in] vector �ݒ肷��XMFLOAT4
	* @details �A�b�v�x�N�g��������(XMFLOAT4)�ɐݒ肷��֐�
	*/
	void Set_Up(const XMFLOAT4& vector) {
		Up = XMLoadFloat4(&vector);
		Up = XMQuaternionNormalize(Up);

		// �O�̃x�N�g���𐂒��ɂ���
		Front = XMVector3Cross(Right, Up);
		Front = XMVector3Normalize(Front);

		Right = XMVector3Cross(Front, Up);
		Right = XMVector3Normalize(Right);
	};

	/**
	*@brief �A�b�v�x�N�g�����擾����֐�
	* @details �A�b�v�x�N�g�����擾����֐�
	*/
	const XMVECTOR Get_Up() {
		return Up;
	};

	/**
	* @brief ���C�g�x�N�g����ݒ肷��֐�
	* @param[in] vector �ݒ肷��x�N�g��
	* @details ���C�g�x�N�g��������(vector)�ɐݒ肷��֐�
	*/
	void Set_Right(const XMVECTOR& vector) {
		Right = XMQuaternionNormalize(vector);

		// �O�̃x�N�g���𐂒��ɂ���
		Up = XMVector3Cross(Front, Right);
		Up = XMVector3Normalize(Up);

		Front = XMVector3Cross(Up, Right);
		Front = XMVector3Normalize(Front);
	};

	/**
	* @brief ���C�g�x�N�g����ݒ肷��֐�
	* @param[in] vector �ݒ肷��XMFLOAT4
	* @details ���C�g�x�N�g��������(XMFLOAT4)�ɐݒ肷��֐�
	*/
	void Set_Right(const XMFLOAT4& vector) {
		Right = XMLoadFloat4(&vector);
		Right = XMQuaternionNormalize(Right);

		// �O�̃x�N�g���𐂒��ɂ���
		Up = XMVector3Cross(Front, Right);
		Up = XMVector3Normalize(Up);

		Front = XMVector3Cross(Up, Right);
		Front = XMVector3Normalize(Front);
	};

	/**
	* @brief ���C�g�x�N�g�����擾����֐�
	* @details ���C�g�x�N�g�����擾����֐�
	*/
	const XMVECTOR Get_Right() {
		return Right;
	};

	/**
	* @brief �t�����g�x�N�g������]������֐�
	* @param[in] angle ��]������p�x(�f�B�O���[)
	* @details �t�����g�x�N�g����(angle)�x�ŉ�]������֐�(�㉺)
	*/
	void Rotation_Front(const float angle) {
		XMMATRIX mtxRotation = XMMatrixRotationAxis(Front, XMConvertToRadians(angle));

		Right = XMVector3TransformNormal(Right, mtxRotation);
		Right = XMVector3Normalize(Right);

		Up = XMVector3TransformNormal(Up, mtxRotation);
		Up = XMVector3Normalize(Up);

		Front = XMVector3TransformNormal(Front, mtxRotation);
		Front = XMVector3Normalize(Front);

		Set(Front, Up, Right);
	}

	/**
	* @brief ���C�g�x�N�g������]������֐�
	* @param[in] angle ��]������p�x(�f�B�O���[)
	* @details ���C�g�x�N�g����(angle)�x�ŉ�]������֐�(���E)
	*/
	void Rotation_Up(const float angle) {
		XMMATRIX mtxRotation = XMMatrixRotationAxis(Up, angle);

		Right = XMVector3TransformNormal(Right, mtxRotation);
		Right = XMVector3Normalize(Right);

		Front = XMVector3TransformNormal(Front, mtxRotation);
		Front = XMVector3Normalize(Front);

		Up = XMVector3TransformNormal(Up, mtxRotation);
		Up = XMVector3Normalize(Up);

		Set(Front, Up, Right);
	}

	/**
	* @brief �A�b�v�x�N�g������]������֐�
	* @param[in] angle ��]������p�x(�f�B�O���[)
	* @details �A�b�v�x�N�g����(angle)�x�ŉ�]������֐�
	*/
	void Rotation_Right(const float angle) {
		XMMATRIX mtxRotation = XMMatrixRotationAxis(Right, XMConvertToRadians(angle));

		Front = XMVector3TransformNormal(Front, mtxRotation);
		Front = XMVector3Normalize(Front);

		Up = XMVector3TransformNormal(Up, mtxRotation);
		Up = XMVector3Normalize(Up);

		Right = XMVector3TransformNormal(Right, mtxRotation);
		Right = XMVector3Normalize(Right);

		Set(Front, Up, Right);
	}

	//void Rotation_X(const float angle) {
	//	XMMATRIX mtxRotation = XMMatrixRotationX(XMConvertToRadians(angle));

	//	Front = XMVector3TransformNormal(Front, mtxRotation);
	//	Front = XMVector3Normalize(Front);

	//	Up = XMVector3TransformNormal(Up, mtxRotation);
	//	Up = XMVector3Normalize(Up);

	//	Right = XMVector3TransformNormal(Right, mtxRotation);
	//	Right = XMVector3Normalize(Right);

	//	Set(Front, Up, Right);
	//}

	//void Rotation_Y(const float angle) {
	//	XMMATRIX mtxRotation = XMMatrixRotationY(XMConvertToRadians(angle));

	//	Front = XMVector3TransformNormal(Front, mtxRotation);
	//	Front = XMVector3Normalize(Front);

	//	Up = XMVector3TransformNormal(Up, mtxRotation);
	//	Up = XMVector3Normalize(Up);

	//	Right = XMVector3TransformNormal(Right, mtxRotation);
	//	Right = XMVector3Normalize(Right);

	//	Set(Front, Up, Right);
	//}

	//void Rotation_Z(const float angle) {
	//	XMMATRIX mtxRotation = XMMatrixRotationZ(XMConvertToRadians(angle));

	//	Front = XMVector3TransformNormal(Front, mtxRotation);
	//	Front = XMVector3Normalize(Front);

	//	Up = XMVector3TransformNormal(Up, mtxRotation);
	//	Up = XMVector3Normalize(Up);

	//	Right = XMVector3TransformNormal(Right, mtxRotation);
	//	Right = XMVector3Normalize(Right);

	//	Set(Front, Up, Right);
	//}

	/*void Rotation(const XMVECTOR& axis, const float angle) {
		XMMATRIX mtxRotation = XMMatrixRotationAxis(axis, XMConvertToRadians(angle));

		Front = XMVector3TransformNormal(Front, mtxRotation);
		Front = XMVector3Normalize(Front);

		Up = XMVector3TransformNormal(Up, mtxRotation);
		Up = XMVector3Normalize(Up);

		Right = XMVector3TransformNormal(Right, mtxRotation);
		Right = XMVector3Normalize(Right);

		Set(Front, Up, Right);
	}*/

	template<class Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<COMPONENT>(this));

		XMStoreFloat4(&F, Front);
		XMStoreFloat4(&U, Up);
		XMStoreFloat4(&R, Right);

		ar(F);
		ar(U);
		ar(R);

		Front = XMLoadFloat4(&F);
		Up = XMLoadFloat4(&U);
		Right = XMLoadFloat4(&R);
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