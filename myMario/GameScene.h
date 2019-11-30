
#include "T_Scene.h"
#include "GameMap.h"
#include "MinionFactory.h"
typedef vector<Minion*> LMinion;
class GameScene :
	public T_Scene
{
private:
	LMinion pMinions;				// 怪物、奖励指针，包含在sceneLayers中
public:
	GameScene();
	~GameScene();
	virtual bool LoadTxtMap(const char* txtmap_path);
	void update();
	void RePosition(int wnd_width, int wnd_height);			// 地图重定位

	void appendMinion(MINION_TYPE type,int px,int py)
	{ 
		GAMELAYER gameLayer;
		Minion *minion = MinionFactory::getMinion(type, px, py);
		gameLayer.layer = minion;
		gameLayer.type_id = LAYER_TYPE::LAYER_NPC;
		gameLayer.z_order = getSceneLayers()->size() + 1;
		gameLayer.layer->setZorder(gameLayer.z_order);
		Append(gameLayer);
		pMinions.push_back(minion); 
	}
};
