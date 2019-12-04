#pragma once
#include "T_Map.h"
#include "Player.h"
#include "BrickFactory.h"
typedef vector<Brick*> LBrick;

class GameMap :
	public T_Map
{
private:
	//���ש��ȿɱ䳡������
	LBrick	pBricks;				// ����ש��ָ�룬������sceneLayers��
	COLLIDBLOCKS collideBlocks;		//��ҷ�����ײ�ĵ�ͼ�飬������������ָ����ͼ����

	// ħ��
	bool init;						// ��¼λͼ�Ƿ񱻳�ʼ��	����˿�֡����
	//HDC memDC;// = CreateCompatibleDC(hdc);
	//HBITMAP OldMemBmp;
	// ħ��end	ħ��ʧ�� fk!
	

public:
	GameMap(LAYERINFO layerInfo);		//ʹ�õ�ͼ���ݹ���
	GameMap(LPCTSTR imgFilepath);		//ʹ�ñ���ͼƬ����
	~GameMap();
	void setCollideBlocks(COLLIDBLOCKS collideBlocks) {	this->collideBlocks = collideBlocks; }
	virtual void update();
	void CreateBricks(BRICK_TYPE type);
	void Redraw(HDC hdc);
	void Draw(HDC hdc);
};

