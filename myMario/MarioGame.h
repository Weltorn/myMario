#pragma once

#include "T_Engine.h"
#include "GameScene.h"
#include "T_AI.h"
#include "T_Audio.h"
#include "T_Menu.h"
#include "Player.h"

class MarioGame:public T_Engine
{
private:	
	int scn_width, scn_height;						// 地图场景宽高
	int wnd_width, wnd_height;						// 游戏窗口宽高
	int gameTime;									// 游戏已进行时间

	//游戏常量
	static const int TOTAL_LEVEL = 5;			// 游戏总关数
	static const int SUPER_TIME = 5000;			// 玩家角色无敌时间/ms
	static const int BEGIN_TIME = 5000;			// 游戏开始音乐播放时间
	static const int WARN_TIME = 15000;			// 游戏时间即将耗尽，剩余时间

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
	T_Graph* status;								// 状态栏小图片
	//...
	
	
	// 与游戏对象类相关的变量
	Player* player;								// 游戏玩家
	GameScene* gameScene;						// 游戏场景
	T_Menu* gameMenu;							// 游戏菜单
	
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
	void LoadMap();									// 加载游戏地图场景	
	void LoadGameMenu(int gameStatus);					// 加载游戏菜单

	//音频播放，信息显示
	void PlayGameSound();							// 管理游戏中各时间段背景声音播放
	void DisplayInfo(HDC hdc);						//显示顶部游戏状态信息
	void ReleaseDXBuffer(AudioDXBuffer* dxBuf);		// 释放声音缓冲

};