
#include "T_Scene.h"
#include "GameMap.h"
#include "Brick.h"
typedef vector<Minion*> LMinion;
typedef vector<Brick*> LBrick;
class GameScene :
	public T_Scene
{
private:
	LMinion pMinions;				// �������ָ�룬������sceneLayers��
	LBrick	pBricks;				// ����ש��ָ�룬������sceneLayers��
public:
	GameScene();
	~GameScene();
	virtual bool LoadTxtMap(const char* txtmap_path);
	void update();
	void RePosition(int wnd_width, int wnd_height);			// ��ͼ�ض�λ

	// ���ز���ָ���ĵ�ͼ�ļ����������еĵ�ͼ���ݣ������浽����ͼ����
	//virtual bool LoadTxtMap(const char* txtmap_path);

};
