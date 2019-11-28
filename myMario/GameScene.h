
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
	void RePosition(int wnd_width, int wnd_height);			// 地图重定位

	// 加载参数指定的地图文件，解析其中的地图数据，并保存到场景图层中
	//virtual bool LoadTxtMap(const char* txtmap_path);

};
