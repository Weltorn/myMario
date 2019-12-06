#pragma once
#include "Minion.h"
class BrickPiece :
	public Minion
{
public:
	BrickPiece(LPCTSTR imgPath, int frameWidth, int frameHeight);
	~BrickPiece();

	virtual string ClassName() { return "BrickPiece"; }
	virtual void updateFrame();
	//¸üÐÂ¹ÖÎï×´Ì¬
	virtual void update();
};

