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
	GameState = GAME_RUN;			//调试准备，跳过菜单
	LoadGameLevel(gameLevel);		//加载关卡资源、地图、玩家
	//LoadGameMenu(GameState);		//加载主菜单	
}

//游戏逻辑处理(处理GameState)
void MarioGame::GameLogic()	
{
	switch (GameState)
	{
		case GAME_START:		//游戏开始菜单
		{
			//gameMenu->update();
			break;
		}
		case GAME_RUN:			//游戏进行时界面
		{
			GameKeyAction();
			gameTime = GetTickCount();		//更新游戏已运行时间	

			//更新玩家	
			if (!player->IsDead() && player->IsVisible())	//未死亡或播放死亡动画未播放完
				player->update();
			if (player->IsDead() && !player->IsVisible())	//玩家死亡，死亡动画播放完
			{
				if (player->getLifeCount() == 0)
				{
					GameState = GAME_OVER;				//玩家生命值为0，游戏结束
				}
				else
				{
					GameState = GAME_UPGRADE;			//玩家生命值不为0，继续游戏
					//加载复活点
				}
			}
			//gameScene->ScrollScene(player);				//根据玩家位置，滚动场景
			gameScene->update();						//更新地图、怪物、玩家状态
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
	switch (GameState)
	{
	case GAME_START:		//游戏开始菜单
	{
		//gameScene->Draw(hdc);
		//DisplayInfo(hdc);		//显示顶部游戏状态信息
		//gameMenu->DrawMenu(hdc);
		break;
	}
	case GAME_RUN:			//游戏进行时界面
	{
		//DisplayInfo(hdc);		//显示顶部游戏状态信息
		T_Graph::PaintBlank(hdc,0,0,wnd_width,wnd_height,RGB(100,200,200),255);
		gameScene->Draw(hdc);
		break;
	}
	case GAME_PAUSE:		//暂停游戏界面
	{
		//gameScene->Draw(hdc);
		//DisplayInfo(hdc);		//显示顶部游戏状态信息
		//gameMenu->DrawMenu(hdc);
		break;
	}
	case GAME_UPGRADE:		//新关卡加载界面
	{
		//显示加载界面
		//DisplayInfo(hdc);		//显示顶部游戏状态信息
		break;
	}
	case GAME_OVER:			//游戏结束界面
	{
		//显示加载界面
		//DisplayInfo(hdc);		//显示顶部游戏状态信息
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
	switch (GameState)
	{
	case GAME_START:		//游戏开始菜单
	{		
		if (Action == KEY_DOWN)		//按下键
		{
			if (keys[VK_RETURN])		//Enter键
			{
			}
			else if(keys[VK_UP])		//PgUp
			{
			}
			else if (keys[VK_DOWN])		//PgDn
			{

			}
		}
		else if (Action == KEY_UP)	//释放键
		{
		}
		break;
	}
	case GAME_RUN:			//游戏进行时界面
	{
		if (player->IsActive() && !player->isInEvent())	//玩家处于活跃状态，监听用户按键
		{
			if (Action == KEY_DOWN)		//按下键
			{
				if (keys[VK_A])
				{
					if (!keys[VK_D])		//若按住反向键，无效
					{
						//在地面或其他支持物上
						if (player->isOnPlantform() && !player->isSliding())
						{
							player->SetDir(DIR_LEFT);
							if (!preA && !keys[VK_S]) {					//如果是第一次按下键D,设置为开始移动
								player->startMove();
								preA = true;
							}
						}
						if (player->isOnPlantform() && player->isSliding() && player->GetDir() == DIR_RIGHT) {
							player->setDirChanged(true);
						}

						//在空中
						if (!player->isOnPlantform())
						{
							player->SetDir(DIR_LEFT);
							if (!preA) {					//如果是第一次按下键D,设置为开始移动
								player->startMove();
								preA = true;
							}
						}
					}
				}
				if (keys[VK_D])
				{
					if (!keys[VK_A])		//若按住反向键，无效
					{		
						//在地面或其他支持物上
						if (player->isOnPlantform()&&!player->isSliding())
						{
							player->SetDir(DIR_RIGHT);
							if (!preD && !keys[VK_S]) {					//如果是第一次按下键D,设置为开始移动
								player->startMove();
								preD = true;
							}						
						}
						if (player->isOnPlantform() && player->isSliding() && player->GetDir() == DIR_LEFT) {
							player->setDirChanged(true);
						}

						//在空中
						if (!player->isOnPlantform())
						{
							player->SetDir(DIR_RIGHT);
							if (!preD) {					//如果是第一次按下键D,设置为开始移动
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
			else if (Action == KEY_UP)	//释放键
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
							player->stopBooting();		//释放键，停止加速
							if (player->GetSpeedY() > 3) {
								player->SetSpeedY(3);
							}
						}
					}
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
	if (!gameScene->LoadTxtMap("res\\map\\marioMap.txt"))
	{
		Util::myprintf(L"Load map failure!\n");
		exit(1);
	}
	scn_width = gameScene->getBarrier()->GetWidth();
	scn_height = gameScene->getBarrier()->GetHeight();

	// 视图初始位置以地图作为参照
	int scn_x = 0;
	int scn_y = -gameScene->getBarrier()->getTileHeight()/2;
	// 将游戏地图初始化
	gameScene->InitScene(scn_x, scn_y, scn_width, scn_height, wnd_width, wnd_height);

	// 将所有地图图层定位
	SCENE_LAYERS::iterator p;
	for (p = gameScene->getSceneLayers()->begin(); p != gameScene->getSceneLayers()->end(); p++)
	{
		if (p->layer->ClassName() == "T_Map") p->layer->SetPosition(scn_x, scn_y);
	}
}

 //加载游戏玩家角色
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

	// ----- 初始化马里奥的开始状态
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


	// ----- 初始化马里奥嗑药后的状态
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
	gameScene = NULL;

	if (gameScene == NULL) gameScene = new GameScene();
	if (gameMenu == NULL) gameMenu = new T_Menu();

	//LoadSound(m_hWnd);
	//LoadImageRes();

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

