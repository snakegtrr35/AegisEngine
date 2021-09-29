#include	"Renderer.h"
#include	"Input.h"
#include	"Debug_Camera.h"
#include	"manager.h"

DEBUG_CAMERA* DEBUG_CAMERA::pDebugCamera = nullptr;
XMMATRIX DEBUG_CAMERA::m_ViewMatrix;
XMMATRIX DEBUG_CAMERA::m_ProjectionMatrix;

float DEBUG_CAMERA::Lenght = 15.0f;

DEBUG_CAMERA::DEBUG_CAMERA()
{
	XMFLOAT4 at = XMFLOAT4(0.f, 0.f, 0.f, 0.f);

	Viewing_Angle = 80.0f;

	Front = XMVectorSet(0.f, 0.f, 1.0f, 0.f);
	Front = XMVector3Normalize(Front);

	Up = XMVectorSet(0.f, 1.0f, 0.f, 0.f);

	Right = XMVectorSet(1.0f, 0.f, 0.f, 0.f);


	// 三つのベクトルを垂直にする
	Right = XMVector3Cross(Up, Front);
	Right = XMVector3Normalize(Right);

	Up = XMVector3Cross(Front, Right);
	Up = XMVector3Normalize(Up);

	At = XMLoadFloat4(&at);

	//Pos = At - Front * Lenght;
	Pos = At - (Front * Lenght - Up * 4.0f);

	// ビューポートの設定設定
	Viewport.left = 0;
	Viewport.top = 0;
	Viewport.right = SCREEN_WIDTH;
	Viewport.bottom = SCREEN_HEIGHT;

	Rotate = 90.0f;

	RotateEnable = MoveEnable = true;

	Rotation = XMFLOAT3(0, 0, 0);
}

void DEBUG_CAMERA::Init()
{
	XMFLOAT4 at = XMFLOAT4(0.f, 0.f, 0.f, 0.f);

	Viewing_Angle = 80.0f;

	Front = XMVectorSet(0.f, 0.f, 1.0f, 0.f);
	Front = XMVector3Normalize(Front);

	Up = XMVectorSet(0.f, 1.0f, 0.f, 0.f);

	Right = XMVectorSet(1.0f, 0.f, 0.f, 0.f);


	// 三つのベクトルを垂直にする
	Right = XMVector3Cross(Up, Front);
	Right = XMVector3Normalize(Right);

	Up = XMVector3Cross(Front, Right);
	Up = XMVector3Normalize(Up);

	At = XMLoadFloat4(&at);

	//Pos = At - Front * Lenght;
	Pos = At - (Front * Lenght - Up * 4.0f);

	// ビューポートの設定設定
	Viewport.left = 0;
	Viewport.top = 0;
	Viewport.right = SCREEN_WIDTH;
	Viewport.bottom = SCREEN_HEIGHT;

	Rotate = 90.0f;

	RotateEnable = MoveEnable = true;

	Rotation = XMFLOAT3(0, 0, 0);
}

void DEBUG_CAMERA::Uninit()
{
}

void DEBUG_CAMERA::Update(float delta_time)
{
	XMFLOAT2 point = MOUSE::Get_Mouse()->Get_Position();

	bool flag = KEYBOARD::Press_Keyboard(VK_SHIFT);

	bool flag2 = KEYBOARD::Press_Keyboard(VK_RBUTTON);

	if (flag)
	{
		//At = Front * Lenght + Pos;
		At = (Front * Lenght - Up * 4.0f) + Pos;
	}

	XMVECTOR f(Front);
	XMFLOAT4 front_vec;
	XMStoreFloat4(&front_vec, f);
	//front_vec.y = 0.0f;
	f = XMLoadFloat4(&front_vec);

	f = DirectX::XMVector3Normalize(f);

	XMVECTOR r(Right);
	XMFLOAT4 right_vec;
	XMStoreFloat4(&right_vec, r);
	r = XMLoadFloat4(&right_vec);

	r = DirectX::XMVector3Normalize(r);

	// 回転
	if (flag2 && MOUSE::Get_Mouse()->Get_Move_Flag())
	{
		XMMATRIX mtxRotation;

		mtxRotation = XMMatrixRotationAxis(Right, XMConvertToRadians(point.y * delta_time * 20.0f));

		Front = XMVector3TransformNormal(Front, mtxRotation);
		Front = XMVector3Normalize(Front);

		Up = XMVector3TransformNormal(Up, mtxRotation);
		Up = XMVector3Normalize(Up);

		Rotation.x += (point.y * delta_time * 20.0f);
	}

	if (flag2 && MOUSE::Get_Mouse()->Get_Move_Flag())
	{
		XMMATRIX mtxRotation;

		mtxRotation = XMMatrixRotationY(XMConvertToRadians(point.x * delta_time * 20.0f));


		Front = XMVector3TransformNormal(Front, mtxRotation);
		Front = XMVector3Normalize(Front);

		Up = XMVector3TransformNormal(Up, mtxRotation);
		Up = XMVector3Normalize(Up);

		Right = XMVector3TransformNormal(Right, mtxRotation);
		Right = XMVector3Normalize(Right);

		Rotation.y += (point.x * delta_time * 20.0f);
	}

	if (flag)
	{
		//Pos = At - Front * Lenght;
		Pos = At - (Front * Lenght - Up * 4.0f);
	}

	// 移動
	if (MoveEnable)
	{
		if (KEYBOARD::Press_Keyboard(VK_W))
		{
			Pos += f * delta_time * 15.0f;
		}

		if (KEYBOARD::Press_Keyboard(VK_S))
		{
			Pos -= f * delta_time * 15.0f;
		}

		if (KEYBOARD::Press_Keyboard(VK_D))
		{
			Pos += r * delta_time * 15.0f;
		}

		if (KEYBOARD::Press_Keyboard(VK_A))
		{
			Pos -= r * delta_time * 15.0f;
		}

		if ((MOUSE::Get_Mouse()->Get_Wheel_Move_Flag() == WHEEL_MOVE_ENUM::UP) && false == CManager::Get_Instance()->Get_Mouse_Over_ImGui())
		{
			Pos += f * delta_time * 150.0f;
		}

		if ((MOUSE::Get_Mouse()->Get_Wheel_Move_Flag() == WHEEL_MOVE_ENUM::DOWN) && false == CManager::Get_Instance()->Get_Mouse_Over_ImGui())
		{
			Pos -= f * delta_time * 150.0f;
		}
	}

	if (false == flag)
	{
		//XMVECTOR f = XMVector3Normalize(Front);
		//XMVECTOR u = XMVector3Normalize(Up);

		//At = Front * Lenght + Pos;
		At = (Front * Lenght - Up * 4.0f) + Pos;

		//Pos = At - Front * Lenght;
		Pos = At - (Front * Lenght - Up * 4.0f);
	}

	XMStoreFloat3(&Position, Pos);
	XMStoreFloat3(&A, At);

	XMStoreFloat3(&F, Front);
	XMStoreFloat3(&U, Up);
	XMStoreFloat3(&R, Right);
	
}

void DEBUG_CAMERA::Draw()
{
	// ビューポート設定
	D3D11_VIEWPORT dxViewport;
	dxViewport.Width = (float)(Viewport.right - Viewport.left);
	dxViewport.Height = (float)(Viewport.bottom - Viewport.top);
	dxViewport.MinDepth = 0.0f;
	dxViewport.MaxDepth = 1.0f;
	dxViewport.TopLeftX = (float)Viewport.left;
	dxViewport.TopLeftY = (float)Viewport.top;

	// ビューマトリクス設定
	m_ViewMatrix = XMMatrixLookAtLH(Pos, At, Up);

	// プロジェクションマトリクス設定
	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(Viewing_Angle), dxViewport.Width / dxViewport.Height, 0.001f, 1000.0f);
}

void DEBUG_CAMERA::Draw_DPP()
{
	Draw();
}