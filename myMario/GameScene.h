
#include "T_Scene.h"
#include "GameMap.h"
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
};
