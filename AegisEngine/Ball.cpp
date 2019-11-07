#include	"Ball.h"

BALL::BALL() : Quaternion(XMQuaternionIdentity())
{
}

void BALL::Init(void)
{
}

void BALL::Draw(void)
{

}

void BALL::Update(void)
{
	// ÉLÅ[ì¸óÕ
	{
		XMFLOAT3 vz = XMFLOAT3(0.f, 0.f, 1.0f);
		XMVECTOR axis = XMLoadFloat3(&vz);

		XMVECTOR rotation = XMQuaternionRotationAxis(axis, 0.2f);

		Quaternion = XMQuaternionMultiply(Quaternion, rotation);
		Quaternion = XMQuaternionNormalize(Quaternion);

		XMMATRIX rotate = XMMatrixRotationQuaternion(Quaternion);
	}
}

void BALL::Uninit(void)
{
	pVertexBuffer.reset(nullptr);
	pIndexBuffer.reset(nullptr);
}