#include	"common.h"
#include	<locale.h>

std::wstring stringTowstring(std::string& font)
{
	std::wstring f;
	wchar_t	wStrW[1024];

	size_t wLen = 0;
	errno_t err = 0;

	//ÉçÉPÅ[ÉãéwíË
	setlocale(LC_ALL, "japanese");

	err = mbstowcs_s(&wLen, wStrW, font.size() * 2, font.c_str(), _TRUNCATE);

	f = wStrW;

	return f;
}