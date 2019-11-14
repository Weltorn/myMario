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

	// ----- MOVE STATUS
	bool bMove;		//�Ƿ��ˮƽ�ƶ�״̬
	bool bSquat;	//�Ƿ��¶�״̬
	bool bJump;		//�Ƿ���Ծ״̬

	// -----JUMP STATUS
	int jumpStatus;	//��Ծ״̬0��������1���½�
	bool isBooting;	//�Ƿ���Ծ����״̬
		
	// ----- MOVE
	bool dirChanged;
	const int maxMoveSpeedX;
	const int maxRunSpeedX;
	int currentMaxSpeed;

	// ----- JUMP
	int originSpeedY;		//��Ծ��ʼ�ٶ�
	int maxBootTime;		//������ʱ�䣨��ס��Ծ������Чʱ�䣩
	int gravity;			//�����������ٶ�



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

	//------JUMP
	void startJump(int iH);
	void resetJump();

	// ----- SQUAT
	bool getSquat() { return bSquat; }
	void setSquat(bool bSquat) { this->bSquat = bSquat; }

	//�����������
	void updatePosition();
	//����֡ͼ
	void updateFrame();
	// ����ɫ��ײ, distance�����ײ�ľ���
	virtual bool CollideWith(T_Sprite* target, int distance = 0)override { return false; }
	// ����ͼ��ײ
	virtual bool CollideWith(IN T_Map* map)override { return false; }
	//���ݸ������״̬��Ϣ������λ�á�֡ͼ������ͼ��ײ
	virtual void update();
};

