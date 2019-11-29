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
	int deathFrame;			//死亡帧
	int levelUpFrame;		//升级帧
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
	int col;
	int row;
	GAME_DIR dir;
}COLLIDBLOCK;
typedef vector<COLLIDBLOCK> COLLIDBLOCKS;

enum EVENTTYPE
{
	PLAYER_DEATH,
	PLAYER_LEVELUP,
	PLAYER_LEVELDOWN,
	PLAYER_AFTERPOLE,
	NPC_DEATH_CRASH,
	NPC_DEATH_TURNOVER
};

class Player :
	public T_Sprite
{
private:
	// ----- PLAYER STATUS
	int lifeCount;					//生命值
	PLAYERSTATUS playerStatus;		//角色展示状态
	bool starStatus;				//是否无敌（星星）状态
	int currentFrmIndex;
		
	//PLAYER MODE
	PLAYERMODE* currentMode;
	PLAYERMODE* normalMode;
	PLAYERMODE* bigRedMode;

	//PLAYER EVENT
	unsigned eventTimer;
	bool inEvent;					//是否在游戏事件中
	int eventId;					//变大、变小、死亡
	int currentStep;

	// ----- MOVE STATUS
	bool bMove;		//水平移动状态
	bool bSquat;	//下蹲状态
	bool bJump;		//跳跃状态
	bool bSlide;	//减速滑行状态
	bool dirChanged;//滑行状态改变方向

	// -----JUMP STATUS
	bool onPlantform;
	int jumpStatus;		//跳跃状态0：上升，1：下降
	bool isBooting;		//是否跳跃加速状态
	int startHeight;	//跳跃起始高度
	unsigned jumpTimer;		//计时器
		
	// ----- MOVE	
	int currentMaxSpeedX;
	float friction;		//水平摩擦，控制惯性滑行距离	
	float gravity;			//基础重力加速度
	unsigned moveTimer;		//计时器

public:
	Player(LPCTSTR imgPath, int frameWidth = 0, int frameHeight = 0);
	~Player();
	virtual string ClassName() { return "Player"; }

	void setLifeCount(int lifeCount) { this->lifeCount = lifeCount; }
	int  getLifeCount() { return lifeCount; }


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


	
	//垂直加速状态设置
	bool getBooting() { return isBooting; }
	void stopBooting()
	{
		isBooting = false;
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
	bool isDirChanged() { return dirChanged; }
	void setDirChanged(bool dirchanged) { dirChanged = dirchanged; }
	bool isOnPlantform() { return onPlantform; }
	//是否加速状态（shift）
	bool isSpeedUp() { return currentMaxSpeedX == currentMode->maxRunSpeedX; }
	//更新玩家坐标
	void updatePosition();
	void updatePositionY();			//竖直移动
	void updatePositionX();			//水平移动
	//更新帧图
	void updateFrame();
	
	//根据根据玩家状态信息，更新位置、帧图，检测地图碰撞
	virtual void update();
	//设置玩家模式（普通、吃了红色蘑菇、吃了绿色蘑菇）
	void setPlayerMode(PLAYERSTATUS status);

	//玩家绘制方法
	virtual void Draw(HDC hdc);

	// 检测角色碰撞, distance检测碰撞的距离
	virtual bool CollideWith(T_Sprite* target, int distance = 0)override { return false; }
	// 检测地图碰撞
	virtual bool CollideWith(IN T_Map* map)override;	
	// 检查玩家是否站在支持物上
	bool checkOnplantForm(T_Map* map);

	//游戏事件相关
	bool isInEvent() { return inEvent; }
	void startEvent(int eventId);
	void stopEvent()
	{
		inEvent = false;
		currentStep = -1;		

		//解除运动状态
		onPlantform = false;
		stopMove(true);
		resetJump();
		setSquat(false);
	}
	void playAnimation();

	void deathAnimation();
	void levelUpAnimation();

	void playerDeath(bool immediately);

	
};

