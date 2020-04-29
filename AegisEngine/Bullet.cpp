#include	"manager.h"
#include	"Scene_Manager.h"
#include	"Scene.h"

#include	"Bullet.h"
#include	"ModelLoader.h"
#include	"Component_Manager.h"
#include	"Bounding_Aabb.h"
#include	"Bounding_Sphere.h"
#include	"Score.h"
#include	"Player.h"
#include	"Enemy.h"
#include	"Billboard.h"

#include	"audio_clip.h"

#include	<sstream>

BULLET::BULLET() : MoveVector(XMFLOAT3(0.0f, 0.0f, 0.0f))
{
	Position = XMFLOAT3(0.0f, 0.0f, 0.0f);

	Scaling = XMFLOAT3(0.1f, 0.1f, 0.1f);

	Model = make_unique<CMODEL>();

	HP = 200;
}

BULLET::BULLET(XMFLOAT3& position, XMFLOAT3& move_vector) : MoveVector(move_vector)
{
	Position = position;

	Scaling = XMFLOAT3(0.1f, 0.1f, 0.1f);

	Model = make_unique<CMODEL>();

	HP = 200;
}

BULLET::~BULLET()
{
	Uninit();
}

void BULLET::Init()
{
	string name("asset/model/bullet.fbx");

	Model->Load(name);

	auto scene = CManager::Get_Instance()->Get_Scene();
	auto sphere = Get_Component()->Add_Component<BOUNDING_SHPERE>(scene->Get_Game_Object(this));

	sphere->Set_Radius(1.0f);

	sphere->Set_Scaling(XMFLOAT3(0.11f, 0.11f, 0.11f));

	GAME_OBJECT::Init();
}

void BULLET::Draw()
{
	Model->Draw();

	GAME_OBJECT::Draw();
}

void BULLET::Draw_DPP()
{
	Model->Draw_DPP();
}

void BULLET::Update(float delta_time)
{
	Position.x += MoveVector.x * delta_time * 5.0f;
	Position.y += MoveVector.y * delta_time * 5.0f;
	Position.z += MoveVector.z * delta_time * 5.0f;

	if (HP <= 0)
	{
		CManager::Get_Instance()->Get_Scene()->Destroy_Game_Object(this);

		// ビルボード
		{
			auto name = this->Get_Object_Name();

			string str(name);
			ExtratNum(str);
			if (false == str.empty())
			{
				const int x = std::stoi(str);

				BILL_BOARD_ANIMATION* bba = CManager::Get_Instance()->Get_Scene()->Add_Game_Object<BILL_BOARD_ANIMATION>(LAYER_NAME::EFFECT, "explosion" + to_string(x));
				bba->Set_Position(&Position);
				bba->SetWH(XMFLOAT2(1.0f, 1.0f));
				bba->SetParam(6, 4, 4);
				bba->Init();
			}
		}

		return;
	}

	fps += delta_time;

	if (ANIMETION_FRAME_60 <= fps)
	{
		HP--;

		fps = 0.0f;
	}
	
	auto scene = CManager::Get_Instance()->Get_Scene();

	// 敵と弾の当たり判定
	{
		vector<ENEMY*> enemys = scene->Get_Game_Objects<ENEMY>();

		for (auto enemy : enemys)
		{
			auto bullet_collision = this->Get_Component()->Get_Component<BOUNDING_SHPERE>();
			auto enemy_collision = enemy->Get_Component()->Get_Component<BOUNDING_AABB>();

			if (ContainmentType::DISJOINT != bullet_collision->Get_Collition().Contains(enemy_collision->Get_Collition()))
			{
				// ビルボード
				{
					auto name = this->Get_Object_Name();

					string str(name);
					ExtratNum(str);
					if (!str.empty())
					{
						const int x = std::stoi(str);

						BILL_BOARD_ANIMATION* bba = CManager::Get_Instance()->Get_Scene()->Add_Game_Object<BILL_BOARD_ANIMATION>(LAYER_NAME::EFFECT, "explosion" + to_string(x));
						bba->Set_Position(&Position);
						bba->SetWH(XMFLOAT2(1.0f, 1.0f));
						bba->SetParam(6, 4, 4);
						bba->Init();
					}
				}

				CManager::Get_Instance()->Get_Scene()->Destroy_Game_Object(this);
				CManager::Get_Instance()->Get_Scene()->Destroy_Game_Object(enemy);

				AUDIO_MANAGER::Play_Sound_Object(SOUND_INDEX::SOUND_INDEX_EXPLOSION);

				return;
			}

			// プレイヤーと弾の当たり判定
			{
				auto player = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<PLAYER>("player");

				if (ContainmentType::DISJOINT != bullet_collision->Get_Collition().Contains(player.lock()->Get_Component()->Get_Component<BOUNDING_AABB>()->Get_Collition()))
				{
					auto billboards = scene->Get_Game_Objects<BILL_BOARD_ANIMATION>();

					// ビルボード
					{
						auto name = this->Get_Object_Name();

						string str(name);
						ExtratNum(str);

						if (!str.empty())
						{
							const int x = std::stoi(str);

							BILL_BOARD_ANIMATION* bba = CManager::Get_Instance()->Get_Scene()->Add_Game_Object<BILL_BOARD_ANIMATION>(LAYER_NAME::EFFECT, "explosion" + to_string(x));
							bba->Set_Position(&Position);
							bba->SetWH(XMFLOAT2(1.0f, 1.0f));
							bba->SetParam(6, 4, 4);
							bba->Init();
						}
					}

					player.lock()->Add_HP(-1.0f);

					CManager::Get_Instance()->Get_Scene()->Destroy_Game_Object(this);

					AUDIO_MANAGER::Play_Sound_Object(SOUND_INDEX::SOUND_INDEX_EXPLOSION);

					return;
				}
			}
		}
	}

	Model->Set_Position(Position);
	Model->Set_Rotation(Rotation);
	Model->Set_Scaling(Scaling);

	GAME_OBJECT::Update(delta_time);
}

void BULLET::Uninit()
{
	Model.reset(nullptr);
}

void BULLET::Set_Move_Vector(const XMFLOAT3 move_vector)
{
	MoveVector = move_vector;
}