#include	"Keyboard.h"

BYTE KEYBOARD::KeyTable[256];
BYTE KEYBOARD::Old_Key_Table[256];

void KEYBOARD::Init(void)
{
	memset(KeyTable, 0, 256);
	memset(Old_Key_Table, 0, 256);
}

void KEYBOARD::Update(void)
{
	memcpy(Old_Key_Table, KeyTable, 256);

	GetKeyboardState(KeyTable);
}

void KEYBOARD::Uninit(void)
{
}

bool KEYBOARD::Press_Keyboard(const BYTE key)
{
	return (KeyTable[key] & 0x80);
}

bool KEYBOARD::Trigger_Keyboard(BYTE key)
{
	return ( (KeyTable[key] & 0x80) && !(Old_Key_Table[key] & 0x80) );
}

bool KEYBOARD::Release_Keyboard(BYTE key)
{
	return ( !(KeyTable[key] & 0x80) && Old_Key_Table[key] & 0x80);
}

/*RiDetector<RiListener> KEYBOARD::g_KeyboardDetector;
RiKeyboardState KEYBOARD::g_KeyboardState[1];

void KEYBOARD::Init(void)
{
}

void KEYBOARD::Update(void)
{
}

void KEYBOARD::Uninit(void)
{
}*/