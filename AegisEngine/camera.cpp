#include	"Renderer.h"
#include	"manager.h"
#include	"Scene.h"
#include	"Input.h"
#include	"camera.h"

CCamera* CCamera::pCamera = nullptr;
XMMATRIX CCamera::m_ViewMatrix;

float CCamera::Lenght = 10.0f;

void CCamera::Init()
{
	XMFLOAT4 at = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	Viewing_Angle = 55.0f;

	Lenght = 5.0f;

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

void CCamera::Uninit()
{
}

void CCamera::Update()
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
	//if (KEYBOARD::Press_Keyboard(VK_K))
	if (flag2 && MOUSE::Get_Move_Flag())
	{
		XMMATRIX mtxRotation;

		//mtxRotation = XMMatrixRotationAxis(Right, XMConvertToRadians(-1.0f));
		mtxRotation = XMMatrixRotationAxis(Right, XMConvertToRadians(point.y));

		Front = XMVector3TransformNormal(Front, mtxRotation);
		Front = XMVector3Normalize(Front);

		Up = XMVector3TransformNormal(Up, mtxRotation);
		Up = XMVector3Normalize(Up);
	}

	/*//if (KEYBOARD::Press_Keyboard(VK_I))
	if (flag2 && MOUSE::Get_Move_Flag() && point.y > 0)
	{
		XMMATRIX mtxRotation;

		//mtxRotation = XMMatrixRotationAxis(Right, XMConvertToRadians(1.0f));
		mtxRotation = XMMatrixRotationAxis(Right, XMConvertToRadians(point.y));

		Front = XMVector3TransformNormal(Front, mtxRotation);
		Front = XMVector3Normalize(Front);

		Up = XMVector3TransformNormal(Up, mtxRotation);
		Up = XMVector3Normalize(Up);
	}*/

	//if (KEYBOARD::Press_Keyboard(VK_L))
	if (flag2 && MOUSE::Get_Move_Flag())
	{
		XMMATRIX mtxRotation;

		mtxRotation = XMMatrixRotationY(XMConvertToRadians(point.x));
		//mtxRotation = XMMatrixRotationAxis(Right, XMConvertToRadians(point.x));
		//mtxRotation = XMMatrixRotationAxis(Up, XMConvertToRadians(point.x));//


		Front = XMVector3TransformNormal(Front, mtxRotation);
		Front = XMVector3Normalize(Front);

		Up = XMVector3TransformNormal(Up, mtxRotation);
		Up = XMVector3Normalize(Up);

		Right = XMVector3TransformNormal(Right, mtxRotation);
		Right = XMVector3Normalize(Right);
	}

	/*//if (KEYBOARD::Press_Keyboard(VK_J))
	if (flag2 && MOUSE::Get_Move_Flag() && point.x < 0)
	{
		XMMATRIX mtxRotation;

		mtxRotation = XMMatrixRotationY(XMConvertToRadians(point.x));
		//mtxRotation = XMMatrixRotationAxis(Right, XMConvertToRadians(point.x));
		//mtxRotation = XMMatrixRotationAxis(Up, XMConvertToRadians(point.x));//

		Front = XMVector3TransformNormal(Front, mtxRotation);
		Front = XMVector3Normalize(Front);

		Up = XMVector3TransformNormal(Up, mtxRotation);
		Up = XMVector3Normalize(Up);

		Right = XMVector3TransformNormal(Right, mtxRotation);
		Right = XMVector3Normalize(Right);
	}*/
	
	{
		XMFLOAT3 position;

		XMStoreFloat3(&position, At);

		//position.x = std::clamp(position.x, -50.0f, 50.0f);
		//position.z = std::clamp(position.z, -50.0f, 50.0f);

		At = XMLoadFloat3(&position);
	}

	if (flag)
		Pos = At - Front * Lenght;

	// 移動
	if (MoveEnable)
	{
		if (KEYBOARD::Press_Keyboard(VK_W) || (MOUSE::Get_Wheel_Move_Flag() == WHEEL_MOVE_ENUM::UP) )
		{
			Pos += f * 0.2f;
		}

		if (KEYBOARD::Press_Keyboard(VK_S) || (MOUSE::Get_Wheel_Move_Flag() == WHEEL_MOVE_ENUM::DOWN))
		{
			Pos -= f * 0.2f;
		}

		if (KEYBOARD::Press_Keyboard(VK_D))
		{
			Pos += r * 0.2f;
		}

		if (KEYBOARD::Press_Keyboard(VK_A))
		{
			Pos -= r * 0.2f;
		}
	}

	if (false == flag)
	{
		At = Front * Lenght + Pos;
		Pos = At - Front * Lenght;
	}
}

void CCamera::Draw()
{
	//XMMATRIX m_InvViewMatrix;
	XMMATRIX m_ProjectionMatrix;

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

	//// ビューマトリクス設定
	//m_InvViewMatrix = XMMatrixRotationRollPitchYaw(Rotation.x, Rotation.y, Rotation.z);
	//m_InvViewMatrix *= XMMatrixTranslation(Position.x, Position.y, Position.z);

	/*XMVECTOR det;
	m_ViewMatrix = XMMatrixInverse(&det, m_InvViewMatrix);*/

	// プロジェクションマトリクス設定
	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(Viewing_Angle), dxViewport.Width / dxViewport.Height, 1.0f, 1000.0f);
	CRenderer::SetProjectionMatrix(&m_ProjectionMatrix);
}

CCamera* const CCamera::Get_Camera(void)
{
	return pCamera;
}

const XMMATRIX CCamera::Get_Camera_View(void)
{
	return m_ViewMatrix;
}