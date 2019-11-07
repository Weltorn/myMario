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

	//�˶����Ʒ���
	// ----- MOVE
	void startMove();			//����Ϊ�����ƶ�״̬
	
	void startSpeedup();		//����Ϊ����״̬
	void Player::resetSpeedup();//�ָ�Ϊ�����ƶ�״̬

	void stopMove(bool immediately);			//ֹͣˮƽ�ƶ���immediately���Ƿ񺬹��Դ���

	//------JUMP
	void startJump(int iH);
	void resetJump();

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

