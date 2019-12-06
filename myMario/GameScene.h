
#include "T_Scene.h"
#include "GameMap.h"
#include "MinionFactory.h"
#include "PlayerBullet.h"
#include "Brick.h"
typedef vector<Minion*> LMinion;
typedef vector<Minion*> LAward;
typedef vector<Minion*> LPlayerBullet;
typedef vector<Minion*> LDecoration;

class GameScene :
	public T_Scene
{
private:
	static GameScene* instance;
	LMinion pMinions;				// �������ָ�룬������sceneLayers��
	LPlayerBullet pPlayerBullets;	// ����ڵ�
	LAward pBonus;					//��ҽ���������Ģ�������ǣ�
	LDecoration pDecoration;		//Ч��ͼ�㣺��ש���÷�
	GameScene();
public:
	virtual ~GameScene();
	static GameScene* getInstance()
	{
		if (instance == NULL) {
			instance = new GameScene();
		}
		return instance;
	}
	virtual bool LoadTxtMap(const char* txtmap_path);
	void update();
	void updateObjectList(vector<Minion*> *LObj);
	void collideWithPlayer(vector<Minion*> *LObj);
	void RePosition(int wnd_width, int wnd_height);			// ��ͼ�ض�λ

	void appendMinion(MINION_TYPE type, int px, int py, int dir = DIR_LEFT);
	//void appendPlayerBullet(int px, int py, int dir);

	void WakeMinion() {}			// ���ѹ���
};
