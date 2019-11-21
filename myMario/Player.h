#pragma once
#include "T_Sprite.h"
class T_Scene;
class GameMap;

//���ģʽ
enum PLAYERSTATUS {
	PLAYER_NONE,
	PLAYER_NORMAL,
	PLAYER_BIGRED,
	PLAYER_BIGGREEN
};

//���֡����
typedef struct {
	T_Graph img;			//ͼƬ
	int frameWidth;			//֡���
	int frameHeight;		//֡�߶�
	int squatHeight;		//�¶�ʱ�߶�
	int* runFrmSequence;	//����֡����
	int nRunFrames;			//����֡���г���
	int stopFrame;			//��Ҿ�ֹ֡
	int speedDownFrame;		//���ٶ���֡
	int squatFrame;			//�¶�֡
	int jumpFrame;			//��Ծ֡
}PLAYERFRAME;

//�����������
typedef struct {
	PLAYERFRAME frameMode;
	// ----- MOVE
	int maxMoveSpeedX;		//��������ٶ�
	int maxRunSpeedX;		//������ٶȣ�shift��
	int basicSpeedX;		//���������ٶ�

	// ----- JUMP
	int basicJumpSpeedY;	//��Ծ��ʼ�ٶ�
	int maxBootTime;		//������ʱ�䣨��ס��Ծ������Чʱ�䣩

	bool canSquat;			//�Ƿ���¶�
}PLAYERMODE;

//��ײ��
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
	int lifeCount;					//����ֵ
	bool isInEnvnt;					//�Ƿ�����Ϸ�¼���
	int eventId;					//��󡢱�С������
	PLAYERSTATUS playerStatus;		//��ɫչʾ״̬
	bool starStatus;				//�Ƿ��޵У����ǣ�״̬
		
	//PLAYER MODE
	PLAYERMODE* currentMode;
	PLAYERMODE* normalMode;
	PLAYERMODE* bigRedMode;

	// ----- MOVE STATUS
	bool bMove;		//ˮƽ�ƶ�״̬
	bool bSquat;	//�¶�״̬
	bool bJump;		//��Ծ״̬
	bool bSlide;	//���ٻ���״̬ �ݲ�ʹ��

	// -----JUMP STATUS
	int jumpStatus;	//��Ծ״̬0��������1���½�
	bool isBooting;	//�Ƿ���Ծ����״̬
	unsigned timer;		//��ʱ��
		
	// ----- MOVE	
	int currentMaxSpeedX;
	float friction;		//ˮƽĦ�������ƹ��Ի��о���	
	float gravity;			//�����������ٶ�

	// ----- COLLISION
	int lastX;			//��һ�δ�ӡλ�ú�����
	int lastY;			//��һ�δ�ӡλ��������

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

	//�˶����Ʒ���
	// ----- MOVE
	void startMove();			//����Ϊ�����ƶ�״̬
	void startSpeedup();		//����Ϊ����״̬
	void resetSpeedup();		//�ָ�Ϊ�����ƶ�״̬
	void stopMove(bool immediately);			//ֹͣˮƽ�ƶ���immediately���Ƿ񺬹��Դ���
	

	//------JUMP
	bool isJump() { return bJump; }
	void startJump();		//����������Ϊ����״̬
	void resetJump();		//��أ�������ֱ��ֹ
	void gravityEffect();


	void updatePositionY();			//��ֱ�ƶ�
	void updatePositionX();			//ˮƽ�ƶ�
	//��ֱ����״̬����
	bool getBooting() { return isBooting; }
	void stopBooting()
	{
		isBooting = false;
	}

	//�Ƿ�����Ϸ�¼���
	bool isInEvent() { return isInEnvnt; }
	bool setEvent(int eventId)
	{ 
		if (eventId == 0)		//�����¼�����
		{
			isInEnvnt = false;
		}
		else
		{
			isInEnvnt = true;	//�����¼���ʼ
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
		else if (this->bSquat == false && bSquat == true)//����
		{
			this->Y = this->Y+(currentMode->frameMode.frameHeight- currentMode->frameMode.squatHeight);		//���º�վ����֡ͼ�ĸ߶Ȳ�
			SetHeight(currentMode->frameMode.squatHeight);
			this->bSquat = bSquat;
		}
		else if (this->bSquat == true && bSquat == false)//վ����
		{
			this->Y = this->Y - (currentMode->frameMode.frameHeight - currentMode->frameMode.squatHeight);	// ���º�վ����֡ͼ�ĸ߶Ȳ�
			SetHeight(currentMode->frameMode.frameHeight);
			this->bSquat = bSquat;
		}
	}

	bool isSliding() { return bSlide; }
	//�Ƿ����״̬��shift��
	bool isSpeedUp() { return currentMaxSpeedX == currentMode->maxRunSpeedX; }
	//�����������
	void updatePosition();
	//����֡ͼ
	void updateFrame();
	
	//���ݸ������״̬��Ϣ������λ�á�֡ͼ������ͼ��ײ
	virtual void update();

	void setPlayerMode(PLAYERSTATUS status);
	virtual void Draw(HDC hdc);

	// ����ɫ��ײ, distance�����ײ�ľ���
	virtual bool CollideWith(T_Sprite* target, int distance = 0)override { return false; }
	// ����ͼ��ײ
	virtual bool CollideWith(IN T_Map* map)override;

	GAME_DIR getCollideDir( RECT target);
};

