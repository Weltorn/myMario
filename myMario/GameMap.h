#pragma once
#include "T_Map.h"
class GameMap :
	public T_Map
{
private:
	//添加砖块等可变场景对象

	COLLIDBLOCKS collideBlocks;	//玩家发生碰撞的地图块，包括受碰方向，指导地图更新

public:
	GameMap(LAYERINFO layerInfo);		//使用地图数据构造
	GameMap(LPCTSTR imgFilepath);		//使用背景图片构造
	~GameMap();
	void setCollideBlocks(COLLIDBLOCKS collideBlocks) { this->collideBlocks = collideBlocks; }
	virtual void update();
};

