#pragma once
#include "T_Sprite.h"
#include "Util.h"
class T_Scene;
class GameMap;

enum MINION_TYPE
{
	MINION_STAR,			// 星星
	MINION_REDMUSHROOM,		// 红蘑菇
	MINION_GREENMUSHROOM,	// 绿蘑菇
	MINION_FLOWER,			// 花

	MINION_GOOMBA,				// 板栗仔（蘑菇一样的怪物）
	MINION_KOOPA,				// 库帕龟
};

class Minion :
	public T_Sprite
{
private:
	MINION_TYPE minionType;		//怪物类型

	//状态信息
	bool onPlantform;		//是否在支持物上
	int currentFrmIndex;

	// ----- MOVE		
	float gravity;			//基础重力加速度
	unsigned timer;			//计时器

	//EVENT
	unsigned eventTimer;
	bool inEvent;					//是否在游戏事件中
	int eventId;					//变大、变小、死亡
	int currentStep;

	// ----- COLLISION
	int lastX;			//上一次打印位置横坐标
	int lastY;			//上一次打印位置纵坐标
public:
	Minion(LPCTSTR imgPath, int frameWidth, int frameHeight);
	~Minion();
	virtual string ClassName() { return "Minion"; }
	MINION_TYPE getMinionType() { return minionType; }
	void setMinionType(MINION_TYPE minionType) {
		this->minionType = minionType;
	}

	// 判断怪物与目标矩形的碰撞方向（相对于怪物的方向）
	GAME_DIR getCollideDir(RECT target);
	virtual bool CollideWith(IN T_Map* map);
	virtual bool CollideWith(T_Sprite* target, int distance = 0);

	//更新怪物状态
	virtual void update();
	
	// 检查是否站在支持物上
	bool checkOnplantForm(T_Map* map);
	bool isOnPlantform() { return onPlantform; }	

	//重力效果
	void gravityEffect();
	//更新坐标
	void updatePosition();
	void updatePositionY();			//竖直移动
	void updatePositionX();			//水平移动									
	virtual void updateFrame();				//更新帧图
	
	//事件相关
	bool isInEvent() { return inEvent; }
	virtual void startEvent(int eventId);
	virtual void playAnimation();

};

