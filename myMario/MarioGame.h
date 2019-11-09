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
	int scn_width, scn_height;						// ��ͼ�������
	int wnd_width, wnd_height;						// ��Ϸ���ڿ��
	int gameTime;									// ��Ϸ�ѽ���ʱ��

	//��Ϸ����
	static const int TOTAL_LEVEL = 5;			// ��Ϸ�ܹ���
	static const int SUPER_TIME = 5000;			// ��ҽ�ɫ�޵�ʱ��/ms
	static const int BEGIN_TIME = 5000;			// ��Ϸ��ʼ���ֲ���ʱ��
	static const int WARN_TIME = 15000;			// ��Ϸʱ�伴���ľ���ʣ��ʱ��

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
	T_Graph* status;								// ״̬��СͼƬ
	//...
	
	
	// ����Ϸ��������صı���
	Player* player;								// ��Ϸ���
	GameScene* gameScene;						// ��Ϸ����
	T_Menu* gameMenu;							// ��Ϸ�˵�
	
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
	void LoadMap();									// ������Ϸ��ͼ����	
	void LoadGameMenu(int gameStatus);					// ������Ϸ�˵�

	//��Ƶ���ţ���Ϣ��ʾ
	void PlayGameSound();							// ������Ϸ�и�ʱ��α�����������
	void DisplayInfo(HDC hdc);						//��ʾ������Ϸ״̬��Ϣ
	void ReleaseDXBuffer(AudioDXBuffer* dxBuf);		// �ͷ���������

};