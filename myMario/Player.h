#pragma once
#include "T_Sprite.h"
class Player :
	public T_Sprite
{
public:
	Player(LPCTSTR imgPath, int frameWidth = 0, int frameHeight = 0);
	~Player();

	//根据根据玩家状态信息，更新位置、帧图，检测地图碰撞
	virtual void update();
};

