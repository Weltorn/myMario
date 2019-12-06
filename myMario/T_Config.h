//*******************************************************************
// TinyEngine����  
// ����: ������(WanLizhong)
// ����: www.wanlizhong.com 
// ����: 2013-08-02
// ��Ȩ���� 2007-2019 ������
// (C) 2007-2019 WanLizhong All Rights Reserved
//*******************************************************************

#ifndef TCONFIG_H
#define TCONFIG_H

#include <windows.h>
#include <typeinfo.h>
#include <time.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;
#include <gdiplus.h>
#pragma comment(lib,"Msimg32.lib")
#pragma comment(lib,"gdiplus.lib")
using namespace Gdiplus;


static const int WIN_WIDTH		 = 802;				// Ĭ����Ϸ���ڿ�
static const int WIN_HEIGHT		 = 453;				// Ĭ����Ϸ���ڸ�
static const COLORREF WIN_BKCLR = RGB(50, 50, 50);	// Ĭ�ϴ��ڱ���ɫ(����: 2019-09-28)

static const int FRAME_SPEED	 = 75;				// Ĭ����Ϸ֡Ƶ

static const int MENU_WIDTH		 = 200;				// Ĭ�ϲ˵���
static const int MENU_HEIGHT	 = 60;				// Ĭ�ϲ˵���
static const int MENU_SPACE		 = 10;				// Ĭ�ϲ˵��������
static const int MENU_ALPHA		 = 180;				// Ĭ�ϲ˵�������͸����
static const COLORREF MENU_BKCLR = RGB(0, 0, 0);	// Ĭ�ϲ˵�����ɫ


static const int LAYER_MAX		= 1000;				// Ĭ����Ϸͼ�����ֵ

static const LPCTSTR WIN_CLASS	
	= TEXT("TinyEngine");;							// Ĭ��ע��Ĵ�������
static const LPCTSTR WIN_TITLE						// Ĭ�ϴ��ڱ���
	= TEXT("TinyEngine Ver 1.0 ����:������(C) 2007-2013");

// ��Ϸ����������ֵΪ�������Ӧ�ļ�ֵ
enum GAME_DIR
{
	DIR_LEFT,					// ��
	DIR_RIGHT,					// ��
	DIR_UP,						// ��
	DIR_DOWN,					// ��
	DIR_LEFT_UP,				// ����
	DIR_LEFT_DOWN,				// ����
	DIR_RIGHT_UP,				// ����
	DIR_RIGHT_DOWN,				// ����
	DIR_NONE
};

// ͼ����ƴ�����
enum TRANSFER
{
	TRANS_NONE,					// ԭʼͼ��
	TRANS_NOFLIP_ROT90,			// ԭʼͼ��˳ʱ����ת90��
	TRANS_NOFLIP_ROT180,		// ԭʼͼ��˳ʱ����ת180��
	TRANS_NOFLIP_ROT270,		// ԭʼͼ��˳ʱ����ת270��
	TRANS_VFLIP_NOROT,			// ԭʼͼ��ֱ��ת
	TRANS_VFLIP_ROT90,			// ԭʼͼ��ֱ��ת��˳ʱ����ת90��
	TRANS_VFLIP_ROT180,			// ԭʼͼ��ֱ��ת��˳ʱ����ת180��
	TRANS_VFLIP_ROT270,			// ԭʼͼ��ֱ��ת��˳ʱ����ת27��
	TRANS_HFLIP_NOROT,			// ԭʼͼ��ˮƽ��ת
	TRANS_HFLIP_ROT90,			// ԭʼͼ��ˮƽ��ת��˳ʱ����ת90��
	TRANS_HFLIP_ROT180,			// ԭʼͼ��ˮƽ��ת��˳ʱ����ת180��
	TRANS_HFLIP_ROT270			// ԭʼͼ��ˮƽ��ת��˳ʱ����ת270��
};

// ��Ϸ״̬ö�ٳ�������
enum GAME_STATE
{
	GAME_START,					// ��ʼ
	GAME_RUN,					// ����
	GAME_PAUSE,					// ��ͣ
	GAME_UPGRADE,				// ����
	GAME_WIN,					// ͨ��
	GAME_OVER,					// ����
	GAME_HELP,					// ����״̬
	GAME_ABOUT,					// ����״̬
	GAME_SETTING				// ����״̬
};
 
// ������Ϊ����
enum KM_ACTION
{
	KEY_SYS_NONE,				// ��ϵͳ��������
	KEY_UP,						// �ͷŰ���
	KEY_DOWN,					// ���¼�
	MOUSE_MOVE,					// ����ƶ�
	MOUSE_LCLICK,				// ���������
	MOUSE_RCLICK				// ����Ҽ����
};

// ͼ������
enum LAYER_TYPE
{
	LAYER_PLY,					// ���
	LAYER_NPC,					// NPC
	LAYER_PLY_BOMB,				// ����ڵ�
	LAYER_NPC_BOMB,				// NPC�ڵ�
	LAYER_MAP_BACK,				// ��ͼ������
	LAYER_MAP_BARR,				// ��ͼ�ϰ���
	LAYER_MAP_MASK,				// ��ͼ���ֲ�
	LAYER_EXPLOSION,			// ��ըЧ��ͼ��
	LAYER_TARGET,				// �����ͱ�����Ŀ��
	LAYER_BONUS,				// ��Ϸ�еĽ���
	LAYER_NONE,					// ��Ϸ�з���Ϸ����
	LAYER_NORMAL_BRICK,
	LAYER_PROP_BRICK,
	LAYER_BRICK,
	LAYER_DECORATION			//װ�Ρ�Ч��ͼ��
};
// ͼ������
enum VK_KEYVALUE
{
	VK_A = 0x41,					
	VK_B,VK_C,VK_D,VK_E,VK_F,VK_G,VK_H,VK_I,VK_J,VK_K,VK_L,VK_M,
	VK_N,VK_O,VK_P,VK_Q,VK_R,VK_S,VK_T,VK_U,VK_V,VK_W,VK_X, VK_Y, VK_Z
};

// ��Ϸ��ɫ�ؼ���Ϣ(ÿ��������ɫ��������ȫ��ֵ)
typedef struct
{
	int		X;					// ��ɫ��ʼX����
	int		Y;					// ��ɫ��ʼY����
	int		Dir;				// ��ɫ��ʼ����(ȡֵΪGAME_DIR����)
	int		SpeedX;				// ��ɫ��ʼ��ˮƽ�ƶ��ٶ�
	int		SpeedY;				// ��ɫ��ʼ�Ĵ�ֱ�ƶ��ٶ�
	int		Level;				// ��ɫ��ʼ�ĵȼ�
	int		Score;				// ��ɫ��ɫ��ֵ 
	bool	Visible;			// ��ɫ����Ļ���Ƿ�ɼ�
	bool	Active;				// ��ɫ��״̬���Ƿ��ƶ���
	bool	Dead;				// ��ɫ�Ƿ�������״̬
	BYTE	Alpha;				// ��ɫ͸����
	float	Ratio;				// ����֡ͼƬ�Ŵ����С��
								// ����1ԭʼ������1�Ŵ�С��1��С
	int		Rotation;			// ֡ͼƬ�Ƿ���任(ȡֵΪTRANSFER����)
}SPRITEINFO;

//��ͼÿ��ͼ����Ϣ
typedef struct
{
	int		type_id;			// ͼ���ʶ��(ȡֵΪLAYER_TYPE����)
	int		map_cols;			// ��ͼ���(ͼ������)
	int		map_rows;			// ��ͼ�߶�(ͼ������)
	int		tile_width;			// ͼ����
	int		tile_height;		// ͼ��߶�
	wstring	tile_path;			// ͼ��ԭʼͼƬ·��
	int		first_gid;			// ��ͼ���ݴ�0����1��ʼ
								// Tiled��ͼ�༭���Ĭ��Ϊ1
	vector<vector<int>> data;	// ͼ������	
}LAYERINFO;

// �˵�����Ϣ
typedef struct  
{
	POINT pos;
	wstring ItemName;
}MENUITEM;

// �˵���Ϣ
typedef struct  
{
	int		width;				// �˵����
	int		height;				// �˵��߶�
	int		space;				// �˵���֮��������
	int		align;				// ���ֶ��뷽ʽ(0:�����; 1:����; 2:�Ҷ���)
//	int		fontHeight;			// �����С
	LPCTSTR fontName;			// ��������
	bool	isBold;				// �Ƿ����
	Color	normalTextColor;	// �����˵�������ɫ
	Color	focusTextColor;		// ����˵�������ɫ
}MENU_INFO;

enum EVENTTYPE
{
	PLAYER_DEATH,
	PLAYER_LEVELUP,
	PLAYER_LEVELDOWN,
	PLAYER_AFTERPOLE,
	NPC_DEATH_CRASH,
	NPC_DEATH_TURNOVER,
	BULLET_EXPLODE
};
#endif