#include "stdafx.h"
#include <curl/curl.h>

#define snPos string::npos
const wstring szAppName = _T("D:\\livestreamer-v1.12.2\\livestreamer.exe");
const wstring szUrl = _T(" http://www.douyutv.com/");
const wstring szViewParam = _T(" best -p \"D:/vlc/vlc --file-caching=5000\"");

const string szDYUrl = "http://www.douyutv.com/";

int iConnectTimeOut = 10, iTimeOut = 10;

#if defined(_DEBUG)
#pragma comment(lib, "libcurld.lib")
#else
#pragma comment(lib, "libcurl.lib")
#endif

#pragma comment (lib, "ws2_32.lib" )
#pragma comment(lib, "ssleay32.lib") 
#pragma comment(lib, "zlibstat.lib") 
#pragma comment(lib, "libeay32.lib")





unsigned int _stdcall GetVedio(void *pParam)
{
	// 	ShellExecute(NULL, L"open", L"D:\\livestreamer-v1.12.2\\livestreamer.exe",
	// 		L" http://www.douyutv.com/465193 best -o z:/bbbbbb.mp4", L"", SW_SHOW);
	//WinExec("D:\\livestreamer-v1.12.2\\livestreamer.exe  http://www.douyutv.com/mengxiaomei best -o z:/bbbbbb.mp4", WM_SHOWWINDOW);

	pMyInfo pMyInfoA = (pMyInfo)pParam;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	wstring  szCmdline;
	SYSTEMTIME  sys;
	GetLocalTime(&sys);
	wchar_t szFileName[256] = {};
	swprintf_s(szFileName, L"%s-%4d-%02d-%02d-%02d-%02d-%02d-%03d.mp4",
		pMyInfoA->RoomID.c_str(), sys.wYear, sys.wMonth, sys.wDay, sys.wHour,
		sys.wMinute, sys.wSecond, sys.wMilliseconds);

	szCmdline = szAppName + szUrl + pMyInfoA->RoomID + _T(" best -o z:\\");
	szCmdline += szFileName;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));


	// Start the child process.
	if (!CreateProcess(/*(LPTSTR)szAppName.c_str()*/NULL,
		(LPTSTR)szCmdline.c_str(),      // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		CREATE_NO_WINDOW,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		//printf("CreateProcess failed (%d)./n", GetLastError());
		return 5;
	}
	pMyInfoA->hProcess = pi.hProcess;
	Sleep(2000);
	::PostMessage(MainHwnd, WM_MYMSG, pMyInfoA->iButtonIdx, MSG00_EnableCloseBtn);
	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);
	::PostMessage(MainHwnd, WM_MYMSG, pMyInfoA->iButtonIdx, MSG01_EnableStartBtn);
	// Close process and thread handles.
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return 0;
}

unsigned int _stdcall ViewVedio(void *pParam)
{
	pMyInfo pMyInfoA = (pMyInfo)pParam;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	wstring  szCmdline;


	szCmdline = szAppName + szUrl + pMyInfoA->RoomID + szViewParam;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));


	// Start the child process.
	if (!CreateProcess(/*(LPTSTR)szAppName.c_str()*/NULL,
		(LPTSTR)szCmdline.c_str(),      // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		CREATE_NO_WINDOW,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		//printf("CreateProcess failed (%d)./n", GetLastError());
		return 5;
	}
	//pMyInfoA->hProcess = pi.hProcess;
	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);
	::PostMessage(MainHwnd, WM_MYMSG, pMyInfoA->iButtonIdx, MSG03_EnableVedioBtn);
	// Close process and thread handles.
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return 0;
}


size_t ReplyCallback(void *ptr, size_t size, size_t nmemb, void *userdata)
{
	std::string *str = static_cast<std::string *>(userdata);
	//if (str == CURLAPI_NULLPTR)
	//throw ParameterException("null pointer parameter: str");
	str->append(static_cast<char *>(ptr), size * nmemb);
	return size * nmemb;
}


unsigned int __stdcall GetRoomID(void * pParam)
{
	pMyInfo spInfo = (pMyInfo)pParam;
	//string strUrl = szDYUrl + spInfo->szUrl;
	string strUrl = spInfo->szUrl;
	string response;


	CURL * curlctx_ = ::curl_easy_init();
	::curl_easy_setopt(curlctx_, CURLOPT_URL, strUrl.c_str());
	::curl_easy_setopt(curlctx_, CURLOPT_HTTPGET, 1L);
	::curl_easy_setopt(curlctx_, CURLOPT_HEADER, 0L);
	::curl_easy_setopt(curlctx_, CURLOPT_NOSIGNAL, 1);
	::curl_easy_setopt(curlctx_, CURLOPT_CONNECTTIMEOUT, iConnectTimeOut);
	::curl_easy_setopt(curlctx_, CURLOPT_TIMEOUT, iTimeOut);
	::curl_easy_setopt(curlctx_, CURLOPT_WRITEFUNCTION, &ReplyCallback);
	::curl_easy_setopt(curlctx_, CURLOPT_WRITEDATA, &response);

	CURLcode errcode;
	errcode = ::curl_easy_perform(curlctx_);
	if (errcode == CURLE_OK) {
		//OutputDebugStringA(response.c_str());
		int sa = response.find("room_id\":");
		if (snPos != sa)
		{
			int sb = response.find(",", sa);
			if (snPos != sb)
			{
				strUrl.clear();
				strUrl = response.substr(sa + 9, sb - sa - 9);
			}
		}
	}
	if (!strUrl.empty())
	{
		for (int i = 0; i < 5; i++)
		{
			if (MyInfo[i].bUnUsing)
			{
				MyInfo[i].RoomID.clear();
				stringToWstring(strUrl, MyInfo[i].RoomID);
				MyInfo[i].bUnUsing = false;
				::PostMessage(MainHwnd, WM_MYMSG, i, MSG02_SetRoomID);
				break;
			}
		}
	}
	::curl_easy_cleanup(curlctx_);
	return 0;
}