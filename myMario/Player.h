#pragma once
#include "T_Sprite.h"
class T_Scene;
class GameMap;

//���ģʽ
enum PLAYERSTATUS {
	PLAYER_NONE,
	PLAYER_NORMAL,
	PLAYER_BIGNORMAL,
	PLAYER_BIGRED
};

//���֡����
typedef struct {
	T_Graph img;			//ͼƬ
	int frameWidth;			//֡���
	int frameHeight;		//֡�߶�
	int squatHeight;		//�¶�ʱ�߶�
	int* runFrmSequence;	//����֡����
	int nRunFrames;			//����֡���г���
	int *levelUpFrmSequence;//����֡����
	int nlevelUpFrames;
	int *levelDownFrmSequence;//����֡����
	int nlevelDownFrames;
	int stopFrame;			//��Ҿ�ֹ֡
	int speedDownFrame;		//���ٶ���֡
	int squatFrame;			//�¶�֡
	int jumpFrame;			//��Ծ֡
	int deathFrame;			//����֡
	int levelUpFrame;		//����֡
	int fireBallFrame;		//�����ڵ�֡
}PLAYERFRAME;

//�����������
typedef struct {

	// ----- MOVE
	int maxMoveSpeedX;		//��������ٶ�
	int maxRunSpeedX;		//������ٶȣ�shift��
	int basicSpeedX;		//���������ٶ�

	// ----- JUMP
	int basicJumpSpeedY;	//��Ծ��ʼ�ٶ�
	int maxBootTime;		//������ʱ�䣨��ס��Ծ������Чʱ�䣩

	bool canSquat;			//�Ƿ���¶�
	bool canCreateFireBall;	//�Ƿ�ɷ������
}PLAYERMODE;

//��ײ��
typedef struct {
	int col;
	int row;
	GAME_DIR dir;
}COLLIDBLOCK;
typedef vector<COLLIDBLOCK> COLLIDBLOCKS;



class Player :
	public T_Sprite
{
private:
	// ----- PLAYER STATUS
	int lifeCount;					//����ֵ
	PLAYERSTATUS playerStatus;		//��ɫչʾ״̬
	bool starStatus;				//�Ƿ��޵У����ǣ�״̬
	int currentFrmIndex;
		
	//FRAME MODE
	PLAYERFRAME* currentFrame;
	PLAYERFRAME* bigFrame;
	PLAYERFRAME* smallFrame;

	//PLAYER MODE
	PLAYERMODE* currentMode;
	PLAYERMODE* normalMode;
	PLAYERMODE* bigNormalMode;
	//PLAYERMODE* bigRedMode;

	//PLAYER EVENT
	unsigned eventTimer;
	bool inEvent;					//�Ƿ�����Ϸ�¼���
	int eventId;					//��󡢱�С������
	int currentStep;				//Ŀǰ�¼�����

	// ----- MOVE STATUS
	bool bMove;		//ˮƽ�ƶ�״̬
	bool bSquat;	//�¶�״̬
	bool bJump;		//��Ծ״̬
	bool bSlide;	//���ٻ���״̬
	bool dirChanged;//����״̬�ı䷽��
	bool bSafe;		//������İ�ȫ״̬,�����������ײ
	unsigned safeTime;
	unsigned blinkCount;	//���ư�ȫ״̬��˸Ƶ��

	// -----JUMP STATUS
	bool onPlantform;
	int jumpStatus;		//��Ծ״̬0��������1���½�
	bool isBooting;		//�Ƿ���Ծ����״̬
	int startHeight;	//��Ծ��ʼ�߶�
	unsigned jumpTimer;		//��ʱ��,���Ƽ���ʱ��
		
	// ----- MOVE	
	int currentMaxSpeedX;
	bool bSpeedUp;		//�Ƿ��ڼ���״̬
	float friction;		//ˮƽĦ�������ƹ��Ի��о���	
	float gravity;			//�����������ٶ�
	unsigned moveTimer;		//��ʱ��,���Ƽ���ʱ��

	// -----FIREBALL
	bool onCreateFireBall;
	unsigned fireballCD;
	unsigned fireballTimer;
public:
	Player(LPCTSTR imgPath, int frameWidth = 0, int frameHeight = 0);
	virtual ~Player();
	virtual string ClassName() { return "Player"; }

	void setLifeCount(int lifeCount) { this->lifeCount = lifeCount; }
	int  getLifeCount() { return lifeCount; }
	int  getPlayerStatus() { return playerStatus; }

	//PLAYER FRAME
	void initBigNormalMode(PLAYERMODE* bigRedMode);
	void initNormalMode(PLAYERMODE* normalMode);
	void initSmallFrameMode(PLAYERFRAME* smallFrame);
	void initBigFrameMode(PLAYERFRAME* bigFrame);
	//�������ģʽ����ͨ�����˺�ɫĢ����������ɫĢ����
	void setPlayerMode(PLAYERSTATUS status);

	//�˶����Ʒ���
	// ----- MOVE
	void startMove();			//����Ϊ�����ƶ�״̬
	void startSpeedup();		//����Ϊ����״̬
	void resetSpeedup();		//�ָ�Ϊ�����ƶ�״̬
	void stopMove(bool immediately);			//ֹͣˮƽ�ƶ���immediately���Ƿ񺬹��Դ���
	
	//�����ڵ�
	void createFireBall();
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
			this->Y = this->Y+(currentFrame->frameHeight- currentFrame->squatHeight);		//���º�վ����֡ͼ�ĸ߶Ȳ�
			SetHeight(currentFrame->squatHeight);
			this->bSquat = bSquat;
		}
		else if (this->bSquat == true && bSquat == false)//վ����
		{
			this->Y = this->Y - (currentFrame->frameHeight - currentFrame->squatHeight);	// ���º�վ����֡ͼ�ĸ߶Ȳ�
			SetHeight(currentFrame->frameHeight);
			this->bSquat = bSquat;
		}
	}

	bool isSliding() { return bSlide; }
	bool isDirChanged() { return dirChanged; }
	void setDirChanged(bool dirchanged) { dirChanged = dirchanged; }
	bool isOnPlantform() { return onPlantform; }
	//�Ƿ����״̬��shift��
	bool isSpeedUp() { return currentMaxSpeedX == currentMode->maxRunSpeedX; }
	bool isSafe() { return bSafe; }
	//�����������
	void updatePosition();
	void updatePositionY();			//��ֱ�ƶ�
	void updatePositionX();			//ˮƽ�ƶ�
	//����֡ͼ
	void updateFrame();
	
	//���ݸ������״̬��Ϣ������λ�á�֡ͼ������ͼ��ײ
	virtual void update();
	

	//��һ��Ʒ���
	virtual void Draw(HDC hdc);

	// ����ɫ��ײ, distance�����ײ�ľ���
	virtual bool CollideWith(T_Sprite* target, int distance = 0)override { return false; }
	// ����ͼ��ײ
	virtual bool CollideWith(IN T_Map* map)override;	
	// �������Ƿ�վ��֧������
	bool checkOnplantForm(T_Map* map);

	//��Ϸ�¼����
	bool isInEvent() { return inEvent; }
	void startEvent(int eventId);
	void stopEvent()
	{
		inEvent = false;
		currentStep = -1;		

		//����˶�״̬
		onPlantform = false;
		stopMove(true);
		resetJump();
		setSquat(false);
	}
	void playAnimation();

	void deathAnimation();
	void levelUpAnimation();
	void levelDownAnimation();

	void playerDeath(bool immediately);

	
};

