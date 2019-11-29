#include "BrickFactory.h"
#include "NormalBrick.h"
#include "PropBrick.h"



BrickFactory::BrickFactory()
{
}


BrickFactory::~BrickFactory()
{
}

Brick* BrickFactory::getBrick(BRICK_TYPE type, int px, int py)
{
	Brick* brick = NULL;
	SPRITEINFO info;
	switch (type)
	{
	case NORMAL_BRICK:
		brick = new NormalBrick(L".\\res\\map\\normalBrick.png",32,32);

		info.X = px;									
		info.Y = py;
		info.Dir = 0;
		info.Rotation = 0;
		info.Ratio = 1;
		info.Alpha = 255;
		info.Visible = true;
		brick->Initiate(info);

		break;
	case PROP_BRICK:

		break;
	}
	return brick;
}
