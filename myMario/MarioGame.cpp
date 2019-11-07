

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
	gameStatus = GAME_START;
	LoadGameLevel(gameLevel);		//���عؿ���Դ����ͼ�����
	LoadGameMenu(gameStatus);		//�������˵�
}

//��Ϸ�߼�����(����GameState)
void MarioGame::GameLogic()	
{
	switch (gameStatus)
	{
		case GAME_START:		//��Ϸ��ʼ�˵�
		{
			//gameMenu->update();
			break;
		}
		case GAME_RUN:			//��Ϸ����ʱ����
		{
			gameTime = GetTickCount();		//������Ϸ������ʱ��
			
			t_scene->update();				//���³��������״̬
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
	switch (gameStatus)
	{
	case GAME_START:		//��Ϸ��ʼ�˵�
	{
		t_scene->Draw(hdc);
		DisplayInfo(hdc);		//��ʾ������Ϸ״̬��Ϣ
		gameMenu->DrawMenu(hdc);
		break;
	}
	case GAME_RUN:			//��Ϸ����ʱ����
	{
		t_scene->Draw(hdc);
		DisplayInfo(hdc);		//��ʾ������Ϸ״̬��Ϣ
		break;
	}
	case GAME_PAUSE:		//��ͣ��Ϸ����
	{
		t_scene->Draw(hdc);
		DisplayInfo(hdc);		//��ʾ������Ϸ״̬��Ϣ
		gameMenu->DrawMenu(hdc);
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
		DisplayInfo(hdc);		//��ʾ������Ϸ״̬��Ϣ
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
	switch (gameStatus)
	{
	case GAME_START:		//��Ϸ��ʼ�˵�
	{		
		if (Action == KEY_DOWN)		//���¼�
		{
			if (keys[VK_RETURN])		//Enter��
			{
				Util::myprintf(L"press key enter\n");
			}
			else if(keys[VK_UP])		//PgUp
			{
				Util::myprintf(L"release key up\n");
			}
			else if (keys[VK_DOWN])		//PgDn
			{

			}
		}
		else if (Action == KEY_UP)	//�ͷż�
		{
			Util::myprintf(L"release key \n");
		}
		break;
	}
	case GAME_RUN:			//��Ϸ����ʱ����
	{
		if (Action == KEY_DOWN)		//���¼�
		{
			if (keys[VK_A])		
			{

			}
			else if (keys[VK_D])		
			{

			}
			else if (keys[VK_S])		
			{

			}
			else if (keys[VK_SHIFT])		
			{

			}
		}
		else if (Action == KEY_UP)	//�ͷż�
		{
			if (keys[VK_A])
			{

			}
			else if (keys[VK_D])
			{

			}
			else if (keys[VK_S])
			{

			}
			else if (keys[VK_SHIFT])
			{

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
	t_scene->LoadTxtMap("res\\game\\tankmap.txt");
	if (t_scene->getBarrier() == NULL)
	{
		Util::myprintf(L"Load map failure!\n");
		exit(1);
	}
	scn_width = t_scene->getBarrier()->GetWidth();
	scn_height = t_scene->getBarrier()->GetHeight();

	// ��ͼ��ʼλ���Ե�ͼ��Ϊ����
	int scn_x = 0;
	int scn_y = -t_scene->getBarrier()->getTileHeight()/2;
	// ����Ϸ��ͼ��ʼ��
	t_scene->InitScene(scn_x, scn_y, scn_width, scn_height, wnd_width, wnd_height);

	// �����е�ͼͼ�㶨λ
	SCENE_LAYERS::iterator p;
	for (p = t_scene->getSceneLayers()->begin(); p != t_scene->getSceneLayers()->end(); p++)
	{
		if (p->layer->ClassName() == "T_Map") p->layer->SetPosition(scn_x, scn_y);
	}
}
// ������Ϸ��ҽ�ɫ
void MarioGame::LoadPlayer()
{
	GAMELAYER gameLayer;
	SPRITEINFO player_Info;
	player = new Player(L".\\res\\sprite\\playertank.png", 40, 40);	

	player_Info.Active = false;
	player_Info.Dead = false;
	player_Info.Dir = DIR_RIGHT;
	player_Info.Rotation = TRANS_NONE;
	player_Info.Ratio = 1.0f;
	player_Info.Level = 0;
	player_Info.Score = 0;
	player_Info.SpeedX = 2;
	player_Info.SpeedY = 0;
	player_Info.Alpha = 220;
	player_Info.X = wnd_width / 5;
	player_Info.Y = (wnd_height - player->GetHeight()) / 2;
	player_Info.Visible = true;
	player->Initiate(player_Info);
	//player->SetSequence(TANK_FRAME_UP, 4);
	player->SetLayerTypeID(LAYER_PLY);

	gameLayer.layer = player;
	gameLayer.type_id = LAYER_PLY;
	gameLayer.z_order = t_scene->getSceneLayers()->size() + 1;
	gameLayer.layer->setZorder(gameLayer.z_order);
	t_scene->Append(gameLayer);
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
	t_scene = NULL;

	if (t_scene == NULL) t_scene = new T_Scene();
	if (gameMenu == NULL) gameMenu = new T_Menu();

	LoadSound(m_hWnd);
	LoadImageRes();

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
	switch (gameStatus)
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

