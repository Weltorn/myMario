#pragma once
#include "T_Map.h"
#include "Player.h"
#include "BrickFactory.h"
#include "Util.h"
//#include "GameScene.h"
typedef vector<Brick*> LBrick;

class GameMap :
	public T_Map
{
private:
	//���ש��ȿɱ䳡������
	LBrick	pBricks;				// ����ש��ָ�룬������sceneLayers��
	COLLIDBLOCKS collideBlocks;		// ��ҷ�����ײ�ĵ�ͼ�飬������������ָ����ͼ����
	bool init;						// ��¼λͼ�Ƿ񱻳�ʼ��	ʵ��λͼ�ֲ�ˢ�� �����֡����

	

public:
	GameMap(LAYERINFO layerInfo);		//ʹ�õ�ͼ���ݹ���
	GameMap(LPCTSTR imgFilepath);		//ʹ�ñ���ͼƬ����
	virtual ~GameMap();
	void setCollideBlocks(COLLIDBLOCKS collideBlocks) { this->collideBlocks = collideBlocks; }
	virtual void update();
	void CreateBricks(BRICK_TYPE type);
	void Redraw(HDC hdc);
	void Draw(HDC hdc);


};

