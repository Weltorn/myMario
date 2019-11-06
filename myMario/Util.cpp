
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

//���������Ϣ
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
* @function: ��ȡcate_dirĿ¼�µ������ļ���
* @param: cate_dir - string����
* @result��vector<wstring>����
*/

DWORD Util::EnumerateFileInPath(LPWSTR szPath, vector<wstring>* filelist)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hListFile;
	WCHAR szFilePath[MAX_PATH];

	//ƥ��pngͼƬ
	lstrcpy(szFilePath, szPath);
	lstrcat(szFilePath, L"\\*.png");

	// ���ҵ�һ���ļ�/Ŀ¼����ò��Ҿ��
	hListFile = FindFirstFile(szFilePath, &FindFileData);
	// �жϾ��
	if (hListFile == INVALID_HANDLE_VALUE)
	{
		Util::myprintf(L"����%d\n", GetLastError());
		return 1;
	}
	else
	{
		do
		{
			//����ʾ������Ŀ¼���ϼ�Ŀ¼�ġ�.���͡�..����			
			if (lstrcmp(FindFileData.cFileName, TEXT(".")) == 0 ||
				lstrcmp(FindFileData.cFileName, TEXT("..")) == 0)
			{
				continue;
			}
			// �ж��ļ����ԣ��Ƿ�Ϊ�����ļ������ļ���
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED)
			{
				Util::myprintf(L"<����> ");
			}
			// �ж��ļ����ԣ��Ƿ�Ϊ�����ļ����ļ���
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
			{
				Util::myprintf(L"<����> ");
			}
			// �ж��ļ����ԣ��Ƿ�ΪĿ¼
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				Util::myprintf(L"<DIR> ");
			}
			else
			{		//͸��ͼƬidΪ0
				if (lstrcmp(FindFileData.cFileName, TEXT("transparent.png")) == 0)
				{
					filelist->insert(filelist->begin(), FindFileData.cFileName);
				}
				else
				{
					filelist->push_back(FindFileData.cFileName);
				}
			}
			// ���߿ɸ����ļ����Ա��е�����������ӡ��ж��ļ�����
			Util::myprintf(L"\n");
		} while (FindNextFile(hListFile, &FindFileData));
	}
	return 0;
}






