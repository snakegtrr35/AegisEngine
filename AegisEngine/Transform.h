#pragma once

#ifndef _INCLEDE_TRANSFORM_H_
#define _INCLEDE_TRANSFORM_H_

namespace Aegis
{
	class Transform
	{
	protected:
		//! 座標
		XMFLOAT3 Position;
		//! 回転量
		XMFLOAT3 Rotation;
		//! 拡大縮小値
		XMFLOAT3 Scaling;

	public:

		/**
		* @brief コンストラクタ
		* @details 引数無しコンストラクタ
		*/
		Transform() :
		Position(XMFLOAT3(0.f, 0.f, 0.f)), Rotation(XMFLOAT3(0.f, 0.f, 0.f)), Scaling(XMFLOAT3(1.0f, 1.0f, 1.0f))
		{}

		/**
		* @brief デストラクタ
		* @details デストラクタ
		*/
		virtual ~Transform() {}

		/**
		* @brief 座標を取得する関数
		* @return XMFLOAT3* 座標(XMFLOAT3*)
		* @details　オブジェクトの座標を取得する関数
		*/
		XMFLOAT3* const Get_Position() {
			return &Position;
		};

		/**
		* @brief 回転を取得する関数
		* @return XMFLOAT3* 回転(XMFLOAT3*)
		* @details　オブジェクトの回転を取得する関数
		*/
		XMFLOAT3* const Get_Rotation() {
			return &Rotation;
		};

		/**
		* @brief スケートを取得する関数
		* @return XMFLOAT3* スケート(XMFLOAT3*)
		* @details　オブジェクトのスケートを取得する関数
		*/
		XMFLOAT3* const Get_Scaling() {
			return &Scaling;
		};

		/**
		* @brief 座標を設定する関数
		* @param position 座標(XMFLOAT3*)
		* @details オブジェクトの座標を設定する関数
		*/
		void Set_Position(XMFLOAT3* position) {
			Position = *position;
		}

		/**
		* @brief 座標を設定する関数
		* @param position 座標(XMFLOAT3)
		* @details オブジェクトの座標を設定する関数
		*/
		void Set_Position(XMFLOAT3& position) {
			Position = position;
		};

		/**
		* @brief 回転を設定する関数
		* @param position 回転量(XMFLOAT3*)
		* @details オブジェクトの回転を設定する関数
		*/
		void Set_Rotation(XMFLOAT3* rotation) {
			Rotation = *rotation;
		};

		/**
		* @brief 回転を設定する関数
		* @param position 回転量(XMFLOAT3)
		* @details オブジェクトの回転を設定する関数
		*/
		void Set_Rotation(XMFLOAT3& rotation) {
			Rotation = rotation;
		};

		/**
		* @brief スケールを設定する関数
		* @param position スケール(XMFLOAT3*)
		* @details オブジェクトのスケールを設定する関数
		*/
		void Set_Scaling(XMFLOAT3* scaling) {
			Scaling = *scaling;
		};

		/**
		* @brief スケールを設定する関数
		* @param position スケール(XMFLOAT3)
		* @details オブジェクトのスケールを設定する関数
		*/
		void Set_Scaling(XMFLOAT3& scaling) {
			Scaling = scaling;
		};

		template<class Archive>
		void serialize(Archive& ar)
		{
			ar(Position);
			ar(Rotation);
			ar(Scaling);
		}
	};
}

#endif // !_INCLEDE_TRANSFORM_H_