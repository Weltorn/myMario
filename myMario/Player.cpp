#include "Player.h"
#include "T_Scene.h"
Player::Player(LPCTSTR imgPath, int frameWidth, int frameHeight)
	:T_Sprite(imgPath, frameWidth, frameHeight)
{
	// ----- role status
	lifeCount = 3;
	isInEnvnt = false;
	eventId = -1;
	timer = GetTickCount();

	dir = DIR_RIGHT;		//��ʼ������

	//------------------------------------------------------------��ʼ��λ�ã��ݲ�ȷ��
	X;
	Y;


	// ----- MOVE STATUS
	bMove = false;		//�Ƿ�ˮƽ�ƶ�״̬
	bSquat = false;		//�Ƿ��¶�״̬
	bJump = false;		//�Ƿ���Ծ״̬

	// -----JUMP STATUS
	jumpStatus =-1;	//��Ծ״̬0��������1���½�
	isBooting = false;	//�Ƿ���Ծ����״̬

	// ----- MOVE--------------------------------------------------�ݲ�ȷ��������ʼ��
	maxMoveSpeedX;
	maxRunSpeedX;
	currentMaxSpeed;
	friction;		//ˮƽĦ�������ƹ��Ի��о���

	// ----- JUMP--------------------------------------------------�ݲ�ȷ��������ʼ��
	originJumpSpeedY;		//��Ծ��ʼ�ٶ�
	maxBootTime;		//������ʱ�䣨��ס��Ծ������Чʱ�䣩
	gravity;			//�����������ٶ�
}


Player::~Player()
{
}
//������Һ�����
void Player::updatePositionX()
{
	if (!bSquat)		//���¶�״̬�¿�ˮƽ�ƶ�
	{
		if (!bMove)		//ˮƽ��ֹ����Ի���״̬
		{
			if (speedX > 0){		//���Ի���״̬������
				speedX -= friction;
			}
			else {				//�ָ�ˮƽ��ֹ
				speedX = 0;
			}
		}	

		int ispeedX;
		//���ݷ��������ٶȷ���
		if (dir = DIR_LEFT)
			ispeedX = -abs(speedX);
		else if (dir = DIR_RIGHT)
			ispeedX = abs(speedX);

		X += ispeedX;
	}
}
//�������������
void Player::updatePositionY()
{
	gravityEffect();
	Y = Y + speedY;
	if (speedY <= 0)		//�ٶ�С���㣨���£�������Ϊ����״̬
		jumpStatus = 1;
}
//��ʼ��Ծ
void Player::startJump()
{
	timer = GetTickCount();		//��¼����ʱ�䣬���ڿ��Ƽ���ʱ��(isBooting)

	bJump = true;		//������Ծ״̬
	jumpStatus = 0;		//����Ϊ�����׶�
	isBooting = true;
	speedY = originJumpSpeedY;	//������Ծ��ʼ�ٶ�
}
//���
void Player::resetJump()
{
	bJump = false;			//������Ծ״̬
	jumpStatus = -1;		//����Ծ״̬
	speedY = 0;				//������ֱ�ٶ�����
	isBooting = false;		//����״̬����

	stopMove(false);	//ˮƽ��ֹ,�й���
}
//��������
void  Player::gravityEffect()
{
	float currentGravity = gravity;		//������Ծ
	if (jumpStatus == 0 && isBooting)	//������Ծ
	{
		currentGravity = gravity*0.6f;	//��С�������ȼ����ṩ������
	}
	else if (jumpStatus == 1)			//��Ծ������׶�
	{
		currentGravity = gravity*2;		//�������󣬼ӿ�����
	}
	speedY = (int)round(speedY - gravity);	//��������
}
//�����������
void Player::updatePosition()
{
	if (bSquat)
	{
		return;
	}	
	updatePositionX();
	updatePositionY();
	CollideWith(T_Scene::getBarrier());	//������ϰ�����ײ���
}
//����֡ͼ
void Player::updateFrame()
{
	
}
void Player::update()
{
	if (isBooting && timer + maxBootTime <= GetTickCount())	//��Ծ����ʱ�����
	{
		isBooting = false;
	}
	updatePosition();	//�����������
	updateFrame();		//����֡ͼ
}

//���������ƶ�״̬�����ٶȺ��ٶ�����
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
		bMove = false;
	}
}

