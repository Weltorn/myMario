

#include "MarioGame.h"

// 类的构造函数 
MarioGame::MarioGame(HINSTANCE h_instance, LPCTSTR sz_winclass, LPCTSTR sz_title, 
				   WORD icon, WORD sm_icon, int winwidth, int winheight)
:T_Engine(h_instance, sz_winclass, sz_title, icon, sm_icon, winwidth, winheight)
{	
	wnd_width = winwidth;
	wnd_height = winheight;
}

// 类的析构函数
MarioGame::~MarioGame(void)
{
	
}

//游戏初始化接口函数
void MarioGame::GameInit() 
{
	gameLevel = 1;
	gameStatus = GAME_START;
	LoadGameLevel(gameLevel);		//加载关卡资源、地图、玩家
	LoadGameMenu(gameStatus);		//加载主菜单
}

//游戏逻辑处理(处理GameState)
void MarioGame::GameLogic()	
{
	switch (gameStatus)
	{
		case GAME_START:		//游戏开始菜单
		{
			//gameMenu->update();
			break;
		}
		case GAME_RUN:			//游戏进行时界面
		{
			gameTime = GetTickCount();		//更新游戏已运行时间
			
			t_scene->update();				//更新场景、玩家状态
			break;
		}
		case GAME_PAUSE:		//暂停游戏界面
		{
			break;
		}
		case GAME_UPGRADE:		//新关卡加载界面
		{
			break;
		}
		case GAME_OVER:			//游戏结束界面
		{
			break;
		}
		case GAME_HELP:			//游戏帮助界面
		{
			break;
		}
		case GAME_ABOUT:		//游戏制作信息界面
		{
			break;
		}
	}

}

//游戏显示
void MarioGame::GamePaint(HDC hdc)	
{
	switch (gameStatus)
	{
	case GAME_START:		//游戏开始菜单
	{
		t_scene->Draw(hdc);
		DisplayInfo(hdc);		//显示顶部游戏状态信息
		gameMenu->DrawMenu(hdc);
		break;
	}
	case GAME_RUN:			//游戏进行时界面
	{
		t_scene->Draw(hdc);
		DisplayInfo(hdc);		//显示顶部游戏状态信息
		break;
	}
	case GAME_PAUSE:		//暂停游戏界面
	{
		t_scene->Draw(hdc);
		DisplayInfo(hdc);		//显示顶部游戏状态信息
		gameMenu->DrawMenu(hdc);
		break;
	}
	case GAME_UPGRADE:		//新关卡加载界面
	{
		//显示加载界面
		DisplayInfo(hdc);		//显示顶部游戏状态信息
		break;
	}
	case GAME_OVER:			//游戏结束界面
	{
		//显示加载界面
		DisplayInfo(hdc);		//显示顶部游戏状态信息
		break;
	}
	case GAME_HELP:			//游戏帮助界面
	{
		//显示加载界面
		break;
	}
	case GAME_ABOUT:		//游戏制作信息界面
	{
		//显示加载界面
		break;
	}
	}
}

//游戏退出处理
void MarioGame::GameEnd()
{
	
}

//游戏按键处理,keys[] true:按下，false:释放
//键按下时会持续发出WM_KEYDOWN消息
void MarioGame::GameKeyAction(int Action)
{
	switch (gameStatus)
	{
	case GAME_START:		//游戏开始菜单
	{		
		if (Action == KEY_DOWN)		//按下键
		{
			if (keys[VK_RETURN])		//Enter键
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
		else if (Action == KEY_UP)	//释放键
		{
			Util::myprintf(L"release key \n");
		}
		break;
	}
	case GAME_RUN:			//游戏进行时界面
	{
		if (Action == KEY_DOWN)		//按下键
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
		else if (Action == KEY_UP)	//释放键
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
	case GAME_PAUSE:		//暂停游戏界面
	{
		break;
	}
	case GAME_UPGRADE:		//新关卡加载界面
	{
		break;
	}
	case GAME_OVER:			//游戏结束界面
	{
		break;
	}
	case GAME_HELP:			//游戏帮助界面
	{
		break;
	}
	case GAME_ABOUT:		//游戏制作信息界面
	{
		break;
	}
	}
}

//游戏鼠标行为处理(参数值为KEY_MOUSE_ACTION)
void MarioGame::GameMouseAction(int x, int y, int Action)	
{
	
}
// 加载游戏地图场景,可包括背景、障碍、遮罩层
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

	// 视图初始位置以地图作为参照
	int scn_x = 0;
	int scn_y = -t_scene->getBarrier()->getTileHeight()/2;
	// 将游戏地图初始化
	t_scene->InitScene(scn_x, scn_y, scn_width, scn_height, wnd_width, wnd_height);

	// 将所有地图图层定位
	SCENE_LAYERS::iterator p;
	for (p = t_scene->getSceneLayers()->begin(); p != t_scene->getSceneLayers()->end(); p++)
	{
		if (p->layer->ClassName() == "T_Map") p->layer->SetPosition(scn_x, scn_y);
	}
}
// 加载游戏玩家角色
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

// 加载游戏菜单
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
// 加载游戏图片资源
void MarioGame::LoadImageRes()
{
	if (status == NULL)	status = new T_Graph(L".\\res\\game\\npc.png");
	
}
// 加载游戏声音资源
void MarioGame::LoadSound(HWND hwnd)
{
	gameOverSound = new AudioDXBuffer;
	gameWinSound = new AudioDXBuffer;
	
	if (!dxSnd.CreateDS(hwnd))  return;
	gameOverSound->LoadWave(dxSnd, L".\\res\\sound\\gameover.wav");
	gameWinSound->LoadWave(dxSnd, L".\\res\\sound\\victory.wav");
}

// 管理游戏中的各种声音播放
void MarioGame::PlayGameSound()
{
	bkgMusic->Terminate();			// 游戏背景音乐
	gameOverSound->Terminate();		// 游戏结束声音
	gameWinSound->Terminate();		// 游戏胜利声音

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
// 释放声音缓冲
void MarioGame::ReleaseDXBuffer(AudioDXBuffer* dxBuf)
{
	if (dxBuf != NULL)
	{
		dxBuf->Release();
		delete dxBuf;
		dxBuf = NULL;
	}
}
// 加载关卡数据
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

// 清除关卡数据
void MarioGame::ClearGameLevel()
{

}

// 根据游戏状态，显示信息
void MarioGame::DisplayInfo(HDC hdc)
{
	int FontHeight = 0;//字号
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

