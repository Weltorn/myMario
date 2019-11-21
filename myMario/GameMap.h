#pragma once
#include "T_Map.h"
class GameMap :
	public T_Map
{
private:
	//���ש��ȿɱ䳡������

	COLLIDBLOCKS collideBlocks;	//��ҷ�����ײ�ĵ�ͼ�飬������������ָ����ͼ����

public:
	GameMap(LAYERINFO layerInfo);		//ʹ�õ�ͼ���ݹ���
	GameMap(LPCTSTR imgFilepath);		//ʹ�ñ���ͼƬ����
	~GameMap();
	void setCollideBlocks(COLLIDBLOCKS collideBlocks) { this->collideBlocks = collideBlocks; }
	virtual void update();
};

