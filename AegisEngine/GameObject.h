/**
 * @file GameObject.h
 * @brief GameObjectのヘッダーファイル
 */

#pragma once

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Component_Manager.h"
#include "Transform.h"
#include "include/engine/core/uuid/uuid.h"

#include "include\engine\core\AegisObject.h"

 /**
 * @brief ゲームオブジェクトクラス
 * @details ゲームオブジェクトクラス
 */
class GameObject : public aegis::AegisObject {

	ABSTRACT_OBJECT_TYPE_INFO(aegis::AegisObject, GameObject)

private:
	//! ゲームオブジェクトの名称一覧(静的なもの)
	static aegis::unordered_set<aegis::string> Object_Name_Map;

	//! コンポーネント
	aegis::vector< std::weak_ptr<COMPONENT> > Components;

	bool IsInited = false;

protected:

	//! オブジェクトの名前
	aegis::string Object_Name;

	//! 削除するかのフラグ
	bool DestroyFlag;

	aegis::Transform Transform;

	////! コンポーネント
	//std::unique_ptr<COMPONENT_MANEGER, Delete> Component;

	aegis::uuid Uuid;

public:
	/**
	* @brief コンストラクタ
	* @details 引数無しコンストラクタ
	*/
	GameObject();

	/**
	* @brief デストラクタ
	* @details デストラクタ
	*/
	virtual ~GameObject();

	/**
	* @brief 初期化関数
	* @details 仮想初期化関数
	*/
	virtual void Init();

	/**
	* @brief 描画関数
	* @details 仮想描画関数
	*/
	virtual void Draw();

	/**
	* @brief 描画(shdow-pass)関数
	* @details 仮想shdow-pass関数
	*/
	virtual void Draw_Shadow();

	/**
	* @brief 描画(Depth-pre-pass)関数
	* @details 純粋仮想Depth-pre-pass関数
	*/
	virtual void Draw_DPP() = 0;

	/**
	* @brief 更新関数
	* @details 仮想更新関数
	*/
	virtual void Update(float delta_time);

	/**
	* @brief 終了処理関数
	* @details 純粋仮想終了処理関数
	*/
	virtual void Uninit();

	/**
	* @brief 削除要請関数
	* @details 削除をしてもらう為の関数(削除フラグを立てる)
	*/
	void Set_Destroy();

	/**
	* @brief 削除判定関数
	* @return bool 削除判定のフラグ
	* @details 削除をするか判断する関数(手動では呼ばないで下さい)
	*/
	bool Destroy();

	/**
	* @brief 名前を取得する関数
	* @return string オブジェクトの名前
	* @details オブジェクトの名前を取得する関数
	*/
	const aegis::string& Get_Object_Name() {
		return Object_Name;
	};

	/**
	* @brief 名前を設定する関数
	* @param name 名前(string)
	* @details オブジェクトの名前を設定する関数
	*/
	void Set_Object_Name(const aegis::string& name);

	aegis::uuid GetId() const { return Uuid; }

	void InitEnd() { IsInited = true; }
	bool IsInit() const { return IsInited; }

	void* operator new(aegis::uint64 size);
	void* operator new(aegis::uint64 size, std::align_val_t alignment);
	void* operator new[](aegis::uint64 size);
	void* operator new[](aegis::uint64 size, std::align_val_t alignment);

	void operator delete(void* ptr, aegis::uint64 size) noexcept;
	void operator delete(void* ptr, std::align_val_t alignment) noexcept;
	void operator delete(void* ptr, aegis::uint64 size, std::align_val_t alignment) noexcept;
	void operator delete[](void* ptr, aegis::uint64 size) noexcept;
	void operator delete[](void* ptr, std::align_val_t alignment) noexcept;
	void operator delete[](void* ptr, aegis::uint64 size, std::align_val_t alignment) noexcept;

	aegis::Transform& Get_Transform()
	{
		return Transform;
	}

	template<class T>
	T* AddComponent()
	{
		for (const auto& component : Components)
		{
			if (component.lock()->GetTypeId() == T::getDTI()->GetTypeID())
			{
				return static_cast<T*>(component.lock().get());
			}
		}
		
		auto component = COMPONENT_MANEGER::getInstance()->AddComponent<T>(Uuid);
		component->Set_Owner(this);
		Components.emplace_back(component);
		return component.get();
	}

	template<class T>
	T* GetComponent()
	{
		for (const auto& component : Components)
		{
			if (component.lock()->GetTypeId() == T::getDTI()->GetTypeID())
			{
				return static_cast<T*>(component.lock().get());
			}
		}
		return nullptr;
	}

	/**
	* @brief コンポーネントを取得する関数
	* @return COMPONENT_MANEGER* 座標(COMPONENT_MANEGER*)
	* @details オブジェクトのコンポーネントを取得する関数
	*/
	//COMPONENT_MANEGER* const Get_Component() {
	//	return Component.get();
	//}
	aegis::vector< std::weak_ptr<COMPONENT> >* const GetComponents() {
		return &Components;
	}

	/**
	* @brief ゲームオブジェクトネームマップを取得する関数
	* @return unordered_set<string> ゲームオブジェクトネームマップ(unordered_set<string>)
	* @details ゲームオブジェクトネームマップを取得する関数
	*/
	static const aegis::unordered_set<aegis::string>& Get_Object_Name_Map() {
		return Object_Name_Map;
	}

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(cereal::make_nvp("Object_Name", std::string(Object_Name)));
		archive(cereal::make_nvp("Id", Uuid));
		archive(cereal::make_nvp("Transform", Transform));
		archive(cereal::make_nvp("Components", Components));
	}

	template<class Archive>
	void load(Archive& archive)
	{
		std::string s;
		archive(cereal::make_nvp("Object_Name", s));
		Object_Name.reserve(s.size());
		Object_Name = s;

		archive(cereal::make_nvp("Id", Uuid));
		archive(cereal::make_nvp("Transform", Transform));
		archive(cereal::make_nvp("Components", Components));
	
		Set_Object_Name(Object_Name);
		
		for (auto& component : Components)
		{
			component.lock()->Set_Owner(this);
			COMPONENT_MANEGER::getInstance()->AddComponent(Uuid, std::move(component.lock()));
		}
	}
};

namespace cereal
{
	//// JSON
	//// serialize
	//void prologue(cereal::JSONInputArchive&, GameObject const& data);
	//void epilogue(cereal::JSONInputArchive&, GameObject const& data);

	//void prologue(cereal::JSONOutputArchive&, GameObject const& data);
	//void epilogue(cereal::JSONOutputArchive&, GameObject const& data);

	//// Binary
	//// deserialize
	//void prologue(cereal::BinaryInputArchive&, GameObject const& data);
	//void epilogue(cereal::BinaryInputArchive&, GameObject const& data);

	//void prologue(cereal::BinaryOutputArchive&, GameObject const& data);
	//void epilogue(cereal::BinaryOutputArchive&, GameObject const& data);
}

#endif // !GAME_OBJECT_H