﻿#include "Vector3.h"

namespace aegis
{
	const Vector3 Vector3::Front(0.f, 0.f, 1.0f);
	const Vector3 Vector3::Right(1.0f, 0.f, 0.f);
	const Vector3 Vector3::Back(0.f, 0.f, -1.0f);
	const Vector3 Vector3::Left(-1.0f, 0.f, 0.f);
	const Vector3 Vector3::Up(0.f, 1.0f, 0.f);
	const Vector3 Vector3::Down(0.f, -1.0f, 0.f);
	const Vector3 Vector3::Zero(0.f, 0.f, 0.f);
	const Vector3 Vector3::One(1.0f, 1.0f, 1.0f);
	const Vector3 Vector3::Minus(-1.0f, -1.0f, -1.0f);

	//Vector3 Vector3::Transform(const Vector3& v, const Quaternion& q)
	//{
	//	return toVector3(DirectX::XMVector3Rotate(toXMVECTOR(v), DirectX::XMLoadFloat4(&q.Quat)));
	//}
}