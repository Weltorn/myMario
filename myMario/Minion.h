#pragma once
#include "T_Sprite.h"
enum MINION_TYPE
{
	MINION_STAR,			// ÐÇÐÇ
	MINION_REDMUSHROOM,		// ºìÄ¢¹½
	MINION_GREENMUSHROOM,	// ÂÌÄ¢¹½
	MINION_FLOWER,			// »¨

	MINION_GOOMBA,				// °åÀõ×Ð£¨Ä¢¹½Ò»ÑùµÄ¹ÖÎï£©
	MINION_KOOPA,				// ¿âÅÁ¹ê
};

class Minion :
	public T_Sprite
{
public:
	Minion(LPCTSTR imgPath, int frameWidth, int frameHeight);
	~Minion();
	virtual void update();
	virtual bool CollideWith(IN T_Map* map);
	virtual bool CollideWith(T_Sprite* target, int distance = 0);
};

