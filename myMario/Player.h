#pragma once
#include "T_Sprite.h"
class T_Scene;
class Player :
	public T_Sprite
{
private:
	// ----- role status
	int lifeCount;
	bool isInEnvnt;	//�Ƿ�����Ϸ�¼���
	int eventId;
	unsigned timer;		//��ʱ��
	

	// ----- MOVE STATUS
	bool bMove;		//�Ƿ��ˮƽ�ƶ�״̬
	bool bSquat;	//�Ƿ��¶�״̬
	bool bJump;		//�Ƿ���Ծ״̬

	// -----JUMP STATUS
	int jumpStatus;	//��Ծ״̬0��������1���½�
	bool isBooting;	//�Ƿ���Ծ����״̬
		
	// ----- MOVE
	int maxMoveSpeedX;
	int maxRunSpeedX;
	int currentMaxSpeed;
	int basicSpeedX;
	float friction;		//ˮƽĦ�������ƹ��Ի��о���

	// ----- JUMP
	int originJumpSpeedY;		//��Ծ��ʼ�ٶ�
	int maxBootTime;		//������ʱ�䣨��ס��Ծ������Чʱ�䣩
	float gravity;			//�����������ٶ�

	// ----- SQUAT
	int squatHeight;			//�¶�ʱ��С
	int squatWidth;

	int normalWidth;			//վ������Ծʱ��С
	int normalHeight;

	// ----- COLLISION
	int lastX;			//��һ�δ�ӡλ�ú�����
	int lastY;			//��һ�δ�ӡλ��������
	int collisionStatus;


public:
	Player(LPCTSTR imgPath, int frameWidth = 0, int frameHeight = 0);
	~Player();
	virtual string ClassName() { return "Player"; }

	void setLifeCount(int lifeCount) { this->lifeCount = lifeCount; }
	int  getLifeCount() { return lifeCount; }
	//�˶����Ʒ���
	// ----- MOVE
	void startMove();			//����Ϊ�����ƶ�״̬
	
	void startSpeedup();		//����Ϊ����״̬
	void resetSpeedup();		//�ָ�Ϊ�����ƶ�״̬

	void stopMove(bool immediately);			//ֹͣˮƽ�ƶ���immediately���Ƿ񺬹��Դ���

	void updatePositionX();			//ˮƽ�ƶ�

	//------JUMP
	bool isJump() { return bJump; }
	void startJump();		//����������Ϊ����״̬
	void resetJump();		//��أ�����ˮƽ����ֱ��ֹ
	void updatePositionY();			//��ֱ�ƶ�
	void gravityEffect();
	
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
			this->Y = this->Y+(normalHeight- squatHeight);		//���º�վ����֡ͼ�ĸ߶Ȳ�
			SetHeight(squatHeight);
			this->bSquat = bSquat;
		}
		else if (this->bSquat == true && bSquat == false)//վ����
		{
			this->Y = this->Y - (normalHeight - squatHeight);		// ���º�վ����֡ͼ�ĸ߶Ȳ�
			SetHeight(normalHeight);
			this->bSquat = bSquat;
		}
	}
	//�Ƿ����״̬��shift��
	bool isSpeedUp() { return currentMaxSpeed == maxRunSpeedX; }
	//�����������
	void updatePosition();
	//����֡ͼ
	void updateFrame();
	// ����ɫ��ײ, distance�����ײ�ľ���
	virtual bool CollideWith(T_Sprite* target, int distance = 0)override { return false; }
	// ����ͼ��ײ
	virtual bool CollideWith(IN T_Map* map)override;
	//���ݸ������״̬��Ϣ������λ�á�֡ͼ������ͼ��ײ
	virtual void update();

	virtual void Draw(HDC hdc);
};

