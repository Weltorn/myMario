#include "Player.h"
#include "T_Scene.h"
#include "Util.h"
Player::Player(LPCTSTR imgPath, int frameWidth, int frameHeight)
	:T_Sprite(imgPath, frameWidth, frameHeight)
{
	// ----- role status
	lifeCount = 3;
	isInEnvnt = false;
	eventId = -1;
	playerStatus = PLAYER_NORMAL;		//��ɫģʽ
	starStatus = false;					//�Ƿ��޵У����ǣ�״̬
	
	//------------------------------------------------------------��ʼ��λ�ã��ݲ�ȷ��
	X;
	Y;
	speedX = 0;
	speedY = 0;

	// ----- MOVE STATUS
	bMove = false;		//�Ƿ�ˮƽ�ƶ�״̬
	bSquat = false;		//�Ƿ��¶�״̬
	bJump = false;		//�Ƿ���Ծ״̬

	// -----JUMP STATUS
	jumpStatus =-1;		//��Ծ״̬0��������1���½�
	isBooting = false;	//�Ƿ���Ծ����״̬

	// ----- MOVE--------------------------------------------------�ݲ�ȷ��������ʼ��	
	friction = 1.0;		//ˮƽĦ�������ƹ��Ի��о���
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
	Util::myprintf(L"current speedx: %d\n",speedX);
	if (!bSquat)		//���¶�״̬�¿�ˮƽ�ƶ�
	{
		if (!bMove)		//ˮƽ��ֹ����Ի���״̬
		{
			if (speedX > 0 && (currentMaxSpeedX - speedX)*80 <= GetTickCount() - endTime){		//���Ի���״̬������
				speedX-=(int)friction;
			
				if (speedX < 0)			//�ָ�ˮƽ��ֹ
				{
					speedX = 0;
				}
			}
			
		}
		else if(bMove)
		{
			if ((100 + 35 * speedX) <= GetTickCount()- startTime && speedX < currentMaxSpeedX)		//���ٹ���,100ms�����
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
	
	if (Y <= 500) {
		Y = Y - speedY;
	}
	if(Y >= 500)
	{
		Y = 500;
		resetJump();
	}
	Util::myprintf(L"current SpeedY: %d\n",speedY);
	if (speedY < 0)		//�ٶ�С���㣨���£�������Ϊ����״̬
	{
		//bJump = true;		//�����������Ծ���䣬����ʽ��ͬ
		jumpStatus = 1;
	}
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
		currentGravity = gravity*1.0f;		//�������󣬼ӿ�����
	}

	speedY = (int)round(currentMode->basicJumpSpeedY - (float)(GetTickCount()-timer)* currentGravity / 500);	//��������
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
	//CollideWith(T_Scene::getBarrier());	//������ϰ�����ײ���
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

bool Player::CollideWith(IN T_Map* map)
{
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

	// ���ݽ�ɫ�����ϡ��¡����ҵľ��������ж��ĸ���������Ϊ�ϰ�
	for (int row = startRow; row <= endRow; ++row)
	{
		for (int col = startCol; col <= endCol; ++col)
		{
			// �����ǰ�������ڵ�ͼ���ڵ�ͼ������Ϊ��0���ͱ�ʾ���ϰ�
			if (map->getTile(col, row) != 0)
			{
				mapBlockPT.x = col;	// ��¼��ǰ�ϰ�ͼ�����
				mapBlockPT.y = row;	// ��¼��ǰ�ϰ�ͼ�����

				// ���ݽ�ɫ��ǰ�ķ��������ײǰ��λ��
				int x = GetX(), y = GetY();
				switch (GetDir())
				{
				case DIR_LEFT:
					x = (col + 1)*map->getTileWidth() + 1;	//�����ϰ��Ҳ�
					y = GetY();
					break;
				case DIR_RIGHT:
					x = col*map->getTileWidth() - GetWidth() - 1;  //�����ϰ����
					y = GetY();
					break;
				case DIR_UP:
					x = GetX();
					y = (row + 1)*map->getTileHeight() + 1;		//�����ϰ��²�
					break;
				case DIR_DOWN:
					x = GetX();
					y = row*map->getTileHeight() - GetHeight() - 1;  //�����ϰ��ϲ�
					break;
				}
				// ����ɫ��λ���ϰ���߽�
				SetPosition(x, y);
				return true;
			}
		}
	}
	return false;

}
void Player::initBigRedMode(PLAYERMODE* bigRedMode)
{
	this->bigRedMode = (PLAYERMODE*)malloc(sizeof(PLAYERMODE));
	memcpy(this->bigRedMode, bigRedMode, sizeof(PLAYERMODE));

}
void Player::initNormaldMode(PLAYERMODE* normalMode)
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
		break;
	}
	case PLAYER_BIGREDGER:
	{
		if (bigRedMode == NULL) {
			Util::myprintf(L"Player::setPlayerMode : bigRedMode == NULL\n");
		}
		currentMode = bigRedMode;
		break;
	}	
	}
	SetImage(&(currentMode->frameMode->img));
	SetWidth(currentMode->frameMode->frameWidth);
	SetHeight(currentMode->frameMode->frameHeight);

	frameCols = currentMode->frameMode->img.GetImageWidth() / GetWidth();		// ����֡ͼƬ������
	frameRows = currentMode->frameMode->img.GetImageHeight() / GetHeight();		// ����֡ͼƬ������
	
	rawFrames = frameCols*frameRows;							// ��¼ԭʼ������֡��
	forward = 0;												// ��ǰ֡������ʼ��
	backward = totalFrames - 1;

	frameSequence = currentMode->frameMode->runFrmSequence;
	totalFrames = currentMode->frameMode->nRunFrames;			// ������֡��
	loopForward = true;

	//�ָ���ֹ״̬		
	stopMove(true);
	resetJump();
	setSquat(false);
}
void Player::Draw(HDC hdc) {
	int frmIndex;
	
	if (!bMove && !bJump && !bSquat)
	{
		if (speedX != 0)		//��ͣ֡
		{
			frmIndex = currentMode->frameMode->speedDownFrame;
		}
		else {					//��ֹ֡
			frmIndex = currentMode->frameMode->stopFrame;
		}
	}
	else if (currentMode->canSquat && bSquat)
	{
		frmIndex = currentMode->frameMode->squatHeight;
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
		spImg.PaintRegion(spImg.GetBmpHandle(),hdc,X,Y,currentMode->frameMode->frameWidth *frmIndex, 
			currentMode->frameMode->frameHeight - currentMode->frameMode->squatHeight,
			currentMode->frameMode->frameHeight, currentMode->frameMode->squatHeight,frameRatio, frameRotate, frameAlpha);
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

