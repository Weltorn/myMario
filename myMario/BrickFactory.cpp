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

		info.X = px*32;									
		info.Y = py*32;
		info.Dir = 0;
		info.Rotation = 0;
		info.Ratio = 1;
		info.Active = false;
		info.Alpha = 255;
		info.Visible = true;
		info.Dead = false;
		brick->Initiate(info);
		brick->setCol(px);
		brick->setRow(py);
		break;
	case PROP_BRICK:

		break;
	}
	return brick;
}
