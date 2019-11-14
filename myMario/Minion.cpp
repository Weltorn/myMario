#include "Minion.h"



Minion::Minion(LPCTSTR imgPath, int frameWidth, int frameHeight)
	:T_Sprite(imgPath, frameWidth, frameHeight)
{
}
Minion::~Minion()
{
}
bool Minion::CollideWith(IN T_Map* map)
{
	return false;
}
bool Minion::CollideWith(T_Sprite* target, int distance)
{
	return false;
}
void Minion::update()
{}