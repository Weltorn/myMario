#pragma once
#include "T_Sprite.h"

enum BRICK_TYPE
{
	NORMAL_BRICK,		// ÆÕÍ¨×©¿é
	PROP_BRICK,			// µÀ¾ß×©
	CRACK_BRICK,		// ×©¿éËéÆ¬
};

class Brick :
	public T_Sprite
{
protected:
	int col;
	int row;
//	bool isActive;		
public:
	Brick(LPCTSTR imgPath, int frameWidth, int frameHeight);
	~Brick();
	void setCol(int c) { col = c; }
	void setRow(int r) { row = r; }
	int getCol() { return col; }
	int getRow() { return row; }
};

