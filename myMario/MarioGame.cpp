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
	menuIsInit = false;
	GameState = GAME_ABOUT;			//���� ABOUT_MENU
	LoadGameLevel(gameLevel);		//���عؿ���Դ����ͼ�����
	LoadGameMenu(GameState);		//�������˵�	
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
			//GameKeyAction();
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
			gameScene->ScrollScene(player);			//�������λ�ã���������
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
			
			//������ɫ�任
			T_Util::ChangeRGB(&red,&green,&blue,&lastRed,&lastBlue,&lastGreen,&changeIndex);
			// ���괦��
			gameScene->ScrollScene(-3);
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
	//	DisplayInfo(hdc);		//��ʾ������Ϸ״̬��Ϣ
		gameMenu->DrawMenu(hdc);
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
		T_Graph::PaintBlank(hdc,0,0,wnd_width,wnd_height,RGB(ROUND(red),ROUND(green),ROUND(blue)),255);
		gameScene->Draw(hdc);
		DisplayInfo(hdc);
	//	gameMenu->DrawMenu(hdc);
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
					if (!keys[VK_D] && !player->isSliding()) {
						player->SetDir(DIR_LEFT);
						if (!preA) {					//����ǵ�һ�ΰ��¼�A,����Ϊ��ʼ�ƶ�
							player->startMove();
							preA = true;
						}
					}
				}
				if (keys[VK_D])
				{
					if (!keys[VK_A] && !player->isSliding()) {
						player->SetDir(DIR_RIGHT);
						if (!preD) {					//����ǵ�һ�ΰ��¼�D,����Ϊ��ʼ�ƶ�
							player->startMove();
							preD = true;
						}
					}
				}
				if (keys[VK_S])
				{
					if (!keys[VK_A] && !keys[VK_D] && !keys[VK_SPACE]) {
						if (!preS)
						{
							player->setSquat(true);
							preS = true;
						}
					}
				}
				if (keys[VK_SHIFT])
				{
					if (keys[VK_A] || keys[VK_D]) {
						if (!preShift)
						{
							player->startSpeedup();
							preShift = true;
						}
					}
				}
				if (keys[VK_SPACE])
				{
					if (!player->getSquat()&& !player->isJump()&&!preSpace)
					{
						player->startJump();
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
					if (!keys[VK_SHIFT])
					{
						preShift = false;
						if (player->isSpeedUp() == true)
						{
							player->resetSpeedup();
						}
					}
					if (!keys[VK_SPACE])
					{
						preSpace = false;
						if (player->getBooting())
						{
							player->stopBooting();		//�ͷż���ֹͣ����
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
// ������Ϸ��ҽ�ɫ
//void MarioGame::LoadPlayer()
//{
//	GAMELAYER gameLayer;
//	SPRITEINFO player_Info;
//	player = new Player(L".\\res\\sprite\\bMario.png", 32, 64);	
//	int sequence[9] = {1,1,2,2,2,3,3,3,1};
//
//	player_Info.Active = true;
//	player_Info.Dead = false;
//	player_Info.Dir = DIR_RIGHT;
//	player_Info.Rotation = TRANS_NONE;
//	player_Info.Ratio = 1.0f;
//	player_Info.Level = 0;
//	player_Info.Score = 0;
//	player_Info.SpeedX = 0;
//	player_Info.SpeedY = 0;
//	player_Info.Alpha = 220;
//	player_Info.X = wnd_width / 5;
//	player_Info.Y = (wnd_height - player->GetHeight()) / 2;
//	player_Info.Visible = true;
//	player->Initiate(player_Info);
//	player->SetSequence(sequence, 9);
//	player->SetLayerTypeID(LAYER_PLY);
//
//	gameLayer.layer = player;
//	gameLayer.type_id = LAYER_PLY;
//	gameLayer.z_order = gameScene->getSceneLayers()->size() + 1;
//	gameLayer.layer->setZorder(gameLayer.z_order);
//	gameScene->Append(gameLayer);
//	player->SetStartTime(GetTickCount());
//}
 //������Ϸ��ҽ�ɫ
void MarioGame::LoadPlayer()
{
	GAMELAYER gameLayer;
	SPRITEINFO player_Info;
	PLAYERFRAME player_frame;
	PLAYERMODE player_mode;

	player = new Player(L".\\res\\sprite\\sMario.png", 32, 32);	
	int sSequence[10] = {4,4,5,5,5,6,6,6,4,4};
	int bSequence[10] = { 1,1,1,2,2,2,3,3,3,1 };

	player_Info.Active = true;
	player_Info.Dead = false;
	player_Info.Dir = DIR_RIGHT;
	player_Info.Rotation = TRANS_NONE;
	player_Info.Ratio = 1.0f;
	player_Info.Level = 0;
	player_Info.Score = 0;
	player_Info.SpeedX = 0;
	player_Info.SpeedY = 0;
	player_Info.Alpha = 220;
	player_Info.X = wnd_width / 5;
	player_Info.Y = 200;//(wnd_height - player->GetHeight()) / 2;
	player_Info.Visible = true;
	player->Initiate(player_Info);
	player->SetSequence(sSequence, 10);
	player->SetLayerTypeID(LAYER_PLY);

	// ----- ��ʼ������µĿ�ʼ״̬
	player_frame.frameHeight = 32;
	player_frame.frameWidth = 24;
	player_frame.img = T_Graph(L".\\res\\sprite\\sMario.png");	
	player_frame.nRunFrames = 10;
	player_frame.runFrmSequence = (int*)malloc(sizeof(int)*player_frame.nRunFrames);
	memcpy(player_frame.runFrmSequence, sSequence, sizeof(int)*player_frame.nRunFrames);

	player_frame.jumpFrame = 7;
	player_frame.speedDownFrame = 3;
	player_frame.squatFrame = 0;
	player_frame.squatHeight = 32;
	player_frame.stopFrame = 0;

	player_mode.frameMode = player_frame;
	player_mode.basicJumpSpeedY = 6;
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
	player_frame.nRunFrames = 10;
	player_frame.runFrmSequence = (int*)malloc(sizeof(int)*player_frame.nRunFrames);
	memcpy(player_frame.runFrmSequence, bSequence, sizeof(int)*player_frame.nRunFrames);

	player_frame.jumpFrame = 5;
	player_frame.speedDownFrame = 4;
	player_frame.squatFrame = 6;
	player_frame.squatHeight = 64*2/3;
	player_frame.stopFrame = 0;

	player_mode.frameMode = player_frame;
	player_mode.basicJumpSpeedY = 6;
	player_mode.basicSpeedX = 0;
	player_mode.canSquat = false;
	player_mode.maxBootTime = 1500;
	player_mode.maxMoveSpeedX = 4;
	player_mode.maxRunSpeedX = 6;
	player->initBigRedMode(&player_mode);

	player->setPlayerMode(PLAYER_NORMAL);
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
	if (!gameMenu) delete gameMenu;
	gameMenu = new T_Menu();

//	gameMenu->SetMenuIndex(-1);
	if (menuIsInit)	return;
	MENU_INFO menuInfo;
	Color normalClr, focusClr;
	int btnWidth = 200, btnHeight = 80;
	if (type == GAME_START)
	{

	}
	else if (type == GAME_ABOUT) {

		// �˵���ʼ��
	//	wstring mainMenuItems[] = { L"MAIN MENU" };
	//	gameMenu->SetMenuBkg(L".\\res\\menubck.png", 180);

	//	//�����ΰ�ť�˵���
	//	normalClr = Color::Red;
	//	focusClr = Color::White;
	//	gameMenu->SetBtnBmp(NULL, btnWidth, btnHeight);

	//	//���ò˵���Ϣ
	//	menuInfo.align = 1;
	//	menuInfo.space = MENU_SPACE;
	//	menuInfo.width = btnWidth;
	//	menuInfo.height = btnHeight;
	//	menuInfo.fontName = L"����";
	//	menuInfo.isBold = true;
	//	menuInfo.normalTextColor = normalClr;
	//	menuInfo.focusTextColor = focusClr;
	//	gameMenu->SetMenuInfo(menuInfo);

	////	for (int i = 0; i < 3; i++) {
	//		//��ֱ���ֵ�����
	////		x = (wndWidth - btn_width) / 4;
	////		y = i*(btn_height + MENU_SPACE) + (wnd_height - 3 * btn_height - 3 * MENU_SPACE) / 2;
	//		MENUITEM mItem;
	//		mItem.pos.x = 500;
	//		mItem.pos.y = 500;
	//		mItem.ItemName = mainMenuItems[0];
	//		gameMenu->AddMenuItem(mItem);

	//	}
		// �볡����صı���
		red = green = blue = 11;
		gameScene->SetScenePos(-1, 0);	//�������¶�λ
		changeIndex = 0;
		menuIsInit = true;
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
//	if (gameMenu == NULL) gameMenu = new T_Menu();

	//LoadSound(m_hWnd);
	//LoadImageRes();
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
	LPCTSTR fontName = L"Comic Sans Ms"; // ����
	RectF textRect;			 // ������
	vector<LPCTSTR> content; // ��������

	switch (GameState)
	{
	case GAME_START:
		break;
	case GAME_RUN:

		FontHeight = 16;
		textRect.X = wnd_width / 12;
		textRect.Y = 0;
		textRect.Width = (float)wnd_width/8;
		textRect.Height = (float)wnd_height / 12;

		content.push_back(L"MARIO");
		content.push_back(L" ");	
		content.push_back(L"WORLD");	
		content.push_back(L"TIME");


		content.push_back(L"test");
	//	content.push_back(T_Util::int_to_wstring(00));
	//	content.push_back(T_Util::int_to_wstring(400));
	//	content.push_back(T_Util::int_to_wstring(400));
		

		for (int i = 0; i < 4; i++)
		{
			T_Graph::PaintText(hdc, textRect, content[i], FontHeight, fontName,
				Color::White, FontStyleBold, StringAlignmentNear);
			textRect.X += wnd_width / 4;
		}	

		textRect.Y = (float)wnd_height / 12;
		T_Graph::PaintText(hdc, textRect, content[4], FontHeight, fontName,
			Color::White, FontStyleBold, StringAlignmentNear);
	/*	for (int i = 4; i < 5; i++)
		{
			T_Graph::PaintText(hdc, textRect, content[i], FontHeight, fontName,
				Color::White, FontStyleBold, StringAlignmentNear);
			textRect.X += wnd_width / 4;
		}*/

	/*	for (int i = 0; i < content.size(); i++)
		{
			T_Graph::PaintText(hdc, textRect, content[i], FontHeight, fontName,
				Color::White, FontStyleBold, StringAlignmentNear);
			textRect.X += wnd_width/4;
			if (i == 4)	textRect.Y += wnd_height / 12;

		}*/


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

	//	fontName = L"Comic Sans Ms";

		textRect.X = wnd_width/5;
		textRect.Y = wnd_height/6;
		textRect.Width = (float)wnd_width;
		textRect.Height = (float)wnd_height / 4;

		content.push_back(L"MARIO V1.0  - C++ ");
		content.push_back(L" ");
		content.push_back(L"AUTHOR: Weidong Chen 8002117021");
		content.push_back(L"            Jinzhou  Luo   8002117017");
		content.push_back(L"ADVISER: Lizhong Wan");
		content.push_back(L" ");
		content.push_back(L"NOV 2019");

		for (int i = 0; i < content.size(); i++)
		{
			T_Graph::PaintText(hdc, textRect, content[i], FontHeight, fontName,
				Color::White, FontStyleBold, StringAlignmentNear);
			textRect.Y += 25;
		}
		break;
	}
}

