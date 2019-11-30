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
	int scn_width, scn_height;						// ��ͼ�������
	int wnd_width, wnd_height;						// ��Ϸ���ڿ��
	int gameTime;									// ��Ϸ�ѽ���ʱ��

	//ǰһ������״̬
	bool preA;
	bool preS;
	bool preD;
	bool preL;
	bool preCapital;


	//��Ϸ����
	static const int TOTAL_LEVEL = 5;			// ��Ϸ�ܹ���
	static const int SUPER_TIME = 5000;			// ��ҽ�ɫ�޵�ʱ��/ms
	static const int BEGIN_TIME = 5000;			// ��Ϸ��ʼ���ֲ���ʱ��
	static const int WARN_TIME = 15000;			// ��Ϸʱ�伴���ľ���ʣ��ʱ��

	// ������Щ������Ҫ�Ƿ��� ��˸��С��� ���ж�λ ��״̬����С��ҵ�λ����Ϣ���������ֵ�λ����Ϣ������һ���ʼ����
	static const int STATUS_BEGIN_X = WIN_WIDTH / 12;				// ״̬��������ʼ������
	static const int STATUS_BEGIN_Y = 0;							// ״̬��������ʼ������
	static const int STATUS_TEXT_WIDTH = WIN_WIDTH / 8;
	static const int STATUS_TEXT_HEIGHT = WIN_HEIGHT / 18;
	static const int STATUS_TEXT_HORIZON_SPACE = WIN_WIDTH / 4;		// ״̬�����ּ��
	static const int STATUS_TEXT_VERTICAL_SPACE = WIN_HEIGHT / 19;	// ״̬����ֱ���

	//�ؿ��������
	int timeLimit = 300000;						// �ؿ�ʱ������
	int gameLevel;								// ��ǰ�ؿ�

	//��Դ�ļ�
	//����
	AudioDX			dxSnd;							// DirectSound����
	AudioDXBuffer*	gameOverSound;					// ��Ϸ��������
	AudioDXBuffer*	gameWinSound;					// ��Ϸʤ������
	AudioDXBuffer*	gameStartSound;					// ��Ϸ��ʼ����
	AudioDXBuffer*	bkgMusic;						// ��Ϸ��������
	AudioDXBuffer*	timeLastingSound;				// ��Ϸʱ�伴���ľ�����
	//...
	//ͼƬ
	T_Sprite* statusCoin;							// ״̬��С���ͼƬ
	T_Sprite* statusMario;							// �ؿ��������� ���е������ͼƬ
	//...
	
	
	// ����Ϸ��������صı���
	Player* player;									// ��Ϸ���
	GameScene* gameScene;							// ��Ϸ����
	Color bkColor;									// ��Ϸ����ɫ


	//------------------��˵���صı���-----------------------------------
	GameMenu* gameMenu;								// ��Ϸ�˵�

	//�� MAIN_MENU ��صı���
	GAMELAYER marioBoard;							// ���������ɫ����

	//�� ABOUT_MENU ��صı���
	int changeIndex;								// �任����ɫ����	0-R 1-G 2-B
	double red, green, blue;						// ��ԭɫ ���ڿ��Ʊ���ɫ�ı�
	double lastRed, lastGreen, lastBlue;

	//---------------------------------------------------------

public:
	
	MarioGame(HINSTANCE h_instance, LPCTSTR sz_winclass, LPCTSTR sz_title, 
			 WORD icon = NULL, WORD sm_icon = NULL, 
			 int winwidth = 1024, int winheight = 768);
	virtual ~MarioGame(void);
	
	
	void GameInit();								// ��Ϸ��ʼ��	
	void GameLogic();								// ��Ϸ�߼�����
	void GameEnd();									// ��Ϸ��������
	void GamePaint(HDC hdc);						// ��Ϸ��ʾ
	void GameKeyAction(int Action=KEY_SYS_NONE);	// ������Ϊ����	
	void GameMouseAction(int x, int y, int Action); // �����Ϊ����	

	//�ؿ���ز���
	void LoadGameLevel(int level);
	void ClearGameLevel();

	//��Դ����
	void LoadSound(HWND hwnd);						// ������Ϸ������Դ
	void LoadImageRes();							// ������ϷͼƬ��Դ
	
	//��Ϸ�������
	void LoadPlayer();								// ������Ϸ��ҽ�ɫ
	void playerRelife()
	{
		//�������״̬
		preA = preS = preD = preL = preCapital = false;
		
		player->stopEvent();
		player->SetPosition(wnd_width / 5, 200);
		player->SetDead(false);
		player->SetActive(true);
		player->SetVisible(true);
	}
	void LoadMap();									// ������Ϸ��ͼ����	
	void LoadGameMenu();							// ������Ϸ�˵�

	//��Ƶ���ţ���Ϣ��ʾ
	void PlayGameSound();							// ������Ϸ�и�ʱ��α�����������
	void DisplayInfo(HDC hdc);						// ��ʾ......��ؿ������е���Ϣ
	void ShowTitleInfo(HDC hdc);					// ��ʾ������Ϸ״̬��Ϣ
	void ReleaseDXBuffer(AudioDXBuffer* dxBuf);		// �ͷ���������

	//��ͼ�ض�λ
	//void RePosition();
};