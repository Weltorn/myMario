#pragma once
#include "T_Map.h"
#include "Player.h"
#include "BrickFactory.h"
typedef vector<Brick*> LBrick;

class GameMap :
	public T_Map
{
private:
	//添加砖块等可变场景对象
	LBrick	pBricks;				// 各种砖块指针，包含在sceneLayers中
	COLLIDBLOCKS collideBlocks;		//玩家发生碰撞的地图块，包括受碰方向，指导地图更新

	// 魔改
	bool init;						// 记录位图是否被初始化	解决了卡帧问题
	//HDC memDC;// = CreateCompatibleDC(hdc);
	//HBITMAP OldMemBmp;
	// 魔改end	魔改失败 fk!
	

public:
	GameMap(LAYERINFO layerInfo);		//使用地图数据构造
	GameMap(LPCTSTR imgFilepath);		//使用背景图片构造
	~GameMap();
	void setCollideBlocks(COLLIDBLOCKS collideBlocks) {	this->collideBlocks = collideBlocks; }
	virtual void update();
	void CreateBricks(BRICK_TYPE type);
	void Redraw(HDC hdc);
	void Draw(HDC hdc);
};

