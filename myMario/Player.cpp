#include "Player.h"
#include "T_Scene.h"
Player::Player(LPCTSTR imgPath, int frameWidth, int frameHeight)
	:T_Sprite(imgPath, frameWidth, frameHeight)
{

}


Player::~Player()
{
}
//�����������
void Player::updatePosition()
{
	//���ݷ��������ٶȷ���
	int ispeedX;
	int ispeedY;
	if (dir = DIR_LEFT)
		ispeedX = -abs(speedX);
	else if(dir = DIR_RIGHT)
		ispeedX = abs(speedX);

	if (!bSquat)
	{
		if (bMove)
		{
			X += ispeedX;
		}
	}

	CollideWith(T_Scene::getBarrier());	//������ϰ�����ײ���
}
//����֡ͼ
void Player::updateFrame()
{

}
void Player::update()
{
	updatePosition();	//�����������
	updateFrame();		//����֡ͼ
}

//����Ϊ�����ƶ�״̬�����ٶȺ��ٶ�����
void Player::startMove() {
	startTime = GetTickCount();
	endTime = GetTickCount();
	speedX = 1;
	currentMaxSpeed = maxMoveSpeedX;
	bMove = true;
}

//��סshift������߼�������
void Player::startSpeedup() {
	currentMaxSpeed = maxRunSpeedX;
}

//�ɿ�shift�����ָ���������
//����ǰ�ٶȴ��������ƶ��ٶ����ޣ����ٶ�����Ϊ�����ƶ�����
void Player::resetSpeedup() {
	currentMaxSpeed = maxMoveSpeedX;
	if (speedX > maxMoveSpeedX)
	{
		speedX = maxMoveSpeedX;
	}
}
//ֹͣˮƽ�ƶ�
void Player::stopMove(bool immediately) {
	if (immediately)
	{
		speedX = 0;
		bMove = false;
	}
	else
	{
		--speedX;		//�𽥼���
		bMove = false;
	}
	
}

