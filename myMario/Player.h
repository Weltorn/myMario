#pragma once
#include "T_Sprite.h"
class T_Scene;
class Player :
	public T_Sprite
{
private:
	// ----- STATUS
	bool bMove;
	bool bSquat;
	bool bJump;

	// ----- MOVE
	bool dirChanged;	
	const int maxMoveSpeedX = 4;
	const int maxRunSpeedX = 6;
	int currentMaxSpeed;

	// ----- JUMP



public:
	Player(LPCTSTR imgPath, int frameWidth = 0, int frameHeight = 0);
	~Player();

	//运动控制方法
	// ----- MOVE
	void startMove();			//设置为正常移动状态
	
	void startSpeedup();		//设置为加速状态
	void Player::resetSpeedup();//恢复为正常移动状态

	void stopMove(bool immediately);			//停止水平移动，immediately：是否含惯性处理

	//------JUMP
	void startJump(int iH);
	void resetJump();

	//更新玩家坐标
	void updatePosition();
	//更新帧图
	void updateFrame();
	// 检测角色碰撞, distance检测碰撞的距离
	virtual bool CollideWith(T_Sprite* target, int distance = 0)override { return false; }
	// 检测地图碰撞
	virtual bool CollideWith(IN T_Map* map)override { return false; }
	//根据根据玩家状态信息，更新位置、帧图，检测地图碰撞
	virtual void update();
};

