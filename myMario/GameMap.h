#pragma once
#include "T_Map.h"
class GameMap :
	public T_Map
	//���ש��ȿɱ䳡������

{
public:
	GameMap(LAYERINFO layerInfo);		//ʹ�õ�ͼ���ݹ���
	GameMap(LPCTSTR imgFilepath);		//ʹ�ñ���ͼƬ����
	~GameMap();
	virtual void update();
};

