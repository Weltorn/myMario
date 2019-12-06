
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
	LMinion pMinions;				// 怪物、奖励指针，包含在sceneLayers中
	LPlayerBullet pPlayerBullets;	// 玩家炮弹
	LAward pBonus;					//玩家奖励（花、蘑菇、星星）
	LDecoration pDecoration;		//效果图层：碎砖、得分
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
	void RePosition(int wnd_width, int wnd_height);			// 地图重定位

	void appendMinion(MINION_TYPE type, int px, int py, int dir = DIR_LEFT);
	//void appendPlayerBullet(int px, int py, int dir);

	void WakeMinion() {}			// 唤醒怪物
};
