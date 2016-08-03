// stdafx.cpp : source file that includes just the standard includes
//	livestreamerGUI.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#if (_ATL_VER < 0x0700)
#include <atlimpl.cpp>
#endif //(_ATL_VER < 0x0700)

void stringToWstring(const std::string& str, std::wstring &wstr)
{
	LPCSTR pszSrc = str.c_str();
	int nLen = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, NULL, 0);
	if (nLen == 0)
		return;

	wchar_t* pwszDst = new wchar_t[nLen];
	if (!pwszDst)
		return;

	MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pwszDst, nLen);
	wstr.append(pwszDst);
	delete[] pwszDst;
	pwszDst = NULL;

}

void wstringToString(const std::wstring& wstr, std::string &str)
{
	LPCWSTR pwszSrc = wstr.c_str();
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
	if (nLen == 0)
		return;

	char* pszDst = new char[nLen];
	if (!pszDst)
		return;

	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
	str.append(pszDst);
	delete[] pszDst;
	pszDst = NULL;

}