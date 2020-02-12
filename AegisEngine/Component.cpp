#include	"Component.h"

void COMPONENT::Set_Owner(const weak_ptr<GAME_OBJECT>& owner)
{
	Owner = owner;
}

void COMPONENT::SetEnable(const bool flag)
{
	Enable = flag;
}

const bool COMPONENT::GetEnable()
{
	return Enable;
}

void COMPONENT::SetDestroy()
{
	DestroyFlag = true;
}

void COMPONENT::Set_Draw_Enable(const bool flag)
{
	Draw_Enable = flag;
}

bool COMPONENT::Get_Draw_Enable()
{
	return Draw_Enable;
}

XMFLOAT3* const COMPONENT::Get_Position()
{
	return &Position;
};

XMFLOAT3* const COMPONENT::Get_Rotation()
{
	return &Rotation;
};

XMFLOAT3* const COMPONENT::Get_Scaling()
{
	return &Scaling;
};



void COMPONENT::Set_Position(XMFLOAT3* position)
{
	Position = *position;
}

void COMPONENT::Set_Position(XMFLOAT3& position)
{
	Position = position;
};

void COMPONENT::Set_Rotation(XMFLOAT3* rotation)
{
	Rotation = *rotation;
};

void COMPONENT::Set_Rotation(XMFLOAT3& rotation)
{
	Rotation = rotation;
};

void COMPONENT::Set_Scaling(XMFLOAT3* scaling)
{
	Scaling = *scaling;
};

void COMPONENT::Set_Scaling(XMFLOAT3& scaling)
{
	Scaling = scaling;
};

// ÉÅÉÇÉäè„Ç©ÇÁÇÃçÌèú
bool COMPONENT::Destroy()
{
	if (DestroyFlag)
	{
		return true;
	}
	return false;
}