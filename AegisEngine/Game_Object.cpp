#include	"Game_Object.h"

#include	"Component.h"

unordered_set<string> GAME_OBJECT::Object_Name_Map;



void GAME_OBJECT::Update(float delta_time)
{
	Component->Update(delta_time);
}