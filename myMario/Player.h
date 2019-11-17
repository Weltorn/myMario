#pragma once
#include "T_Sprite.h"
class T_Scene;
class Player :
	public T_Sprite
{
private:
	// ----- role status
	int lifeCount;
	bool isInEnvnt;	//是否在游戏事件中
	int eventId;
	unsigned timer;		//定时器
	

	// ----- MOVE STATUS
	bool bMove;		//是否可水平移动状态
	bool bSquat;	//是否下蹲状态
	bool bJump;		//是否跳跃状态

	// -----JUMP STATUS
	int jumpStatus;	//跳跃状态0：上升，1：下降
	bool isBooting;	//是否跳跃加速状态
		
	// ----- MOVE
	int maxMoveSpeedX;
	int maxRunSpeedX;
	int currentMaxSpeed;
	int basicSpeedX;
	float friction;		//水平摩擦，控制惯性滑行距离

	// ----- JUMP
	int originJumpSpeedY;		//跳跃初始速度
	int maxBootTime;		//最大加速时间（按住跳跃键的有效时间）
	float gravity;			//基础重力加速度

	// ----- SQUAT
	int squatHeight;			//下蹲时大小
	int squatWidth;

	int normalWidth;			//站立、跳跃时大小
	int normalHeight;

	// ----- COLLISION
	int lastX;			//上一次打印位置横坐标
	int lastY;			//上一次打印位置纵坐标
	int collisionStatus;


public:
	Player(LPCTSTR imgPath, int frameWidth = 0, int frameHeight = 0);
	~Player();
	virtual string ClassName() { return "Player"; }

	void setLifeCount(int lifeCount) { this->lifeCount = lifeCount; }
	int  getLifeCount() { return lifeCount; }
	//运动控制方法
	// ----- MOVE
	void startMove();			//设置为正常移动状态
	
	void startSpeedup();		//设置为加速状态
	void resetSpeedup();		//恢复为正常移动状态

	void stopMove(bool immediately);			//停止水平移动，immediately：是否含惯性处理

	void updatePositionX();			//水平移动

	//------JUMP
	bool isJump() { return bJump; }
	void startJump();		//起跳，设置为加速状态
	void resetJump();		//落地，设置水平、竖直静止
	void updatePositionY();			//竖直移动
	void gravityEffect();
	
	//垂直加速状态设置
	bool getBooting() { return isBooting; }
	void stopBooting()
	{
		isBooting = false;
	}

	//是否在游戏事件中
	bool isInEvent() { return isInEnvnt; }
	bool setEvent(int eventId)
	{ 
		if (eventId == 0)		//设置事件结束
		{
			isInEnvnt = false;
		}
		else
		{
			isInEnvnt = true;	//设置事件开始
		}
		this->eventId = eventId;
	}

	// ----- SQUAT
	bool getSquat() { return bSquat; }
	void setSquat(bool bSquat)
	{
		if (this->bSquat == bSquat)
		{
			return;
		}
		else if (this->bSquat == false && bSquat == true)//蹲下
		{
			this->Y = this->Y+(normalHeight- squatHeight);		//蹲下和站起来帧图的高度差
			SetHeight(squatHeight);
			this->bSquat = bSquat;
		}
		else if (this->bSquat == true && bSquat == false)//站起来
		{
			this->Y = this->Y - (normalHeight - squatHeight);		// 蹲下和站起来帧图的高度差
			SetHeight(normalHeight);
			this->bSquat = bSquat;
		}
	}
	//是否加速状态（shift）
	bool isSpeedUp() { return currentMaxSpeed == maxRunSpeedX; }
	//更新玩家坐标
	void updatePosition();
	//更新帧图
	void updateFrame();
	// 检测角色碰撞, distance检测碰撞的距离
	virtual bool CollideWith(T_Sprite* target, int distance = 0)override { return false; }
	// 检测地图碰撞
	virtual bool CollideWith(IN T_Map* map)override;
	//根据根据玩家状态信息，更新位置、帧图，检测地图碰撞
	virtual void update();

	virtual void Draw(HDC hdc);
};

