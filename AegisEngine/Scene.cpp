#ifndef AAAAAA_H
#define AAAAAA_H

#include	"Scene.h"
#include	"Scene_Manager.h"


//#include	"Game_Object.h"
//
//#include	"camera.h"
//#include	"Debug_Camera.h"
//#include	"Field.h"
//#include	"Player.h"
//#include	"Enemy.h"
//#include	"Sprite.h"
//#include	"Sprite_Animation.h"
//#include	"Polygon_3D.h"
//#include	"Billboard.h"
//#include	"Score.h"
//
//#include	"Grid.h"
//#include	"XYZ_Axis.h"
//#include	"Light.h"
//
////#include	"Bounding.h"
//#include	"Bounding_Shpere.h"
//#include	"Bounding_Aabb.h"
//#include	"Bounding_Obb.h"
//
//#include	"Mesh_Field.h"
//#include	"Mesh_Cylinder.h"
//#include	"Mesh_Dome.h"

list<shared_ptr<GAME_OBJECT>> SCENE::GameObjects[(int)LAYER_NAME::MAX_LAYER];
bool SCENE::PauseEnable = false;
LIGHTS	SCENE::Light_Manager;

//SCENE* SCENE_MANAGER::pScene = nullptr;

//template CCamera* SCENE::Add_Game_Object<CCamera>(LAYER_NAME layer, const string& name);
//template FIELD* SCENE::Add_Game_Object<FIELD>(LAYER_NAME layer, const string& name);
//template DEBUG_CAMERA* SCENE::Add_Game_Object<DEBUG_CAMERA>(LAYER_NAME layer, const string& name);
//template FIELD* SCENE::Add_Game_Object<FIELD>(LAYER_NAME layer, const string& name);
//template PLAYER* SCENE::Add_Game_Object<PLAYER>(LAYER_NAME layer, const string& name);
//template ENEMY* SCENE::Add_Game_Object<ENEMY>(LAYER_NAME layer, const string& name);
//template SPRITE* SCENE::Add_Game_Object<SPRITE>(LAYER_NAME layer, const string& name);
//template SPRITE_ANIMATION* SCENE::Add_Game_Object<SPRITE_ANIMATION>(LAYER_NAME layer, const string& name);
//template TEXTS* SCENE::Add_Game_Object<TEXTS>(LAYER_NAME layer, const string& name);
//template POLYGON_3D* SCENE::Add_Game_Object<POLYGON_3D>(LAYER_NAME layer, const string& name);
//template BILL_BOARD* SCENE::Add_Game_Object<BILL_BOARD>(LAYER_NAME layer, const string& name);
//template SCORE* SCENE::Add_Game_Object<SCORE>(LAYER_NAME layer, const string& name);
//template GRID* SCENE::Add_Game_Object<GRID>(LAYER_NAME layer, const string& name);
//template AXIS* SCENE::Add_Game_Object<AXIS>(LAYER_NAME layer, const string& name);
////template BOUNDING* SCENE::Add_Game_Object<BOUNDING>(LAYER_NAME layer, const string& name);
//template BOUNDING_SHPERE* SCENE::Add_Game_Object<BOUNDING_SHPERE>(LAYER_NAME layer, const string& name);
//template BOUNDING_AABB* SCENE::Add_Game_Object<BOUNDING_AABB>(LAYER_NAME layer, const string& name);
//template BOUNDING_OBB* SCENE::Add_Game_Object<BOUNDING_OBB>(LAYER_NAME layer, const string& name);
//template MESH_FIELD* SCENE::Add_Game_Object<MESH_FIELD>(LAYER_NAME layer, const string& name);
//template MESH_CYlLINDER* SCENE::Add_Game_Object<MESH_CYlLINDER>(LAYER_NAME layer, const string& name);
//template MESH_DOOM* SCENE::Add_Game_Object<MESH_DOOM>(LAYER_NAME layer, const string& name);

//// リストへの追加
//template <typename T>
//static T* SCENE::Add_Game_Object(LAYER_NAME layer, const string& name)
//{
//	//T* object = new T();
//	shared_ptr<T> object(new T());
//
//	object->Set_Object_Name(name);
//
//	GameObjects[(int)layer].emplace_back(object);
//
//	return object.get();
//}
//
//// リストから特定の名前のオブジェクトの取得
//template <typename T>
//static weak_ptr<T> SCENE::Get_Game_Object(const string& name)
//{
//	for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
//	{
//		/*for (auto object = GameObjects[i].begin(); object != GameObjects[i].end(); object++)
//		{
//			if (typeid(T) == typeid(*object->get()))
//			{
//				if (name == object->get()->Get_Object_Name())
//				{
//					return object->get();
//				}
//			}
//		}*/
//
//		for (auto object : GameObjects[i])
//		{
//			if (typeid(T) == typeid(*object.get()))
//			{
//				if (name == object.get()->Get_Object_Name())
//				{
//					weak_ptr<T> obj(static_pointer_cast<T>(object));
//
//					return  obj;
//				}
//			}
//		}
//	}
//
//	weak_ptr<T> obj;
//	return obj;
//}
//
//// リストから特定のオブジェクト（複数）の取得
//template <typename T>
//static vector<T*> SCENE::Get_Game_Objects()
//{
//	vector<T*> objects;
//	for (int i = 0; i < (int)LAYER_NAME::MAX_LAYER; i++)
//	{
//		for (auto object = GameObjects[i].begin(); object != GameObjects[i].end(); object++)
//		{
//			if (typeid(T) == typeid(*object->get()))
//			{
//				objects.emplace_back((T*)object->get());
//			}
//		}
//	}
//	return objects;
//}


#endif // !AAAAAA_H