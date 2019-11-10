#include	"Effekseer.h"
#include	"Renderer.h"
#include	"audio_clip.h"

::Effekseer::Manager*			EFFEKSEER_MANAGER::Manager = nullptr;
::EffekseerRenderer::Renderer*	EFFEKSEER_MANAGER::Renderer = nullptr;
::EffekseerSound::Sound*		EFFEKSEER_MANAGER::Sound = nullptr;

map<string, EFFECT>				EFFEKSEER_MANAGER::Effects;

bool EFFEKSEER_MANAGER::Init()
{
	// �`��p�C���X�^���X�̐���
	Renderer = ::EffekseerRendererDX11::Renderer::Create(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), 2000);
	if (nullptr == Renderer)
	{
		return false;
	}

	{
		// �G�t�F�N�g�Ǘ��p�C���X�^���X�̐���
		Manager = ::Effekseer::Manager::Create(2048);
		if(nullptr == Manager)
		{
			return false;
		}

		// �`��p�C���X�^���X����`��@�\��ݒ�
		Manager->SetSpriteRenderer(Renderer->CreateSpriteRenderer());
		Manager->SetRibbonRenderer(Renderer->CreateRibbonRenderer());
		Manager->SetRingRenderer(Renderer->CreateRingRenderer());
		Manager->SetTrackRenderer(Renderer->CreateTrackRenderer());
		Manager->SetModelRenderer(Renderer->CreateModelRenderer());

		// ���W�n�̎w��( LH�ō���n )
		Manager->SetCoordinateSystem(Effekseer::CoordinateSystem::RH);

		// �`��p�C���X�^���X����e�N�X�`���̓Ǎ��@�\��ݒ�
		// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
		Manager->SetTextureLoader(Renderer->CreateTextureLoader());
		Manager->SetModelLoader(Renderer->CreateModelLoader());
	}

	{
		// ���Đ��p�C���X�^���X�̐���
		Sound = ::EffekseerSound::Sound::Create(AUDIO_MANAGER::Get_Xaudio(), 16, 16);
		if (nullptr == Sound)
		{
			return false;
		}

		// ���Đ��p�C���X�^���X����Đ��@�\���w��
		Manager->SetSoundPlayer(Sound->CreateSoundPlayer());

		// ���Đ��p�C���X�^���X����T�E���h�f�[�^�̓Ǎ��@�\��ݒ�
		// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
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