#include	"common.h"
#include	<locale.h>

std::wstring stringTowstring(const std::string& font)
{
	//ロケール指定
	setlocale(LC_ALL, "japanese");

	size_t length = font.size();

	unique_ptr<wchar_t[]>  StrW = make_unique<wchar_t[]>(length * 2);

	size_t wLen = 0;

	mbstowcs_s(&wLen, StrW.get(), length * 2, font.c_str(), _TRUNCATE);

	wstring s(StrW.get());

	StrW.reset(nullptr);

	return s;
}

std::string wstringTostring(const std::wstring& font)
{
	//ロケール指定
	setlocale(LC_ALL, "japanese");

	size_t length = font.size();

	unique_ptr<char[]>  Str = make_unique<char[]>(length * 2);

	size_t Len = 0;

	wcstombs_s(&Len, Str.get(), length * 2, font.c_str(), length * 2);

	string s(Str.get());

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
void Erroer_Message(const std::wstring& str1, const std::wstring& str2)
{
	MessageBox(GetWindow(), str1.c_str(), str2.c_str(), MB_OK | MB_ICONWARNING);
}

void Erroer_Message(const std::string& str1, const std::string& str2)
{
	std::wstring str01, str02;

	str01 = stringTowstring(str1);
	str02 = stringTowstring(str2);

	MessageBox(GetWindow(), str01.c_str(), str02.c_str(), MB_OK | MB_ICONWARNING);
}
#else
void Erroer_Message(const std::string& str1, const std::string& str2)
{
	MessageBox(GetWindow(), str1.c_str(), str2.c_str(), MB_OK | MB_ICONWARNING);
}
#endif // !UNICODE

string Replace_String(string& replacedStr, const string& from, const string& to)
{
	if (replacedStr.empty() || from.empty() || to.empty())
	{
		return replacedStr;
	}

	if (from.size() != to.size())
	{
		return replacedStr;
	}

	const UINT pos = (UINT)replacedStr.find(from);
	const UINT len = (UINT)from.length();

	if (replacedStr.size() < pos)
	{
		return replacedStr;
	}

	return replacedStr.replace(pos, len, to);
}