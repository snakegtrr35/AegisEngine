#include	"Effekseer.h"
#include	"main.h"
#include	"Renderer.h"
#include	"manager.h"
#include	"Scene.h"
#include	"audio_clip.h"

#include	"camera.h"
#include	"Debug_Camera.h"

::Effekseer::Manager*			EFFEKSEER_MANAGER::Manager = nullptr;
::EffekseerRenderer::Renderer*	EFFEKSEER_MANAGER::Renderer = nullptr;
::EffekseerSound::Sound*		EFFEKSEER_MANAGER::Sound = nullptr;

map<string, EFFECT>				EFFEKSEER_MANAGER::Effects;


bool EFFEKSEER_MANAGER::Init()
{
	// 描画用インスタンスの生成
	Renderer = ::EffekseerRendererDX11::Renderer::Create(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), 2000);
	if (nullptr == Renderer)
	{
		return false;
	}

	{
		// エフェクト管理用インスタンスの生成
		Manager = ::Effekseer::Manager::Create(2048);
		if(nullptr == Manager)
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
		Manager->SetCoordinateSystem(Effekseer::CoordinateSystem::RH);

		// 描画用インスタンスからテクスチャの読込機能を設定
		// 独自拡張可能、現在はファイルから読み込んでいる。
		Manager->SetTextureLoader(Renderer->CreateTextureLoader());
		Manager->SetModelLoader(Renderer->CreateModelLoader());
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

	Effects["test"].Effect = Effekseer::Effect::Create(Manager, (const EFK_CHAR*)L"test.efk");

	Effects["test"].Handle = Manager->Play(Effects["test"].Effect, 0, 0, 0);
	Manager->SetRotation(Effects["test"].Handle, Effekseer::Vector3D(0, 1, 0), 45.0f);
	//Manager->SetSpeed(Effects["test"].Handle, 0.8f);

	return true;
}

void EFFEKSEER_MANAGER::Uninit()
{
	// エフェクトの破棄
	for (auto effect : Effects)
	{
		// エフェクトの停止
		Manager->StopEffect(effect.second.Handle);

		if (nullptr != effect.second.Effect)
		{
			effect.second.Effect->Release();
			effect.second.Effect = nullptr;
		}
	}
	Effects.clear();

	if (nullptr != Manager) Manager->Destroy(); Manager = nullptr;
	if (nullptr != Sound) Sound->Destroy(); Sound = nullptr;
	if (nullptr != Renderer) Renderer->Destroy(); Renderer = nullptr;
}

void EFFEKSEER_MANAGER::Draw()
{
	Set();

	// エフェクトの描画開始処理を行う。
	Renderer->BeginRendering();

	// エフェクトの描画を行う。
	Manager->Draw();

	// エフェクトの描画終了処理を行う。
	Renderer->EndRendering();
}

void EFFEKSEER_MANAGER::Updata()
{
	// エフェクトの更新処理を行う
	Manager->Update();
}

void EFFEKSEER_MANAGER::Set()
{
	DEBUG_CAMERA* D_camera = nullptr;
	CCamera* camera = CManager::Get_Scene()->Get_Game_Object<CCamera>();
	if (nullptr == camera)
	{
		D_camera = CManager::Get_Scene()->Get_Game_Object<DEBUG_CAMERA>();
	}

	{
		XMFLOAT3* pos;
		float angle;
		::Effekseer::Vector3D position;
		::Effekseer::Vector3D at;
		::Effekseer::Vector3D up;
		if(nullptr != camera)
		{
			position.X = camera->Get_Position()->x;
			position.Y = camera->Get_Position()->y;
			position.Z = camera->Get_Position()->z;

			XMFLOAT3 vec;
			XMStoreFloat3(&vec, *camera->Get_At());

			at.X = vec.x;
			at.Y = vec.y;
			at.Z = vec.z;

			XMStoreFloat3(&vec, *camera->Get_Up());

			up.X = vec.x;
			up.Y = vec.y;
			up.Z = vec.z;

			angle = camera->Get_Viewing_Angle();
		}
		else
		{
			position.X = D_camera->Get_Position()->x;
			position.Y = D_camera->Get_Position()->y;
			position.Z = D_camera->Get_Position()->z;

			XMFLOAT3 vec;
			XMStoreFloat3(&vec, *D_camera->Get_At());

			at.X = vec.x;
			at.Y = vec.y;
			at.Z = vec.z;

			XMStoreFloat3(&vec, *D_camera->Get_Up());

			up.X = vec.x;
			up.Y = vec.y;
			up.Z = vec.z;

			angle = D_camera->Get_Viewing_Angle();
		}

		// 投影行列を設定
		Renderer->SetProjectionMatrix(
			::Effekseer::Matrix44().PerspectiveFovRH(XMConvertToRadians(angle), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 1000.0f));

		// カメラ行列を設定
		Renderer->SetCameraMatrix(
			::Effekseer::Matrix44().LookAtRH(position, at, up));

		// 3Dサウンド用リスナー設定の更新
		Sound->SetListener(position, at, up);
	}
}

void EFFEKSEER_MANAGER::Play(const string& name)
{
	Effects[name].Handle = Manager->Play(Effects[name].Effect, 0, 0, 0);
}

::Effekseer::Manager* const EFFEKSEER_MANAGER::Get_Manager()
{
	return Manager;
}

const map<string, EFFECT>& EFFEKSEER_MANAGER::Get_Effects()
{
	return Effects;
}

const EFFECT& EFFEKSEER_MANAGER::Get_Effect(const string& name)
{
	return Effects[name];
}