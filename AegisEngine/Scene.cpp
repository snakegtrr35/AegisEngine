#include	"Scene.h"
#include	"Scene_Manager.h"

list<shared_ptr<GAME_OBJECT>>	SCENE::GameObjects[(int)LAYER_NAME::MAX_LAYER];
bool							SCENE::PauseEnable = false;
LIGHTS							SCENE::Light_Manager;
std::mutex						SCENE::Load_Mutex;
bool							SCENE::Load_Enable = true;

SCENE::SCENE()
{
	std::lock_guard<std::mutex>  lock(Load_Mutex);
	Load_Enable = true;
}

void SCENE::SetLockLoad()
{
	std::lock_guard<std::mutex>  lock(Load_Mutex);
	Load_Enable = false;
}

bool SCENE::GetLockLoad()
{
	std::lock_guard<std::mutex>  lock(Load_Mutex);
	return Load_Enable;
}