#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

class GAME_OBJECT;

// コンポーネントクラス
class COMPONENT {
protected:

	weak_ptr<GAME_OBJECT> Owner;

	bool Enable;
	bool DestroyFlag;

public:

	COMPONENT() : Enable(true), DestroyFlag(false) {
		Owner.reset();
	}

	virtual ~COMPONENT() {};

	void Init(shared_ptr<GAME_OBJECT> owner) {
		Owner = owner;

	}

	virtual void Update(float delta_time) = 0;

	virtual void Uninit() = 0;

	void SetEnable(const bool flag) {
		Enable = flag;
	}

	const bool GetEnable() {
		return Enable;
	}

	void Set_Destroy() {
		DestroyFlag = true;
	}

	// メモリ上からの削除
	bool Destroy() {
		if (DestroyFlag)
		{
			return true;
		}
		return false;
	}

	template<class Archive>
	void serialize(Archive& ar)
	{
		//ar(ptr);
	}

	/*template<class Archive>
	void save(Archive& ar) const
	{
		ar(Owner);
	}

	template<class Archive>
	void load(Archive& ar)
	{
		ar(Owner);
	}*/
};





class COMPONENT_MANEGER {
private:

	list< unique_ptr<COMPONENT> > Conponent_List;

public:

	// リストへのメニューオブジェクトの追加
	template <typename T>
	T* Add_Component()
	{
		for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
		{
			if (typeid(T) == typeid(*object->get()))
			{
				return (T*)object->get();
			}
		}

		T* object = new T();

		Conponent_List.emplace_back(object);

		return object;
	}

	// リストから特定のメニューオブジェクトの取得
	template <typename T>
	T* Get_Component() {
		for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
		{
			if (typeid(T) == typeid(*object->get()))
			{
				return (T*)object->get();
			}
		}
		return nullptr;
	}

	COMPONENT_MANEGER() {}

	~COMPONENT_MANEGER() {
		Uninit();
	}

	void Update(float delta_time) {
		for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
		{
			object->get()->Update(delta_time);
		}

		Conponent_List.remove_if([](auto& object) { return object->Destroy(); }); // リストから削除
	}

	void Uninit() {
		for (auto object = Conponent_List.begin(); object != Conponent_List.end(); object++)
		{
			object->reset(nullptr);
		}
		Conponent_List.clear();
	}
};

#endif // COMPONENT_H!