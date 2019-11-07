#include "Player.h"
#include "T_Scene.h"
Player::Player(LPCTSTR imgPath, int frameWidth, int frameHeight)
	:T_Sprite(imgPath, frameWidth, frameHeight)
{

}


Player::~Player()
{
}
//更新玩家坐标
void Player::updatePosition()
{
	CollideWith(T_Scene::getBarrier());	//玩家与障碍层碰撞检测
}
//更新帧图
void Player::updateFrame()
{

}
void Player::update()
{
	updatePosition();	//更新玩家坐标
	updateFrame();		//更新帧图
}

//设置为正常移动状态
void Player::startMove() {
	startTime = GetTickCount();
	endTime = GetTickCount();
	speedX = 1;
	currentMaxSpeed = maxMoveSpeedX;
	bMove = true;
}

//按住shift键，提高加速上限
void Player::startSpeedup() {
	currentMaxSpeed = maxRunSpeedX;
}

//松开shift键，恢复加速上限
//若当前速度大于正常移动速度上限，将速度设置为正常移动上限
void Player::resetSpeedup() {
	currentMaxSpeed = maxMoveSpeedX;
	if (speedX > maxMoveSpeedX)
	{
		speedX = maxMoveSpeedX;
	}
}
//停止水平移动
void Player::stopMove(bool immediately) {
	if (immediately)
	{
		speedX = 0;
		bMove = false;
	}
	else
	{
		--speedX;		//逐渐减速
		bMove = false;
	}
	
}

