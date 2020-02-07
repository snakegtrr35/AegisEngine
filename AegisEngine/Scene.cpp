#include	"Scene.h"
#include	"Scene_Manager.h"

list<shared_ptr<GAME_OBJECT>> SCENE::GameObjects[(int)LAYER_NAME::MAX_LAYER];
bool SCENE::PauseEnable = false;
LIGHTS	SCENE::Light_Manager;

//SCENE* SCENE_MANAGER::pScene = nullptr;