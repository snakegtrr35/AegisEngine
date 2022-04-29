#pragma once

#ifndef _INCLEDE_TRANSFORM_H_
#define _INCLEDE_TRANSFORM_H_

namespace aegis
{
	class Transform
	{
	protected:
		//! 座標
		Vector3 Position;
		//! 回転量
		Vector3 Rotation;
		//! 拡大縮小値
		Vector3 Scaling;

	public:

		/**
		* @brief コンストラクタ
		* @details 引数無しコンストラクタ
		*/
		Transform() :
		Position(Vector3(0.f, 0.f, 0.f)), Rotation(Vector3(0.f, 0.f, 0.f)), Scaling(Vector3(1.0f, 1.0f, 1.0f))
		{}

		/**
		* @brief デストラクタ
		* @details デストラクタ
		*/
		virtual ~Transform() {}

		/**
		* @brief 座標を取得する関数
		* @return Vector3* 座標(Vector3*)
		* @details　オブジェクトの座標を取得する関数
		*/
		const Vector3 Get_Position() {
			return Position;
		};

		/**
		* @brief 回転を取得する関数
		* @return Vector3* 回転(Vector3*)
		* @details　オブジェクトの回転を取得する関数
		*/
		const Vector3 Get_Rotation() {
			return Rotation;
		};

		/**
		* @brief スケートを取得する関数
		* @return Vector3* スケート(Vector3*)
		* @details　オブジェクトのスケートを取得する関数
		*/
		const Vector3 Get_Scaling() {
			return Scaling;
		};

		/**
		* @brief 座標を設定する関数
		* @param position 座標(Vector3*)
		* @details オブジェクトの座標を設定する関数
		*/
		void Set_Position(Vector3* position) {
			Position = *position;
		}

		/**
		* @brief 座標を設定する関数
		* @param position 座標(Vector3)
		* @details オブジェクトの座標を設定する関数
		*/
		void Set_Position(Vector3& position) {
			Position = position;
		};

		/**
		* @brief 回転を設定する関数
		* @param position 回転量(Vector3*)
		* @details オブジェクトの回転を設定する関数
		*/
		void Set_Rotation(Vector3* rotation) {
			Rotation = *rotation;
		};

		/**
		* @brief 回転を設定する関数
		* @param position 回転量(Vector3)
		* @details オブジェクトの回転を設定する関数
		*/
		void Set_Rotation(Vector3& rotation) {
			Rotation = rotation;
		};

		/**
		* @brief スケールを設定する関数
		* @param position スケール(Vector3*)
		* @details オブジェクトのスケールを設定する関数
		*/
		void Set_Scaling(Vector3* scaling) {
			Scaling = *scaling;
		};

		/**
		* @brief スケールを設定する関数
		* @param position スケール(Vector3)
		* @details オブジェクトのスケールを設定する関数
		*/
		void Set_Scaling(Vector3& scaling) {
			Scaling = scaling;
		};

		template<class Archive>
		void save(Archive& archive) const
		{
			archive(cereal::make_nvp("Position", Position),
					cereal::make_nvp("Rotation", Rotation),
					cereal::make_nvp("Scaling", Scaling)
			);
		}

		template<class Archive>
		void load(Archive& archive)
		{
			archive(cereal::make_nvp("Position", Position),
					cereal::make_nvp("Rotation", Rotation),
					cereal::make_nvp("Scaling", Scaling)
			);
		}
	};
}

#endif // !_INCLEDE_TRANSFORM_H_