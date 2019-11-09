#pragma once
#include "T_Map.h"
class GameMap :
	public T_Map
	//添加砖块等可变场景对象

{
public:
	GameMap(LAYERINFO layerInfo);		//使用地图数据构造
	GameMap(LPCTSTR imgFilepath);		//使用背景图片构造
	~GameMap();
	virtual void update();
};

