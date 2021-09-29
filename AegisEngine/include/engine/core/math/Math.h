/**
 * @file Math.h
 * @brief �Z�p�֌W�̃w�b�_�[�t�@�C��
 */

#pragma once

#ifndef MATH_H
#define MATH_H

#include "MathDef.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Int2.h"
#include "Quaternion.h"

namespace Aegis
{
	namespace Math
	{
		// �����֌W�̊֐�

		// ��l����
		inline const int Get_Random(const int min, const int max);
		inline const float Get_Random(const float min, const float max);
		inline const double Get_Random(const double min, const double max);

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
		float LerpEx(const float y1, const float y2, const float tx, const float x1 = 0.f, const float x2 = 1.0f);

		// �ϐ��̃��[�v����
		short Loop_Plus(short tx, short max);
		short Loop_Minus(short tx, short max);


		// ���ݕϊ��֐�

		inline XMVECTOR Vector3ToXMVECTOR(const Vector3& vector)
		{
			return XMLoadFloat3(&XMFLOAT3(vector.x, vector.y, vector.z));
		}

		inline Vector3 XMFLOAT3ToVector3(const XMFLOAT3& vector)
		{
			return Vector3(vector.x, vector.y, vector.z);
		}

		inline Vector3 XMVECTORToVector3(const XMVECTOR& vector)
		{
			return Vector3(vector.m128_f32[0], vector.m128_f32[1], vector.m128_f32[2]);
		}

		inline XMVECTOR QuaternionToXMVECTOR(const Quaternion& quaternion)
		{
			return XMVectorSet(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
		}

		inline Quaternion XMVECTORToQuaternion(const XMVECTOR& vector)
		{
			XMFLOAT4 vec;
			XMStoreFloat4(&vec, vector);

			return Quaternion(vec.x, vec.y, vec.z, vec.w);
		}

		//�N�H�[�^�j�I������]�s��ɂ���
		inline XMMATRIX QuaternionToMatrix(Quaternion& q) {
			//MATRIX ret;
			XMFLOAT4X4 ret;
			float sx = q.x * q.x;
			float sy = q.y * q.y;
			float sz = q.z * q.z;
			float cx = q.y * q.z;
			float cy = q.x * q.z;
			float cz = q.x * q.y;
			float wx = q.w * q.x;
			float wy = q.w * q.y;
			float wz = q.w * q.z;

			ret._11 = 1.0f - 2.0f * (sy + sz);
			ret._12 = 2.0f * (cz + wz);
			ret._13 = 2.0f * (cy - wy);
			ret._21 = 2.0f * (cz - wz);
			ret._22 = 1.0f - 2.0f * (sx + sz);
			ret._23 = 2.0f * (cx + wx);
			ret._31 = 2.0f * (cy + wy);
			ret._32 = 2.0f * (cx - wx);
			ret._33 = 1.0f - 2.0f * (sx + sy);
			ret._41 = 0.0f;
			ret._42 = 0.0f;
			ret._43 = 0.0f;

			//return ret;
			return XMLoadFloat4x4(&ret);
		}

		//��]�s����N�H�[�^�j�I���ɂ���
		inline Quaternion MatrixToQuaternion(XMMATRIX& mat) {
			Quaternion q;

			XMFLOAT4X4 ret;
			XMStoreFloat4x4(&ret, mat);

			float s;
			float tr = ret._11 + ret._22 + ret._33 + 1.0f;
			if (tr >= 1.0f) {
				s = 0.5f / sqrt(tr);
				q.w = 0.25f / s;
				q.x = (ret._23 - ret._32) * s;
				q.y = (ret._31 - ret._13) * s;
				q.z = (ret._12 - ret._21) * s;
				return q;
			}
			else {
				float max;
				if (ret._22 > ret._33) {
					max = ret._22;
				}
				else {
					max = ret._33;
				}

				if (max < ret._11) {
					s = sqrt(ret._11 - (ret._22 + ret._33) + 1.0f);
					float x = s * 0.5f;
					s = 0.5f / s;
					q.x = x;
					q.y = (ret._12 + ret._21) * s;
					q.z = (ret._31 + ret._13) * s;
					q.w = (ret._23 - ret._32) * s;
					return q;
				}
				else if (max == ret._22) {
					s = sqrt(ret._22 - (ret._33 + ret._11) + 1.0f);
					float y = s * 0.5f;
					s = 0.5f / s;
					q.x = (ret._12 + ret._21) * s;
					q.y = y;
					q.z = (ret._23 + ret._32) * s;
					q.w = (ret._31 - ret._13) * s;
					return q;
				}
				else {
					s = sqrt(ret._33 - (ret._11 + ret._22) + 1.0f);
					float z = s * 0.5f;
					s = 0.5f / s;
					q.x = (ret._31 + ret._13) * s;
					q.y = (ret._23 + ret._32) * s;
					q.z = z;
					q.w = (ret._12 - ret._21) * s;
					return q;
				}
			}
		}

	}

	Quaternion RotateAxis(const Vector3& axis, const float angle);
}

XMFLOAT3 operator+(const XMFLOAT3& left, const XMFLOAT3 right);

XMFLOAT3 operator-(const XMFLOAT3& left, const XMFLOAT3 right);

XMFLOAT3 operator*(const XMFLOAT3& left, const XMFLOAT3 right);

XMFLOAT3 operator*(const XMFLOAT3& left, const float s);

XMFLOAT3 operator/(const XMFLOAT3& left, const XMFLOAT3 right);

XMFLOAT3 operator/(const float s, const XMFLOAT3 right);


XMFLOAT4 operator*(const XMFLOAT4& left, const float s);

inline XMMATRIX XMFLOAT4X4ToXMMATRIX(const XMFLOAT4X4 m)
{
	return XMLoadFloat4x4(&m);
}

inline XMFLOAT4X4 XMMATRIXToXMFLOAT4X4(const XMMATRIX m)
{
	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, m);
	return matrix;
}

#endif // !MATH_H