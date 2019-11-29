#include "GameScene.h"
#include "Util.h"

GameScene::GameScene()
	:T_Scene()
{
}


GameScene::~GameScene()
{
}
// 加载参数指定的地图文件，解析其中的地图数据，并保存到场景图层中
//根据图层信息，生成砖块等对象，保存到GameMap中
bool GameScene::LoadTxtMap(const char* txtmap_path)
{
	//***************************************************************************
	//txt纯文本地图格式：
	//map_width=50					//等号后为地图图块列数
	//map_height=50					//等号后为地图图块行数
	//tile_width=32					//等号后为地图图块宽度
	//tile_height=32				//等号后为地图图块高度
	//tile_path=map\Forest.png		//等号后为地图图块图像文件路径
	//tile_grid_id=1				//等号后为地图图块编号起始号
	//back_layer_name=background	//等号后为地图背景图层名称
	//barrier_layer_name=obstacle	//等号后为地图障碍检测图层名称
	//mask_layer_name=mask			//等号后为地图遮罩图层名称
	//<layer name=background>		//<layer>标记,后面为地图数据,name为图层名称
	//两个<layer>标记之间粘贴csv格式一维地图数组
	//</layer>						//地图数据结束的标记
	//......						//继续仿照以上格式使用<layer>标记添加其它图层
	//***************************************************************************

	string str;
	wifstream infile(txtmap_path, ios::in);

	if (!infile)
	{
		return false;
	}

	wchar_t* l_str = new wchar_t[1024];
	wstring line_str;
	wstring mapdata = L"";
	bool startReadMapData = false;
	int layerCount = 0;
	wstring barrierName = L"";
	wstring maskName = L"";
	wstring bkgName = L"";
	wstring minionName = L"";
	wstring propBrickname = L"";
	wstring normalBrickName = L"";
	wstring currentlayerName = L"";

	LAYERINFO layerInfo;
	GAMELAYER mapLayer;

	while (!infile.eof())
	{
		infile.getline(l_str, 1024);//按行读取,读取结果存入l_str
		line_str = wstring(l_str);
		if (infile.fail()) // blank line 
			infile.clear();
		//以下判断读取的每行数据中是否含有要查找的关键字, 然后解析出对应的数据
		GetTxtMapValue(line_str, L"map_cols", layerInfo.map_cols);
		GetTxtMapValue(line_str, L"map_rows", layerInfo.map_rows);
		GetTxtMapValue(line_str, L"tile_width", layerInfo.tile_width);
		GetTxtMapValue(line_str, L"tile_height", layerInfo.tile_height);
		GetTxtMapValue(line_str, L"tile_path", layerInfo.tile_path);
		GetTxtMapValue(line_str, L"tile_grid_id", layerInfo.first_gid);
		GetTxtMapValue(line_str, L"back_layer_name", bkgName);
		GetTxtMapValue(line_str, L"barrier_layer_name", barrierName);
		GetTxtMapValue(line_str, L"prop_brick_layer_name", propBrickname);
		GetTxtMapValue(line_str, L"normal_brick_layer_name", normalBrickName);
		GetTxtMapValue(line_str, L"minion_layer_name", minionName);
		GetTxtMapValue(line_str, L"mask_layer_name", maskName);

		// 查找是否有layer标记, 并获取对应的图层名称
		if (line_str.find(L"<layer") != -1)
		{
			GetTxtMapValue(line_str, L"name", currentlayerName);

			//检查">"后是否有数据
			int pos = (int)(line_str.find(L">"));
			if (pos>0 && pos < (int)(line_str.length() - 1))
			{
				line_str = line_str.substr(pos + 1, line_str.length());
				mapdata = mapdata + line_str;
			}
			startReadMapData = true;
			continue;
		}
		// 解析地图图层数据
		if (startReadMapData == true)
		{
			// 如果没有到结束的</layer>标记, 不断将图层数据添加在字符串中
			if (line_str.find(L"</layer>") == -1)
			{
				mapdata = mapdata + line_str;
			}
			// 如果到达结束的</layer>标记
			if (line_str.find(L"</layer>") != -1)
			{
				startReadMapData = false;
				// 解析出字符串中的图层数据
				parseCsvData(mapdata, layerInfo);
				// 用图层数据构造GameMap对象
				mapLayer.layer = new GameMap(layerInfo);
				// 判断当前图层是否为背景图层
				if (currentlayerName == bkgName &&
					currentlayerName != L"" && bkgName != L"")
				{
					mapLayer.layer->SetLayerTypeID(LAYER_MAP_BACK);
					mapLayer.type_id = LAYER_MAP_BACK;
					layerInfo.type_id = LAYER_MAP_BACK;
				}
				// 判断当前图层是否为障碍图层
				else if (currentlayerName == barrierName &&
					currentlayerName != L"" && barrierName != L"")
				{
					pBarrier = (GameMap*)mapLayer.layer;//指定碰撞图层
					mapLayer.layer->SetLayerTypeID(LAYER_MAP_BARR);
					mapLayer.type_id = LAYER_MAP_BARR;
					layerInfo.type_id = LAYER_MAP_BARR;

				}
				//如果为遮罩层,则z_order为LAYER_MAX
				else if (currentlayerName == maskName &&
					currentlayerName != L"" && maskName != L"")
				{
					mapLayer.z_order = LAYER_MAX;
					mapLayer.layer->setZorder(LAYER_MAX);
					pMask = (GameMap*)mapLayer.layer;//指定遮罩图层
					mapLayer.layer->SetLayerTypeID(LAYER_MAP_MASK);
					mapLayer.type_id = LAYER_MAP_MASK;
					layerInfo.type_id = LAYER_MAP_MASK;
				}
				//如果为怪物层,则z_order为LAYER_MAX
				else if (currentlayerName == minionName &&
					currentlayerName != L"" && minionName != L"")
				{
					continue;
				//	mapLayer.z_order = layerCount;
				//	mapLayer.layer->setZorder(layerCount);
				}
				//如果为普通砖层,则z_order为LAYER_MAX
				else if (currentlayerName == normalBrickName &&
					currentlayerName != L"" && normalBrickName != L"")
				{
				//	continue;
					mapLayer.z_order = LAYER_MAX;
					mapLayer.layer->setZorder(LAYER_MAX);
					//父指针无法直接调用子类函数
					dynamic_cast<GameMap *>(mapLayer.layer)->CreateBricks(NORMAL_BRICK);
					pNormalBrick = (GameMap*)mapLayer.layer;
					//	pMask = (GameMap*)mapLayer.layer;
					mapLayer.layer->SetLayerTypeID(LAYER_NORMAL_BRICK);
					mapLayer.type_id = LAYER_NORMAL_BRICK;
					layerInfo.type_id = LAYER_NORMAL_BRICK;
				}
				//如果为道具转层,则z_order为LAYER_MAX
				else if (currentlayerName == propBrickname &&
					currentlayerName != L"" && propBrickname != L"")
				{
					mapLayer.z_order = LAYER_MAX;
					mapLayer.layer->setZorder(LAYER_MAX);
				//	pMask = (GameMap*)mapLayer.layer;
					mapLayer.layer->SetLayerTypeID(LAYER_PROP_BRICK);
					mapLayer.type_id = LAYER_PROP_BRICK;
					layerInfo.type_id = LAYER_PROP_BRICK;
					//continue;
				}
				else
				{
					mapLayer.z_order = layerCount;
					mapLayer.layer->setZorder(layerCount);
				}
				// 将图层对象添加到图层容器中
				sceneLayers.push_back(mapLayer);

				// 根据图层信息生成对象
				//...

				mapdata = L"";
				layerInfo.data.clear();
				layerInfo.type_id = -1;
				layerCount = layerCount + 1;
			}
			Util::myprintf(L"Load map successful!\n");
		}
	}
	delete[] l_str;
	return true;
}
/*bool GameScene::LoadTxtMap(const char* txtmap_path)
{
	//***************************************************************************
	//txt纯文本地图格式：
	//map_width=50					//等号后为地图图块列数
	//map_height=50					//等号后为地图图块行数
	//tile_width=32					//等号后为地图图块宽度
	//tile_height=32				//等号后为地图图块高度
	//tile_path=map\Forest.png		//等号后为地图图块图像文件路径
	//tile_grid_id=1				//等号后为地图图块编号起始号
	//back_layer_name=background	//等号后为地图背景图层名称
	//barrier_layer_name=obstacle	//等号后为地图障碍检测图层名称
	//mask_layer_name=mask			//等号后为地图遮罩图层名称
	//<layer name=background>		//<layer>标记,后面为地图数据,name为图层名称
	//两个<layer>标记之间粘贴csv格式一维地图数组
	//</layer>						//地图数据结束的标记
	//......						//继续仿照以上格式使用<layer>标记添加其它图层
	//***************************************************************************

	string str;
	wifstream infile(txtmap_path, ios::in);

	if (!infile)
	{
		return false;
	}

	wchar_t* l_str = new wchar_t[1024];
	wstring line_str;
	wstring mapdata = L"";
	bool startReadMapData = false;
	int layerCount = 0;
	wstring barrierName = L"";
	wstring maskName = L"";
	wstring bkgName = L"";
	wstring currentlayerName = L"";

	LAYERINFO layerInfo;
	GAMELAYER mapLayer;

	while (!infile.eof())
	{
		infile.getline(l_str, 1024);//按行读取,读取结果存入l_str
		line_str = wstring(l_str);
		if (infile.fail()) // blank line 
			infile.clear();
		//以下判断读取的每行数据中是否含有要查找的关键字, 然后解析出对应的数据
		GetTxtMapValue(line_str, L"map_cols", layerInfo.map_cols);
		GetTxtMapValue(line_str, L"map_rows", layerInfo.map_rows);
		GetTxtMapValue(line_str, L"tile_width", layerInfo.tile_width);
		GetTxtMapValue(line_str, L"tile_height", layerInfo.tile_height);
		GetTxtMapValue(line_str, L"tile_path", layerInfo.tile_path);
		GetTxtMapValue(line_str, L"tile_grid_id", layerInfo.first_gid);
		GetTxtMapValue(line_str, L"back_layer_name", bkgName);
		GetTxtMapValue(line_str, L"barrier_layer_name", barrierName);
		GetTxtMapValue(line_str, L"mask_layer_name", maskName);

		// 查找是否有layer标记, 并获取对应的图层名称
		if (line_str.find(L"<layer") != -1)
		{
			GetTxtMapValue(line_str, L"name", currentlayerName);

			//检查">"后是否有数据
			int pos = (int)(line_str.find(L">"));
			if (pos>0 && pos < (int)(line_str.length() - 1))
			{
				line_str = line_str.substr(pos + 1, line_str.length());
				mapdata = mapdata + line_str;
			}
			startReadMapData = true;
			continue;
		}
		// 解析地图图层数据
		if (startReadMapData == true)
		{
			// 如果没有到结束的</layer>标记, 不断将图层数据添加在字符串中
			if (line_str.find(L"</layer>") == -1)
			{
				mapdata = mapdata + line_str;
			}
			// 如果到达结束的</layer>标记
			if (line_str.find(L"</layer>") != -1)
			{
				startReadMapData = false;
				// 解析出字符串中的图层数据
				parseCsvData(mapdata, layerInfo);
				// 用图层数据构造T_Map对象
				mapLayer.layer = new GameMap(layerInfo);
				// 判断当前图层是否为背景图层
				if (currentlayerName == bkgName &&
					currentlayerName != L"" && bkgName != L"")
				{
					mapLayer.layer->SetLayerTypeID(LAYER_MAP_BACK);
					mapLayer.type_id = LAYER_MAP_BACK;
					layerInfo.type_id = LAYER_MAP_BACK;
				}
				// 判断当前图层是否为障碍图层
				if (currentlayerName == barrierName &&
					currentlayerName != L"" && barrierName != L"")
				{
					pBarrier = (GameMap*)mapLayer.layer;//指定碰撞图层
					mapLayer.layer->SetLayerTypeID(LAYER_MAP_BARR);
					mapLayer.type_id = LAYER_MAP_BARR;
					layerInfo.type_id = LAYER_MAP_BARR;

				}

				//如果为遮罩层,则z_order为LAYER_MAX
				if (currentlayerName == maskName &&
					currentlayerName != L"" && maskName != L"")
				{
					mapLayer.z_order = LAYER_MAX;
					mapLayer.layer->setZorder(LAYER_MAX);
					pMask = (GameMap*)mapLayer.layer;//指定遮罩图层
					mapLayer.layer->SetLayerTypeID(LAYER_MAP_MASK);
					mapLayer.type_id = LAYER_MAP_MASK;
					layerInfo.type_id = LAYER_MAP_MASK;
				}
				else
				{
					mapLayer.z_order = layerCount;
					mapLayer.layer->setZorder(layerCount);
				}
				// 将图层对象添加到图层容器中
				sceneLayers.push_back(mapLayer);

				// 根据图层信息生成对象
				//...

				mapdata = L"";
				layerInfo.data.clear();
				layerInfo.type_id = -1;
				layerCount = layerCount + 1;
			}
			Util::myprintf(L"Load map successful!\n");
		}
	}
	delete[] l_str;
	return true;
}*/
void  GameScene::update() 
{

	//更新障碍地图
	if (pBarrier->IsVisible())
		pBarrier->update();

	//更新怪物图层
	LMinion::iterator p1;
	LMinion::iterator p2;
	for (p1 = pMinions.begin(); p1 != pMinions.end();)
	{
		if ((*p1)->IsDead())
			(*p1)->update();
		else if (!(*p1)->IsDead())		//怪物死亡，删除对象
		{		
			p2 = p1;
			p1 = pMinions.erase(p2);
		}
	}
	
	//怪物与玩家的碰撞
	LMinion::iterator pm;
	for (pm = pMinions.begin(); pm != pMinions.end(); pm++)
	{
		(*pm)->CollideWith(pPlayer);	//设置怪物、玩家碰撞后状态（如死亡、升/降级）
	}
	// 如果图层发生过任何变化
	if (LayerChanged == true)
	{
		SortLayers();//对图层重新排序
	}

//	ScrollScene(pPlayer);
}

void GameScene::RePosition(int wnd_width,int wnd_height)
{
	int scn_width = this->getBarrier()->GetWidth();
	int scn_height = this->getBarrier()->GetHeight();
	// 视图初始位置以地图作为参照
	int scn_x = 0;
	int scn_y = -this->getBarrier()->getTileHeight() / 2;
	// 将游戏地图初始化
	this->InitScene(scn_x, scn_y, scn_width, scn_height, wnd_width, wnd_height);

	// 将所有地图图层定位
	SCENE_LAYERS::iterator p;
	for (p = this->getSceneLayers()->begin(); p != this->getSceneLayers()->end(); p++)
	{
		if (p->layer->ClassName() == "T_Map") p->layer->SetPosition(scn_x, scn_y);
	}
}