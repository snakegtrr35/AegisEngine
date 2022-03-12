#include	"Model.h"
#include	"Model_Manager.h"

#include	"manager.h"
#include	"Scene.h"
#include	"camera.h"
#include	"Debug_Camera.h"

using namespace aegis;

MODEL::MODEL() : FileName(std::string()), Key(0)
{
}

void MODEL::Init()
{
}

void MODEL::Draw()
{
	CRenderer* render = CRenderer::getInstance();

	const auto camera = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");

	if (!camera.expired())
	{
		if (false == camera.lock()->Get_Visibility(Position))
		{
			return;
		}
	}

	const auto pos = *(Owner.lock()->Get_Transform().Get_Position());
	const auto rota = *(Owner.lock()->Get_Transform().Get_Rotation());
	const auto scale = *(Owner.lock()->Get_Transform().Get_Scaling());

	XMMATRIX matrix = XMMatrixScaling(scale.x, scale.y, scale.z);
	matrix *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rota.x), XMConvertToRadians(rota.y), XMConvertToRadians(rota.z));
	matrix *= XMMatrixTranslation(pos.x, pos.y, pos.z);

	{
		auto camera01 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<CCamera>("camera");
		auto camera02 = CManager::Get_Instance()->Get_Scene()->Get_Game_Object<DEBUG_CAMERA>("camera");

		{
			XMVECTOR camera_pos;
			LIGHT light = *render->Get_Light();

			XMVECTOR light_pos = XMVectorSet(light.Direction.x, light.Direction.y, light.Direction.z, light.Direction.w);

			light_pos = XMVectorScale(light_pos, 10.0f);

			if (!camera01.expired() && Empty_weak_ptr<CCamera>(camera01))
			{
				camera_pos = *camera01.lock()->Get_Pos();

				Vector4 pos;
				XMStoreFloat4(&pos, camera_pos);
				camera_pos = XMLoadFloat4(&pos);
			}
			else
			{
				camera_pos = *camera02.lock()->Get_Pos();

				Vector4 pos;
				XMStoreFloat4(&pos, camera_pos);
				camera_pos = XMLoadFloat4(&pos);
			}

			render->Set_MatrixBuffer01(camera_pos);
		}

		if (CManager::Get_Instance()->Get_ShadowMap()->Get_Enable())
		{
			render->Set_Shader(SHADER_INDEX_V::SHADOW_MAP, SHADER_INDEX_P::SHADOW_MAP);
		}
		else
		{
			render->Set_Shader();
		}
	}

	auto meshs = MODEL_MANEGER::Get_Instance()->Get_Mesh(Key);

	if (nullptr == meshs) return;

	// 普通の描画
	for (auto& mesh : meshs->Get_Meshs())
	{
		//for (auto& m : mesh.Get_Meshs())
		{
			mesh.Draw(matrix, meshs->Get_Textures());
		}
	}

	render->Set_Shader();
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

void MODEL::Set_Model_Name(const std::string& file_name)
{
	if (file_name != FileName)
	{
		MODEL_MANEGER::Get_Instance()->Sub_ReferenceCnt(Key);
		FileName = file_name;
		Key = std::hash<std::string>()(file_name);
		MODEL_MANEGER::Get_Instance()->Add_ReferenceCnt(Key);
	}
}

const std::string& MODEL::Get_Model_Name()
{
	return FileName;
}

#include	"imgui/imgui.h"

void MODEL::Draw_Inspector()
{
	auto str = (char*)u8"メッシュ(MODEL)";

	ImGui::Text(str);

	COMPONENT::Draw_Inspector();

	float position[3] = { Position.x, Position.y, Position.z };
	float rotate[3] = { Rotation.x, Rotation.y, Rotation.z };
	float scale[3] = { Scaling.x, Scaling.y, Scaling.z };

	ImGui::DragFloat3("Position##MODEL", position, 0.01f);
	ImGui::DragFloat3("Radius##MODEL", rotate, 0.01f);
	ImGui::DragFloat3("Scaling##MODEL", scale, 0.01f);

	Position = Vector3(position[0], position[1], position[2]);
	Rotation = Vector3(rotate[0], rotate[1], rotate[2]);
	Scaling = Vector3(scale[0], scale[1], scale[2]);
}