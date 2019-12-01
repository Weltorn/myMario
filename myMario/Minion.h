#pragma once
#include "T_Sprite.h"
#include "Util.h"
class T_Scene;
class GameMap;

enum MINION_TYPE
{
	MINION_STAR,			// ����
	MINION_REDMUSHROOM,		// ��Ģ��
	MINION_GREENMUSHROOM,	// ��Ģ��
	MINION_FLOWER,			// ��

	MINION_GOOMBA,				// �����У�Ģ��һ���Ĺ��
	MINION_KOOPA,				// ������
	MINION_PLAYERBULLET,
};

class Minion :
	public T_Sprite
{
protected:
	MINION_TYPE minionType;		//��������

	//״̬��Ϣ
	bool onPlantform;		//�Ƿ���֧������
	int currentFrmIndex;

	// ----- MOVE		
	float gravity;			//�����������ٶ�
	unsigned timer;			//��ʱ��

	//EVENT
	unsigned eventTimer;
	bool inEvent;					//�Ƿ�����Ϸ�¼���
	int eventId;					//��󡢱�С������
	int currentStep;	
public:
	Minion(LPCTSTR imgPath, int frameWidth, int frameHeight);
	~Minion();
	virtual string ClassName() { return "Minion"; }
	MINION_TYPE getMinionType() { return minionType; }
	void setMinionType(MINION_TYPE minionType) {
		this->minionType = minionType;
	}

	virtual bool CollideWith(IN T_Map* map);
	virtual bool CollideWith(T_Sprite* target, int distance = 0);

	//���¹���״̬
	virtual void update();
	
	// ����Ƿ�վ��֧������
	bool checkOnplantForm(T_Map* map);
	bool isOnPlantform() { return onPlantform; }	

	//����Ч��
	void gravityEffect();
	//��������
	void updatePosition();
	void updatePositionY();			//��ֱ�ƶ�
	void updatePositionX();			//ˮƽ�ƶ�									
	virtual void updateFrame();				//����֡ͼ
	
	//�¼����
	bool isInEvent() { return inEvent; }
	virtual void startEvent(int eventId);
	virtual void playAnimation();
	virtual void Draw(HDC hdc)override {}

};

