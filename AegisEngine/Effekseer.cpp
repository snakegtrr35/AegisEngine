#include	"Effekseer.h"
#include	"Renderer.h"
#include	"audio_clip.h"

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

	return true;
}

bool EFFEKSEER_MANAGER::Uninit()
{
	if (nullptr != Sound) Sound->Destroy();
	if (nullptr != Renderer) Renderer->Destroy();
	if (nullptr != Manager) Manager->Destroy();

	return true;
}