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

Effekseer::Matrix44 To(XMMATRIX& mtr);

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
		Manager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

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

	Effects["test"].Effect = Effekseer::Effect::Create(Manager, (const EFK_CHAR*)L"test.efk");

	Effects["test"].Handle = Manager->Play(Effects["test"].Effect, 0, 0, 0);
	Manager->SetRotation(Effects["test"].Handle, Effekseer::Vector3D(0, 1, 0), 45.0f);
	Manager->SetSpeed(Effects["test"].Handle, 0.1f);

	return true;
}

void EFFEKSEER_MANAGER::Uninit()
{
	// �G�t�F�N�g�̔j��
	for (auto effect : Effects)
	{
		// �G�t�F�N�g�̒�~
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

	// �G�t�F�N�g�̕`��J�n�������s���B
	Renderer->BeginRendering();

	// �G�t�F�N�g�̕`����s���B
	Manager->Draw();

	// �G�t�F�N�g�̕`��I���������s���B
	Renderer->EndRendering();
}

void EFFEKSEER_MANAGER::Updata()
{
	auto player = CManager::Get_Scene()->Get_Game_Object("player");

	Manager->SetLocation(Effects["test"].Handle, Effekseer::Vector3D(player->Get_Position()->x, player->Get_Position()->y, player->Get_Position()->z) );

	// �G�t�F�N�g�̍X�V�������s��
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
		static float angle;
		static ::Effekseer::Vector3D position;
		static ::Effekseer::Vector3D at;
		static ::Effekseer::Vector3D up;

		static bool flag = true;

		if (nullptr != camera)
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

		XMMATRIX proje;
		XMMATRIX view;

		if (nullptr != camera)
		{
			proje = camera->Get_Camera_Projection();
			view = camera->Get_Camera_View();
		}
		else
		{
			proje = D_camera->Get_Camera_Projection();
			view = D_camera->Get_Camera_View();
		}

		// ���e�s���ݒ�
		//Renderer->SetProjectionMatrix(
		//	::Effekseer::Matrix44().PerspectiveFovRH(XMConvertToRadians(angle), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 1000.0f));
		Renderer->SetProjectionMatrix(To(proje));

		// �J�����s���ݒ�
		//Renderer->SetCameraMatrix(
		//	::Effekseer::Matrix44().LookAtRH(position, at, up));
		Renderer->SetCameraMatrix(To(view));

		// 3D�T�E���h�p���X�i�[�ݒ�̍X�V
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



Effekseer::Matrix44 To(XMMATRIX& mtr)
{
	XMFLOAT4X4 m;
	XMStoreFloat4x4(&m, mtr);

	Effekseer::Matrix44 matrix;

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			matrix.Values[y][x] = m.m[y][x];
		}
	}

	return matrix;
}