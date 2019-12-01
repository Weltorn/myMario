#include "MarioGame.h"

// ��Ĺ��캯�� 
MarioGame::MarioGame(HINSTANCE h_instance, LPCTSTR sz_winclass, LPCTSTR sz_title, 
				   WORD icon, WORD sm_icon, int winwidth, int winheight)
:T_Engine(h_instance, sz_winclass, sz_title, icon, sm_icon, winwidth, winheight)
{	
	wnd_width = winwidth;
	wnd_height = winheight;



}

// �����������
MarioGame::~MarioGame(void)
{
	
}

//��Ϸ��ʼ���ӿں���
void MarioGame::GameInit() 
{
	gameLevel = 1;
	GameState = GAME_START;			//���� ABOUT_MENU
	LoadGameLevel(gameLevel);		//���عؿ���Դ����ͼ�����
	LoadGameMenu();					//�������˵�	
}

//��Ϸ�߼�����(����GameState)
void MarioGame::GameLogic()	
{
	switch (GameState)
	{
		case GAME_START:		//��Ϸ��ʼ�˵�
		{
			//gameMenu->update();
			break;
		}
		case GAME_RUN:			//��Ϸ����ʱ����
		{
			GameKeyAction();
			gameTime = GetTickCount();		//������Ϸ������ʱ��	

			//�������	
			if (player->IsVisible())	//δ�����򲥷���������δ������
				player->update();
			if (player->IsDead())	//�����������������������
			{
				if (player->getLifeCount() == 0)
				{
					GameState = GAME_OVER;				//�������ֵΪ0����Ϸ����
				}
				else
				{
					GameState = GAME_UPGRADE;			//�������ֵ��Ϊ0��������Ϸ
					//���ظ����
					playerRelife();
				}
			}
			gameScene->update();						//���µ�ͼ��������״̬
			gameScene->ScrollScene(player);			//�������λ�ã���������
			
			break;
		}
		case GAME_PAUSE:		//��ͣ��Ϸ����
		{
			break;
		}
		case GAME_UPGRADE:		//�¹ؿ����ؽ���
		{
			if (T_Util::Timer(3))	//�ؿ����ؽ���...��Ȼ��ʱ��û����Դ�ں�̨���� ��������Ҫ��ʽ��
				GameState = GAME_RUN;
			break;
		}
		case GAME_OVER:			//��Ϸ��������
		{
			break;
		}
		case GAME_HELP:			//��Ϸ��������
		{
			break;
		}
		case GAME_ABOUT:		//��Ϸ������Ϣ����
		{
			
			//������ɫ�任
			T_Util::ChangeRGB(&red,&green,&blue,&lastRed,&lastBlue,&lastGreen,&changeIndex);
			// ���괦��
			gameScene->ScrollScene(-3);	// �Լ�����
			break;
		}
	}

}

//��Ϸ��ʾ
void MarioGame::GamePaint(HDC hdc)	
{
	switch (GameState)
	{
	case GAME_START:		//��Ϸ��ʼ�˵�
	{
		T_Graph::PaintBlank(hdc, 0, 0, wnd_width, wnd_height, RGB(93, 148, 252), 255);
		gameScene->Draw(hdc);
		gameMenu->DrawMenu(hdc);
		DisplayInfo(hdc);		//��ʾ������Ϸ״̬��Ϣ
		break;
	}
	case GAME_RUN:			//��Ϸ����ʱ����
	{
		T_Graph::PaintBlank(hdc,0,0,wnd_width,wnd_height,RGB(100,200,200),255);
		gameScene->Draw(hdc);
		DisplayInfo(hdc);		//��ʾ������Ϸ״̬��Ϣ

		break;
	}
	case GAME_PAUSE:		//��ͣ��Ϸ����
	{
		//gameScene->Draw(hdc);
		//DisplayInfo(hdc);		//��ʾ������Ϸ״̬��Ϣ
		//gameMenu->DrawMenu(hdc);
		break;
	}
	case GAME_UPGRADE:		//�¹ؿ����ؽ���
	{
		//��ʾ���ؽ���
		DisplayInfo(hdc);		//��ʾ������Ϸ״̬��Ϣ
		break;
	}
	case GAME_OVER:			//��Ϸ��������
	{
		//��ʾ���ؽ���
		//DisplayInfo(hdc);		//��ʾ������Ϸ״̬��Ϣ
		break;
	}
	case GAME_HELP:			//��Ϸ��������
	{
		//��ʾ���ؽ���
		break;
	}
	case GAME_ABOUT:		//��Ϸ������Ϣ����
	{
		T_Graph::PaintBlank(hdc,0,0,wnd_width,wnd_height,RGB(ROUND(red),ROUND(green),ROUND(blue)),255);
		gameScene->Draw(hdc);
		DisplayInfo(hdc);
		gameMenu->DrawMenu(hdc);
		break;
	}
	}
}

//��Ϸ�˳�����
void MarioGame::GameEnd()
{
	
}

//��Ϸ��������,keys[] true:���£�false:�ͷ�
//������ʱ���������WM_KEYDOWN��Ϣ
void MarioGame::GameKeyAction(int Action)
{
	switch (GameState)
	{
	case GAME_START:		//��Ϸ��ʼ�˵�
	{		
		if (Action == KEY_DOWN)		//���¼�
		{
			if (keys[VK_RETURN])		//Enter��
			{
				gameScene->Remove(marioBoard);					//����ѡ���ĸ����ܣ�����Ҫ�Ƚ�������Ƴ�

				switch (gameMenu->GetMenuIndex()) {
				case 0:	//��ʼ
					GameState = GAME_UPGRADE;
				//	LoadGameMenu();
					break;
				case 1:	
					//GameState = GAME_SETTING;
					break;
				case 2:	//����
					GameState = GAME_ABOUT;
					LoadGameMenu();
				}
			}
			else if (keys[VK_UP])		//PgUp
			{
				gameMenu->MenuKeyDown(VK_UP);
			}
			else if (keys[VK_DOWN])		//PgDn
			{
				gameMenu->MenuKeyDown(VK_DOWN);
			}
		}
		break;
	}
	case GAME_RUN:			//��Ϸ����ʱ����
	{
		if (player->IsActive() && !player->isInEvent())	//��Ҵ��ڻ�Ծ״̬�������û�����
		{
			if (Action == KEY_DOWN)		//���¼�
			{
				if (keys[VK_A])
				{
					if (!keys[VK_D])		//����ס���������Ч
					{
						//�ڵ��������֧������
						if (player->isOnPlantform() && !player->isSliding())
						{
							player->SetDir(DIR_LEFT);
							if (!preA && !keys[VK_S]) {					//����ǵ�һ�ΰ��¼�D,����Ϊ��ʼ�ƶ�
								player->startMove();
								preA = true;
							}
						}
						if (player->isOnPlantform() && player->isSliding() && player->GetDir() == DIR_RIGHT) {
							player->setDirChanged(true);
						}

						//�ڿ���
						if (!player->isOnPlantform())
						{
							player->SetDir(DIR_LEFT);
							if (!preA) {					//����ǵ�һ�ΰ��¼�D,����Ϊ��ʼ�ƶ�
								player->startMove();
								preA = true;
							}
						}
					}
				}
				if (keys[VK_D])
				{
					if (!keys[VK_A])		//����ס���������Ч
					{		
						//�ڵ��������֧������
						if (player->isOnPlantform()&&!player->isSliding())
						{
							player->SetDir(DIR_RIGHT);
							if (!preD && !keys[VK_S]) {					//����ǵ�һ�ΰ��¼�D,����Ϊ��ʼ�ƶ�
								player->startMove();
								preD = true;
							}						
						}
						if (player->isOnPlantform() && player->isSliding() && player->GetDir() == DIR_LEFT) {
							player->setDirChanged(true);
						}

						//�ڿ���
						if (!player->isOnPlantform())
						{
							player->SetDir(DIR_RIGHT);
							if (!preD) {					//����ǵ�һ�ΰ��¼�D,����Ϊ��ʼ�ƶ�
								player->startMove();
								preD = true;
							}
						}
					}					
				}
				if (keys[VK_S])
				{
					if (!keys[VK_A] && !keys[VK_D] && !keys[VK_L]&&!player->isSliding()) {
						if (!preS)
						{
							player->setSquat(true);
							preS = true;
						}
					}
				}
				if (keys[VK_CAPITAL])
				{
					if ((keys[VK_A] || keys[VK_D])&&!player->isJump()) {
						if (!preCapital)
						{
							player->startSpeedup();
							preCapital = true;
						}
					}
				}
				if (keys[VK_L])
				{
					Util::myprintf(L"Squat: %d,isJump: %d,preSpace: %d,isOnPlantform: %d\n",
						player->getSquat() , player->isJump() , preL, player->isOnPlantform());
					if (!player->isSliding() && !player->getSquat()&& !player->isJump()&&!preL&&player->isOnPlantform())
					{
						player->startJump();
						preL = true;
					}
				}
				if (keys[VK_SPACE])
				{
					Util::myprintf(L"fire ball \n");
						
					if (!player->isSliding() && !player->getSquat() && !preSpace)
					{
						player->createFireBall();
						preSpace = true;
					}
				}
			}
			else if (Action == KEY_UP)	//�ͷż�
				{
					if (!keys[VK_A])
					{
						preA = false;
						if (player->GetDir() == DIR_LEFT) {
							player->stopMove(false);
						}
					}
					if (!keys[VK_D])
					{
						preD = false;
						if (player->GetDir() == DIR_RIGHT) {
							player->stopMove(false);
						}
					}
					if (!keys[VK_S])
					{
						preS = false;
						if (player->getSquat() == true)
						{
							player->setSquat(false);
						}
					}
					if (!keys[VK_CAPITAL])
					{
						preCapital = false;
						if (player->isSpeedUp() == true)
						{
							player->resetSpeedup();
						}
					}
					if (!keys[VK_L])
					{
						preL = false;
						if (player->getBooting())
						{
							player->stopBooting();		//�ͷż���ֹͣ����
							if (player->GetSpeedY() > 3) {
								player->SetSpeedY(3);
							}
						}
					}
					if (!keys[VK_SPACE])
					{
						preSpace = false;
					}
				}
		}
		break;
	}
	case GAME_PAUSE:		//��ͣ��Ϸ����
	{
		break;
	}
	case GAME_UPGRADE:		//�¹ؿ����ؽ���
	{
		break;
	}
	case GAME_OVER:			//��Ϸ��������
	{
		break;
	}
	case GAME_HELP:			//��Ϸ��������
	{
		break;
	}
	case GAME_ABOUT:		//��Ϸ������Ϣ����
	{
		if (Action == KEY_DOWN)		//���¼�
		{
			if (keys[VK_RETURN])		//Enter��
			{
				switch (gameMenu->GetMenuIndex()) {
				case 0:	//��ʼ
					GameState = GAME_START;
					gameScene->RePosition(wnd_width,wnd_height);
					LoadGameMenu();
					break;
				}
			}
			else if (keys[VK_UP])		//PgUp
			{
				gameMenu->MenuKeyDown(VK_UP);
			}
			else if (keys[VK_DOWN])		//PgDn
			{
				gameMenu->MenuKeyDown(VK_DOWN);
			}
		}
		break;
	}
	}
}

//��Ϸ�����Ϊ����(����ֵΪKEY_MOUSE_ACTION)
void MarioGame::GameMouseAction(int x, int y, int Action)	
{
}
// ������Ϸ��ͼ����,�ɰ����������ϰ������ֲ�
void MarioGame::LoadMap()
{
	if (!gameScene->LoadTxtMap("res\\map\\marioMap.txt"))
	{
		Util::myprintf(L"Load map failure!\n");
		exit(1);
	}

	gameScene->RePosition(wnd_width,wnd_height);
	//scn_width = gameScene->getBarrier()->GetWidth();
	//scn_height = gameScene->getBarrier()->GetHeight();

	//// ��ͼ��ʼλ���Ե�ͼ��Ϊ����
	//int scn_x = 0;
	//int scn_y = -gameScene->getBarrier()->getTileHeight()/2;
	//// ����Ϸ��ͼ��ʼ��
	//gameScene->InitScene(scn_x, scn_y, scn_width, scn_height, wnd_width, wnd_height);

	//// �����е�ͼͼ�㶨λ
	//SCENE_LAYERS::iterator p;
	//for (p = gameScene->getSceneLayers()->begin(); p != gameScene->getSceneLayers()->end(); p++)
	//{
	//	if (p->layer->ClassName() == "T_Map") p->layer->SetPosition(scn_x, scn_y);
	//}
}

 //������Ϸ��ҽ�ɫ
void MarioGame::LoadPlayer()
{
	GAMELAYER gameLayer;
	SPRITEINFO player_Info;
	PLAYERFRAME player_frame;
	PLAYERMODE player_mode;

	player = new Player(L".\\res\\sprite\\sMario.png", 24, 32);	
	int sRunSequence[3] = {4,5,6};
	int bRunSequence[3] = { 1,2,3 };
	int levelUpSequence[2] = {15,0};
	int levelDownSequence[2] = { 0,15 };

	player_Info.Active = true;
	player_Info.Dead = false;
	player_Info.Dir = DIR_RIGHT;
	player_Info.Rotation = TRANS_NONE;
	player_Info.Ratio = 1.0f;
	player_Info.Level = 0;
	player_Info.Score = 0;
	player_Info.SpeedX = 0;
	player_Info.SpeedY = 0;
	player_Info.Alpha = 255;
	player_Info.X = wnd_width / 5;
	player_Info.Y = 200;
	player_Info.Visible = true;
	player->Initiate(player_Info);
	player->SetSequence(sRunSequence, 3);
	player->SetLayerTypeID(LAYER_PLY);

	// ----- ��ʼ������µĿ�ʼ״̬
	player_frame.frameHeight = 32;
	player_frame.frameWidth = 24;
	player_frame.img = T_Graph(L".\\res\\sprite\\sMario.png");	

	player_frame.nRunFrames = 3;
	player_frame.runFrmSequence = (int*)malloc(sizeof(int)*player_frame.nRunFrames);
	memcpy(player_frame.runFrmSequence, sRunSequence, sizeof(int)*player_frame.nRunFrames);

	player_frame.nlevelUpFrames = 2;
	player_frame.levelUpFrmSequence = (int*)malloc(sizeof(int)*player_frame.nlevelUpFrames);
	memcpy(player_frame.levelUpFrmSequence, levelUpSequence, sizeof(int)*player_frame.nlevelUpFrames);

	player_frame.nlevelDownFrames = 2;
	player_frame.levelDownFrmSequence = (int*)malloc(sizeof(int)*player_frame.nlevelDownFrames);
	memcpy(player_frame.levelDownFrmSequence, levelDownSequence, sizeof(int)*player_frame.nlevelDownFrames);

	player_frame.jumpFrame = 7;
	player_frame.speedDownFrame = 3;
	player_frame.squatFrame = 0;
	player_frame.squatHeight = 32;
	player_frame.stopFrame = 0;
	player_frame.deathFrame = 9;

	player_mode.frameMode = player_frame;
	player_mode.basicJumpSpeedY = 8;
	player_mode.basicSpeedX = 0;
	player_mode.canSquat = false;
	player_mode.maxBootTime = 1500;
	player_mode.maxMoveSpeedX = 4;
	player_mode.maxRunSpeedX = 6;
	player->initNormalMode(&player_mode);


	// ----- ��ʼ��������ҩ���״̬
	player_frame.frameHeight = 64;
	player_frame.frameWidth = 32;
	player_frame.img = T_Graph(L".\\res\\sprite\\bMario.png");
	
	player_frame.nRunFrames = 3;
	player_frame.runFrmSequence = (int*)malloc(sizeof(int)*player_frame.nRunFrames);
	memcpy(player_frame.runFrmSequence, bRunSequence, sizeof(int)*player_frame.nRunFrames);

	player_frame.nlevelUpFrames = 2;
	player_frame.levelUpFrmSequence = (int*)malloc(sizeof(int)*player_frame.nlevelUpFrames);
	memcpy(player_frame.levelUpFrmSequence, levelUpSequence, sizeof(int)*player_frame.nlevelUpFrames);

	player_frame.nlevelDownFrames = 2;
	player_frame.levelDownFrmSequence = (int*)malloc(sizeof(int)*player_frame.nlevelDownFrames);
	memcpy(player_frame.levelDownFrmSequence, levelDownSequence, sizeof(int)*player_frame.nlevelDownFrames);
	player_frame.jumpFrame = 5;
	player_frame.speedDownFrame = 4;
	player_frame.squatFrame = 6;
	player_frame.squatHeight = 64*2/3;
	player_frame.stopFrame = 0;
	player_frame.levelUpFrame = 15;

	player_mode.frameMode = player_frame;
	player_mode.basicJumpSpeedY = 8;
	player_mode.basicSpeedX = 0;
	player_mode.canSquat = true;
	player_mode.maxBootTime = 1500;
	player_mode.maxMoveSpeedX = 4;
	player_mode.maxRunSpeedX = 6;
	player->initBigRedMode(&player_mode);

	//player->setPlayerMode(PLAYER_NORMAL);
	player->setPlayerMode(PLAYER_BIGNORMAL);
	gameLayer.layer = player;
	gameLayer.type_id = LAYER_PLY;
	gameLayer.z_order = gameScene->getSceneLayers()->size() + 1;
	gameLayer.layer->setZorder(gameLayer.z_order);
	gameScene->Append(gameLayer);
	player->SetStartTime(GetTickCount());

}

// ������Ϸ�˵�
void MarioGame::LoadGameMenu()
{
	int x, y;
	int btn_width, btn_height;
	Color normalClr, focusClr;
	MENU_INFO menuInfo;

	if (!gameMenu) delete gameMenu;
	gameMenu = new GameMenu();

	if (GameState == GAME_START)
	{
		//----------------��ʼ�˵���ʼ��----------------
		x = 0, y = 0;
		btn_width = 0, btn_height = 0;

		wstring menuItems[] = { L"1 PLAYER GAME",L"OPTIONS",L"ABOUT" };

		//�˵���
		btn_width = 200;
		btn_height = 32;
		normalClr = Color::White;
		focusClr = Color::White;
		gameMenu->SetBtnBmp(L".\\res\\map\\option_active.png", btn_width, btn_height);

		//���ò˵���Ϣ
		menuInfo.align = 1;
		menuInfo.space = MENU_SPACE;
		menuInfo.width = btn_width;
		menuInfo.height = btn_height;
		menuInfo.fontName = L"Comic Sans Ms";
		menuInfo.isBold = true;
		menuInfo.normalTextColor = normalClr;
		menuInfo.focusTextColor = focusClr;
		gameMenu->SetFontHeight(16);
		gameMenu->SetMenuInfo(menuInfo);

		for (int i = 0; i < 3; i++) {
			//��ֱ���ֵ�����
			x = (wndWidth - btn_width) / 4;
			y = i*(btn_height + MENU_SPACE) + (wnd_height - 5 * btn_height - 2 * MENU_SPACE) ;
			MENUITEM mItem;
			mItem.pos.x = x;
			mItem.pos.y = y;
			mItem.ItemName = menuItems[i];
			gameMenu->AddMenuItem(mItem);

		}

		marioBoard.type_id = LAYER_MAP_MASK;
		marioBoard.layer = new T_Map(L".\\res\\map\\superMarioBros.png");
		marioBoard.z_order = LAYER_MAX;
		marioBoard.layer->setZorder(LAYER_MAX);
		marioBoard.layer->SetLayerTypeID(LAYER_MAP_MASK);
		marioBoard.layer->SetVisible(true);
		marioBoard.layer->SetPosition(wnd_width / 12 , 48 + 18);	// ������ƫ��18px
		gameScene->Append(marioBoard);
	}
	else if (GameState == GAME_UPGRADE) {
		
	}
	else if (GameState == GAME_ABOUT) {

		x = 0, y = 0;
		btn_width = 0, btn_height = 0;

		wstring menuItems[] = { L"MAIN MENU"};

		//�˵���
		btn_width = 200;
		btn_height = 32;
		normalClr = Color::White;
		focusClr = Color::White;
		gameMenu->SetBtnBmp(L".\\res\\map\\option_active.png", btn_width, btn_height);

		//���ò˵���Ϣ
		menuInfo.align = 1;
		menuInfo.space = MENU_SPACE;
		menuInfo.width = btn_width;
		menuInfo.height = btn_height;
		//	menuInfo.fontHeight = 8;
		menuInfo.fontName = L"Comic Sans Ms";
		menuInfo.isBold = true;
		menuInfo.normalTextColor = normalClr;
		menuInfo.focusTextColor = focusClr;
		gameMenu->SetFontHeight(16);
		gameMenu->SetMenuInfo(menuInfo);
		//	gameMenu->ClearItem();

		for (int i = 0; i < 1; i++) {
			//��ֱ���ֵ�����
			x = wnd_width / 6;
			y = wnd_height / 6 + 9 * 25;
			MENUITEM mItem;
			mItem.pos.x = x;
			mItem.pos.y = y;
			mItem.ItemName = menuItems[i];
			gameMenu->AddMenuItem(mItem);

		}
		
		// �볡����صı���
		red = green = blue = 11;
		gameScene->SetScenePos(-1, 0);	//�������¶�λ
		changeIndex = 0;
	}
}

// ������ϷͼƬ��Դ
void MarioGame::LoadImageRes()
{
	// ��˸��С���
	if (statusCoin == NULL)
		statusCoin = new T_Sprite(L".\\res\\map\\statusCoin.png", 10, 16);

	// �ؿ�����ʱ λ�����м�������ͼƬ
	if (statusMario == NULL)
		statusMario = new T_Sprite(L".\\res\\sprite\\sMario.png", 24, 32);

	SPRITEINFO info;
	// ���
	info.X = STATUS_BEGIN_X + STATUS_TEXT_HORIZON_SPACE + STATUS_TEXT_WIDTH/6;									// ����ռ����֮һ�ɣ�һ��Ҳû���ٽ�� ���ֲ�����ͼ���ص�
	info.Y = STATUS_BEGIN_Y + STATUS_TEXT_VERTICAL_SPACE + STATUS_TEXT_HEIGHT/2 - statusCoin->GetHeight()/2;	// ͼ��������ˮƽ�߾���
	info.Dir = 0;
	info.Rotation = 0;
	info.Ratio = 1;
	info.Alpha = 255;
	info.Visible = true;
	statusCoin->Initiate(info);

	// �����
	info.X = wnd_width*4/9 ;									
	info.Y = wnd_height/2;				
	info.Dir = 0;
	info.Rotation = 0;
	info.Ratio = 1;
	info.Alpha = 255;
	info.Visible = true;
	statusMario->Initiate(info);


}
// ������Ϸ������Դ
void MarioGame::LoadSound(HWND hwnd)
{
	gameOverSound = new AudioDXBuffer;
	gameWinSound = new AudioDXBuffer;
	
	if (!dxSnd.CreateDS(hwnd))  return;
	gameOverSound->LoadWave(dxSnd, L".\\res\\sound\\gameover.wav");
	gameWinSound->LoadWave(dxSnd, L".\\res\\sound\\victory.wav");
}

// ������Ϸ�еĸ�����������
void MarioGame::PlayGameSound()
{
	bkgMusic->Terminate();			// ��Ϸ�������� 
	gameOverSound->Terminate();		// ��Ϸ��������
	gameWinSound->Terminate();		// ��Ϸʤ������

	switch (GameState)
	{
	case GAME_START:		
		break;
	case GAME_RUN:
		bkgMusic->Play(true);
		break;
	case GAME_WIN:
		gameWinSound->Play(false);
		break;
	case GAME_OVER:
		gameOverSound->Play(false);
		break;
	case GAME_UPGRADE:
		gameWinSound->Play(false);
		break;
	}
}
// �ͷ���������
void MarioGame::ReleaseDXBuffer(AudioDXBuffer* dxBuf)
{
	if (dxBuf != NULL)
	{
		dxBuf->Release();
		delete dxBuf;
		dxBuf = NULL;
	}
}
// ���عؿ�����
void MarioGame::LoadGameLevel(int level)
{
	SetFrame(FRAME_SPEED);
	gameScene = NULL;

	if (gameScene == NULL) gameScene = GameScene::getInstance();
//	if (gameMenu == NULL) gameMenu = new T_Menu();
	gameScene->appendMinion(MINION_TYPE::MINION_GOOMBA,1100,200);
	//LoadSound(m_hWnd);
	LoadImageRes();
//	LoadMenu();
	LoadMap();
	LoadPlayer();
}

// ����ؿ�����
void MarioGame::ClearGameLevel()
{

}

// ������Ϸ״̬����ʾ��Ϣ
void MarioGame::DisplayInfo(HDC hdc)
{
	int FontHeight = 16;	 // �ֺ�
	wstring fontName = L"Comic Sans Ms"; // ����
	RectF textRect;			 // ������
	vector<wstring> content; // ��������
	switch (GameState)
	{
	case GAME_START: 
		ShowTitleInfo(hdc);
		break;
	case GAME_RUN:
		ShowTitleInfo(hdc);
		break;
	case GAME_OVER:
		break;
	case GAME_UPGRADE:
		T_Graph::PaintBlank(hdc,0,0,wnd_width,wnd_height,RGB(0,0,0),255);	// ����
		ShowTitleInfo(hdc);													// ����״̬��
		statusMario->Draw(hdc);												// �����
		
		// �ؿ���Ϣ
		textRect.X = (REAL)statusMario->GetX()*1.0 - 2 * statusMario->GetWidth();
		textRect.Y = (REAL)statusMario->GetY()*1.0 - 2 * statusMario->GetHeight(); 
		textRect.Width = wnd_width;
		textRect.Height = 30;
		content.push_back(L" WORLD 1-1 ");
		T_Graph::PaintText(hdc, textRect, content[0].c_str(), FontHeight, fontName.c_str(),
			Color::White, FontStyleBold, StringAlignmentNear);

		// �����������Ϣ														
		textRect.X = statusMario->GetX() + statusMario->GetWidth();
		textRect.Y = statusMario->GetY();// statusMario->GetHeight()/3;
		textRect.Width = wnd_width*1.0;
		textRect.Height = 30;
		content.push_back(L" X ");
		content[1].append(T_Util::int_to_wstring(player->getLifeCount()));
		T_Graph::PaintText(hdc, textRect, content[1].c_str(), FontHeight, fontName.c_str(),	
			Color::White, FontStyleBold, StringAlignmentNear);
		
		// �ײ���ʾ��
		textRect.X = (REAL)wnd_width / 8;
		textRect.Y = (REAL)7*wnd_height/8;
		textRect.Width = (REAL)wnd_width;
		textRect.Height = (REAL)30;
		content.push_back(L"REMEMBER THAT YOU CAN RUN WITH LSHIFT");
		T_Graph::PaintText(hdc, textRect, content[2].c_str(), FontHeight, fontName.c_str(),	
			Color::White, FontStyleBold, StringAlignmentNear);
		break;
	case GAME_WIN:
		break;
	case GAME_HELP:
		break;
	case GAME_ABOUT:

		//������Ϣ
		textRect.X = (REAL)wnd_width/5.0;
		textRect.Y = (REAL)wnd_height/6.0;
		textRect.Width = (REAL)wnd_width * 1.0;
		textRect.Height = (REAL)wnd_height / 4.0;

		
		content.push_back(L"MARIO V1.0  - C++ ");
		content.push_back(L" ");
		content.push_back(L"AUTHOR: Weidong Chen 8002117021");
		content.push_back(L"            Jinzhou  Luo   8002117017");
		content.push_back(L"ADVISER: Lizhong Wan");
		content.push_back(L" ");
		content.push_back(L"NOV 2019");

		for (int i = 0; i < content.size(); i++)
		{
			T_Graph::PaintText(hdc, textRect, content[i].c_str(), FontHeight, fontName.c_str(),
				Color::White, FontStyleBold, StringAlignmentNear);
			textRect.Y += 25;
		}
		break;
	}
}

// ��ʾ����״̬��
void MarioGame::ShowTitleInfo(HDC hdc) {

	int FontHeight = 16;	 // �ֺ�
	wstring fontName = L"Comic Sans Ms"; // ����
	RectF textRect;			 // ������
	vector<wstring> content; // ��������


	FontHeight = 16;
	textRect.X = STATUS_BEGIN_X;
	textRect.Y = STATUS_BEGIN_Y;
	textRect.Width = (float)STATUS_TEXT_WIDTH;
	textRect.Height = (float)STATUS_TEXT_HEIGHT;

	content.push_back(L"MARIO");
	content.push_back(L" ");	
	content.push_back(L"WORLD");	
	content.push_back(L"TIME");


	content.push_back(T_Util::int_to_wstring(00));
	content.push_back(T_Util::int_to_wstring(00));
	content.push_back(L"1-1");
	content.push_back(T_Util::int_to_wstring(400));

	for (int i = 0; i < content.size(); i++)
	{
		if (i == 5) {	//�����ǰ��Ҫ��ӡһ��ͼƬ
			statusCoin->Draw(hdc);
			statusCoin->LoopFrame(20,true);	//�����¶��ά��֡��������������
		}	
		
		T_Graph::PaintText(hdc, textRect, content[i].c_str(), FontHeight, fontName.c_str(),
			Color::White, FontStyleBold, StringAlignmentCenter);
		textRect.X += STATUS_TEXT_HORIZON_SPACE;

		// ÿ����ĸ����������һ�λ���
		if ((i+1) % 4 == 0) {
			textRect.X = STATUS_BEGIN_X;
			textRect.Y += STATUS_TEXT_VERTICAL_SPACE;
		}
	}
}
