#include	"Renderer.h"
#include	"manager.h"
#include	"Scene.h"
#include	"Input.h"
#include	"camera.h"

using namespace aegis;

CCamera* CCamera::pCamera = nullptr;
XMMATRIX CCamera::m_ViewMatrix;
XMMATRIX CCamera::m_ProjectionMatrix;

float CCamera::Lenght_Z = 15.0f;
float CCamera::Lenght_Y = 4.0f;

void CCamera::Init()
{
	Vector4 at = Vector4(0.0f, 0.0f, 0.0f, 0.0f);

	Viewing_Angle = 55.0f;

	Front = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	Right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);


	// 三つのベクトルを垂直にする
	Right = XMVector3Cross(Up, Front);
	Right = XMVector3Normalize(Right);

	Up = XMVector3Cross(Front, Right);
	Up = XMVector3Normalize(Up);

	At = XMLoadFloat4(&at);
	{
		//Pos = At - Front * Lenght;

		XMVECTOR vec = At;

		vec -= Front * Lenght_Z;
		vec += Up * Lenght_Y;

		Pos = vec;
	}

	// ビューポート設定
	D3D11_VIEWPORT dxViewport;
	dxViewport.Width = (float)SCREEN_WIDTH;
	dxViewport.Height = (float)SCREEN_HEIGHT;
	dxViewport.MinDepth = 0.0f;
	dxViewport.MaxDepth = 1.0f;
	dxViewport.TopLeftX = 0.0f;
	dxViewport.TopLeftY = 0.0f;

	//CRenderer::GetDeviceContext()->RSSetViewports(1, &dxViewport);

	RotateEnable = MoveEnable = true;
}

void CCamera::Uninit()
{
}

void CCamera::Update(float delta_time)
{
	Vector2 point = MOUSE::Get_Mouse()->Get_Position();

	bool flag = KEYBOARD::Press_Keyboard(VK_RBUTTON);

	{
		auto player = CManager::Get_Instance()->Get_Scene()->Get_Game_Object("player");
		if (nullptr == player)
		{
			//At = XMLoadFloat3(player->Get_Position());

			//At = Front * Lenght + Pos;

			Pos = XMLoadFloat3(player->Get_Transform().Get_Position());
		}
	}

	XMVECTOR f(Front);
	Vector4 front_vec;
	XMStoreFloat4(&front_vec, f);
	//front_vec.y = 0.0f;
	f = XMLoadFloat4(&front_vec);

	f = DirectX::XMVector3Normalize(f);
		
	XMVECTOR r(Right);
	Vector4 right_vec;
	XMStoreFloat4(&right_vec, r);
	//right_vec.y = 0.0f;
	r = XMLoadFloat4(&right_vec);

	r = DirectX::XMVector3Normalize(r);

	// 回転
	{
		if (flag && MOUSE::Get_Mouse()->Get_Move_Y_Flag())
		{
			float angle;

			if (0 < point.y)
			{
				angle = 2.0f;
			}
			else
			{
				angle = -2.0f;
			}

			aegis::Quaternion q = aegis::quaternion::RotateAxis(aegis::Math::XMVECTORToVector3(Right), angle);

			aegis::Vector3 f = aegis::Vector3::Transform(aegis::Math::XMVECTORToVector3(Front), q);

			At = Pos + Front * Lenght_Z;//

			Front = aegis::Math::Vector3ToXMVECTOR(f);
			Front = XMVector3Normalize(Front);

			f = aegis::Vector3::Transform(aegis::Math::XMVECTORToVector3(Up), q);

			Up = aegis::Math::Vector3ToXMVECTOR(f);
			Up = XMVector3Normalize(Up);
		}

		if (flag && MOUSE::Get_Mouse()->Get_Move_X_Flag())
		{
			float angle;

			if (0 < point.x)
			{
				angle = 2.0f;
			}
			else
			{
				angle = -2.0f;
			}

			aegis::Quaternion q = aegis::quaternion::RotateAxis(aegis::Math::XMVECTORToVector3(Up), angle);

			aegis::Vector3 f = Vector3::Transform(aegis::Math::XMVECTORToVector3(Front), q);

			At = Pos + Front * Lenght_Z;//

			Front = aegis::Math::Vector3ToXMVECTOR(f);
			Front = XMVector3Normalize(Front);

			f = aegis::Vector3::Transform(aegis::Math::XMVECTORToVector3(Right), q);

			Right = aegis::Math::Vector3ToXMVECTOR(f);
			Right = XMVector3Normalize(Right);
		}

		{
			/*XMVECTOR vec = At;

			vec -= Front * Lenght_Z;
			vec += Up * Lenght_Y;

			Pos = vec;*/

			//At = Pos - Front * Lenght_Z;
			//At = Pos - Up * Lenght_Y;
		}
	}


	// 移動
	if (MoveEnable)
	{
		if (KEYBOARD::Press_Keyboard(VK_UP))
		{
			Pos += f * 0.5f;
		}

		if (KEYBOARD::Press_Keyboard(VK_DOWN))
		{
			Pos -= f * 0.5f;
		}

		if (KEYBOARD::Press_Keyboard(VK_RIGHT))
		{
			Pos += r * 0.5f;
		}

		if (KEYBOARD::Press_Keyboard(VK_LEFT))
		{
			Pos -= r * 0.5f;
		}

		if ((MOUSE::Get_Mouse()->Get_Wheel_Move_Flag() == WHEEL_MOVE_ENUM::UP) && false == CManager::Get_Instance()->Get_Mouse_Over_ImGui())
		{
			Pos += f * 2.0f;
		}

		if ((MOUSE::Get_Mouse()->Get_Wheel_Move_Flag() == WHEEL_MOVE_ENUM::DOWN) && false == CManager::Get_Instance()->Get_Mouse_Over_ImGui())
		{
			Pos -= f * 2.0f;
		}
	}

	Vector4 pos;
	XMStoreFloat4(&pos, Pos);

	XMStoreFloat3(Get_Transform().Get_Position(), Pos);
}

void CCamera::Draw()
{
	RECT rect;
	GetWindowRect(GetWindow(), &rect);

	// ビューポート設定
	D3D11_VIEWPORT dxViewport;
	dxViewport.Width = (float)(rect.right - rect.left);
	dxViewport.Height = (float)(rect.bottom - rect.top);
	dxViewport.MinDepth = 0.0f;
	dxViewport.MaxDepth = 1.0f;
	dxViewport.TopLeftX = (float)rect.left;
	dxViewport.TopLeftY = (float)rect.top;

	// ビューマトリクス設定
	m_ViewMatrix = XMMatrixLookAtLH(Pos, At, Up);

	// プロジェクションマトリクス設定
	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(Viewing_Angle), dxViewport.Width / dxViewport.Height, 0.001f, 1000.0f);
}

void CCamera::Draw_DPP()
{
	Draw();
}

bool CCamera::Get_Visibility(const Vector3& position)
{
	XMVECTOR world_pos, view_pos, projection_pos;
	Vector3 projection_pos_F;

	world_pos = XMLoadFloat3(&position);

	view_pos = XMVector3TransformCoord(world_pos, m_ViewMatrix);

	projection_pos = XMVector3TransformCoord(view_pos, m_ProjectionMatrix);

	XMStoreFloat3(&projection_pos_F, projection_pos);

	if (-1.0f <= projection_pos_F.x && projection_pos_F.x <= 1.0f &&
		-1.0f <= projection_pos_F.y && projection_pos_F.y <= 1.0f &&
		0.0f < projection_pos_F.z && projection_pos_F.z < 1.0f)
	{
		return true;
	}

	return false;
}