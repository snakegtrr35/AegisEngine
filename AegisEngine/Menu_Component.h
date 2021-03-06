#pragma once

#ifndef MENU_COMPONENT_H
#define MENU_COMPONENT_H

#include	"Component.h"

class SPRITE;

class MENU_COMPONENT : public COMPONENT {
private:

protected:
	list<MENU_COMPONENT*> MenuObjects;

public:
	// リストへのメニューオブジェクトの追加
	template <typename T>
	T* Add_Menu_Object()
	{
		for (auto object : MenuObjects)
		{
			if (typeid(T) == typeid(*object))
			{
				return (T*)object;
			}
		}

		T* object = new T();

		MenuObjects.push_back(object);

		return object;
	}

	// リストから特定のメニューオブジェクトの取得
	template <typename T>
	T* Get_Menu_Object()
	{
		for (auto object : MenuObjects)
		{
			if (typeid(T) == typeid(*object))
			{
				return (T*)object;
			}
		}
		return nullptr;
	}

	MENU_COMPONENT() {};
	virtual ~MENU_COMPONENT() {
		Uninit();
	};

	void Update(float delta_time) override {};
	void Uninit() override {
		for (auto object : MenuObjects)
		{
			SAFE_DELETE(object);
		}
		MenuObjects.clear();
	};

};

class MOUSE_EVENT_COMPONENT : public MENU_COMPONENT {
private:

protected:

public:
	MOUSE_EVENT_COMPONENT();
	~MOUSE_EVENT_COMPONENT();

	void Update(float delta_time) override;
	void Uninit() override;

	bool Mouse_Click_Left(const string& const name, SPRITE* me);
	bool Mouse_Click_Right(const string& const name, SPRITE* me);
	bool Mouse_Over(const string& const name, SPRITE* me);
};

class KEYBOARD_EVENT_COMPONENT : public MENU_COMPONENT {
private:

protected:

public:
	KEYBOARD_EVENT_COMPONENT();
	~KEYBOARD_EVENT_COMPONENT();

	void Update(float delta_time) override;
	void Uninit() override;
};

#endif // !MENU_COMPONENT_H
