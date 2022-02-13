#include	"Effekseer.h"
#include	"Renderer.h"
#include	"manager.h"
#include	"Scene.h"
#include	"audio_clip.h"

#include	"camera.h"
#include	"Debug_Camera.h"

::Effekseer::Manager*			EFFEKSEER_MANAGER::Manager = nullptr;
::EffekseerRenderer::Renderer*	EFFEKSEER_MANAGER::Renderer = nullptr;
::EffekseerSound::Sound*		EFFEKSEER_MANAGER::Sound = nullptr;

aegis::unordered_map<std::string, ::Effekseer::Effect*>	EFFEKSEER_MANAGER::Effects;
aegis::unordered_map<std::string, ::Effekseer::Handle>	EFFEKSEER_MANAGER::Handles;

using namespace aegis;

::Effekseer::Matrix44 XMMATRIXToMatrix44(const XMMATRIX& matrix)
{
	XMFLOAT4X4 m;

	XMStoreFloat4x4(&m, matrix);

	::Effekseer::Matrix44 mtr;

	for (char y = 0; y < 4; y++)
	{
		for (char x = 0; x < 4; x++)
		{
			mtr.Values[y][x] = m.m[y][x];
		}
	}
	return mtr;
}

bool EFFEKSEER_MANAGER::Init()
{
	{
		CRenderer* render = CRenderer::getInstance();

		// 描画用インスタンスの生成
		Renderer = ::EffekseerRendererDX11::Renderer::Create(render->GetDevice(), render->GetDeviceContext(), 2000);
		if (nullptr == Renderer)
		{
			return false;
		}
	}

	{
		// エフェクト管理用インスタンスの生成
		Manager = ::Effekseer::Manager::Create(2048);
		if (nullptr == Manager)
		{
			return false;
		}

		// 描画用インスタンスから描画機能を設定
		Manager->SetSpriteRenderer(Renderer->CreateSpriteRenderer());
		Manager->SetRibbonRenderer(Renderer->CreateRibbonRenderer());
		Manager->SetRingRenderer(Renderer->CreateRingRenderer());
		Manager->SetTrackRenderer(Renderer->CreateTrackRenderer());
		Manager->SetModelRenderer(Renderer->CreateModelRenderer());

		// 座標系の指定( LHで左手系 )
		Manager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
		// 描画用インスタンスからテクスチャの読込機能を設定
		Manager->SetTextureLoader(Renderer->CreateTextureLoader());
		// 独自拡張可能、現在はファイルから読み込んでいる。
		Manager->SetModelLoader(Renderer->CreateModelLoader());

		//// カリングを行う範囲を設定
		//// 範囲内にエフェクトが存在するとカリングが高速に実行される
		//// layerCountが大きいほうが高速にカリングを行うがメモリも消費する。最大6程度。
		//Manager->CreateCullingWorld(500.0f, 500.0f, 500.0f, 5);

	}

	{
		// 音再生用インスタンスの生成
		Sound = ::EffekseerSound::Sound::Create(AUDIO_MANAGER::Get_Xaudio(), 16, 16);
		if (nullptr == Sound)
		{
			return false;
		}

		// 音再生用インスタンスから再生機能を指定
		Manager->SetSoundPlayer(Sound->CreateSoundPlayer());

		// 音再生用インスタンスからサウンドデータの読込機能を設定
		// 独自拡張可能、現在はファイルから読み込んでいる。
		Manager->SetSoundLoader(Sound->CreateSoundLoader());
	}

	Effects["test"] = Effekseer::Effect::Create(Manager, (const EFK_CHAR*)L"test.efk");

	//Handles["test"] = Manager->Play(Effects["test"].Effect, 0, 0, 0);
	//Manager->SetRotation(Handles["test"], Effekseer::Vector3D(0, 1, 0), 45.0f);
	Manager->SetSpeed(Handles["test"], 0.5f);

	return true;
}



void EFFEKSEER_MANAGER::Uninit()
{
	// エフェクトの停止
	Manager->StopAllEffects();

	// エフェクトの破棄
	for (auto effect : Effects)
	{
		if (nullptr != effect.second)
		{
			effect.second->Release();
			effect.second = nullptr;
		}
	}
	Effects.clear();

	Handles.clear();

	if (nullptr != Manager) Manager->Destroy(); Manager = nullptr;
	if (nullptr != Sound) Sound->Destroy(); Sound = nullptr;
	if (nullptr != Renderer) Renderer->Destroy(); Renderer = nullptr;
}

void EFFEKSEER_MANAGER::Draw()
{
	// エフェクトの描画開始処理を行う。
	Renderer->BeginRendering();


	//// 視錐体内に存在するエフェクトを計算する。
	//// カリングの設定がないエフェクトは常に描画される。
	//Manager->CalcCulling(Renderer->GetCameraProjectionMatrix(), false);

	// エフェクトの描画を行う。
	Manager->Draw();

	// エフェクトの描画終了処理を行う。
	Renderer->EndRendering();
}

void EFFEKSEER_MANAGER::Update(float delta_time)
{
	auto player = CManager::Get_Instance()->Get_Scene()->Get_Game_Object("player");

	XMFLOAT3* position = player->Get_Transform().Get_Position();

	if(nullptr != player)
		Set_Location("test", XMFLOAT3(position->x, position->y, position->z));

	//Manager->SetLocation(Handles["test"], Effekseer::Vector3D(player->Get_Position()->x, player->Get_Position()->y, player->Get_Position()->z));
	Manager->SetScale(Handles["test"], 0.5f, 0.5f, 0.5f);
	Manager->SetRotation(Handles["test"], Effekseer::Vector3D(0, 1, 0), XMConvertToRadians(90.0f));

	Set();

	// エフェクトの更新処理を行う
	Manager->Update();
}

void EFFEKSEER_MANAGER::Set()
{
	std::weak_ptr<DEBUG_CAMERA> D_camera;
	auto camera = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");

	if (camera.expired() && Empty_weak_ptr<CCamera>(camera))
	{
		D_camera = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");
	}

	if (camera.expired() && Empty_weak_ptr<CCamera>(camera))
	{
		if (D_camera.expired() && Empty_weak_ptr<DEBUG_CAMERA>(D_camera))
		{
			return;
		}
	}

	{
		XMFLOAT3* pos;
		static float angle;
		static ::Effekseer::Vector3D position;
		static ::Effekseer::Vector3D at;
		static ::Effekseer::Vector3D up;

		if (!camera.expired() && Empty_weak_ptr<CCamera>(camera))
		{
			position.X = camera.lock()->Get_Transform().Get_Position()->x;
			position.Y = camera.lock()->Get_Transform().Get_Position()->y;
			position.Z = camera.lock()->Get_Transform().Get_Position()->z;

			XMFLOAT3 vec;
			XMStoreFloat3(&vec, *camera.lock()->Get_At());

			at.X = vec.x;
			at.Y = vec.y;
			at.Z = vec.z;

			XMStoreFloat3(&vec, *camera.lock()->Get_Up());

			up.X = vec.x;
			up.Y = vec.y;
			up.Z = vec.z;

			angle = camera.lock()->Get_Viewing_Angle();
		}
		else
		{
			position.X = D_camera.lock()->Get_Transform().Get_Position()->x;
			position.Y = D_camera.lock()->Get_Transform().Get_Position()->y;
			position.Z = D_camera.lock()->Get_Transform().Get_Position()->z;

			XMFLOAT3 vec;
			XMStoreFloat3(&vec, *D_camera.lock()->Get_At());

			at.X = vec.x;
			at.Y = vec.y;
			at.Z = vec.z;

			XMStoreFloat3(&vec, *D_camera.lock()->Get_Up());

			up.X = vec.x;
			up.Y = vec.y;
			up.Z = vec.z;

			angle = D_camera.lock()->Get_Viewing_Angle();
		}

		{
			XMMATRIX mtr;
			if (!camera.expired() && Empty_weak_ptr<CCamera>(camera))
			{
				mtr = camera.lock()->Get_Camera_Projection();
			}
			else
			{
				mtr = D_camera.lock()->Get_Camera_Projection();
			}

			::Effekseer::Matrix44 matrix = XMMATRIXToMatrix44(mtr);

			// 投影行列を設定
			Renderer->SetProjectionMatrix(matrix);
		}

		{
			XMMATRIX mtr;

			if (!camera.expired() && Empty_weak_ptr<CCamera>(camera))
			{
				mtr = camera.lock()->Get_Camera_View();
			}
			else
			{
				mtr = D_camera.lock()->Get_Camera_View();
			}

			::Effekseer::Matrix44 matrix = XMMATRIXToMatrix44(mtr);

			// カメラ行列を設定
			Renderer->SetCameraMatrix(matrix);
		}

		// 3Dサウンド用リスナー設定の更新
		Sound->SetListener(position, at, up);
	}
}

void EFFEKSEER_MANAGER::Play(const std::string& name)
{
	Manager->StopEffect(Handles[name]);

	auto player = CManager::Get_Instance()->Get_Scene()->Get_Game_Object("player");

	XMFLOAT3* position = player->Get_Transform().Get_Position();

	Handles[name] = Manager->Play(Effects[name], position->x, position->y, position->z);

	//Effects[name].Handle = Manager->Play(Effects[name].Effect, 0, 0, 0);
}

void EFFEKSEER_MANAGER::Play(const std::string& handle_name, const std::string& effect_name, const XMFLOAT3& position)
{
	// ロードしていないエフェクトの判定
#ifdef _DEBUG
	if (Effects.find(effect_name) == Effects.end())
	{
		std::string text("存在しないエフェクトです\n");
		std::string t(effect_name.c_str());
		text += t;

		Erroer_Message(text);

		return;
	}
#endif // _DEBUG

	Manager->StopEffect(Handles[handle_name]);

	Handles[handle_name] = Manager->Play(Effects[effect_name], position.x, position.y, position.z);
}



void EFFEKSEER_MANAGER::Play(const std::string& handle_name, const std::string& effect_name, const Vector3& position)
{
	// ロードしていないエフェクトの判定
#ifdef _DEBUG
	if (Effects.find(effect_name) == Effects.end())
	{
		std::string text("存在しないエフェクトです\n");
		std::string t(effect_name.c_str());
		text += t;

		Erroer_Message(text);

		return;
	}
#endif // _DEBUG

	Manager->StopEffect(Handles[handle_name]);

	Handles[handle_name] = Manager->Play(Effects[effect_name], position.x, position.y, position.z);
}

::Effekseer::Manager* const EFFEKSEER_MANAGER::Get_Manager()
{
	return Manager;
}

const aegis::unordered_map<std::string, ::Effekseer::Effect*>& EFFEKSEER_MANAGER::Get_Effects()
{
	return Effects;
}

void EFFEKSEER_MANAGER::Set_Location(const std::string& handle_name, const XMFLOAT3& position)
{
	Manager->SetLocation(Handles[handle_name], Effekseer::Vector3D(position.x, position.y, position.z));
}

void EFFEKSEER_MANAGER::Set_Location(const std::string& handle_name, const Vector3& position)
{
	Manager->SetLocation(Handles[handle_name], Effekseer::Vector3D(position.x, position.y, position.z));
}

void EFFEKSEER_MANAGER::Set_Rotation(const std::string& handle_name, const XMFLOAT3& axis, const float angle)
{
	Manager->SetRotation(Handles[handle_name], Effekseer::Vector3D(axis.x, axis.y, axis.z), XMConvertToRadians(angle));
}

void EFFEKSEER_MANAGER::Set_Rotation(const std::string& handle_name, const Vector3& axis, const float angle)
{
	Manager->SetRotation(Handles[handle_name], Effekseer::Vector3D(axis.x, axis.y, axis.z), XMConvertToRadians(angle));
}

void EFFEKSEER_MANAGER::Set_Scale(const std::string& handle_name, const XMFLOAT3& scale)
{
	Manager->SetScale(Handles[handle_name], scale.x, scale.y, scale.z);
}

void EFFEKSEER_MANAGER::Set_Scale(const std::string& handle_name, const Vector3& scale)
{
	Manager->SetScale(Handles[handle_name], scale.x, scale.y, scale.z);
}

void EFFEKSEER_MANAGER::Set_Speed(const std::string& handle_name, const float speed)
{
	Manager->SetSpeed(Handles[handle_name], speed);
}


//const EFFECT& EFFEKSEER_MANAGER::Get_Effect(const std::string& name)
//{
//	return Handles[name];
//}