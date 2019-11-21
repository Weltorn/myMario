#pragma once
#include "T_Sprite.h"
class T_Scene;
class GameMap;

//玩家模式
enum PLAYERSTATUS {
	PLAYER_NONE,
	PLAYER_NORMAL,
	PLAYER_BIGRED,
	PLAYER_BIGGREEN
};

//玩家帧设置
typedef struct {
	T_Graph img;			//图片
	int frameWidth;			//帧宽度
	int frameHeight;		//帧高度
	int squatHeight;		//下蹲时高度
	int* runFrmSequence;	//奔跑帧序列
	int nRunFrames;			//奔跑帧序列长度
	int stopFrame;			//玩家静止帧
	int speedDownFrame;		//减速动作帧
	int squatFrame;			//下蹲帧
	int jumpFrame;			//跳跃帧
}PLAYERFRAME;

//玩家能力设置
typedef struct {
	PLAYERFRAME frameMode;
	// ----- MOVE
	int maxMoveSpeedX;		//最大行走速度
	int maxRunSpeedX;		//最大奔跑速度（shift）
	int basicSpeedX;		//基础行走速度

	// ----- JUMP
	int basicJumpSpeedY;	//跳跃初始速度
	int maxBootTime;		//最大加速时间（按住跳跃键的有效时间）

	bool canSquat;			//是否可下蹲
}PLAYERMODE;

//碰撞块
typedef struct {
	int x;
	int y;
	GAME_DIR dir;
}COLLIDBLOCK;

typedef vector<COLLIDBLOCK> COLLIDBLOCKS;
class Player :
	public T_Sprite
{
private:
	// ----- PLAYER STATUS
	int lifeCount;					//生命值
	bool isInEnvnt;					//是否在游戏事件中
	int eventId;					//变大、变小、死亡
	PLAYERSTATUS playerStatus;		//角色展示状态
	bool starStatus;				//是否无敌（星星）状态
		
	//PLAYER MODE
	PLAYERMODE* currentMode;
	PLAYERMODE* normalMode;
	PLAYERMODE* bigRedMode;

	// ----- MOVE STATUS
	bool bMove;		//水平移动状态
	bool bSquat;	//下蹲状态
	bool bJump;		//跳跃状态
	bool bSlide;	//减速滑行状态 暂不使用

	// -----JUMP STATUS
	int jumpStatus;	//跳跃状态0：上升，1：下降
	bool isBooting;	//是否跳跃加速状态
	unsigned timer;		//计时器
		
	// ----- MOVE	
	int currentMaxSpeedX;
	float friction;		//水平摩擦，控制惯性滑行距离	
	float gravity;			//基础重力加速度

	// ----- COLLISION
	int lastX;			//上一次打印位置横坐标
	int lastY;			//上一次打印位置纵坐标

public:
	Player(LPCTSTR imgPath, int frameWidth = 0, int frameHeight = 0);
	~Player();
	virtual string ClassName() { return "Player"; }

	void setLifeCount(int lifeCount) { this->lifeCount = lifeCount; }
	int  getLifeCount() { return lifeCount; }

	// ----- PLAYER MODE getter
	//PLAYER FRAME
	//T_Graph* getFrameImg(){ return &(currentMode->frameMode->img); }
	//int getFrameHeight() { return currentMode->frameMode->frameHeight; }
	//int getFrameWidth() { return currentMode->frameMode->frameWidth; }
	//int getSquatHeight() { return currentMode->frameMode->squatHeight; }
	//int* getRunFrmSequence() { return currentMode->frameMode->runFrmSequence; }
	//int getNRunFrames() { return currentMode->frameMode->nRunFrames; }
	//int getStopFrame() { return currentMode->frameMode->stopFrame; }
	//int getSpeedDownFrame() { return currentMode->frameMode->speedDownFrame; }
	//int getSquatFrame() { return currentMode->frameMode->speedDownFrame; }
	//int getJumpFrame() { return currentMode->frameMode->speedDownFrame; }

	////PLAYER ABILITY getter
	//int getMaxMoveSpeedX() { return currentMode->maxMoveSpeedX; }
	//int getMaxRunSpeedX() { return currentMode->maxRunSpeedX; }
	//int getBasicSpeedX() { return currentMode->basicSpeedX; }
	//int getBasicJumpSpeedY() { return currentMode->basicJumpSpeedY; }
	//int getMaxBootTime() { return currentMode->maxBootTime; }


	//PLAYER FRAME
	void initBigRedMode(PLAYERMODE* bigRedMode);
	void initNormalMode(PLAYERMODE* normalMode);

	//运动控制方法
	// ----- MOVE
	void startMove();			//设置为正常移动状态
	void startSpeedup();		//设置为加速状态
	void resetSpeedup();		//恢复为正常移动状态
	void stopMove(bool immediately);			//停止水平移动，immediately：是否含惯性处理
	

	//------JUMP
	bool isJump() { return bJump; }
	void startJump();		//起跳，设置为加速状态
	void resetJump();		//落地，设置竖直静止
	void gravityEffect();


	void updatePositionY();			//竖直移动
	void updatePositionX();			//水平移动
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
			this->Y = this->Y+(currentMode->frameMode.frameHeight- currentMode->frameMode.squatHeight);		//蹲下和站起来帧图的高度差
			SetHeight(currentMode->frameMode.squatHeight);
			this->bSquat = bSquat;
		}
		else if (this->bSquat == true && bSquat == false)//站起来
		{
			this->Y = this->Y - (currentMode->frameMode.frameHeight - currentMode->frameMode.squatHeight);	// 蹲下和站起来帧图的高度差
			SetHeight(currentMode->frameMode.frameHeight);
			this->bSquat = bSquat;
		}
	}

	bool isSliding() { return bSlide; }
	//是否加速状态（shift）
	bool isSpeedUp() { return currentMaxSpeedX == currentMode->maxRunSpeedX; }
	//更新玩家坐标
	void updatePosition();
	//更新帧图
	void updateFrame();
	
	//根据根据玩家状态信息，更新位置、帧图，检测地图碰撞
	virtual void update();

	void setPlayerMode(PLAYERSTATUS status);
	virtual void Draw(HDC hdc);

	// 检测角色碰撞, distance检测碰撞的距离
	virtual bool CollideWith(T_Sprite* target, int distance = 0)override { return false; }
	// 检测地图碰撞
	virtual bool CollideWith(IN T_Map* map)override;

	GAME_DIR getCollideDir( RECT target);
};

