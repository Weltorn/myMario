
#include "T_Scene.h"
#include "GameMap.h"
#include "MinionFactory.h"
#include "PlayerBullet.h"
#include "Brick.h"
typedef vector<Minion*> LMinion;
typedef vector<Minion*> LPlayerBullet;
class GameScene :
	public T_Scene
{
private:
	static GameScene* instance;
	LMinion pMinions;				// 怪物、奖励指针，包含在sceneLayers中
	LPlayerBullet pPlayerBullets;	//玩家炮弹
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
	void RePosition(int wnd_width, int wnd_height);			// 地图重定位

	void appendMinion(MINION_TYPE type, int px, int py, int dir = DIR_LEFT);
	void appendPlayerBullet(int px, int py, int dir);
};
