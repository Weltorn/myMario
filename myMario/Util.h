#pragma once

#include "T_Engine.h"
class Util
{
public:
	Util();
	~Util();
	static void createDoubleBuffer(HWND,HDC&,HBITMAP&);
	static void drawLine(HDC, POINT begin, POINT end);
	//���������Ϣ������̨
	static void myprintf(TCHAR * szFormat, ...);
	//��ȡĿ¼���ļ��б�
	static DWORD EnumerateFileInPath(LPWSTR szPath, vector<wstring>* filelist);


};


