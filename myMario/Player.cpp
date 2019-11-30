#include "Player.h"
#include "T_Scene.h"
#include "GameMap.h"
#include "Util.h"
Player::Player(LPCTSTR imgPath, int frameWidth, int frameHeight)
	:T_Sprite(imgPath, frameWidth, frameHeight)
{
	// ----- role status
	lifeCount = 3;
	inEvent = false;
	eventId = -1;
	playerStatus = PLAYER_NONE;		//��ɫģʽ
	starStatus = false;					//�Ƿ��޵У����ǣ�״̬
	
	// ---��ʼ��λ��
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

	// ----- MOVE
	friction = 1.0;		//ˮƽĦ�������ƹ��Ի��о���

	// ----- JUMP
	gravity = 6;				//�����������ٶ�
	jumpTimer = GetTickCount();
	
}


Player::~Player()
{
}
//������Һ�����
void Player::updatePositionX()
{
	Util::myprintf(L"current x: %d\n", X-T_Scene::getBarrier()->GetX());

	if (!bSquat)		//���¶�״̬�¿�ˮƽ�ƶ�
	{
		if (!bMove)		//ˮƽ��ֹ����Ի���״̬
		{
			if (onPlantform && bSlide && (currentMaxSpeedX - speedX)*100 + (int)(moveTimer*friction) <= (int)(GetTickCount()*friction))
			{
				//���Ի���״̬������(���в�����)				
				speedX -= 1;		
				if (speedX <= 0)			//�ָ�ˮƽ��ֹ
				{
					speedX = 0;
					bSlide = false;
					//�������ڼ䳢�Ըı䷽�򣬴�ʱ�޸ķ���
					if (dirChanged) {
						dirChanged = false;
						dir = (dir == DIR_LEFT) ?  DIR_RIGHT : DIR_LEFT;
					}
				}
			}
			if (!onPlantform && bSlide && (currentMaxSpeedX - speedX) * 100 + (int)(moveTimer*friction) <= (int)(GetTickCount()*friction))
			{
				
			}
		}
		else if(bMove)
		{
			if (( 200 * speedX) + moveTimer <= GetTickCount() && speedX < currentMaxSpeedX)		//���ٹ���,ÿ150ms�����
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
		X += ispeedX;
	}
}
//�������������
void Player::updatePositionY()
{
	if (isBooting && jumpTimer + currentMode->maxBootTime <= GetTickCount())	//��Ծ����ʱ�����
	{
		isBooting = false;
	}
	if (onPlantform && jumpStatus == 1)	//���Ϊ���䵽ƽ̨������Ծ״̬
	{
		resetJump();
	}
	
	gravityEffect();		//��������	
	Y = Y - speedY;
	
	if (Y > T_Scene::getBarrier()->GetHeight())
	{
		this->playerDeath(true);
	}
}
//��ʼ��Ծ
void Player::startJump()
{
	jumpTimer = GetTickCount();		//��¼����ʱ�䣬���ڿ��Ƽ���ʱ��(isBooting)
	onPlantform = false;
	bJump = true;		//������Ծ״̬
	jumpStatus = 0;		//����Ϊ�����׶�
	isBooting = true;
	startHeight = GetY();	//������Ծ��ʼ�߶�
	speedY = currentMode->basicJumpSpeedY;	//������Ծ��ʼ�ٶ�
}
//���
void Player::resetJump()
{	
	bJump = false;			//������Ծ״̬
	jumpStatus = -1;		//����Ծ״̬
	speedY = 0;				//������ֱ�ٶ�����
	isBooting = false;		//����״̬����		
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
		if (!onPlantform && (GetTickCount() - jumpTimer)* currentGravity / 270 >currentMode->basicJumpSpeedY- abs(speedY) )
		{
			speedY -= 1;	//��������
		}
		if (speedY < 0)		//�ٶ�С���㣨���£�������Ϊ����״̬
		{
			jumpStatus = 1;
		}
		
	//Util::myprintf(L"current SpeedY: %d\n", speedY);
	
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
}
//����֡ͼ
void Player::updateFrame()
{
	//�������
	if (dir == DIR_LEFT)
	{
		
		if (bSlide&&dirChanged)		//��ͣ֡
		{
			frameRotate = TRANS_NONE;
		}
		else
		{
			frameRotate = TRANS_HFLIP_NOROT;
		}
	}
	else if (dir == DIR_RIGHT)
	{
		if (bSlide&&dirChanged)		//��ͣ֡
		{
			frameRotate = TRANS_HFLIP_NOROT;
		}
		else
		{
			frameRotate = TRANS_NONE;
		}
	}

	//֡ͼѡ��
	if (!bMove && !bJump && !bSquat)
	{
		if (bSlide&&dirChanged)		//��ͣ֡
		{
			currentFrmIndex = currentMode->frameMode.speedDownFrame;
		}
		else {					//��ֹ֡
			currentFrmIndex = currentMode->frameMode.stopFrame;
		}
	}
	if (currentMode->canSquat && bSquat)
	{
		currentFrmIndex = currentMode->frameMode.squatFrame;
	}
	if ((bMove && !bJump) || (bSlide && !dirChanged))
	{
		LoopFrame();
		currentFrmIndex = frameSequence[forward];
	}
	if (bJump)
	{
		currentFrmIndex = currentMode->frameMode.jumpFrame;
	}	
}
void Player::update()
{
	if (!inEvent)
	{
		checkOnplantForm(T_Scene::getBarrier());		
		updatePosition();	//�����������
		CollideWith(T_Scene::getBarrier());	//������ϰ�����ײ���
		updateFrame();		//����֡ͼ
	}
	else
	{
		playAnimation();
	}
	
}

//���������ƶ�״̬�����ٶȺ��ٶ�����
void Player::startMove() {
	moveTimer = GetTickCount();

	currentMaxSpeedX = currentMode->maxMoveSpeedX;
	speedX = 0;
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
	moveTimer = GetTickCount();
	if (immediately)
	{
		speedX = 0;
		bMove = false;
	}
	else
	{
		if (!bJump)
		{
			if (speedX <= 3)
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
		else
		{
			if (speedX > 0)
			{			
				bMove = false;
				bSlide = true;
			}
			else
			{
				bMove = false;
				speedX = 0;
			}
		}
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

	//����T_Sprite����
	SetImage(&(currentMode->frameMode.img));
	SetWidth(currentMode->frameMode.frameWidth);
	SetHeight(currentMode->frameMode.frameHeight);
	colideWidth = GetRatioSize().cx;
	colideHeight = GetRatioSize().cy;

	frameCols = currentMode->frameMode.img.GetImageWidth() / GetWidth();		// ����֡ͼƬ������
	frameRows = currentMode->frameMode.img.GetImageHeight() / GetHeight();		// ����֡ͼƬ������
	
	rawFrames = frameCols*frameRows;							// ��¼ԭʼ������֡��
	forward = 0;												// ��ǰ֡������ʼ��
	backward = totalFrames - 1;

	//��ʼ���˶�֡
	SetSequence(currentMode->frameMode.runFrmSequence, currentMode->frameMode.nRunFrames);	
	loopForward = true;
	
	//�ָ���ֹ״̬		
	stopMove(true);
	resetJump();
	setSquat(false);
}
void Player::Draw(HDC hdc) {
	lastX = X;
	lastY = Y;

	Util::myprintf(L"current frame %d\n",currentFrmIndex);
	if (bSquat)
	{
		spImg.PaintRegion(spImg.GetBmpHandle(),hdc,X,Y,currentMode->frameMode.frameWidth *currentFrmIndex,
			currentMode->frameMode.frameHeight - currentMode->frameMode.squatHeight,
			currentMode->frameMode.frameWidth, currentMode->frameMode.squatHeight,frameRatio, frameRotate, frameAlpha);
	}
	else if (bJump)
	{
		spImg.PaintRegion(spImg.GetBmpHandle(), hdc, X, Y, Width*currentFrmIndex, 0, Width, Height,
			frameRatio, frameRotate, frameAlpha);
	}
	else
	{
		spImg.PaintFrame(
			spImg.GetBmpHandle(), hdc, (int)X, (int)Y, currentFrmIndex,
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

	//��ͼ���ұ߽���ײ
	if (this->X <= mapLeft)
	{
		this->X = mapLeft;
	}
	if (this->X+this->GetRatioSize().cx >= mapRight)
	{
		this->X = mapRight - this->GetRatioSize().cx-1;
	}
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
					x = map->GetX() + (col+1)*map->getTileWidth();
					y = GetY();
					speedX = 0;					
					block = { col ,row ,DIR_RIGHT};		//���淢����ײ�ĵ�ͼ������
					//collideBlocks.push_back(block);
					break;
				case DIR_RIGHT:					
					x = map->GetX() + col*map->getTileWidth()-GetRatioSize().cx;
					y = GetY();
					speedX = 0;				
					block = { col ,row ,DIR_LEFT };		//���淢����ײ�ĵ�ͼ������
					//collideBlocks.push_back(block);
					break;
				case DIR_UP:
					x = GetX();
					y = map->GetY()+(row + 1)*map->getTileHeight();		//�����ϰ��²�
					speedY = -abs(speedY);
					block = { col ,row ,DIR_DOWN };	
					//���淢����ײ�ĵ�ͼ������
					collideBlocks.push_back(block);
					break;
				case DIR_DOWN:
					x = GetX();
					y = map->GetY() + (row)*map->getTileHeight() - GetRatioSize().cy;  //�����ϰ��ϲ�
					onPlantform = true;			
					resetJump();
					block = { col ,row ,DIR_UP };		//���淢����ײ�ĵ�ͼ������
					//collideBlocks.push_back(block);
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
bool Player::checkOnplantForm(T_Map* map)
{
	if (jumpStatus == 0) {
		onPlantform = false;
		return onPlantform;
	}
	if ((Y + GetRatioSize().cy - map->GetY()) % map->getTileHeight() < 2)
	{
		//����·��ĵ�ͼ��
		int row = (Y + GetRatioSize().cy - map->GetY()) / map->getTileHeight();		
		int startCol = (X - map->GetX()+1) / map->getTileWidth();
		int endCol = (X + GetRatioSize().cx - map->GetX()-1) / map->getTileWidth();

		//�Ƿ񳬳���ͼ��Χ
		if (row > map->getMapRows()-1 || row < 0|| startCol<0|| endCol>map->getMapCols()-1)
		{
			onPlantform = false;
			return onPlantform;
		}
		//�������·��ĵ�ͼ��
		for (int i = startCol; i <= endCol; i++)
		{
			if (map->getTile(i, row) != 0)
			{
				onPlantform = true;
				return onPlantform;
			}
		}
	}
	onPlantform = false;
	return onPlantform;
}

void Player::startEvent(int eventId)
{	
	this->eventId = eventId;
	inEvent = true;
	currentStep = 0;
	eventTimer = GetTickCount();
	
	//����˶�״̬
	onPlantform = false;
	stopMove(true);
	resetJump();
	setSquat(false);
}
void Player::playAnimation()
{
	switch (eventId)
	{
	case PLAYER_DEATH:
		deathAnimation();
		break;
	case PLAYER_LEVELUP:
		levelUpAnimation();
		break;
	case PLAYER_LEVELDOWN:
		break;
	case PLAYER_AFTERPOLE:
		break;
	default:
		inEvent = false;
		eventId = -1;
		break;
	}
}

void Player::deathAnimation()
{
	currentFrmIndex = currentMode->frameMode.deathFrame;	//��������֡
	switch (currentStep)
	{
	case 0:
		if (eventTimer + 500 <= GetTickCount())		//��ֹ0.5��
		{
			++currentStep;
		}
		break;
	case 1:
		speedY = 15;	
		++currentStep;
		break;
	case 2:
		updatePosition();
		if (Y - GetRatioSize().cy >= T_Scene::getBarrier()->GetHeight())
		{
			++currentStep;
		}
		break;
	default:
		playerDeath(true);		//��������״̬
		break;
	}
}
void Player::levelUpAnimation()
{	
	switch (currentStep)
	{
	case 0:
		Y -= bigRedMode->frameMode.frameHeight - Height;
		setPlayerMode(PLAYER_BIGRED);
		SetSequence(currentMode->frameMode.levelUpFrmSequence , currentMode->frameMode.nlevelUpFrames);
		forward = 0;		
		SetAlpha(200);
		++currentStep;
		break;
	case 1:
		if (eventTimer + 1200 >= GetTickCount())
		{
			LoopFrame(6);
			currentFrmIndex = frameSequence[forward];
		}
		else
		{
			currentFrmIndex = currentMode->frameMode.stopFrame;
			++currentStep;
		}
		break;
	default:
		SetAlpha(255);
		SetSequence(currentMode->frameMode.runFrmSequence, currentMode->frameMode.nRunFrames);
		forward = 0;
		stopEvent();
		break;
	}
}
void Player::playerDeath(bool immediately)
{
	if (immediately)
	{
		dead = true;
		if(lifeCount>0)
		{ 
			lifeCount--;
		}
	}
	else
	{
		active = false;
		startEvent(PLAYER_DEATH);
	}
}