#pragma once
#include "T_Menu.h"

#define ROUND(x) (int)(x+0.5)	//定义四舍五入的宏

class GameMenu :
	public T_Menu
{
protected:
	int fontHeight;				//菜单项的字体大小
	int beginTick;
//	int nowTick;				

public:
	GameMenu();
	~GameMenu();

	void SetFontHeight(int height) { fontHeight = height; }
	void ClearItem() { gm_menuItems.clear(); }
	void DrawMenu(HDC hdc, int bkgX = 0, int bkgY = 0, BYTE btnTrans = 255, bool startState = true);
};

