/**
 * @file Math.h
 * @brief �Z�p�֌W�̃w�b�_�[�t�@�C��
 */

#pragma once

#ifndef MATH_H
#define MATH_H

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

	float Lerp(float a, float b, float f) { return a + f * (b - a); }

	// �ϐ��̃��[�v����
	short Loop_Plus(short tx, short max);
	short Loop_Minus(short tx, short max);


	struct INT2
	{
		int x;
		int y;

		INT2() :x(0), y(0) {};
		INT2(int _x, int _y) :x(_x), y(_y) {};

		// Vector Addition (a + b)
		inline friend INT2 operator+(const INT2& a, const INT2& b)
		{
			return INT2(a.x + b.x, a.y + b.y);
		}
		// Scalar Addition
		inline friend INT2 operator+(const INT2& Vector, int Scalar)
		{
			return INT2(Vector.x + Scalar, Vector.y + Scalar);
		}
		// Scalar Addition
		inline friend INT2 operator+(float Scalar, const INT2& Vector)
		{
			return INT2(Vector.x + Scalar, Vector.y + Scalar);
		}

		// Vector Subtraction (a - b)
		inline friend INT2 operator-(const INT2& a, const INT2& b)
		{
			return INT2(a.x - b.x, a.y - b.y);
		}
		// Scalar Subtraction
		inline friend INT2 operator-(const INT2& Vector, int Scalar)
		{
			return INT2(Vector.x - Scalar, Vector.y - Scalar);
		}
		// Scalar Subtraction
		inline friend INT2 operator-(float Scalar, const INT2& Vector)
		{
			return INT2(Vector.x - Scalar, Vector.y - Scalar);
		}

		// Component-wise Multiplication
		// (a.x * b.x, ...)
		inline friend INT2 operator*(const INT2& a, const INT2& b)
		{ 
			return INT2(a.x * b.x, a.y * b.y); 
		}
		// Scalar Multiplication
		inline friend INT2 operator*(const INT2& Vector, int Scalar)
		{
			return INT2(Vector.x * Scalar, Vector.y * Scalar);
		}
		// Scalar Multiplication
		inline friend INT2 operator*(float Scalar, const INT2& Vector)
		{
			return INT2(Vector.x * Scalar, Vector.y * Scalar);
		}

		// *=
		inline INT2& operator*=(int Scalar)
		{
			x *= Scalar;
			y *= Scalar;
			return *this;
		}

		// +=
		inline INT2& operator+=(const INT2& vector)
		{
			x += vector.x;
			y += vector.y;
			return *this;
		}

		// -=
		inline INT2& operator-=(const INT2& vector)
		{
			x -= vector.x;
			y -= vector.y;
			return *this;
		}

		// �x�N�g���̒���
		inline float Length()
		{
			return sqrtf(x * x + y * y);
		}

		// ���̃x�N�g���𐳋K������
		inline void Normalize()
		{
			float length = Length();
			x /= length;
			y /= length;
		}

		// �x�N�g���𐳋K������
		inline INT2 Normalize(const INT2& vector)
		{
			INT2 temp = vector;
			temp.Normalize();
			return temp;
		}

		// 2�̃x�N�g���Ԃ̃h�b�g�� (a Dot b)
		inline float Dot(const INT2& a, const INT2& b)
		{
			return (a.x * b.x + a.y * b.y);
		}

		// f�ɂ��A����B�ւ̐��`���
		inline INT2 Lerp(const INT2& a, const INT2& b, float f)
		{
			return INT2(a + f * (b - a));
		}

		// �i���K�����ꂽ�jN�ɂ���V�𔽉f
		inline INT2 Reflect(const INT2& v, const INT2& n)
		{
			return v - 2.0f * INT2::Dot(v, n) * n;
		}
	};

	struct VECTOR2
	{
		float x;
		float y;

		VECTOR2() :x(0.0f), y(0.0f) {};
		VECTOR2(float _x, float _y) :x(_x), y(_y) {};

		// Vector Addition (a + b)
		inline friend VECTOR2 operator+(const VECTOR2& a, const VECTOR2& b)
		{
			return VECTOR2(a.x + b.x, a.y + b.y);
		}
		// Scalar Addition
		inline friend VECTOR2 operator+(const VECTOR2& Vector, float Scalar)
		{
			return VECTOR2(Vector.x + Scalar, Vector.y + Scalar);
		}
		// Scalar Addition
		inline friend VECTOR2 operator+(float Scalar, const VECTOR2& Vector)
		{
			return VECTOR2(Vector.x + Scalar, Vector.y + Scalar);
		}

		// Vector Subtraction (a - b)
		inline friend VECTOR2 operator-(const VECTOR2& a, const VECTOR2& b)
		{
			return VECTOR2(a.x - b.x, a.y - b.y);
		}
		// Scalar Subtraction
		inline friend VECTOR2 operator-(const VECTOR2& Vector, float Scalar)
		{
			return VECTOR2(Vector.x - Scalar, Vector.y - Scalar);
		}
		// Scalar Subtraction
		inline friend VECTOR2 operator-(float Scalar, const VECTOR2& Vector)
		{
			return VECTOR2(Vector.x - Scalar, Vector.y - Scalar);
		}

		// Component-wise Multiplication
		// (a.x * b.x, ...)
		inline friend VECTOR2 operator*(const VECTOR2& a, const VECTOR2& b)
		{
			return VECTOR2(a.x * b.x, a.y * b.y);
		}
		// Scalar Multiplication
		inline friend VECTOR2 operator*(const VECTOR2& Vector, float Scalar)
		{
			return VECTOR2(Vector.x * Scalar, Vector.y * Scalar);
		}
		// Scalar Multiplication
		inline friend VECTOR2 operator*(float Scalar, const VECTOR2& Vector)
		{
			return VECTOR2(Vector.x * Scalar, Vector.y * Scalar);
		}

		// *=
		inline VECTOR2& operator*=(float Scalar)
		{
			x *= Scalar;
			y *= Scalar;
			return *this;
		}

		// +=
		inline VECTOR2& operator+=(const VECTOR2& vector)
		{
			x += vector.x;
			y += vector.y;
			return *this;
		}

		// -=
		inline VECTOR2& operator-=(const VECTOR2& vector)
		{
			x -= vector.x;
			y -= vector.y;
			return *this;
		}

		// �x�N�g���̒���
		inline float Length()
		{
			return sqrtf(x * x + y * y);
		}

		// ���̃x�N�g���𐳋K������
		inline void Normalize()
		{
			float length = Length();
			x /= length;
			y /= length;
		}

		// �x�N�g���𐳋K������
		inline VECTOR2 Normalize(const VECTOR2& vector)
		{
			VECTOR2 temp = vector;
			temp.Normalize();
			return temp;
		}

		// 2�̃x�N�g���Ԃ̃h�b�g�� (a Dot b)
		inline float Dot(const VECTOR2& a, const VECTOR2& b)
		{
			return (a.x * b.x + a.y * b.y);
		}

		// f�ɂ��A����B�ւ̐��`���
		inline VECTOR2 Lerp(const VECTOR2& a, const VECTOR2& b, float f)
		{
			return VECTOR2(a + f * (b - a));
		}

		// �i���K�����ꂽ�jN�ɂ���V�𔽉f
		inline VECTOR2 Reflect(const VECTOR2& v, const VECTOR2& n)
		{
			return v - 2.0f * VECTOR2::Dot(v, n) * n;
		}
	};

	struct VECTOR3
	{
		float x;
		float y;
		float z;

		VECTOR3() :x(0.0f), y(0.0f), z(0.0f) {};
		VECTOR3(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {};

		// Vector Addition (a + b)
		inline friend VECTOR3 operator+(const VECTOR3& a, const VECTOR3& b)
		{
			return VECTOR3(a.x + b.x, a.y + b.y, a.z + b.z);
		}
		// Scalar Addition
		inline friend VECTOR3 operator+(const VECTOR3& Vector, float Scalar)
		{
			return VECTOR3(Vector.x + Scalar, Vector.y + Scalar, Vector.z + Scalar);
		}
		// Scalar Addition
		inline friend VECTOR3 operator+(float Scalar, const VECTOR3& Vector)
		{
			return VECTOR3(Vector.x + Scalar, Vector.y + Scalar, Vector.z + Scalar);
		}

		// Vector Subtraction (a - b)
		inline friend VECTOR3 operator-(const VECTOR3& a, const VECTOR3& b)
		{
			return VECTOR3(a.x - b.x, a.y - b.y, a.z - b.z);
		}
		// Scalar Subtraction
		inline friend VECTOR3 operator-(const VECTOR3& Vector, float Scalar)
		{
			return VECTOR3(Vector.x - Scalar, Vector.y - Scalar, Vector.z - Scalar);
		}
		// Scalar Subtraction
		inline friend VECTOR3 operator-(float Scalar, const VECTOR3& Vector)
		{
			return VECTOR3(Vector.x - Scalar, Vector.y - Scalar, Vector.z - Scalar);
		}

		// Component-wise Multiplication
		inline friend VECTOR3 operator*(const VECTOR3& left, const VECTOR3& right)
		{
			return VECTOR3(left.x * right.x, left.y * right.y, left.z * right.z);
		}

		// Scalar Multiplication
		inline friend VECTOR3 operator*(const VECTOR3& Vector, float Scalar)
		{
			return VECTOR3(Vector.x * Scalar, Vector.y * Scalar, Vector.z * Scalar); }

		// Scalar multiplication
		inline friend VECTOR3 operator*(float Scalar, const VECTOR3& Vector)
		{
			return VECTOR3(Vector.x * Scalar, Vector.y * Scalar, Vector.z * Scalar); }

		// Scalar *=
		inline VECTOR3& operator*=(float Scalar)
		{
			x *= Scalar;
			y *= Scalar;
			z *= Scalar;
			return *this;
		}

		// Vector +=
		inline VECTOR3& operator+=(const VECTOR3& vector)
		{
			x += vector.x;
			y += vector.y;
			z += vector.z;
			return *this;
		}

		// Vector -=
		inline VECTOR3& operator-=(const VECTOR3& vector)
		{
			x -= vector.x;
			y -= vector.y;
			z -= vector.z;
			return *this;
		}

		// �x�N�g���̒���
		inline float Length()
		{
			return (sqrt(x * x + y * y + z * z));
		}

		// ���̃x�N�g���𐳋K������
		inline void Normalize()
		{
			float length = Length();
			x /= length;
			y /= length;
			z /= length;
		}

		// �񋟂��ꂽ�x�N�g���𐳋K������
		inline VECTOR3 Normalize(const VECTOR3& vector)
		{
			VECTOR3 temp = vector;
			temp.Normalize();
			return temp;
		}

		// 2�̃x�N�g���Ԃ̃h�b�g�ρia Dot b�j
		inline static float Dot(const VECTOR3& a, const VECTOR3& b)
		{
			return (a.x * b.x + a.y * b.y + a.z * b.z);
		}

		// 2�̃x�N�g���Ԃ̊O�� (a Cross b)
		inline static VECTOR3 Cross(const VECTOR3& a, const VECTOR3& b)
		{
			VECTOR3 temp;
			temp.x = a.y * b.z - a.z * b.y;
			temp.y = a.z * b.x - a.x * b.z;
			temp.z = a.x * b.y - a.y * b.x;
			return temp;
		}

		// f�ɂ��A����B�ւ̐��`���
		inline VECTOR3 Lerp(const VECTOR3& a, const VECTOR3& b, float f)
		{
			return VECTOR3(a + f * (b - a));
		}

		// �i���K�����ꂽ�jN�ɂ���V�𔽉f
		inline VECTOR3 Reflect(const VECTOR3& v, const VECTOR3& n)
		{
			return v - 2.0f * VECTOR3::Dot(v, n) * n;
		}


		// �N�H�[�^�j�I���ɂ��Vector3�̕ϊ�
		static VECTOR3 Transform(const VECTOR3& v, const class Quaternion& q);
	};

	struct Quaternion
	{
		float x;
		float y;
		float z;
		float w;

		Quaternion() : x(0.f), y(0.f), z(0.f), w(1.0f) {}

		// ����ɂ��A�N�H�[�^�j�I���R���|�[�l���g�����ڐݒ肳��܂�-
		// �� / �p�x�ɂ͎g�p���Ȃ��ł�������
		Quaternion(float inX, float inY, float inZ, float inW) { Set(inX, inY, inZ, inW); }

		// ���Ɗp�x����CQuaternion���\�z���܂�
		// ���͂��łɐ��K������Ă���Ƒz�肳��܂����A�p�x�̓f�B�O���[�ł�
		Quaternion(const VECTOR3& axis, float angle)
		{
			float scalar = sin( XMConvertToRadians(angle) / 2.0f );
			x = axis.x * scalar;
			y = axis.y * scalar;
			z = axis.z * scalar;
			w = cos(angle / 2.0f);
		}

		// �����R���|�[�l���g�𒼐ڐݒ肵�܂�
		void Set(float inX, float inY, float inZ, float inW)
		{
			x = inX;
			y = inY;
			z = inZ;
			w = inW;
		}

		void Conjugate()
		{
			x *= -1.0f;
			y *= -1.0f;
			z *= -1.0f;
		}

		// ���N�H�[�^�j�I�����K������
		void Normalize()
		{
			float length = sqrt(x * x + y * y + z * z + w * w);
			x /= length;
			y /= length;
			z /= length;
			w /= length;
		}

		// �񋟂��ꂽ�N�H�[�^�j�I���𐳋K������
		Quaternion Normalize(const Quaternion& q)
		{
			Quaternion retVal = q;
			retVal.Normalize();
			return retVal;
		}

		// ���`���
		Quaternion Lerp(const Quaternion& a, const Quaternion& b, float f)
		{
			Quaternion retVal;
			retVal.x = Math::Lerp(a.x, b.x, f);
			retVal.y = Math::Lerp(a.y, b.y, f);
			retVal.z = Math::Lerp(a.z, b.z, f);
			retVal.w = Math::Lerp(a.w, b.w, f);
			retVal.Normalize();
			return retVal;
		}

		// 2�̃N�H�[�^�j�I���Ԃ̃h�b�g�ρia Dot b�j
		float Dot(const Quaternion& a, const Quaternion& b)
		{
			return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
		}

		// ���ʐ��`���
		Quaternion Slerp(const Quaternion& a, const Quaternion& b, float f)
		{
			float rawCosm = Quaternion::Dot(a, b);

			float cosom = -rawCosm;
			if (rawCosm >= 0.0f)
			{
				cosom = rawCosm;
			}

			float scale0, scale1;

			if (cosom < 0.9999f)
			{
				const float omega = acos(cosom);
				const float invSin = 1.f / sin(omega);
				scale0 = sin((1.f - f) * omega) * invSin;
				scale1 = sin(f * omega) * invSin;
			}
			else
			{
				// Use linear interpolation if the CQuaternions
				// are collinear
				scale0 = 1.0f - f;
				scale1 = f;
			}

			if (rawCosm < 0.0f)
			{
				scale1 = -scale1;
			}

			Quaternion retVal;
			retVal.x = scale0 * a.x + scale1 * b.x;
			retVal.y = scale0 * a.y + scale1 * b.y;
			retVal.z = scale0 * a.z + scale1 * b.z;
			retVal.w = scale0 * a.w + scale1 * b.w;
			retVal.Normalize();
			return retVal;
		}

		// �A������
		// q��]�Ap��]
		Quaternion Concatenate(const Quaternion& q, const Quaternion& p)
		{
			Quaternion retVal;

			// �x�N�g��������ps * qv + qs * pv + pv x qv
			VECTOR3 qv(q.x, q.y, q.z);
			VECTOR3 pv(p.x, p.y, p.z);
			VECTOR3 newVec = p.w * qv + q.w * pv + VECTOR3::Cross(pv, qv);
			retVal.x = newVec.x;
			retVal.y = newVec.y;
			retVal.z = newVec.z;

			// �X�J���[�R���|�[�l���g�͎��̂Ƃ���ł��B
			// ps * qs - pv�B qv
			retVal.w = p.w * q.w - VECTOR3::Dot(pv, qv);

			return retVal;
		}

		static XMVECTOR Euler(const XMFLOAT3& rotation)
		{
			XMVECTOR Quaternion = XMQuaternionIdentity();

			static const XMVECTOR axisX = XMVectorSet(1.0f, 0.f, 0.f, 0.f);
			static const XMVECTOR axisY = XMVectorSet(0.0f, 1.0f, 0.f, 0.f);
			static const XMVECTOR axisZ = XMVectorSet(0.0f, 0.f, 1.0f, 0.f);

			XMVECTOR rotateX = XMQuaternionRotationAxis(axisX, XMConvertToRadians(rotation.x));
			rotateX = XMVector4Normalize(rotateX);
			XMVECTOR rotateY = XMQuaternionRotationAxis(axisY, XMConvertToRadians(rotation.y));
			rotateY = XMVector4Normalize(rotateY);
			XMVECTOR rotateZ = XMQuaternionRotationAxis(axisZ, XMConvertToRadians(rotation.z));
			rotateZ = XMVector4Normalize(rotateZ);

			Quaternion = XMQuaternionMultiply(Quaternion, rotateX);
			Quaternion = XMQuaternionMultiply(Quaternion, rotateY);
			Quaternion = XMQuaternionMultiply(Quaternion, rotateZ);

			return XMQuaternionNormalize(Quaternion);
		}

		static Quaternion Euler(const float x, const float y, const float z)
		{
			XMVECTOR Quaternion = XMQuaternionIdentity();

			static const XMVECTOR axisX = XMVectorSet(1.0f, 0.f, 0.f, 0.f);
			static const XMVECTOR axisY = XMVectorSet(0.0f, 1.0f, 0.f, 0.f);
			static const XMVECTOR axisZ = XMVectorSet(0.0f, 0.f, 1.0f, 0.f);

			XMVECTOR rotateX = XMQuaternionRotationAxis(axisX, XMConvertToRadians(x));
			rotateX = XMVector4Normalize(rotateX);
			XMVECTOR rotateY = XMQuaternionRotationAxis(axisY, XMConvertToRadians(y));
			rotateY = XMVector4Normalize(rotateY);
			XMVECTOR rotateZ = XMQuaternionRotationAxis(axisZ, XMConvertToRadians(z));
			rotateZ = XMVector4Normalize(rotateZ);

			Quaternion = XMQuaternionMultiply(Quaternion, rotateX);
			Quaternion = XMQuaternionMultiply(Quaternion, rotateY);
			Quaternion = XMQuaternionMultiply(Quaternion, rotateZ);
			Quaternion = XMQuaternionNormalize(Quaternion);

			XMFLOAT4 quat;
			XMStoreFloat4(&quat, Quaternion);

			return Quaternion::Quaternion(quat.x, quat.y, quat.z, quat.w);
		}
	};

	// ���ݕϊ��֐�

	inline XMINT2 INT2ToXMINT2(const INT2& i2)
	{
		return XMINT2(i2.x, i2.y);
	}

	inline INT2 XMINT2ToINT2(const XMINT2& i2)
	{
		return INT2(i2.x, i2.y);
	}

	inline XMFLOAT2 VECTOR2ToXMFLOAT2(const VECTOR2& f2)
	{
		return XMFLOAT2(f2.x, f2.y);
	}

	inline VECTOR2 XMFLOAT2ToVECTOR2(const XMFLOAT2& f2)
	{
		return VECTOR2(f2.x, f2.y);
	}

	inline XMFLOAT3 VECTOR3ToXMFLOAT3(const VECTOR3& f3)
	{
		return XMFLOAT3(f3.x, f3.y, f3.z);
	}

	inline VECTOR3 XMFLOAT3ToVECTOR3(const XMFLOAT3& f3)
	{
		return VECTOR3(f3.x, f3.y, f3.z);
	}

	inline XMVECTOR VECTOR3ToXMVECTOR(const VECTOR3& f3)
	{
		return XMVectorSet(f3.x, f3.y, f3.z, 0.0f);
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

	inline VECTOR3 XMVECTORToVECTOR3(const XMVECTOR& vector)
	{
		XMFLOAT3 vec;
		XMStoreFloat3(&vec, vector);

		return 	VECTOR3(vec.x, vec.y, vec.z);
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

	// �萔

	const INT2 INT2_Zero(0, 0);
	const INT2 INT2_UnitX(1, 0);
	const INT2 INT2_UnitY(0, 1);
	const INT2 INT2_NegUnitX(-1, 0);
	const INT2 INT2_NegUnitY(0, -1);

	const VECTOR2 Vec2_Zero(0.0f, 0.0f);
	const VECTOR2 Vec2_UnitX(1.0f, 0.0f);
	const VECTOR2 Vec2_UnitY(0.0f, 1.0f);
	const VECTOR2 Vec2_NegUnitX(-1.0f, 0.0f);
	const VECTOR2 Vec2_NegUnitY(0.0f, -1.0f);

	const VECTOR3 Vec3_Zero(0.0f, 0.0f, 0.0f);
	const VECTOR3 Vec3_UnitX(1.0f, 0.0f, 0.0f);
	const VECTOR3 Vec3_UnitY(0.0f, 1.0f, 0.0f);
	const VECTOR3 Vec3_UnitZ(0.0f, 0.0f, 1.0f);
	const VECTOR3 Vec3_NegUnitX(-1.0f, 0.0f, 0.0f);
	const VECTOR3 Vec3_NegUnitY(0.0f, -1.0f, 0.0f);
	const VECTOR3 Vec3_NegUnitZ(0.0f, 0.0f, -1.0f);

	const Quaternion Quaternion_Identity(0.0f, 0.0f, 0.0f, 1.0f);
}

using namespace Math;

#endif // !MATH_H