#include	"Renderer.h"
#include	"Input.h"
#include	"Debug_Camera.h"

DEBUG_CAMERA* DEBUG_CAMERA::pDebugCamera = nullptr;
XMMATRIX DEBUG_CAMERA::m_ViewMatrix;
XMMATRIX DEBUG_CAMERA::m_ProjectionMatrix;

float DEBUG_CAMERA::Lenght = 15.0f;


void DEBUG_CAMERA::Init()
{
	XMFLOAT4 at = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	Viewing_Angle = 110.0f;

	Front = XMVectorSet(0.0f, -0.4f, 1.0f, 0.0f);
	Front = XMVector3Normalize(Front);

	Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	Right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);


	// 三つのベクトルを垂直にする
	Right = XMVector3Cross(Up, Front);
	Right = XMVector3Normalize(Right);

	Up = XMVector3Cross(Front, Right);
	Up = XMVector3Normalize(Up);

	At = XMLoadFloat4(&at);

	Pos = At - Front * Lenght;

	// ビューポートの設定設定
	Viewport.left = 0;
	Viewport.top = 0;
	Viewport.right = SCREEN_WIDTH;
	Viewport.bottom = SCREEN_HEIGHT;

	Rotate = 90.0f;

	RotateEnable = MoveEnable = true;
}

void DEBUG_CAMERA::Uninit()
{
}

void DEBUG_CAMERA::Update(float delta_time)
{
	XMFLOAT2 point = MOUSE::Get_Position();

	bool flag = KEYBOARD::Press_Keyboard(VK_SHIFT);

	bool flag2 = KEYBOARD::Press_Keyboard(VK_RBUTTON);

	if (flag)
		At = Front * Lenght + Pos;

	XMVECTOR f(Front);
	XMFLOAT4 front_vec;
	XMStoreFloat4(&front_vec, f);
	//front_vec.y = 0.0f;
	f = XMLoadFloat4(&front_vec);

	f = DirectX::XMVector3Normalize(f);

	XMVECTOR r(Right);
	XMFLOAT4 right_vec;
	XMStoreFloat4(&right_vec, r);
	//right_vec.y = 0.0f;
	r = XMLoadFloat4(&right_vec);

	r = DirectX::XMVector3Normalize(r);

	// 回転
	if (flag2 && MOUSE::Get_Move_Flag())
	{
		XMMATRIX mtxRotation;

		mtxRotation = XMMatrixRotationAxis(Right, XMConvertToRadians(point.y * delta_time * 20.0));

		Front = XMVector3TransformNormal(Front, mtxRotation);
		Front = XMVector3Normalize(Front);

		Up = XMVector3TransformNormal(Up, mtxRotation);
		Up = XMVector3Normalize(Up);
	}

	if (flag2 && MOUSE::Get_Move_Flag())
	{
		XMMATRIX mtxRotation;

		mtxRotation = XMMatrixRotationY(XMConvertToRadians(point.x * delta_time * 20.0));


		Front = XMVector3TransformNormal(Front, mtxRotation);
		Front = XMVector3Normalize(Front);

		Up = XMVector3TransformNormal(Up, mtxRotation);
		Up = XMVector3Normalize(Up);

		Right = XMVector3TransformNormal(Right, mtxRotation);
		Right = XMVector3Normalize(Right);
	}

	if (flag)
		Pos = At - Front * Lenght;

	// 移動
	if (MoveEnable)
	{
		if (KEYBOARD::Press_Keyboard(VK_UP))
		{
			Pos += f * delta_time * 15.0f;
		}

		if (KEYBOARD::Press_Keyboard(VK_DOWN))
		{
			Pos -= f * delta_time * 15.0f;
		}

		if (KEYBOARD::Press_Keyboard(VK_RIGHT))
		{
			Pos += r * delta_time * 15.0f;
		}

		if (KEYBOARD::Press_Keyboard(VK_LEFT))
		{
			Pos -= r * delta_time * 15.0f;
		}

		if ((MOUSE::Get_Wheel_Move_Flag() == WHEEL_MOVE_ENUM::UP))
		{
			Pos += f * delta_time * 150.0f;
		}

		if ((MOUSE::Get_Wheel_Move_Flag() == WHEEL_MOVE_ENUM::DOWN))
		{
			Pos -= f * delta_time * 150.0f;
		}
	}

	if (false == flag)
	{
		At = Front * Lenght + Pos;
		Pos = At - Front * Lenght;
	}

	XMFLOAT4 pos;
	XMStoreFloat4(&pos, Pos);

	CRenderer::SetCamera(&pos);

	XMStoreFloat3(&Position, Pos);
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

	CRenderer::GetDeviceContext()->RSSetViewports(1, &dxViewport);

	// ビューマトリクス設定
	m_ViewMatrix = XMMatrixLookAtLH(Pos, At, Up);
	CRenderer::SetViewMatrix(&m_ViewMatrix);

	// プロジェクションマトリクス設定
	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(Viewing_Angle), dxViewport.Width / dxViewport.Height, 1.0f, 1000.0f);
	CRenderer::SetProjectionMatrix(&m_ProjectionMatrix);
}

DEBUG_CAMERA* const DEBUG_CAMERA::Get_Camera(void)
{
	return pDebugCamera;
}

const XMMATRIX DEBUG_CAMERA::Get_Camera_View(void)
{
	return m_ViewMatrix;
}

const XMMATRIX DEBUG_CAMERA::Get_Camera_Projection()
{
	return m_ProjectionMatrix;
}