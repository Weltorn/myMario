#include "MinionFactory.h"



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
	int Sequence[12] = { 0,0,0,0,0,0,1,1,1,1,1,1 };
	switch (type)
	{
	case MINION_STAR:		
		break;
	case MINION_REDMUSHROOM:
		break;
	case MINION_GREENMUSHROOM:
		break;
	case MINION_FLOWER:
		break;
	case MINION_GOOMBA:
		minion = new Minion(L".\\res\\sprite\\minion\\goomba.png", 32, 32);		
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
		minion_Info.X = 200;
		minion_Info.Y = 200;
		minion_Info.Visible = true;
		minion->Initiate(minion_Info);
		minion->SetSequence(Sequence, 12);
		minion->SetLayerTypeID(LAYER_TYPE::LAYER_NPC);
		minion->setMinionType(MINION_TYPE::MINION_GOOMBA);
		break;
	case MINION_KOOPA:
		break;
	}
	return minion;
}