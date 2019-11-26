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
	GameState = GAME_RUN;			//����׼���������˵�
	LoadGameLevel(gameLevel);		//���عؿ���Դ����ͼ�����
	//LoadGameMenu(GameState);		//�������˵�	
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
			if (!player->IsDead() && player->IsVisible())	//δ�����򲥷���������δ������
				player->update();
			if (player->IsDead() && !player->IsVisible())	//�����������������������
			{
				if (player->getLifeCount() == 0)
				{
					GameState = GAME_OVER;				//�������ֵΪ0����Ϸ����
				}
				else
				{
					GameState = GAME_UPGRADE;			//�������ֵ��Ϊ0��������Ϸ
					//���ظ����
				}
			}
			//gameScene->ScrollScene(player);				//�������λ�ã���������
			gameScene->update();						//���µ�ͼ��������״̬
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
		//gameScene->Draw(hdc);
		//DisplayInfo(hdc);		//��ʾ������Ϸ״̬��Ϣ
		//gameMenu->DrawMenu(hdc);
		break;
	}
	case GAME_RUN:			//��Ϸ����ʱ����
	{
		//DisplayInfo(hdc);		//��ʾ������Ϸ״̬��Ϣ
		T_Graph::PaintBlank(hdc,0,0,wnd_width,wnd_height,RGB(100,200,200),255);
		gameScene->Draw(hdc);
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
		//DisplayInfo(hdc);		//��ʾ������Ϸ״̬��Ϣ
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
		//��ʾ���ؽ���
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
			}
			else if(keys[VK_UP])		//PgUp
			{
			}
			else if (keys[VK_DOWN])		//PgDn
			{

			}
		}
		else if (Action == KEY_UP)	//�ͷż�
		{
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
					if (!keys[VK_A] && !keys[VK_D] && !keys[VK_SPACE]&&!player->isSliding()) {
						if (!preS)
						{
							player->setSquat(true);
							preS = true;
						}
					}
				}
				if (keys[VK_SHIFT])
				{
					if ((keys[VK_A] || keys[VK_D])&&!player->isJump()) {
						if (!preShift)
						{
							player->startSpeedup();
							preShift = true;
						}
					}
				}
				if (keys[VK_L])
				{
					Util::myprintf(L"Squat: %d,isJump: %d,preSpace: %d,isOnPlantform: %d\n",
						player->getSquat() , player->isJump() , preL, player->isOnPlantform());
					if (!player->getSquat()&& !player->isJump()&&!preL&&player->isOnPlantform())
					{
						player->startJump();
						preL = true;
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
					if (!keys[VK_SHIFT])
					{
						preShift = false;
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
	scn_width = gameScene->getBarrier()->GetWidth();
	scn_height = gameScene->getBarrier()->GetHeight();

	// ��ͼ��ʼλ���Ե�ͼ��Ϊ����
	int scn_x = 0;
	int scn_y = -gameScene->getBarrier()->getTileHeight()/2;
	// ����Ϸ��ͼ��ʼ��
	gameScene->InitScene(scn_x, scn_y, scn_width, scn_height, wnd_width, wnd_height);

	// �����е�ͼͼ�㶨λ
	SCENE_LAYERS::iterator p;
	for (p = gameScene->getSceneLayers()->begin(); p != gameScene->getSceneLayers()->end(); p++)
	{
		if (p->layer->ClassName() == "T_Map") p->layer->SetPosition(scn_x, scn_y);
	}
}

 //������Ϸ��ҽ�ɫ
void MarioGame::LoadPlayer()
{
	GAMELAYER gameLayer;
	SPRITEINFO player_Info;
	PLAYERFRAME player_frame;
	PLAYERMODE player_mode;

	player = new Player(L".\\res\\sprite\\sMario.png", 24, 32);	
	int sSequence[12] = {4,4,5,5,5,5,6,6,6,6,4,4};
	int bSequence[12] = { 1,1,2,2,2,2,3,3,3,3,1,1 };

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
	player->SetSequence(sSequence, 12);
	player->SetLayerTypeID(LAYER_PLY);

	// ----- ��ʼ������µĿ�ʼ״̬
	player_frame.frameHeight = 32;
	player_frame.frameWidth = 24;
	player_frame.img = T_Graph(L".\\res\\sprite\\sMario.png");	
	player_frame.nRunFrames = 12;
	player_frame.runFrmSequence = (int*)malloc(sizeof(int)*player_frame.nRunFrames);
	memcpy(player_frame.runFrmSequence, sSequence, sizeof(int)*player_frame.nRunFrames);

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
	player_frame.nRunFrames = 12;
	player_frame.runFrmSequence = (int*)malloc(sizeof(int)*player_frame.nRunFrames);
	memcpy(player_frame.runFrmSequence, bSequence, sizeof(int)*player_frame.nRunFrames);

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

	player->setPlayerMode(PLAYER_NORMAL);
	//player->setPlayerMode(PLAYER_BIGRED);
	gameLayer.layer = player;
	gameLayer.type_id = LAYER_PLY;
	gameLayer.z_order = gameScene->getSceneLayers()->size() + 1;
	gameLayer.layer->setZorder(gameLayer.z_order);
	gameScene->Append(gameLayer);
	player->SetStartTime(GetTickCount());
}

// ������Ϸ�˵�
void MarioGame::LoadGameMenu(int type)
{
	if (gameMenu == NULL) gameMenu = new T_Menu();
	delete gameMenu;

	gameMenu->SetMenuIndex(-1);
	int btnwidth = 200, btnheight = 80;
	if (type == GAME_START)
	{
		
	}
	
}

//
// ������ϷͼƬ��Դ
void MarioGame::LoadImageRes()
{
	if (status == NULL)	status = new T_Graph(L".\\res\\game\\npc.png");
	
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

	if (gameScene == NULL) gameScene = new GameScene();
	if (gameMenu == NULL) gameMenu = new T_Menu();

	//LoadSound(m_hWnd);
	//LoadImageRes();

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
	int FontHeight = 0;//�ֺ�
	Gdiplus::RectF rect;
	wstring Content = L"";
	switch (GameState)
	{
	case GAME_START:
		break;
	case GAME_RUN:
		break;
	case GAME_OVER:
		break;
	case GAME_UPGRADE:
		break;
	case GAME_WIN:
		break;
	case GAME_HELP:
		break;
	case GAME_ABOUT:
		break;
	}
}

