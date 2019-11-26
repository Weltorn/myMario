#include "GameMenu.h"



GameMenu::GameMenu()
{
//	beginTick = 0;
}


GameMenu::~GameMenu()
{
}


void GameMenu::DrawMenu(HDC hdc, int bkgX, int bkgY, BYTE btnTrans, bool startState)
{
	if (gm_menuBkg.GetBmpHandle() != NULL  && startState == true)
	{
		HBITMAP tBmp = T_Graph::CreateBlankBitmap(WIN_WIDTH, WIN_HEIGHT, bkColor);
		SelectObject(hdc, tBmp);

		gm_menuBkg.PaintImage(hdc, bkgX, bkgY, WIN_WIDTH, WIN_HEIGHT, bkImageAlpha);

		DeleteObject(tBmp);
		tBmp = NULL;
	}
	else
	{
		if (gm_menuBkg.GetBmpHandle() != NULL)
		{
			gm_menuBkg.PaintImage(
				hdc, bkgX, bkgY, gm_menuBkg.GetImageWidth(), gm_menuBkg.GetImageHeight(), bkImageAlpha
			);
		}
	}
//	int num = strlen(item);
	int w = menu_info.width;
	int h = menu_info.height;

	int px = 0;
	int w_px = w / (MaxMenuItemLen + 1);	//����ÿ������ռ������
	int h_px = (int)((float)(h / 2.5));
	if (w_px > h_px)
	{
		px = h_px;
	}
	else
	{
		px = w_px;
	}
	//fontHeight = (px * 72) / 72;		//����ÿ���ֵ����ؼ����ֺ�

	if (isItemFocused == FALSE)
	{
		Gdiplus::RectF Rec;
		vector<MENUITEM>::iterator iter;
		for (iter = gm_menuItems.begin(); iter != gm_menuItems.end(); iter++)
		{
			int x = iter->pos.x;
			int y = iter->pos.y;

			Rec.X = (float)x;
			Rec.Y = (float)y;
			Rec.Width = (float)w;
			Rec.Height = (float)h;

			// ���ư�ť����
			LPCTSTR item = iter->ItemName.c_str();
			int num = wcslen(item);

			T_Graph::PaintText(hdc, Rec, item, (REAL)fontHeight, menu_info.fontName,
				menu_info.normalTextColor, GetFontStyle(), GetAlignment());
		}
	}

	if (isItemFocused == TRUE)
	{
		int mIndex = 0;
		Gdiplus::RectF Rec;
		vector<MENUITEM>::iterator iter;
		for (iter = gm_menuItems.begin(); iter != gm_menuItems.end(); iter++)
		{
			int x = iter->pos.x;
			int y = iter->pos.y;

			Rec.X = (float)x;
			Rec.Y = (float)y;
			Rec.Width = (float)w;
			Rec.Height = (float)h;

			if (mIndex != m_index)
			{

				LPCTSTR item = iter->ItemName.c_str();
				T_Graph::PaintText(hdc, Rec, item, (REAL)fontHeight, menu_info.fontName,
					menu_info.normalTextColor, GetFontStyle(), GetAlignment());
			}

			if (mIndex == m_index)
			{
				LPCTSTR item = iter->ItemName.c_str();
				T_Graph::
					T_Graph::PaintText(hdc, Rec, item, (REAL)fontHeight, menu_info.fontName,
						menu_info.focusTextColor, GetFontStyle(), GetAlignment());

				if (BtnDIB.GetBmpHandle() != NULL)
				{
					double num = wcslen(item);					// �ַ���
					//һ���ַ���Լռ16������(ÿ����ĸռ�����ؿ�һ��������ʱȡ����19)
					REAL a = Rec.Width / 2.0;					// ������İ��
					int b = ROUND((num / 2.0)) * 19;			// һ��������ռ������
					int destX = x + a - b;						// СĢ������ʼ����
					int destY = y + h / 2 - BtnDIB.GetImageHeight() / 2;
					BtnDIB.PaintRegion(BtnDIB.GetBmpHandle(), hdc, destX, destY, 0, 0, BtnDIB.GetImageWidth(), BtnDIB.GetImageHeight(), 1, 0, btnTrans);
				}

			}
			mIndex = mIndex + 1;
		}
	}


}

