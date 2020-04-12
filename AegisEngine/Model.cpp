#include	"Model.h"
#include	"Model_Manager.h"

#include	"manager.h"
#include	"Scene.h"
#include	"camera.h"
#include	"Debug_Camera.h"

MODEL::MODEL() : FileName(string()), Key(0)
{
}

void MODEL::Init()
{
}

void MODEL::Draw()
{
	const auto camera = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");

	if (!camera.expired())
	{
		if (false == camera.lock()->Get_Visibility(Position))
		{
			return;
		}
	}

	XMMATRIX matrix = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
	matrix *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));
	matrix *= XMMatrixTranslation(Position.x, Position.y, Position.z);

	{
		auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
		auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

		{
			XMVECTOR camera_pos;
			LIGHT light = *CRenderer::Get_Light();

			XMVECTOR light_pos = XMVectorSet(light.Direction.x, light.Direction.y, light.Direction.z, light.Direction.w);

			light_pos = XMVectorScale(light_pos, 10.0f);

			if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
			{
				camera_pos = *camera01.lock()->Get_Pos();

				XMFLOAT4 pos;
				XMStoreFloat4(&pos, camera_pos);
				camera_pos = XMLoadFloat4(&pos);
			}
			else
			{
				camera_pos = *camera02.lock()->Get_Pos();

				XMFLOAT4 pos;
				XMStoreFloat4(&pos, camera_pos);
				camera_pos = XMLoadFloat4(&pos);
			}

			CRenderer::Set_MatrixBuffer01(camera_pos);
		}

		if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
		{
			CRenderer::Set_Shader(SHADER_INDEX_V::SHADOW_MAP, SHADER_INDEX_P::SHADOW_MAP);
		}
		else
		{
			CRenderer::Set_Shader();
		}
	}

	auto meshs = MODEL_MANEGER::Get_Instance()->Get_Mesh(Key);

	if (nullptr == meshs) return;

	// •’Ê‚Ì•`‰æ
	for (auto mesh : meshs->Get_Meshs())
	{
		for (auto& m : mesh.Get_Meshs())
		{
			m.Draw(matrix, meshs->Get_Textures());
		}
	}

	CRenderer::Set_Shader();
}

void MODEL::Draw_DPP()
{
}

void MODEL::Update(float delta_time)
{
}

void MODEL::Uninit()
{
}

void MODEL::Draw_Inspector()
{

}

void MODEL::Set_Model_Name(const string& file_name)
{
	if (file_name != FileName)
	{
		MODEL_MANEGER::Get_Instance()->Sub_ReferenceCnt(Key);
		FileName = file_name;
		Key = hash<string>()(file_name);
		MODEL_MANEGER::Get_Instance()->Add_ReferenceCnt(Key);
	}
}

const string& MODEL::Get_Model_Name()
{
	return FileName;
}