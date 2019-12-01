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
	GameState = GAME_START;			//调试 ABOUT_MENU
	LoadGameLevel(gameLevel);		//加载关卡资源、地图、玩家
	LoadGameMenu();					//加载主菜单	
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
			if (player->IsVisible())	//未死亡或播放死亡动画未播放完
				player->update();
			if (player->IsDead())	//玩家死亡，死亡动画播放完
			{
				if (player->getLifeCount() == 0)
				{
					GameState = GAME_OVER;				//玩家生命值为0，游戏结束
				}
				else
				{
					GameState = GAME_UPGRADE;			//玩家生命值不为0，继续游戏
					//加载复活点
					playerRelife();
				}
			}
			gameScene->update();						//更新地图、怪物、玩家状态
			gameScene->ScrollScene(player);			//根据玩家位置，滚动场景
			
			break;
		}
		case GAME_PAUSE:		//暂停游戏界面
		{
			break;
		}
		case GAME_UPGRADE:		//新关卡加载界面
		{
			if (T_Util::Timer(3))	//关卡加载界面...虽然此时并没有资源在后台加载 但生活需要仪式感
				GameState = GAME_RUN;
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
			
			//背景颜色变换
			T_Util::ChangeRGB(&red,&green,&blue,&lastRed,&lastBlue,&lastGreen,&changeIndex);
			// 坐标处理
			gameScene->ScrollScene(-3);	// 自己滚！
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
		T_Graph::PaintBlank(hdc, 0, 0, wnd_width, wnd_height, RGB(93, 148, 252), 255);
		gameScene->Draw(hdc);
		gameMenu->DrawMenu(hdc);
		DisplayInfo(hdc);		//显示顶部游戏状态信息
		break;
	}
	case GAME_RUN:			//游戏进行时界面
	{
		T_Graph::PaintBlank(hdc,0,0,wnd_width,wnd_height,RGB(100,200,200),255);
		gameScene->Draw(hdc);
		DisplayInfo(hdc);		//显示顶部游戏状态信息

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
		DisplayInfo(hdc);		//显示顶部游戏状态信息
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
		T_Graph::PaintBlank(hdc,0,0,wnd_width,wnd_height,RGB(ROUND(red),ROUND(green),ROUND(blue)),255);
		gameScene->Draw(hdc);
		DisplayInfo(hdc);
		gameMenu->DrawMenu(hdc);
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
				gameScene->Remove(marioBoard);					//无论选择哪个功能，都需要先将广告牌移除

				switch (gameMenu->GetMenuIndex()) {
				case 0:	//开始
					GameState = GAME_UPGRADE;
				//	LoadGameMenu();
					break;
				case 1:	
					//GameState = GAME_SETTING;
					break;
				case 2:	//关于
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
							player->stopBooting();		//释放键，停止加速
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
		if (Action == KEY_DOWN)		//按下键
		{
			if (keys[VK_RETURN])		//Enter键
			{
				switch (gameMenu->GetMenuIndex()) {
				case 0:	//开始
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

	gameScene->RePosition(wnd_width,wnd_height);
	//scn_width = gameScene->getBarrier()->GetWidth();
	//scn_height = gameScene->getBarrier()->GetHeight();

	//// 视图初始位置以地图作为参照
	//int scn_x = 0;
	//int scn_y = -gameScene->getBarrier()->getTileHeight()/2;
	//// 将游戏地图初始化
	//gameScene->InitScene(scn_x, scn_y, scn_width, scn_height, wnd_width, wnd_height);

	//// 将所有地图图层定位
	//SCENE_LAYERS::iterator p;
	//for (p = gameScene->getSceneLayers()->begin(); p != gameScene->getSceneLayers()->end(); p++)
	//{
	//	if (p->layer->ClassName() == "T_Map") p->layer->SetPosition(scn_x, scn_y);
	//}
}

 //加载游戏玩家角色
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

	// ----- 初始化马里奥的开始状态
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


	// ----- 初始化马里奥嗑药后的状态
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

// 加载游戏菜单
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
		//----------------开始菜单初始化----------------
		x = 0, y = 0;
		btn_width = 0, btn_height = 0;

		wstring menuItems[] = { L"1 PLAYER GAME",L"OPTIONS",L"ABOUT" };

		//菜单项
		btn_width = 200;
		btn_height = 32;
		normalClr = Color::White;
		focusClr = Color::White;
		gameMenu->SetBtnBmp(L".\\res\\map\\option_active.png", btn_width, btn_height);

		//设置菜单信息
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
			//垂直布局的坐标
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
		marioBoard.layer->SetPosition(wnd_width / 12 , 48 + 18);	// 再往下偏移18px
		gameScene->Append(marioBoard);
	}
	else if (GameState == GAME_UPGRADE) {
		
	}
	else if (GameState == GAME_ABOUT) {

		x = 0, y = 0;
		btn_width = 0, btn_height = 0;

		wstring menuItems[] = { L"MAIN MENU"};

		//菜单项
		btn_width = 200;
		btn_height = 32;
		normalClr = Color::White;
		focusClr = Color::White;
		gameMenu->SetBtnBmp(L".\\res\\map\\option_active.png", btn_width, btn_height);

		//设置菜单信息
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
			//垂直布局的坐标
			x = wnd_width / 6;
			y = wnd_height / 6 + 9 * 25;
			MENUITEM mItem;
			mItem.pos.x = x;
			mItem.pos.y = y;
			mItem.ItemName = menuItems[i];
			gameMenu->AddMenuItem(mItem);

		}
		
		// 与场景相关的变量
		red = green = blue = 11;
		gameScene->SetScenePos(-1, 0);	//场景重新定位
		changeIndex = 0;
	}
}

// 加载游戏图片资源
void MarioGame::LoadImageRes()
{
	// 闪烁的小金币
	if (statusCoin == NULL)
		statusCoin = new T_Sprite(L".\\res\\map\\statusCoin.png", 10, 16);

	// 关卡加载时 位于正中间的马里奥图片
	if (statusMario == NULL)
		statusMario = new T_Sprite(L".\\res\\sprite\\sMario.png", 24, 32);

	SPRITEINFO info;
	// 金币
	info.X = STATUS_BEGIN_X + STATUS_TEXT_HORIZON_SPACE + STATUS_TEXT_WIDTH/6;									// 就先占六分之一吧，一局也没多少金币 数字不会与图案重叠
	info.Y = STATUS_BEGIN_Y + STATUS_TEXT_VERTICAL_SPACE + STATUS_TEXT_HEIGHT/2 - statusCoin->GetHeight()/2;	// 图案与文字水平线居中
	info.Dir = 0;
	info.Rotation = 0;
	info.Ratio = 1;
	info.Alpha = 255;
	info.Visible = true;
	statusCoin->Initiate(info);

	// 马里奥
	info.X = wnd_width*4/9 ;									
	info.Y = wnd_height/2;				
	info.Dir = 0;
	info.Rotation = 0;
	info.Ratio = 1;
	info.Alpha = 255;
	info.Visible = true;
	statusMario->Initiate(info);


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

	if (gameScene == NULL) gameScene = GameScene::getInstance();
//	if (gameMenu == NULL) gameMenu = new T_Menu();
	gameScene->appendMinion(MINION_TYPE::MINION_GOOMBA,1100,200);
	//LoadSound(m_hWnd);
	LoadImageRes();
//	LoadMenu();
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
	int FontHeight = 16;	 // 字号
	wstring fontName = L"Comic Sans Ms"; // 字体
	RectF textRect;			 // 文字域
	vector<wstring> content; // 文字内容
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
		T_Graph::PaintBlank(hdc,0,0,wnd_width,wnd_height,RGB(0,0,0),255);	// 背景
		ShowTitleInfo(hdc);													// 顶部状态栏
		statusMario->Draw(hdc);												// 马里奥
		
		// 关卡信息
		textRect.X = (REAL)statusMario->GetX()*1.0 - 2 * statusMario->GetWidth();
		textRect.Y = (REAL)statusMario->GetY()*1.0 - 2 * statusMario->GetHeight(); 
		textRect.Width = wnd_width;
		textRect.Height = 30;
		content.push_back(L" WORLD 1-1 ");
		T_Graph::PaintText(hdc, textRect, content[0].c_str(), FontHeight, fontName.c_str(),
			Color::White, FontStyleBold, StringAlignmentNear);

		// 马里奥生命信息														
		textRect.X = statusMario->GetX() + statusMario->GetWidth();
		textRect.Y = statusMario->GetY();// statusMario->GetHeight()/3;
		textRect.Width = wnd_width*1.0;
		textRect.Height = 30;
		content.push_back(L" X ");
		content[1].append(T_Util::int_to_wstring(player->getLifeCount()));
		T_Graph::PaintText(hdc, textRect, content[1].c_str(), FontHeight, fontName.c_str(),	
			Color::White, FontStyleBold, StringAlignmentNear);
		
		// 底部提示语
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

		//制作信息
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

// 显示顶端状态栏
void MarioGame::ShowTitleInfo(HDC hdc) {

	int FontHeight = 16;	 // 字号
	wstring fontName = L"Comic Sans Ms"; // 字体
	RectF textRect;			 // 文字域
	vector<wstring> content; // 文字内容


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
		if (i == 5) {	//金币数前需要打印一个图片
			statusCoin->Draw(hdc);
			statusCoin->LoopFrame(20,true);	//如此裸露的维护帧计数器。。。。
		}	
		
		T_Graph::PaintText(hdc, textRect, content[i].c_str(), FontHeight, fontName.c_str(),
			Color::White, FontStyleBold, StringAlignmentCenter);
		textRect.X += STATUS_TEXT_HORIZON_SPACE;

		// 每输出四个文字项进行一次换行
		if ((i+1) % 4 == 0) {
			textRect.X = STATUS_BEGIN_X;
			textRect.Y += STATUS_TEXT_VERTICAL_SPACE;
		}
	}
}
