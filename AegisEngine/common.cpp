#include	"common.h"
#include	<locale.h>

std::wstring stringTowstring(const std::string_view font)
{
	//ロケール指定
	setlocale(LC_ALL, "japanese");

	size_t length = font.size();

	std::unique_ptr<wchar_t[]>  StrW = std::make_unique<wchar_t[]>(length * 2);

	size_t wLen = 0;

	mbstowcs_s(&wLen, StrW.get(), length * 2, font.data(), _TRUNCATE);

	std::wstring s(StrW.get());

	StrW.reset(nullptr);

	return s;
}

std::string wstringTostring(const std::wstring_view font)
{
	//ロケール指定
	setlocale(LC_ALL, "japanese");

	size_t length = font.size();

	std::unique_ptr<char[]>  Str = std::make_unique<char[]>(length * 2);

	size_t Len = 0;

	wcstombs_s(&Len, Str.get(), length * 2, font.data(), length * 2);

	std::string s(Str.get());

	Str.reset(nullptr);

	return s;
}

//auto notdigit = not1(std::function<bool(char)>(bind(std::isdigit<char>, std::placeholders::_1, std::locale())));

auto notdigit = [](char s)
{
	return (0 == std::isdigit<unsigned char>(static_cast<unsigned char>(s), std::locale()));
};

void ExtratNum(std::string& str)
{
	str.erase(std::remove_if(str.begin(), str.end(), notdigit), str.end());
}

#ifdef UNICODE
void Erroer_Message(const std::wstring_view str1, const std::wstring_view str2)
{
	MessageBox(GetWindow(), str1.data(), str2.data(), MB_OK | MB_ICONWARNING);
}

void Erroer_Message(const std::string_view str1, const std::string_view str2)
{
	std::wstring str01, str02;

	str01 = stringTowstring(str1);
	str02 = stringTowstring(str2);

	MessageBox(GetWindow(), str01.c_str(), str02.c_str(), MB_OK | MB_ICONWARNING);
}
#else
void Erroer_Message(const std::string_view str1, const std::string_view str2)
{
	MessageBox(GetWindow(), str1.data(), str2.data(), MB_OK | MB_ICONWARNING);
}
#endif // !UNICODE

std::string Replace_String(const std::string_view replacedStr, const std::string_view from, const std::string_view to)
{
	if (replacedStr.empty() || from.empty()/* || to.empty()*/)
	{
		return std::string(replacedStr);
	}

	std::string replaceStr = std::string(replacedStr);
	std::string::size_type pos = 0;
	while (std::string::npos != (pos = replaceStr.find(from, pos)))
	{
		replaceStr.replace(pos, from.length(), to);
		pos += to.length();
	}

	return replaceStr;
}