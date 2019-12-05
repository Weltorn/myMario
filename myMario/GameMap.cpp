#include "GameMap.h"
#include "NormalBrick.h"


GameMap::GameMap(LAYERINFO layerInfo)
	:T_Map(layerInfo)
{
	init = false;
}

GameMap::GameMap(LPCTSTR imgFilepath)
	: T_Map(imgFilepath)
{

}


GameMap::~GameMap()
{
}

void GameMap::update()
{	
	//������ͨש��״̬
	//string name = ClassName();
	int layerID = GetLayerTypeID();
	switch (layerID) {
	case LAYER_NORMAL_BRICK:
		for (unsigned int i = 0; i < pBricks.size(); i++)
		{
			// ����Mario����ײ����ש��Active����
			for (unsigned int j = 0; j < collideBlocks.size(); j++)
			{
				// �����С��������ҵ�ָ��ש�� ������ש�鲻���ڻ�Ծ״̬
				if (collideBlocks[j].col == pBricks[i]->getCol() && collideBlocks[j].row == pBricks[i]->getRow() && pBricks[i]->IsActive() == false)
					pBricks[i]->SetActive(true);
			}

			// ���»�Ծ״̬ש��
			if (pBricks[i]->IsActive()) {
				updated = dynamic_cast<NormalBrick *>(pBricks[i])->Bounce();
			}
		}
		break;
	case LAYER_PROP_BRICK:
		for (unsigned int i = 0; i < pBricks.size(); i++)
			pBricks[i]->LoopFrame(15,true);		//	������˸ ����ˢ������ש��֡�������ֲ�ˢ����ᵼ���������治һ��
		updated = true;
	}

}

//���ݲ���type�Լ���ͼ���� ����ָ����ש���󲢴����pBricks��
void GameMap::CreateBricks(BRICK_TYPE type)
{
	switch (type) {
	case NORMAL_BRICK:
		for (unsigned int i = 0; i < layer_data.size(); i++)
		{
			for (unsigned int j = 0; j < layer_data[0].size(); j++)
			{
				if (layer_data[i][j] != 0)
					pBricks.push_back(BrickFactory::getBrick(type, j, i));
			}
		}
		break;
	case PROP_BRICK:
		for (unsigned int i = 0; i < layer_data.size(); i++)
		{
			for (unsigned int j = 0; j < layer_data[0].size(); j++)
			{
				if (layer_data[i][j] != 0)
					pBricks.push_back(BrickFactory::getBrick(type, j, i));
			}
		}
		break;
	}

}

// ���»��Ƶ�ǰͼ��ȫ��ͼ��
void GameMap::Redraw(HDC hdc)

{
	// �ɶ��󹹳ɵ�ͼ��
	if (pBricks.size() != 0) {

		int width = (int)GetWidth();
		int height = (int)GetHeight();
		SelectObject(dc_buf, hbmp_old);

		if (!init) {
			DeleteObject(hbmp_layer);
			hbmp_layer = NULL;
			hbmp_layer = T_Graph::CreateBlankBitmap(width, height, Color::White);	//�������ܻ�12ms...
		}

		hbmp_old = (HBITMAP)SelectObject(dc_buf, hbmp_layer);
		HDC memDC = CreateCompatibleDC(hdc);
		HBITMAP OldMemBmp = (HBITMAP)SelectObject(memDC, graph->GetBmpHandle());

		switch (GetLayerTypeID()) 
		{
		case LAYER_NORMAL_BRICK:
			if (Visible == true)
			{
				for (unsigned int i = 0; i < pBricks.size(); i++)
				{
					if (!init)// �Ƚ����ж������buffer��
						pBricks[i]->Draw(dc_buf);
					else
					{
						if (pBricks[i]->IsActive())						// ֻ���»�Ծ״̬��ש��
						{
							// ���Ʊ���ɫ���ȱ
							T_Graph::PaintBlank(dc_buf, pBricks[i]->getLastX(), pBricks[i]->getLastY(), pBricks[i]->GetWidth(), pBricks[i]->GetHeight(), RGB(125, 148, 254), 255);	//
							pBricks[i]->Draw(dc_buf);				
						}
					}
				}	
			}
			break;
		case LAYER_PROP_BRICK:

			if (Visible == true)
			{
				for (unsigned int i = 0; i < pBricks.size(); i++)
				{
					// ֻ������ҿɼ�����Χ�ڵ�ש��
					int mapX = -GetX();
					int brickX = pBricks[i]->GetX();
		
					if(brickX >= mapX && brickX <= mapX + WIN_WIDTH)
						pBricks[i]->Draw(dc_buf);
				}			
			}	
			break;
		}
		init = true;	// �Ѿ�����������ש + λͼ�������
		updated = false;
		SelectObject(memDC, OldMemBmp);
		DeleteDC(memDC);//ɾ���ڴ��豸
		DeleteObject(OldMemBmp);//ɾ��λͼ����
	}

	// ��ͼ�鹹�ɵ�ͼ��
	else if (graph->GetImageHeight()>0 && graph->GetImageWidth()>0)
	{
		int width = (int)GetWidth();
		int height = (int)GetHeight();

		SelectObject(dc_buf, hbmp_old);
		DeleteObject(hbmp_layer);
		hbmp_layer = NULL;
		hbmp_layer = T_Graph::CreateBlankBitmap(width, height, Color::White);
		hbmp_old = (HBITMAP)SelectObject(dc_buf, hbmp_layer);

		if (Visible == true)
		{
			int tileIndex = 0;

			int tX = 0, tY = 0;
			int r = 0, c = 0;
			int img_col = 0, img_row = 0;

			int tileImageWidth = graph->GetImageWidth();
			HDC memDC = CreateCompatibleDC(hdc);
			HBITMAP OldMemBmp = (HBITMAP)SelectObject(memDC, graph->GetBmpHandle());

			for (r = 0, tY = 0; r < map_rows; r++, tY += tile_height)
			{
				for (c = 0, tX = 0; c < map_cols; c++, tX += tile_width)
				{
					int imgTotalCols = tileImageWidth / tile_width;
					tileIndex = layer_data[r][c];
					 
					// �����ݲŻ�����
					if (tileIndex != 0) {
						//�õ�ͼƬ�ϵ����к�
						if (first_gid == 1)
						{
							//�õ�ͼƬ�ϵ����к�
							img_col = (tileIndex - 1) % imgTotalCols;
							img_row = (tileIndex - 1) / imgTotalCols;
						}
						else
						{
							img_col = tileIndex % imgTotalCols;
							img_row = tileIndex / imgTotalCols;
						}

						BLENDFUNCTION frame_bf;
						frame_bf.BlendOp = AC_SRC_OVER;
						frame_bf.BlendFlags = 0;
						frame_bf.SourceConstantAlpha = 255;
						frame_bf.AlphaFormat = AC_SRC_ALPHA;
						AlphaBlend(
							dc_buf, tX, tY, tile_width, tile_height,
							memDC, img_col*tile_width, img_row*tile_height,
							tile_width, tile_height, frame_bf
						);
					}
				
				}
			}
			//��ԭ��ʹ��ԭ�������滻�ڴ��豸�е�λͼ����
			SelectObject(memDC, OldMemBmp);
			DeleteDC(memDC);//ɾ���ڴ��豸
			DeleteObject(OldMemBmp);//ɾ��λͼ����
			updated = false;
		}
	}
}

// ͼ�����
void GameMap::Draw(HDC hdc)
{
	//���Ʊ���ͼ��
	if (tile_width == 0 && tile_height == 0)
	{
		if (graph->GetImageHeight()>0 && graph->GetImageWidth()>0)
		{
			graph->PaintImage(hdc, (int)X, (int)Y);
		}
	}
	// �ɶ������ש����ɵ�ͼ��
	else if (pBricks.size() != 0)
	{
		if (updated == true) {
			Redraw(hdc);
		}
		if (updated == false) {
			BLENDFUNCTION frame_bf;
			frame_bf.BlendOp = AC_SRC_OVER;
			frame_bf.BlendFlags = 0;
			frame_bf.SourceConstantAlpha = 255;
			frame_bf.AlphaFormat = AC_SRC_ALPHA;

			AlphaBlend(
				hdc, (int)X, (int)Y, (int)GetWidth(), (int)GetHeight(),
				dc_buf, 0, 0, (int)GetWidth(), (int)GetHeight(), frame_bf
			);
		}      

	}
	//������Tiles��ɵĵ�ͼͼ��
	else if (tile_width > 0 && tile_height > 0)
	{
		if (updated == true)
		{
			Redraw(hdc);
		}

		if (updated == false)
		{
			BLENDFUNCTION frame_bf;
			frame_bf.BlendOp = AC_SRC_OVER;
			frame_bf.BlendFlags = 0;
			frame_bf.SourceConstantAlpha = 255;
			frame_bf.AlphaFormat = AC_SRC_ALPHA;

			AlphaBlend(
				hdc, (int)X, (int)Y, (int)GetWidth(), (int)GetHeight(),
				dc_buf, 0, 0, (int)GetWidth(), (int)GetHeight(), frame_bf
			);
		}
	}
}