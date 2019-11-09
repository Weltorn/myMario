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
		break;
	case MINION_KOOPA:
		break;
	}
	return minion;
}