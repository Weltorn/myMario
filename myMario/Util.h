#pragma once

#include "T_Engine.h"
class Util
{
public:
	Util();
	~Util();
	static void createDoubleBuffer(HWND,HDC&,HBITMAP&);
	static void drawLine(HDC, POINT begin, POINT end);
	//输出调试信息到控制台
	static void myprintf(TCHAR * szFormat, ...);
	//获取目录下文件列表
	static DWORD EnumerateFileInPath(LPWSTR szPath, vector<wstring>* filelist);


};


