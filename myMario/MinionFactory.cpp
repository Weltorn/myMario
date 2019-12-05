#include "MinionFactory.h"
#include "GameScene.h"



MinionFactory::MinionFactory()
{
}


MinionFactory::~MinionFactory()
{
}

Minion* MinionFactory::getMinion(MINION_TYPE type, int px, int py)
{
	Minion* minion=NULL;
	SPRITEINFO minion_Info;
	int starSequence[1] = { 0 };
	int mushRoomSequence[1] = { 0 };
	int GoombaSequence[2] = { 0,1 };
	int fireBallSequence[4] = { 0,1,2,3 };
	int flowerSequence[4] = { 0,1,2,3 };
	switch (type)
	{
	case MINION_STAR:	
	{
		minion = new Star(L".\\res\\sprite\\minion\\star.png", 28, 32);
		minion_Info.Active = true;
		minion_Info.Dead = false;
		minion_Info.Dir = DIR_NONE;
		minion_Info.Rotation = TRANS_NONE;
		minion_Info.Ratio = 1.0f;
		minion_Info.Level = 0;
		minion_Info.Score = 0;
		minion_Info.SpeedX = 3;
		minion_Info.SpeedY = 0;
		minion_Info.Alpha = 255;
		minion_Info.X = px;
		minion_Info.Y = py;
		minion_Info.Visible = true;
		minion->Initiate(minion_Info);
		minion->SetSequence(starSequence, 1);
		minion->SetLayerTypeID(LAYER_TYPE::LAYER_AWARD);
		minion->setMinionType(MINION_TYPE::MINION_STAR);
	}
		break;
	case MINION_REDMUSHROOM:
		minion = new RedMushroom(L".\\res\\sprite\\minion\\redMushroom.png", 32, 32);
		minion_Info.Active = true;
		minion_Info.Dead = false;
		minion_Info.Dir = DIR_RIGHT;
		minion_Info.Rotation = TRANS_NONE;
		minion_Info.Ratio = 1.0f;
		minion_Info.Level = 0;
		minion_Info.Score = 0;
		minion_Info.SpeedX = 3;
		minion_Info.SpeedY = 0;
		minion_Info.Alpha = 255;
		minion_Info.X = px;
		minion_Info.Y = py;
		minion_Info.Visible = true;
		minion->Initiate(minion_Info);
		minion->SetSequence(mushRoomSequence, 1);
		minion->SetLayerTypeID(LAYER_TYPE::LAYER_AWARD);
		minion->setMinionType(MINION_TYPE::MINION_REDMUSHROOM);
		break;
	case MINION_GREENMUSHROOM:
		break;
	case MINION_FLOWER:
	{
		minion = new Star(L".\\res\\sprite\\minion\\flower.png", 28, 32);
		minion_Info.Active = true;
		minion_Info.Dead = false;
		minion_Info.Dir = DIR_NONE;
		minion_Info.Rotation = TRANS_NONE;
		minion_Info.Ratio = 1.0f;
		minion_Info.Level = 0;
		minion_Info.Score = 0;
		minion_Info.SpeedX = 3;
		minion_Info.SpeedY = 0;
		minion_Info.Alpha = 255;
		minion_Info.X = px;
		minion_Info.Y = py;
		minion_Info.Visible = true;
		minion->Initiate(minion_Info);
		minion->SetSequence(flowerSequence, 4);
		minion->SetLayerTypeID(LAYER_TYPE::LAYER_AWARD);
		minion->setMinionType(MINION_TYPE::MINION_FLOWER);
	}
		break;
	case MINION_GOOMBA:
	{
		minion = new Goomba(L".\\res\\sprite\\minion\\goomba.png", 32, 32);
		minion_Info.Active = true;
		minion_Info.Dead = false;
		minion_Info.Dir = DIR_RIGHT;
		minion_Info.Rotation = TRANS_NONE;
		minion_Info.Ratio = 1.0f;
		minion_Info.Level = 0;
		minion_Info.Score = 0;
		minion_Info.SpeedX = 1;
		minion_Info.SpeedY = 0;
		minion_Info.Alpha = 255;
		minion_Info.X = px;
		minion_Info.Y = py;
		minion_Info.Visible = true;
		minion->Initiate(minion_Info);
		minion->SetSequence(GoombaSequence, 2);
		minion->SetLayerTypeID(LAYER_TYPE::LAYER_NPC);
		minion->setMinionType(MINION_TYPE::MINION_GOOMBA);
	}		
		break;
	case MINION_KOOPA:
	{
		minion = new Koopa(L".\\res\\sprite\\minion\\koopa.png", 32, 48);
		minion_Info.Active = true;
		minion_Info.Dead = false;
		minion_Info.Dir = DIR_RIGHT;
		minion_Info.Rotation = TRANS_NONE;
		minion_Info.Ratio = 1.0f;
		minion_Info.Level = 0;
		minion_Info.Score = 0;
		minion_Info.SpeedX = 1;
		minion_Info.SpeedY = 0;
		minion_Info.Alpha = 255;
		minion_Info.X = px;
		minion_Info.Y = py;
		minion_Info.Visible = true;
		minion->Initiate(minion_Info);
		minion->SetSequence(GoombaSequence, 2);
		minion->SetLayerTypeID(LAYER_TYPE::LAYER_NPC);
		minion->setMinionType(MINION_TYPE::MINION_KOOPA);
	}
		break;
	case MINION_PLAYERBULLET:
	{
		minion = new PlayerBullet(L".\\res\\sprite\\fireball.png", 32, 32);
		minion_Info.Active = true;
		minion_Info.Dead = false;
		minion_Info.Dir = DIR_NONE;
		minion_Info.Rotation = TRANS_NONE;
		minion_Info.Ratio = 1.0f;
		minion_Info.Level = 0;
		minion_Info.Score = 0;
		minion_Info.SpeedX = 8;
		minion_Info.SpeedY = -3;
		minion_Info.Alpha = 255;
		minion_Info.X = px;
		minion_Info.Y = py;
		minion_Info.Visible = true;
		minion->Initiate(minion_Info);
		minion->SetSequence(fireBallSequence, 4);
		minion->AdjustCollideRect(-8,-8);		//µ÷ÕûÅö×²Çø
		minion->SetLayerTypeID(LAYER_TYPE::LAYER_PLY_BOMB);
		minion->setMinionType(MINION_TYPE::MINION_PLAYERBULLET);
	}
		break;
	}
	return minion;
}