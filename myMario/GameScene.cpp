#include "GameScene.h"
#include "Util.h"

GameScene::GameScene()
	:T_Scene()
{
}


GameScene::~GameScene()
{
}
// ���ز���ָ���ĵ�ͼ�ļ����������еĵ�ͼ���ݣ������浽����ͼ����
//����ͼ����Ϣ������ש��ȶ��󣬱��浽GameMap��
bool GameScene::LoadTxtMap(const char* txtmap_path)
{
	//***************************************************************************
	//txt���ı���ͼ��ʽ��
	//map_width=50					//�Ⱥź�Ϊ��ͼͼ������
	//map_height=50					//�Ⱥź�Ϊ��ͼͼ������
	//tile_width=32					//�Ⱥź�Ϊ��ͼͼ����
	//tile_height=32				//�Ⱥź�Ϊ��ͼͼ��߶�
	//tile_path=map\Forest.png		//�Ⱥź�Ϊ��ͼͼ��ͼ���ļ�·��
	//tile_grid_id=1				//�Ⱥź�Ϊ��ͼͼ������ʼ��
	//back_layer_name=background	//�Ⱥź�Ϊ��ͼ����ͼ������
	//barrier_layer_name=obstacle	//�Ⱥź�Ϊ��ͼ�ϰ����ͼ������
	//mask_layer_name=mask			//�Ⱥź�Ϊ��ͼ����ͼ������
	//<layer name=background>		//<layer>���,����Ϊ��ͼ����,nameΪͼ������
	//����<layer>���֮��ճ��csv��ʽһά��ͼ����
	//</layer>						//��ͼ���ݽ����ı��
	//......						//�����������ϸ�ʽʹ��<layer>����������ͼ��
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
		infile.getline(l_str, 1024);//���ж�ȡ,��ȡ�������l_str
		line_str = wstring(l_str);
		if (infile.fail()) // blank line 
			infile.clear();
		//�����ж϶�ȡ��ÿ���������Ƿ���Ҫ���ҵĹؼ���, Ȼ���������Ӧ������
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

		// �����Ƿ���layer���, ����ȡ��Ӧ��ͼ������
		if (line_str.find(L"<layer") != -1)
		{
			GetTxtMapValue(line_str, L"name", currentlayerName);

			//���">"���Ƿ�������
			int pos = (int)(line_str.find(L">"));
			if (pos>0 && pos < (int)(line_str.length() - 1))
			{
				line_str = line_str.substr(pos + 1, line_str.length());
				mapdata = mapdata + line_str;
			}
			startReadMapData = true;
			continue;
		}
		// ������ͼͼ������
		if (startReadMapData == true)
		{
			// ���û�е�������</layer>���, ���Ͻ�ͼ������������ַ�����
			if (line_str.find(L"</layer>") == -1)
			{
				mapdata = mapdata + line_str;
			}
			// ������������</layer>���
			if (line_str.find(L"</layer>") != -1)
			{
				startReadMapData = false;
				// �������ַ����е�ͼ������
				parseCsvData(mapdata, layerInfo);
				// ��ͼ�����ݹ���GameMap����
				mapLayer.layer = new GameMap(layerInfo);
				// �жϵ�ǰͼ���Ƿ�Ϊ����ͼ��
				if (currentlayerName == bkgName &&
					currentlayerName != L"" && bkgName != L"")
				{
					mapLayer.layer->SetLayerTypeID(LAYER_MAP_BACK);
					mapLayer.type_id = LAYER_MAP_BACK;
					layerInfo.type_id = LAYER_MAP_BACK;
				}
				// �жϵ�ǰͼ���Ƿ�Ϊ�ϰ�ͼ��
				else if (currentlayerName == barrierName &&
					currentlayerName != L"" && barrierName != L"")
				{
					pBarrier = (GameMap*)mapLayer.layer;//ָ����ײͼ��
					mapLayer.layer->SetLayerTypeID(LAYER_MAP_BARR);
					mapLayer.type_id = LAYER_MAP_BARR;
					layerInfo.type_id = LAYER_MAP_BARR;

				}
				//���Ϊ���ֲ�,��z_orderΪLAYER_MAX
				else if (currentlayerName == maskName &&
					currentlayerName != L"" && maskName != L"")
				{
					mapLayer.z_order = LAYER_MAX;
					mapLayer.layer->setZorder(LAYER_MAX);
					pMask = (GameMap*)mapLayer.layer;//ָ������ͼ��
					mapLayer.layer->SetLayerTypeID(LAYER_MAP_MASK);
					mapLayer.type_id = LAYER_MAP_MASK;
					layerInfo.type_id = LAYER_MAP_MASK;
				}
				//���Ϊ�����,��z_orderΪLAYER_MAX
				else if (currentlayerName == minionName &&
					currentlayerName != L"" && minionName != L"")
				{
					continue;
				//	mapLayer.z_order = layerCount;
				//	mapLayer.layer->setZorder(layerCount);
				}
				//���Ϊ��ͨש��,��z_orderΪLAYER_MAX
				else if (currentlayerName == normalBrickName &&
					currentlayerName != L"" && normalBrickName != L"")
				{
				//	continue;
					mapLayer.z_order = LAYER_MAX;
					mapLayer.layer->setZorder(LAYER_MAX);
					//��ָ���޷�ֱ�ӵ������ຯ��
					dynamic_cast<GameMap *>(mapLayer.layer)->CreateBricks(NORMAL_BRICK);
					pNormalBrick = (GameMap*)mapLayer.layer;
					//	pMask = (GameMap*)mapLayer.layer;
					mapLayer.layer->SetLayerTypeID(LAYER_NORMAL_BRICK);
					mapLayer.type_id = LAYER_NORMAL_BRICK;
					layerInfo.type_id = LAYER_NORMAL_BRICK;
				}
				//���Ϊ����ת��,��z_orderΪLAYER_MAX
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
				// ��ͼ�������ӵ�ͼ��������
				sceneLayers.push_back(mapLayer);

				// ����ͼ����Ϣ���ɶ���
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
	//txt���ı���ͼ��ʽ��
	//map_width=50					//�Ⱥź�Ϊ��ͼͼ������
	//map_height=50					//�Ⱥź�Ϊ��ͼͼ������
	//tile_width=32					//�Ⱥź�Ϊ��ͼͼ����
	//tile_height=32				//�Ⱥź�Ϊ��ͼͼ��߶�
	//tile_path=map\Forest.png		//�Ⱥź�Ϊ��ͼͼ��ͼ���ļ�·��
	//tile_grid_id=1				//�Ⱥź�Ϊ��ͼͼ������ʼ��
	//back_layer_name=background	//�Ⱥź�Ϊ��ͼ����ͼ������
	//barrier_layer_name=obstacle	//�Ⱥź�Ϊ��ͼ�ϰ����ͼ������
	//mask_layer_name=mask			//�Ⱥź�Ϊ��ͼ����ͼ������
	//<layer name=background>		//<layer>���,����Ϊ��ͼ����,nameΪͼ������
	//����<layer>���֮��ճ��csv��ʽһά��ͼ����
	//</layer>						//��ͼ���ݽ����ı��
	//......						//�����������ϸ�ʽʹ��<layer>����������ͼ��
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
		infile.getline(l_str, 1024);//���ж�ȡ,��ȡ�������l_str
		line_str = wstring(l_str);
		if (infile.fail()) // blank line 
			infile.clear();
		//�����ж϶�ȡ��ÿ���������Ƿ���Ҫ���ҵĹؼ���, Ȼ���������Ӧ������
		GetTxtMapValue(line_str, L"map_cols", layerInfo.map_cols);
		GetTxtMapValue(line_str, L"map_rows", layerInfo.map_rows);
		GetTxtMapValue(line_str, L"tile_width", layerInfo.tile_width);
		GetTxtMapValue(line_str, L"tile_height", layerInfo.tile_height);
		GetTxtMapValue(line_str, L"tile_path", layerInfo.tile_path);
		GetTxtMapValue(line_str, L"tile_grid_id", layerInfo.first_gid);
		GetTxtMapValue(line_str, L"back_layer_name", bkgName);
		GetTxtMapValue(line_str, L"barrier_layer_name", barrierName);
		GetTxtMapValue(line_str, L"mask_layer_name", maskName);

		// �����Ƿ���layer���, ����ȡ��Ӧ��ͼ������
		if (line_str.find(L"<layer") != -1)
		{
			GetTxtMapValue(line_str, L"name", currentlayerName);

			//���">"���Ƿ�������
			int pos = (int)(line_str.find(L">"));
			if (pos>0 && pos < (int)(line_str.length() - 1))
			{
				line_str = line_str.substr(pos + 1, line_str.length());
				mapdata = mapdata + line_str;
			}
			startReadMapData = true;
			continue;
		}
		// ������ͼͼ������
		if (startReadMapData == true)
		{
			// ���û�е�������</layer>���, ���Ͻ�ͼ������������ַ�����
			if (line_str.find(L"</layer>") == -1)
			{
				mapdata = mapdata + line_str;
			}
			// ������������</layer>���
			if (line_str.find(L"</layer>") != -1)
			{
				startReadMapData = false;
				// �������ַ����е�ͼ������
				parseCsvData(mapdata, layerInfo);
				// ��ͼ�����ݹ���T_Map����
				mapLayer.layer = new GameMap(layerInfo);
				// �жϵ�ǰͼ���Ƿ�Ϊ����ͼ��
				if (currentlayerName == bkgName &&
					currentlayerName != L"" && bkgName != L"")
				{
					mapLayer.layer->SetLayerTypeID(LAYER_MAP_BACK);
					mapLayer.type_id = LAYER_MAP_BACK;
					layerInfo.type_id = LAYER_MAP_BACK;
				}
				// �жϵ�ǰͼ���Ƿ�Ϊ�ϰ�ͼ��
				if (currentlayerName == barrierName &&
					currentlayerName != L"" && barrierName != L"")
				{
					pBarrier = (GameMap*)mapLayer.layer;//ָ����ײͼ��
					mapLayer.layer->SetLayerTypeID(LAYER_MAP_BARR);
					mapLayer.type_id = LAYER_MAP_BARR;
					layerInfo.type_id = LAYER_MAP_BARR;

				}

				//���Ϊ���ֲ�,��z_orderΪLAYER_MAX
				if (currentlayerName == maskName &&
					currentlayerName != L"" && maskName != L"")
				{
					mapLayer.z_order = LAYER_MAX;
					mapLayer.layer->setZorder(LAYER_MAX);
					pMask = (GameMap*)mapLayer.layer;//ָ������ͼ��
					mapLayer.layer->SetLayerTypeID(LAYER_MAP_MASK);
					mapLayer.type_id = LAYER_MAP_MASK;
					layerInfo.type_id = LAYER_MAP_MASK;
				}
				else
				{
					mapLayer.z_order = layerCount;
					mapLayer.layer->setZorder(layerCount);
				}
				// ��ͼ�������ӵ�ͼ��������
				sceneLayers.push_back(mapLayer);

				// ����ͼ����Ϣ���ɶ���
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

	//�����ϰ���ͼ
	if (pBarrier->IsVisible())
		pBarrier->update();

	//���¹���ͼ��
	LMinion::iterator p1;
	LMinion::iterator p2;
	for (p1 = pMinions.begin(); p1 != pMinions.end();)
	{
		if ((*p1)->IsDead())
			(*p1)->update();
		else if (!(*p1)->IsDead())		//����������ɾ������
		{		
			p2 = p1;
			p1 = pMinions.erase(p2);
		}
	}
	
	//��������ҵ���ײ
	LMinion::iterator pm;
	for (pm = pMinions.begin(); pm != pMinions.end(); pm++)
	{
		(*pm)->CollideWith(pPlayer);	//���ù�������ײ��״̬������������/������
	}
	// ���ͼ�㷢�����κα仯
	if (LayerChanged == true)
	{
		SortLayers();//��ͼ����������
	}

//	ScrollScene(pPlayer);
}

void GameScene::RePosition(int wnd_width,int wnd_height)
{
	int scn_width = this->getBarrier()->GetWidth();
	int scn_height = this->getBarrier()->GetHeight();
	// ��ͼ��ʼλ���Ե�ͼ��Ϊ����
	int scn_x = 0;
	int scn_y = -this->getBarrier()->getTileHeight() / 2;
	// ����Ϸ��ͼ��ʼ��
	this->InitScene(scn_x, scn_y, scn_width, scn_height, wnd_width, wnd_height);

	// �����е�ͼͼ�㶨λ
	SCENE_LAYERS::iterator p;
	for (p = this->getSceneLayers()->begin(); p != this->getSceneLayers()->end(); p++)
	{
		if (p->layer->ClassName() == "T_Map") p->layer->SetPosition(scn_x, scn_y);
	}
}