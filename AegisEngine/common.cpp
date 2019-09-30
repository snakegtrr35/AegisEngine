#include	"common.h"
#include	<locale.h>

std::wstring stringTowstring(const std::string& font)
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

#ifdef UNICODE
void Erroer_Message(const std::wstring& str1, const std::wstring& str2)
{
	MessageBox(GetWindow(), str1.c_str(), str2.c_str(), MB_OK | MB_ICONWARNING);
}

void Erroer_Message(const std::string& str1, const std::string& str2)
{
	std::wstring str01, str02;

	str01 = stringTowstring(str1);
	str02 = stringTowstring(str2);

	MessageBox(GetWindow(), str01.c_str(), str02.c_str(), MB_OK | MB_ICONWARNING)
}
#else
void Erroer_Message(const std::string& str1, const std::string& str2)
{
	MessageBox(GetWindow(), str1.c_str(), str2.c_str(), MB_OK | MB_ICONWARNING);
}
#endif // !UNICODE