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
	//更新普通砖块状态
	//string name = ClassName();
	int layerID = GetLayerTypeID();
	switch (layerID) {
	case LAYER_NORMAL_BRICK:
		for (unsigned int i = 0; i < pBricks.size(); i++)
		{
			// 设置Mario新碰撞到的砖块Active属性
			for (unsigned int j = 0; j < collideBlocks.size(); j++)
			{
				// 更具行、列索引找到指定砖块 且若该砖块不处于活跃状态
				if (collideBlocks[j].col == pBricks[i]->getCol() && collideBlocks[j].row == pBricks[i]->getRow() && pBricks[i]->IsActive() == false)
					pBricks[i]->SetActive(true);
			}

			// 更新活跃状态砖块
			if (pBricks[i]->IsActive()) {
				updated = dynamic_cast<NormalBrick *>(pBricks[i])->Bounce();
			}
		}
		break;
	case LAYER_PROP_BRICK:
		for (unsigned int i = 0; i < pBricks.size(); i++)
			pBricks[i]->LoopFrame(15,true);		//	慢速闪烁 这里刷新所有砖的帧数，若局部刷新则会导致闪动画面不一致
		updated = true;
	}

}

//根据参数type以及地图数据 生成指定的砖对象并存放于pBricks中
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

// 重新绘制当前图层全部图块
void GameMap::Redraw(HDC hdc)

{
	// 由对象构成的图层
	if (pBricks.size() != 0) {

		int width = (int)GetWidth();
		int height = (int)GetHeight();
		SelectObject(dc_buf, hbmp_old);

		if (!init) {
			DeleteObject(hbmp_layer);
			hbmp_layer = NULL;
			hbmp_layer = T_Graph::CreateBlankBitmap(width, height, Color::White);	//这句代码能花12ms...
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
					if (!init)// 先将所有对象绘制buffer中
						pBricks[i]->Draw(dc_buf);
					else
					{
						if (pBricks[i]->IsActive())						// 只更新活跃状态的砖块
						{
							// 绘制背景色填补空缺
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
					// 只更新玩家可见区域范围内的砖块
					int mapX = -GetX();
					int brickX = pBricks[i]->GetX();
		
					if(brickX >= mapX && brickX <= mapX + WIN_WIDTH)
						pBricks[i]->Draw(dc_buf);
				}			
			}	
			break;
		}
		init = true;	// 已经绘制完所有砖 + 位图创建完毕
		updated = false;
		SelectObject(memDC, OldMemBmp);
		DeleteDC(memDC);//删除内存设备
		DeleteObject(OldMemBmp);//删除位图对象
	}

	// 由图块构成的图层
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
					 
					// 有内容才画出来
					if (tileIndex != 0) {
						//得到图片上的行列号
						if (first_gid == 1)
						{
							//得到图片上的行列号
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
			//还原：使用原来对象替换内存设备中的位图对象
			SelectObject(memDC, OldMemBmp);
			DeleteDC(memDC);//删除内存设备
			DeleteObject(OldMemBmp);//删除位图对象
			updated = false;
		}
	}
}

// 图层绘制
void GameMap::Draw(HDC hdc)
{
	//绘制背景图像
	if (tile_width == 0 && tile_height == 0)
	{
		if (graph->GetImageHeight()>0 && graph->GetImageWidth()>0)
		{
			graph->PaintImage(hdc, (int)X, (int)Y);
		}
	}
	// 由多个对象（砖）组成的图层
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
	//绘制由Tiles组成的地图图块
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