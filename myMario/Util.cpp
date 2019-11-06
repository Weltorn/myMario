
#include "Util.h"
#include <tchar.h>     
#include <stdio.h>   



Util::Util()
{
}


Util::~Util()
{
}
void Util::createDoubleBuffer(HWND hwnd, HDC& g_hdc, HBITMAP& hbitmap)
{
	HDC hdc = ::GetDC(hwnd);
	RECT rect;
	::GetClientRect(hwnd, &rect);
	g_hdc = ::CreateCompatibleDC(hdc);
	hbitmap = ::CreateCompatibleBitmap(hdc, rect.right-rect.left, rect.bottom-rect.top);
	::SelectObject(g_hdc, hbitmap);
	::ReleaseDC(hwnd, hdc);
}

void Util::drawLine(HDC hdc, POINT begin, POINT end)
{
	::MoveToEx(hdc, begin.x, begin.y, NULL);
	::LineTo(hdc, end.x, end.y);
}

//输出调试信息
void Util::myprintf(TCHAR* szFormat, ...)
{
	TCHAR   szBuffer[1024];
	va_list pArgList;
	va_start(pArgList, szFormat);
	_vsntprintf_s(szBuffer, sizeof(szBuffer) / sizeof(TCHAR),
		szFormat, pArgList);
	va_end(pArgList);
	OutputDebugString(szBuffer);
}

/**
* @function: 获取cate_dir目录下的所有文件名
* @param: cate_dir - string类型
* @result：vector<wstring>类型
*/

DWORD Util::EnumerateFileInPath(LPWSTR szPath, vector<wstring>* filelist)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hListFile;
	WCHAR szFilePath[MAX_PATH];

	//匹配png图片
	lstrcpy(szFilePath, szPath);
	lstrcat(szFilePath, L"\\*.png");

	// 查找第一个文件/目录，获得查找句柄
	hListFile = FindFirstFile(szFilePath, &FindFileData);
	// 判断句柄
	if (hListFile == INVALID_HANDLE_VALUE)
	{
		Util::myprintf(L"错误：%d\n", GetLastError());
		return 1;
	}
	else
	{
		do
		{
			//不显示代表本级目录和上级目录的“.”和“..”，			
			if (lstrcmp(FindFileData.cFileName, TEXT(".")) == 0 ||
				lstrcmp(FindFileData.cFileName, TEXT("..")) == 0)
			{
				continue;
			}
			// 判断文件属性，是否为加密文件或者文件夹
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED)
			{
				Util::myprintf(L"<加密> ");
			}
			// 判断文件属性，是否为隐藏文件或文件夹
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
			{
				Util::myprintf(L"<隐藏> ");
			}
			// 判断文件属性，是否为目录
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				Util::myprintf(L"<DIR> ");
			}
			else
			{		//透明图片id为0
				if (lstrcmp(FindFileData.cFileName, TEXT("transparent.png")) == 0)
				{
					filelist->insert(filelist->begin(), FindFileData.cFileName);
				}
				else
				{
					filelist->push_back(FindFileData.cFileName);
				}
			}
			// 读者可根据文件属性表中的内容自行添加、判断文件属性
			Util::myprintf(L"\n");
		} while (FindNextFile(hListFile, &FindFileData));
	}
	return 0;
}






