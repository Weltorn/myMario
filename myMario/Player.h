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
	int deathFrame;			//����֡
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

typedef struct {
	bool useTime;		//�Ƿ�ʹ��ʱ��
	bool usePos;		//�Ƿ�ʹ��λ��
	unsigned lastTime;	//�ڵ����ʱ��
	POINT endPt;		//�ڵ���ֹ����
}EVENTSTEP;
typedef vector<EVENTSTEP> EVENT;

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
	int currentFrmIndex;
		
	//PLAYER MODE
	PLAYERMODE* currentMode;
	PLAYERMODE* normalMode;
	PLAYERMODE* bigRedMode;

	//PLAYER EVENT
	EVENT* currentEvent;	//��ǰ�¼�
	unsigned currentStep;
	EVENT deathEvent;
	EVENT levelUpEvent;
	EVENT levelDownEvent;
	EVENT levelEndEvent;


	// ----- MOVE STATUS
	bool bMove;		//ˮƽ�ƶ�״̬
	bool bSquat;	//�¶�״̬
	bool bJump;		//��Ծ״̬
	bool bSlide;	//���ٻ���״̬
	bool dirChanged;//����״̬�ı䷽��

	// -----JUMP STATUS
	bool onPlantform;
	int jumpStatus;		//��Ծ״̬0��������1���½�
	bool isBooting;		//�Ƿ���Ծ����״̬
	int startHeight;	//��Ծ��ʼ�߶�
	unsigned jumpTimer;		//��ʱ��
		
	// ----- MOVE	
	int currentMaxSpeedX;
	float friction;		//ˮƽĦ�������ƹ��Ի��о���	
	float gravity;			//�����������ٶ�
	unsigned moveTimer;		//��ʱ��

	// ----- COLLISION
	int lastX;			//��һ�δ�ӡλ�ú�����
	int lastY;			//��һ�δ�ӡλ��������

public:
	Player(LPCTSTR imgPath, int frameWidth = 0, int frameHeight = 0);
	~Player();
	virtual string ClassName() { return "Player"; }

	void setLifeCount(int lifeCount) { this->lifeCount = lifeCount; }
	int  getLifeCount() { return lifeCount; }


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


	
	//��ֱ����״̬����
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
	bool isDirChanged() { return dirChanged; }
	void setDirChanged(bool dirchanged) { dirChanged = dirchanged; }
	bool isOnPlantform() { return onPlantform; }
	//�Ƿ����״̬��shift��
	bool isSpeedUp() { return currentMaxSpeedX == currentMode->maxRunSpeedX; }
	//�����������
	void updatePosition();
	void updatePositionY();			//��ֱ�ƶ�
	void updatePositionX();			//ˮƽ�ƶ�
	//����֡ͼ
	void updateFrame();
	
	//���ݸ������״̬��Ϣ������λ�á�֡ͼ������ͼ��ײ
	virtual void update();
	//�������ģʽ����ͨ�����˺�ɫĢ����������ɫĢ����
	void setPlayerMode(PLAYERSTATUS status);

	//��һ��Ʒ���
	virtual void Draw(HDC hdc);

	// ����ɫ��ײ, distance�����ײ�ľ���
	virtual bool CollideWith(T_Sprite* target, int distance = 0)override { return false; }
	// ����ͼ��ײ
	virtual bool CollideWith(IN T_Map* map)override;

	// �ж������Ŀ����ε���ײ�����������ҵķ���
	GAME_DIR getCollideDir( RECT target);
	// �������Ƿ�վ��֧������
	bool checkOnplantForm(T_Map* map);

	//��Ϸ�¼����
	bool isInEvent() { return isInEnvnt; }
	void loadEvents();
	void startEvent(int eventId);
	bool checkNextPoint();
	void playAnimation();
	void endEvent();
};

