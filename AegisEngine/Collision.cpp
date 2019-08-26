#include	"Collision.h"
#include	<typeinfo>

bool Collision_HitCircle(const CIRCLE* pA, const CIRCLE* pB)
{
	float abr = pA->Radius + pB->Radius;
	float r = (pA->Position.x - pB->Position.x) * (pA->Position.x - pB->Position.x) + (pA->Position.y - pB->Position.y) * (pA->Position.y - pB->Position.y);

	return  r <= (abr * abr);
}

//bool Collision_HitCircle(const CIRCLE* pA, const CIRCLE* pB, short cnt)
//{
//	short i;
//	float abr, r;
//
//	for (i = 0; i < cnt; i++)
//	{
//		abr = pA->radius + (pB + i)->radius;
//		r = (pA->center_x - (pB + i)->center_x) * (pA->center_x - (pB + i)->center_x) + (pA->center_y - (pB + i)->center_y) * (pA->center_y - (pB + i)->center_y);
//
//		if (r <= (abr * abr)) return true;
//
//		//return r <= (abr * abr);
//	}
//
//	return false;
//}

bool Collision_HitSphere(COLLISION* const pA, COLLISION* const pB)
{
	if (typeid(COLLISIION_SPHERE*) == typeid(pA)) return false;
	if (typeid(COLLISIION_SPHERE*) == typeid(pB)) return false;

	SPHERE* sA = static_cast<COLLISIION_SPHERE*>(pA)->Get_Sphper();
	SPHERE* sB = static_cast<COLLISIION_SPHERE*>(pB)->Get_Sphper();

	float abr = sA->Radius + sB->Radius;
	float r = (sA->Position.x - sB->Position.x) * (sA->Position.x - sB->Position.x) + (sA->Position.y - sB->Position.y) * (sA->Position.y - sB->Position.y) + (sA->Position.z - sB->Position.z) * (sA->Position.z - sB->Position.z);

	return  r <= (abr * abr);
}


//bool Collision_HitSphere(const SPHERE* pA, const SPHERE* pB, short cnt)
//{
//
//}


bool Collision_HitAABB(COLLISION* const pA, COLLISION* const pB)
{
	if (typeid(COLLISIION_AABB*) == typeid(pA)) return false;
	if (typeid(COLLISIION_AABB*) == typeid(pB)) return false;

	AABB* aA = static_cast<COLLISIION_AABB*>(pA)->Get_AABB();
	AABB* aB = static_cast<COLLISIION_AABB*>(pB)->Get_AABB();

	if ( (aA->Position.x - aA->Radius.x) <= (aB->Position.x + aB->Radius.x) && (aB->Position.x - aB->Radius.x) <= (aA->Position.x + aA->Radius.x))
	{
		if ((aA->Position.y - aA->Radius.y) <= (aB->Position.y + aB->Radius.y) && (aB->Position.y - aB->Radius.y) <= (aA->Position.y + aA->Radius.y))
		{
			if ((aA->Position.z - aA->Radius.z) <= (aB->Position.z + aB->Radius.z) && (aB->Position.z - aB->Radius.z) <= (aA->Position.z + aA->Radius.z))
			{
				return true;
			}
		}
	}

	return false;
}
