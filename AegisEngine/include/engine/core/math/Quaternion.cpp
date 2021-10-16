#include "Quaternion.h"
#include "Vector3.h"
#include "MathDef.h"

namespace Aegis
{
	const Quaternion Identity(0.0f, 0.0f, 0.0f, 1.0f);

	Quaternion::Quaternion(float32 _x, float32 _y, float32 _z, float32 _w) noexcept
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	Quaternion Quaternion::Lerp(const Quaternion& a, const Quaternion& b, float32 t)
	{
		Quaternion retVal;
		retVal.x = Math::Lerp(a.x, b.x, t);
		retVal.y = Math::Lerp(a.y, b.y, t);
		retVal.z = Math::Lerp(a.z, b.z, t);
		retVal.w = Math::Lerp(a.w, b.w, t);
		retVal.Normalize();
		return retVal;
	}

	Quaternion Quaternion::Concatenate(const Quaternion& q, const Quaternion& p)
	{
		Quaternion retVal;

		// ベクトル成分はps * qv + qs * pv + pv x qv
		Vector3 qv(q.x, q.y, q.z);
		Vector3 pv(p.x, p.y, p.z);
		Vector3 newVec = p.w * qv + q.w * pv + Vector3::Cross(pv, qv);
		retVal.x = newVec.x;
		retVal.y = newVec.y;
		retVal.z = newVec.z;

		// スカラーコンポーネントは次のとおりです。
		// ps * qs - pv。 qv
		retVal.w = p.w * q.w - Vector3::Dot(pv, qv);

		return retVal;
	}
}