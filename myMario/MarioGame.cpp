

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
	PlayGameSound();				//��������
}

//��Ϸ�߼�����(����GameState)
void MarioGame::GameLogic()	
{
	switch (gameStatus)
	{
		case GAME_START:		//��Ϸ��ʼ�˵�
		{
			
			break;
		}
		case GAME_RUN:			//��Ϸ����ʱ����
		{
			gameTime = GetTickCount();		//������Ϸ������ʱ��
			//player->update();
			//t_scene->update();		//���µ�ͼ״̬

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
		break;
	}
	case GAME_RUN:			//��Ϸ����ʱ����
	{
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

//��Ϸ�˳�����
void MarioGame::GameEnd()
{
	
}

//��Ϸ��������
void MarioGame::GameKeyAction(int Action)
{
	switch (gameStatus)
	{
	case GAME_START:		//��Ϸ��ʼ�˵�
	{

		break;
	}
	case GAME_RUN:			//��Ϸ����ʱ����
	{
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
	SPRITEINFO tank_Info;
	player = new Player(L".\\res\\sprite\\playertank.png", 40, 40);	

	tank_Info.Active = false;
	tank_Info.Dead = false;
	tank_Info.Dir = DIR_UP;
	tank_Info.Rotation = TRANS_NONE;
	tank_Info.Ratio = 1.0f;
	tank_Info.Level = 0;
	tank_Info.Score = 0;
	tank_Info.SpeedX = 2;
	tank_Info.SpeedY = 0;
	tank_Info.Alpha = 220;
	tank_Info.X = wnd_width / 5;
	tank_Info.Y = (wnd_height - player->GetHeight()) / 2;
	tank_Info.Visible = true;
	player->Initiate(tank_Info);
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
