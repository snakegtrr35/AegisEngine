#include	"Scene.h"

list<shared_ptr<GAME_OBJECT>> SCENE::GameObjects[(int)LAYER_NAME::MAX_LAYER];
bool SCENE::PauseEnable = false;

SCENE* SCENE_MANAGER::pScene = nullptr;