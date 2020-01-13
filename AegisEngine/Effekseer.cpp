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

map<string, ::Effekseer::Effect*>	EFFEKSEER_MANAGER::Effects;
map<string, ::Effekseer::Handle>	EFFEKSEER_MANAGER::Handles;


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
		// �`��p�C���X�^���X�̐���
		Renderer = ::EffekseerRendererDX11::Renderer::Create(CRenderer::GetDevice(), CRenderer::GetDeviceContext(), 2000);
		if (nullptr == Renderer)
		{
			return false;
		}
	}

	{
		// �G�t�F�N�g�Ǘ��p�C���X�^���X�̐���
		Manager = ::Effekseer::Manager::Create(2048);
		if (nullptr == Manager)
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
		Manager->SetTextureLoader(Renderer->CreateTextureLoader());
		// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
		Manager->SetModelLoader(Renderer->CreateModelLoader());

		//// �J�����O���s���͈͂�ݒ�
		//// �͈͓��ɃG�t�F�N�g�����݂���ƃJ�����O�������Ɏ��s�����
		//// layerCount���傫���ق��������ɃJ�����O���s�����������������B�ő�6���x�B
		//Manager->CreateCullingWorld(500.0f, 500.0f, 500.0f, 5);

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

	Effects["test"] = Effekseer::Effect::Create(Manager, (const EFK_CHAR*)L"test.efk");

	//Effects["test"].Handle = Manager->Play(Effects["test"].Effect, 0, 0, 0);
	//Manager->SetRotation(Effects["test"].Handle, Effekseer::Vector3D(0, 1, 0), 45.0f);
	//Manager->SetSpeed(Effects["test"].Handle, 0.5f);

	return true;
}



void EFFEKSEER_MANAGER::Uninit()
{
	// �G�t�F�N�g�̒�~
	Manager->StopAllEffects();

	// �G�t�F�N�g�̔j��
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
	// �G�t�F�N�g�̕`��J�n�������s���B
	Renderer->BeginRendering();


	//// �����̓��ɑ��݂���G�t�F�N�g���v�Z����B
	//// �J�����O�̐ݒ肪�Ȃ��G�t�F�N�g�͏�ɕ`�悳���B
	//Manager->CalcCulling(Renderer->GetCameraProjectionMatrix(), false);

	// �G�t�F�N�g�̕`����s���B
	Manager->Draw();

	// �G�t�F�N�g�̕`��I���������s���B
	Renderer->EndRendering();
}

void EFFEKSEER_MANAGER::Update(float delta_time)
{
	auto player = CManager::Get_Scene()->Get_Game_Object("player");

	//	Set_Location("test", XMFLOAT3(player->Get_Position()->x, player->Get_Position()->y, player->Get_Position()->z));

	//Manager->SetLocation(Handles["test"], Effekseer::Vector3D(player->Get_Position()->x, player->Get_Position()->y, player->Get_Position()->z));
	Manager->SetScale(Handles["test"], 0.5f, 0.5f, 0.5f);
	Manager->SetRotation(Handles["test"], Effekseer::Vector3D(0, 1, 0), XMConvertToRadians(90.0f));

	Set();

	// �G�t�F�N�g�̍X�V�������s��
	Manager->Update();
}

void EFFEKSEER_MANAGER::Set()
{
	DEBUG_CAMERA* D_camera = nullptr;
	CCamera* camera = CManager::Get_Scene()->Get_Game_Object<CCamera>("camera");
	if (nullptr == camera)
	{
		D_camera = CManager::Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");
	}

	{
		XMFLOAT3* pos;
		static float angle;
		static ::Effekseer::Vector3D position;
		static ::Effekseer::Vector3D at;
		static ::Effekseer::Vector3D up;

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

		{
			XMMATRIX mtr;
			if (nullptr != camera)
			{
				mtr = camera->Get_Camera_Projection();
			}
			else
			{
				mtr = D_camera->Get_Camera_Projection();
			}

			::Effekseer::Matrix44 matrix = XMMATRIXToMatrix44(mtr);

			// ���e�s���ݒ�
			Renderer->SetProjectionMatrix(matrix);
		}

		{
			XMMATRIX mtr;

			if (nullptr != camera)
			{
				mtr = camera->Get_Camera_View();
			}
			else
			{
				mtr = D_camera->Get_Camera_View();
			}

			::Effekseer::Matrix44 matrix = XMMATRIXToMatrix44(mtr);

			// �J�����s���ݒ�
			Renderer->SetCameraMatrix(matrix);
		}

		// 3D�T�E���h�p���X�i�[�ݒ�̍X�V
		Sound->SetListener(position, at, up);
	}
}

void EFFEKSEER_MANAGER::Play(const string& name)
{
	Manager->StopEffect(Handles[name]);

	auto player = CManager::Get_Scene()->Get_Game_Object("player");

	Handles[name] = Manager->Play(Effects[name], player->Get_Position()->x, player->Get_Position()->y, player->Get_Position()->z);

	//Effects[name].Handle = Manager->Play(Effects[name].Effect, 0, 0, 0);
}

void EFFEKSEER_MANAGER::Play(const string& handle_name, const string& effect_name, const XMFLOAT3& position)
{
	// ���[�h���Ă��Ȃ��G�t�F�N�g�̔���
#ifdef _DEBUG
	if (Effects.find(effect_name) == Effects.end())
	{
		string text("���݂��Ȃ��G�t�F�N�g�ł�\n");
		string t(effect_name.c_str());
		text += t;

		Erroer_Message(text);

		return;
	}
#endif // _DEBUG

	Manager->StopEffect(Handles[handle_name]);

	Handles[handle_name] = Manager->Play(Effects[effect_name], position.x, position.y, position.z);
}



void EFFEKSEER_MANAGER::Play(const string& handle_name, const string& effect_name, const Math::VECTOR3& position)
{
	// ���[�h���Ă��Ȃ��G�t�F�N�g�̔���
#ifdef _DEBUG
	if (Effects.find(effect_name) == Effects.end())
	{
		string text("���݂��Ȃ��G�t�F�N�g�ł�\n");
		string t(effect_name.c_str());
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

const map<string, ::Effekseer::Effect*>& EFFEKSEER_MANAGER::Get_Effects()
{
	return Effects;
}

void EFFEKSEER_MANAGER::Set_Location(const string& handle_name, const XMFLOAT3& position)
{
	Manager->SetLocation(Handles[handle_name], Effekseer::Vector3D(position.x, position.y, position.z));
}

void EFFEKSEER_MANAGER::Set_Location(const string& handle_name, const Math::VECTOR3& position)
{
	Manager->SetLocation(Handles[handle_name], Effekseer::Vector3D(position.x, position.y, position.z));
}

void EFFEKSEER_MANAGER::Set_Rotation(const string& handle_name, const XMFLOAT3& axis, const float angle)
{
	Manager->SetRotation(Handles[handle_name], Effekseer::Vector3D(axis.x, axis.y, axis.z), XMConvertToRadians(angle));
}

void EFFEKSEER_MANAGER::Set_Rotation(const string& handle_name, const Math::VECTOR3& axis, const float angle)
{
	Manager->SetRotation(Handles[handle_name], Effekseer::Vector3D(axis.x, axis.y, axis.z), XMConvertToRadians(angle));
}

void EFFEKSEER_MANAGER::Set_Scale(const string& handle_name, const XMFLOAT3& scale)
{
	Manager->SetScale(Handles[handle_name], scale.x, scale.y, scale.z);
}

void EFFEKSEER_MANAGER::Set_Scale(const string& handle_name, const Math::VECTOR3& scale)
{
	Manager->SetScale(Handles[handle_name], scale.x, scale.y, scale.z);
}

void EFFEKSEER_MANAGER::Set_Speed(const string& handle_name, const float speed)
{
	Manager->SetSpeed(Handles[handle_name], speed);
}


//const EFFECT& EFFEKSEER_MANAGER::Get_Effect(const string& name)
//{
//	return Effects[name];
//}