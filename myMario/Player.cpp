#include "Player.h"
#include "T_Scene.h"
#include "GameMap.h"
#include "Util.h"
Player::Player(LPCTSTR imgPath, int frameWidth, int frameHeight)
	:T_Sprite(imgPath, frameWidth, frameHeight)
{
	// ----- role status
	lifeCount = 3;
	isInEnvnt = false;
	eventId = -1;
	playerStatus = PLAYER_NONE;		//��ɫģʽ
	starStatus = false;					//�Ƿ��޵У����ǣ�״̬
	
	//------------------------------------------------------------��ʼ��λ�ã��ݲ�ȷ��
	X;
	Y;
	speedX = 0;
	speedY = -1;

	// ----- MOVE STATUS
	bMove = false;		//�Ƿ�ˮƽ�ƶ�״̬
	bSquat = false;		//�Ƿ��¶�״̬
	bJump = false;		//�Ƿ���Ծ״̬

	// -----JUMP STATUS
	jumpStatus =-1;		//��Ծ״̬0��������1���½�
	isBooting = false;	//�Ƿ���Ծ����״̬

	// ----- MOVE--------------------------------------------------�ݲ�ȷ��������ʼ��	
	friction = 2.0;		//ˮƽĦ�������ƹ��Ի��о���
	dir = DIR_RIGHT;		//��ʼ������,����

	// ----- JUMP--------------------------------------------------�ݲ�ȷ��������ʼ��
	gravity = 9.8;				//�����������ٶ�
	timer = GetTickCount();
	
}


Player::~Player()
{
}
//������Һ�����
void Player::updatePositionX()
{
	Util::myprintf(L"current speedx: %d\n", speedX);
	Util::myprintf(L"current X: %d\n", X);
	if (!bSquat)		//���¶�״̬�¿�ˮƽ�ƶ�
	{
		if (!bMove)		//ˮƽ��ֹ����Ի���״̬
		{
			if (bSlide && (currentMaxSpeedX - speedX)*20 + endTime <= GetTickCount()){		//���Ի���״̬������
				speedX-=(int)friction;
			
				if (speedX < 0)			//�ָ�ˮƽ��ֹ
				{
					speedX = 0;
					bSlide = false;
				}
			}
			
		}
		else if(bMove)
		{
			if ((100 + 35 * speedX) + startTime <= GetTickCount() && speedX < currentMaxSpeedX)		//���ٹ���,100ms�����
			{
				++speedX;
			}
		}

		int ispeedX;
		//���ݷ��������ٶȷ���
		if (dir == DIR_LEFT)
			ispeedX = -abs(speedX);
		else if (dir == DIR_RIGHT)
			ispeedX = abs(speedX);

		lastX = X;
		X += ispeedX;
	}
}
//�������������
void Player::updatePositionY()
{
	gravityEffect();		//��������
	lastY = Y;
	Y = Y - speedY;

}
//��ʼ��Ծ
void Player::startJump()
{
	timer = GetTickCount();		//��¼����ʱ�䣬���ڿ��Ƽ���ʱ��(isBooting)

	bJump = true;		//������Ծ״̬
	jumpStatus = 0;		//����Ϊ�����׶�
	isBooting = true;
	speedY = currentMode->basicJumpSpeedY;	//������Ծ��ʼ�ٶ�
}
//���
void Player::resetJump()
{
	bJump = false;			//������Ծ״̬
	jumpStatus = -1;		//����Ծ״̬
	speedY = 0;				//������ֱ�ٶ�����
	isBooting = false;		//����״̬����

	//stopMove(false);	//ˮƽ��ֹ,�й���
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
		currentGravity = gravity*0.8f;	//�������󣬼ӿ�����
	}		
	//if (bJump)
	{
		if (!isOnPlantform && (GetTickCount() - timer)* currentGravity / 350 > abs(speedY) )
		{
			speedY -= 2;	//��������
		}
		if (speedY < 0)		//�ٶ�С���㣨���£�������Ϊ����״̬
		{
			jumpStatus = 1;
		}
	}
	Util::myprintf(L"current SpeedY: %d\n", speedY);
	
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
	if (dir == DIR_LEFT)
	{
		frameRotate = TRANS_HFLIP_NOROT;
	}
	else if (dir == DIR_RIGHT)
	{
		frameRotate = TRANS_NONE;
	}
	LoopFrame();
}
void Player::update()
{
	if (isBooting && timer +currentMode->maxBootTime <= GetTickCount())	//��Ծ����ʱ�����
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
	currentMaxSpeedX = currentMode->maxMoveSpeedX;
	speedX = currentMaxSpeedX;
	bMove = true;
}

//��סshift������߼�������
void Player::startSpeedup() {
	currentMaxSpeedX = currentMode->maxRunSpeedX;
}

//�ɿ�shift�����ָ���������
//����ǰ�ٶȴ��������ƶ��ٶ����ޣ����ٶ�����Ϊ�����ƶ�����
void Player::resetSpeedup() {
	currentMaxSpeedX = currentMode->maxMoveSpeedX;
	if (speedX > currentMaxSpeedX)
	{
		speedX = currentMaxSpeedX;
	}
}
//ֹͣˮƽ�ƶ�
void Player::stopMove(bool immediately) {
	endTime = GetTickCount();
	
	if (speedX <= 2||immediately)
	{
		speedX = 0;
		bMove = false;
	}
	else
	{
		bMove = false;
		bSlide = true;
	}
}

void Player::initBigRedMode(PLAYERMODE* bigRedMode)
{
	this->bigRedMode = (PLAYERMODE*)malloc(sizeof(PLAYERMODE));
	memcpy(this->bigRedMode, bigRedMode, sizeof(PLAYERMODE));

}
void Player::initNormalMode(PLAYERMODE* normalMode)
{
	this->normalMode = (PLAYERMODE*)malloc(sizeof(PLAYERMODE));
	memcpy(this->normalMode, normalMode, sizeof(PLAYERMODE));
}
void  Player::setPlayerMode(PLAYERSTATUS status)
{
	if (playerStatus == status) {
		return;
	}
	switch (status)
	{
	case PLAYER_NORMAL:
	{		
		if (normalMode == NULL) {
			Util::myprintf(L"Player::setPlayerMode : normalMode == NULL\n");
		}
		currentMode = normalMode;
		playerStatus = status;
		break;
	}
	case PLAYER_BIGRED:
	{
		if (bigRedMode == NULL) {
			Util::myprintf(L"Player::setPlayerMode : bigRedMode == NULL\n");
		}
		currentMode = bigRedMode;
		playerStatus = status;
		break;
	}	
	}
	SetImage(&(currentMode->frameMode.img));
	SetWidth(currentMode->frameMode.frameWidth);
	SetHeight(currentMode->frameMode.frameHeight);

	frameCols = currentMode->frameMode.img.GetImageWidth() / GetWidth();		// ����֡ͼƬ������
	frameRows = currentMode->frameMode.img.GetImageHeight() / GetHeight();		// ����֡ͼƬ������
	
	rawFrames = frameCols*frameRows;							// ��¼ԭʼ������֡��
	forward = 0;												// ��ǰ֡������ʼ��
	backward = totalFrames - 1;

	frameSequence = currentMode->frameMode.runFrmSequence;
	totalFrames = currentMode->frameMode.nRunFrames;			// ������֡��
	loopForward = true;

	//�ָ���ֹ״̬		
	stopMove(true);
	resetJump();
	setSquat(false);
}
void Player::Draw(HDC hdc) {
	int frmIndex = -1;
	
	if (!bMove && !bJump && !bSquat)
	{
		if (bSlide)		//��ͣ֡
		{
			frmIndex = currentMode->frameMode.speedDownFrame;
		}
		else {					//��ֹ֡
			frmIndex = currentMode->frameMode.stopFrame;
		}
	}
	else if (currentMode->canSquat && bSquat)
	{
		frmIndex = currentMode->frameMode.squatHeight;
	}	
	else if (bMove && !bJump)
	{
		frmIndex = frameSequence[forward];		
	}
	else if (bJump)
	{
		frmIndex = 5;
	}		


	Util::myprintf(L"current frame: %d\n",frmIndex);
	if (bSquat)
	{
		spImg.PaintRegion(spImg.GetBmpHandle(),hdc,X,Y,currentMode->frameMode.frameWidth *frmIndex, 
			currentMode->frameMode.frameHeight - currentMode->frameMode.squatHeight,
			currentMode->frameMode.frameHeight, currentMode->frameMode.squatHeight,frameRatio, frameRotate, frameAlpha);
	}
	else if (bJump)
	{
		spImg.PaintRegion(spImg.GetBmpHandle(), hdc, X, Y, Width*frmIndex, 0, Width, Height,
			frameRatio, frameRotate, frameAlpha);
	}
	else
	{
		spImg.PaintFrame(
			spImg.GetBmpHandle(), hdc, (int)X, (int)Y, frmIndex,
			frameCols, Width, Height, frameRatio, frameRotate, frameAlpha
		);
	}
	
}
bool Player::CollideWith(IN T_Map* map)
{
	bool isCollide = false;
	// �������ΪͼƬ�򲻼���ͼ��ײ
	if (map->getMapRows() == 0 && map->getMapCols() == 0)
	{
		mapBlockPT.x = -1;
		mapBlockPT.y = -1;
		return false;
	}

	// �����ͼ���ɼ����ɫ���ɼ�������ͼ��ײ
	if ((!(map->IsVisible())) || (!(this->IsVisible())))
	{
		mapBlockPT.x = -1;
		mapBlockPT.y = -1;
		return false;
	}

	// ���㵱ǰ��ͼͼ��ľ��η�Χ
	int mapLeft = map->GetX();
	int mapTop = map->GetY();
	int mapRight = mapLeft + map->GetWidth();
	int mapBottom = mapTop + map->GetHeight();

	// ��õ�ͼͼ����ʹ�õ�ͼ��Ŀ��
	int tW = map->getTileWidth();
	int tH = map->getTileHeight();

	// ���㵱ǰ��ɫ�ľ��η�Χ����ײ����
	int spLeft = this->GetCollideRect()->left;
	int spTop = this->GetCollideRect()->top;
	int spRight = this->GetCollideRect()->right;
	int spBottom = this->GetCollideRect()->bottom;

	// ��õ�ǰ��ͼ��ͼ���������
	int tNumCols = map->getMapCols();
	// ��õ�ǰ��ͼ��ͼ���������
	int tNumRows = map->getMapRows();

	// ���㵱ǰ��ɫ��ʵ�ʵĿ��
	int spW = spRight - spLeft;
	int spH = spBottom - spTop;

	RECT lprcDst;
	// �������ϼ����ͼ��ľ��η�Χ�ͽ�ɫ�ľ��η�Χ�����������ζ���
	RECT mapRect = { mapLeft, mapTop, mapRight, mapBottom };
	RECT spRect = { spLeft, spTop, spRight, spBottom };
	// ����������ζ���û�з����κ���ײ�����˳�����
	if ((IntersectRect(&lprcDst, &mapRect, &spRect)) == FALSE)
	{
		mapBlockPT.x = -1;
		mapBlockPT.y = -1;
		return false;
	}

	// ����������ζ���������ײ�����ȼ����ɫ�����ϡ��¡����ҵľ�������
	int startRow = (spTop <= mapTop) ? 0 : (spTop - mapTop) / tH;
	int endRow = (spBottom < mapBottom) ? (spBottom - 1 - mapTop) / tH : tNumRows - 1;	//-1����Ե�ж�
	int startCol = (spLeft <= mapLeft) ? 0 : (spLeft - mapLeft) / tW;
	int endCol = (spRight < mapRight) ? (spRight - 1 - mapLeft) / tW : tNumCols - 1;

	isOnPlantform = false;
	COLLIDBLOCKS collideBlocks;
	// ���ݽ�ɫ�����ϡ��¡����ҵľ��������ж��ĸ���������Ϊ�ϰ�
	for (int row = startRow; row <= endRow; ++row)
	{
		for (int col = startCol; col <= endCol; ++col)
		{
			// �����ǰ�������ڵ�ͼ���ڵ�ͼ������Ϊ��0���ͱ�ʾ���ϰ�
			if (map->getTile(col, row) != 0)
			{
				isCollide = true;
				mapBlockPT.x = col;	// ��¼��ǰ�ϰ�ͼ�����
 				mapBlockPT.y = row;	// ��¼��ǰ�ϰ�ͼ�����

				COLLIDBLOCK block;

				//��ײ�ĵ�ͼ��
				RECT blockRect = { col*map->getTileWidth()+(map->GetX()) ,row*map->getTileHeight()+(map->GetY()),
					(col+1)*map->getTileWidth() + (map->GetX()),(row+1)*map->getTileHeight() + (map->GetY()) };

				int x = GetX(), y = GetY();		
				GAME_DIR DIR = getCollideDir(blockRect);
				switch (DIR)
				{
				case DIR_LEFT:
					x = map->GetX() + (col + 1)*map->getTileWidth();	//�����ϰ��Ҳ�
					y = GetY();
					speedX = 0;
					block = { col ,row ,DIR_RIGHT};		//���淢����ײ�ĵ�ͼ������
					collideBlocks.push_back(block);
					break;
				case DIR_RIGHT:
					x = map->GetX() + col*map->getTileWidth() - GetWidth();  //�����ϰ����
					y = GetY();
					speedX = 0;
					block = { col ,row ,DIR_LEFT };		//���淢����ײ�ĵ�ͼ������
					collideBlocks.push_back(block);
					break;
				case DIR_UP:
					x = GetX();
					y = map->GetY()+(row + 1)*map->getTileHeight();		//�����ϰ��²�
					speedY = -speedY;
					block = { col ,row ,DIR_DOWN };		//���淢����ײ�ĵ�ͼ������
					collideBlocks.push_back(block);
					break;
				case DIR_DOWN:
					x = GetX();
					y = map->GetY() + row*map->getTileHeight() - GetHeight();  //�����ϰ��ϲ�
					isOnPlantform = true;
					resetJump();
					block = { col ,row ,DIR_UP };		//���淢����ײ�ĵ�ͼ������
					collideBlocks.push_back(block);
					break;
				}
				// ����ɫ��λ���ϰ���߽�
				SetPosition(x, y);
			}
		}
	}
	(dynamic_cast<GameMap*>(map))->setCollideBlocks(collideBlocks); //ˢ����ײ��ͼ��
	return isCollide;
}
GAME_DIR Player::getCollideDir(RECT target)
{
	RECT oldRect = *this->GetCollideRect();
	RECT currentRect = *this->GetCollideRect();
	oldRect.left = this->GetCollideRect()->left-(X-lastX);
	oldRect.right = this->GetCollideRect()->right - (X - lastX);
	oldRect.top = this->GetCollideRect()->top -(Y-lastY);
	oldRect.bottom = this->GetCollideRect()->bottom - (Y - lastY);
	
 	if (oldRect.top >= target.bottom && this->GetCollideRect()->top<=target.bottom )
	{
		return DIR_UP;
	}
	if (oldRect.bottom <=target.top && this->GetCollideRect()->bottom >= target.top)
	{
		return DIR_DOWN;
	}
	if (oldRect.left >= target.right && this->GetCollideRect()->left <= target.right)
	{
		return DIR_LEFT;
	}
	if (oldRect.right <= target.left && this->GetCollideRect()->right >= target.left)
	{
		return DIR_RIGHT;
	}
	return DIR_NONE;
}

