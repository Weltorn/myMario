#include "Player.h"
#include "T_Scene.h"
Player::Player(LPCTSTR imgPath, int frameWidth, int frameHeight)
	:T_Sprite(imgPath, frameWidth, frameHeight)
{

}


Player::~Player()
{
}

void Player::update()
{

	CollideWith(T_Scene::getBarrier());	//Íæ¼ÒÓëÕÏ°­²ãÅö×²¼ì²â
}
