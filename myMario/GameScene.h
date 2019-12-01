
#include "T_Scene.h"
#include "GameMap.h"
#include "MinionFactory.h"
#include "PlayerBullet.h"
typedef vector<Minion*> LMinion;
typedef vector<Minion*> LPlayerBullet;
class GameScene :
	public T_Scene
{
private:
	LMinion pMinions;				// 怪物、奖励指针，包含在sceneLayers中
	static LPlayerBullet pPlayerBullets;	//玩家炮弹
public:
	GameScene();
	~GameScene();
	virtual bool LoadTxtMap(const char* txtmap_path);
	void update();
	void RePosition(int wnd_width, int wnd_height);			// 地图重定位

	void appendMinion(MINION_TYPE type,int px,int py,GAME_DIR dir = DIR_LEFT)
	{ 
		GAMELAYER gameLayer;
		Minion *minion = MinionFactory::getMinion(type, px, py);
		minion->SetDir(dir);
		gameLayer.layer = minion;
		gameLayer.type_id = LAYER_TYPE::LAYER_NPC;
		gameLayer.z_order = getSceneLayers()->size() + 1;
		gameLayer.layer->setZorder(gameLayer.z_order);
		Append(gameLayer);
		pMinions.push_back(minion); 
	}
	static void appendPlayerBullet(int px, int py, GAME_DIR dir)
	{
		GAMELAYER gameLayer;
		Minion *bullet = MinionFactory::getMinion(MINION_TYPE::MINION_PLAYERBULLET, px, py);
		bullet->SetDir(dir);
		gameLayer.layer = bullet;
		gameLayer.type_id = LAYER_TYPE::LAYER_PLY_BOMB;
		gameLayer.z_order = getSceneLayers()->size() + 1;
		gameLayer.layer->setZorder(gameLayer.z_order);
		Append(gameLayer);
		pPlayerBullets.push_back(bullet);
	}
};
