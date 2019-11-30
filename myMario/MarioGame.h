#pragma once

#include "T_Engine.h"
#include "GameScene.h"
#include "T_AI.h"
#include "T_Audio.h"
#include "T_Menu.h"
#include "Player.h"
#include "MinionFactory.h"

class MarioGame:public T_Engine
{
private:	
	int scn_width, scn_height;						// 地图场景宽高
	int wnd_width, wnd_height;						// 游戏窗口宽高
	int gameTime;									// 游戏已进行时间

	//前一步按键状态
	bool preA;
	bool preS;
	bool preD;
	bool preL;
	bool preCapital;


	//游戏常量
	static const int TOTAL_LEVEL = 5;			// 游戏总关数
	static const int SUPER_TIME = 5000;			// 玩家角色无敌时间/ms
	static const int BEGIN_TIME = 5000;			// 游戏开始音乐播放时间
	static const int WARN_TIME = 15000;			// 游戏时间即将耗尽，剩余时间

	// 下面这些常量主要是方便 闪烁的小金币 进行定位 （状态栏中小金币的位置信息与其他文字的位置信息并不在一起初始化）
	static const int STATUS_BEGIN_X = WIN_WIDTH / 12;				// 状态栏文字起始横坐标
	static const int STATUS_BEGIN_Y = 0;							// 状态栏文字起始纵坐标
	static const int STATUS_TEXT_WIDTH = WIN_WIDTH / 8;
	static const int STATUS_TEXT_HEIGHT = WIN_HEIGHT / 18;
	static const int STATUS_TEXT_HORIZON_SPACE = WIN_WIDTH / 4;		// 状态栏文字间距
	static const int STATUS_TEXT_VERTICAL_SPACE = WIN_HEIGHT / 19;	// 状态栏垂直间距

	//关卡相关数据
	int timeLimit = 300000;						// 关卡时间限制
	int gameLevel;								// 当前关卡

	//资源文件
	//声音
	AudioDX			dxSnd;							// DirectSound对象
	AudioDXBuffer*	gameOverSound;					// 游戏结束声音
	AudioDXBuffer*	gameWinSound;					// 游戏胜利声音
	AudioDXBuffer*	gameStartSound;					// 游戏开始声音
	AudioDXBuffer*	bkgMusic;						// 游戏背景音乐
	AudioDXBuffer*	timeLastingSound;				// 游戏时间即将耗尽音乐
	//...
	//图片
	T_Sprite* statusCoin;							// 状态栏小金币图片
	T_Sprite* statusMario;							// 关卡升级界面 居中的马里奥图片
	//...
	
	
	// 与游戏对象类相关的变量
	Player* player;									// 游戏玩家
	GameScene* gameScene;							// 游戏场景
	Color bkColor;									// 游戏背景色


	//------------------与菜单相关的变量-----------------------------------
	GameMenu* gameMenu;								// 游戏菜单

	//与 MAIN_MENU 相关的变量
	GAMELAYER marioBoard;							// 超级大的棕色牌子

	//与 ABOUT_MENU 相关的变量
	int changeIndex;								// 变换的颜色索引	0-R 1-G 2-B
	double red, green, blue;						// 三原色 用于控制背景色改变
	double lastRed, lastGreen, lastBlue;

	//---------------------------------------------------------

public:
	
	MarioGame(HINSTANCE h_instance, LPCTSTR sz_winclass, LPCTSTR sz_title, 
			 WORD icon = NULL, WORD sm_icon = NULL, 
			 int winwidth = 1024, int winheight = 768);
	virtual ~MarioGame(void);
	
	
	void GameInit();								// 游戏初始化	
	void GameLogic();								// 游戏逻辑处理
	void GameEnd();									// 游戏结束处理
	void GamePaint(HDC hdc);						// 游戏显示
	void GameKeyAction(int Action=KEY_SYS_NONE);	// 按键行为处理	
	void GameMouseAction(int x, int y, int Action); // 鼠标行为处理	

	//关卡相关操作
	void LoadGameLevel(int level);
	void ClearGameLevel();

	//资源加载
	void LoadSound(HWND hwnd);						// 加载游戏声音资源
	void LoadImageRes();							// 加载游戏图片资源
	
	//游戏对象加载
	void LoadPlayer();								// 加载游戏玩家角色
	void playerRelife()
	{
		//清除按键状态
		preA = preS = preD = preL = preCapital = false;
		
		player->stopEvent();
		player->SetPosition(wnd_width / 5, 200);
		player->SetDead(false);
		player->SetActive(true);
		player->SetVisible(true);
	}
	void LoadMap();									// 加载游戏地图场景	
	void LoadGameMenu();							// 加载游戏菜单

	//音频播放，信息显示
	void PlayGameSound();							// 管理游戏中各时间段背景声音播放
	void DisplayInfo(HDC hdc);						// 显示......如关卡加载中的信息
	void ShowTitleInfo(HDC hdc);					// 显示顶部游戏状态信息
	void ReleaseDXBuffer(AudioDXBuffer* dxBuf);		// 释放声音缓冲

	//地图重定位
	//void RePosition();
};