#pragma once
#include "T_Sprite.h"
class T_Scene;
class Player :
	public T_Sprite
{
public:
	Player(LPCTSTR imgPath, int frameWidth = 0, int frameHeight = 0);
	~Player();

	// 检测角色碰撞, distance检测碰撞的距离
	virtual bool CollideWith(T_Sprite* target, int distance = 0)override { return false; }
	// 检测地图碰撞
	virtual bool CollideWith(IN T_Map* map)override { return false; }
	//根据根据玩家状态信息，更新位置、帧图，检测地图碰撞
	virtual void update();
};

