//*******************************************************************
// TinyEngine����  
// ����: ������(WanLizhong)
// ����: www.wanlizhong.com 
// ����: 2013-08-02
// ����: 2018-12-13
// ��Ȩ���� 2007-2018 ������
// (C) 2007-2018 WanLizhong All Rights Reserved
//*******************************************************************

#include "T_Menu.h"

T_Menu::T_Menu()
{
	m_index = -1;
	key_index = 0;
	isItemFocused = false;
	bkImageAlpha = 255;
	bkColor = RGB(0, 0, 0);

	m_MoveSound = NULL;
	m_ClickSound = NULL;

}

T_Menu::~T_Menu(void)
{
	DestroyAll();
}

void T_Menu::SetMoveSound(AudioDXBuffer* ms_buffer)
{
	m_MoveSound = ms_buffer;
}

void T_Menu::SetClickSound(AudioDXBuffer* mc_buffer)
{
	m_ClickSound = mc_buffer;
}

//���ò˵�����ͼƬ
void T_Menu::SetMenuBkg(wstring img_path, int alphaLevel, COLORREF backColor)
{
	if (img_path.length() > 0)
	{
		gm_menuBkg.LoadImageFile(img_path.c_str());
		bkColor = backColor;
		bkImageAlpha = alphaLevel;
	}
}

//���������˵�ͼƬ
void T_Menu::SetBtnBmp(wstring img_path, int btnWidth, int btnHeight)
{
	if(img_path.length()>0)
	{
		BtnDIB.LoadImageFile(img_path.c_str());
		if(&BtnDIB!=NULL)
		{
			menu_info.width = btnWidth;
			menu_info.height = btnHeight;
		}
	}
}

//���Ӳ˵�������Ŀ
void T_Menu::AddMenuItem(MENUITEM menuItem)
{
	gm_menuItems.push_back(menuItem);
	int len = (int)(menuItem.ItemName.length());
	if(MaxMenuItemLen<len)
		MaxMenuItemLen = len;
}

//�����ײ˵�������꼰�˵��������
void T_Menu::SetMenuInfo(MENU_INFO menuInfo)
{
	menu_info = menuInfo;
}

//�˵�����
void T_Menu::DrawMenu(HDC hdc, BYTE btnTrans, bool startState)
{
	if(&gm_menuBkg != NULL && startState == true) 
	{
		HBITMAP tBmp = T_Graph::CreateBlankBitmap(WIN_WIDTH, WIN_HEIGHT, bkColor);
		SelectObject(hdc, tBmp);
		
		gm_menuBkg.PaintImage(hdc, 0, 0, WIN_WIDTH, WIN_HEIGHT, bkImageAlpha);

		DeleteObject(tBmp);
		tBmp = NULL;
	}
	else
	{
		T_Graph::PaintBlank(hdc, 0, 0, WIN_WIDTH, WIN_HEIGHT, MENU_BKCLR, MENU_ALPHA);
	}

	int w = menu_info.width;
	int h = menu_info.height;

	int FontHeight;
	int px = 0;
	int w_px = w/(MaxMenuItemLen+1);	//����ÿ������ռ������
	int h_px = (int)((float)(h/2.5));
	if(w_px > h_px) 
	{
		px = h_px;
	}
	else
	{
		px = w_px;
	}
	FontHeight = (px*72)/96;		//����ÿ���ֵ����ؼ����ֺ�

	if(isItemFocused == FALSE)
	{
		Gdiplus::RectF Rec;
		vector<MENUITEM>::iterator iter;
		for(iter = gm_menuItems.begin(); iter != gm_menuItems.end(); iter++)
		{
			int x = iter->pos.x;
			int y = iter->pos.y;
			
			// ���ư�ťͼ��
			if(&BtnDIB != NULL)
			{
				BtnDIB.PaintRegion(BtnDIB.GetBmpHandle(), hdc, x, y, 0, 0, w, h, 1, 0, btnTrans);
			}
			Rec.X = (float)x;		
			Rec.Y = (float)y;
			Rec.Width = (float)w;	
			Rec.Height = (float)h;

			// ���ư�ť����
			LPCTSTR item = iter->ItemName.c_str();
			T_Graph::PaintText(hdc, Rec, item, (REAL)FontHeight, menu_info.fontName, 
				               menu_info.normalTextColor, GetFontStyle(), GetAlignment());
		}
	}

	if(isItemFocused == TRUE)
	{
		int mIndex = 0;
		Gdiplus::RectF Rec;
		vector<MENUITEM>::iterator iter;
		for(iter = gm_menuItems.begin(); iter != gm_menuItems.end(); iter++)
		{
			int x = iter->pos.x;
			int y = iter->pos.y;

			Rec.X = (float)x;		
			Rec.Y = (float)y;
			Rec.Width = (float)w;	
			Rec.Height = (float)h;

			if(mIndex != m_index)
			{
				if(&BtnDIB != NULL)
				{
					BtnDIB.PaintRegion(BtnDIB.GetBmpHandle(), hdc, x, y, 0, 0, w, h, 1, 0, btnTrans);
				}

				LPCTSTR item = iter->ItemName.c_str();
				T_Graph::PaintText(hdc, Rec, item, (REAL)FontHeight, menu_info.fontName, 
					               menu_info.normalTextColor, GetFontStyle(), GetAlignment());
			}

			if(mIndex == m_index)
			{
				if(&BtnDIB != NULL)
				{
					BtnDIB.PaintRegion(BtnDIB.GetBmpHandle(), hdc, x, y, 0, h, w, h, 1, 0, btnTrans);
				}

				LPCTSTR item = iter->ItemName.c_str();
				T_Graph::PaintText(hdc, Rec, item, (REAL)FontHeight, menu_info.fontName, 
					               menu_info.focusTextColor, GetFontStyle(), GetAlignment());
			}
		
			mIndex = mIndex + 1;
		}
	}
}

//���ݵ�ǰ����������˵���������
int T_Menu::GetMenuIndex(int x, int y)
{
	int Index = -1;
	POINT pt;
	pt.x = x;
	pt.y = y;
	int w = menu_info.width;
	int h = menu_info.height;
	RECT rec;
	
	int iCount = 0;
	vector<MENUITEM>::iterator iter;
	for(iter = gm_menuItems.begin(); iter != gm_menuItems.end(); iter++)
	{
		rec.left = iter->pos.x;
		rec.top = iter->pos.y;
		rec.right = rec.left+w;
		rec.bottom = rec.top+h;

		if(PtInRect(&rec, pt))
		{
			return iCount;
		}
		iCount++;
	}
	return Index;
}

StringAlignment T_Menu::GetAlignment()
{
	StringAlignment sAlign;
	if(menu_info.align == 0)
	{
		sAlign = StringAlignmentNear;
	}
	if(menu_info.align == 1)
	{
		sAlign = StringAlignmentCenter;
	}
	if(menu_info.align == 2)
	{
		sAlign = StringAlignmentFar;
	}
	return sAlign;
}

FontStyle T_Menu::GetFontStyle()
{
	FontStyle fStyle;
	if(menu_info.isBold==true) 
	{
		fStyle = FontStyleBold;
	}
	else
	{
		fStyle = FontStyleRegular;
	}
	return fStyle;
}

//�˵�������¼�����
int T_Menu::MenuMouseClick(int x, int y)
{
	m_index = GetMenuIndex(x, y);
	if(m_index >= 0) 
	{
		if(m_MoveSound && m_ClickSound)
		{
			m_MoveSound->Restore();
			m_ClickSound->Restore();
			m_ClickSound->Play(false);
		}
	}
	return m_index;
}

//�˵�����ƶ��¼�����
void T_Menu::MenuMouseMove(int x, int y)
{
	lastIndex = m_index;//��¼ǰһ�ε�����ֵ
	m_index = GetMenuIndex(x,y);
	if(m_index >= 0)
	{
		isItemFocused = true;
		//��֤����ƹ���ťֻ����һ������
		if(lastIndex != m_index) 
		{
			if(m_MoveSound && m_ClickSound)
			{
				m_ClickSound->Restore();
				m_MoveSound->Restore();
				m_MoveSound->Play(false);
			}
		}
	}
	else
	{
		isItemFocused = false;
	}	
}

int T_Menu::MenuKeyDown(WPARAM key)
{
	if(key == VK_UP || key == VK_LEFT)
	{
		m_index=m_index-1;
		if(m_index<0)
		{
			m_index = (int)(gm_menuItems.size()-1);
		}

		isItemFocused = true;
		if(m_MoveSound && m_ClickSound)
		{
			m_ClickSound->Restore();
			m_MoveSound->Restore();
			m_MoveSound->Play(false);
		}		
	}
	if(key == VK_DOWN || key == VK_RIGHT)
	{
		m_index=m_index+1;
		if(m_index>(int)(gm_menuItems.size()-1))
		{
			m_index=0;
		}

		isItemFocused = true;
		if(m_MoveSound && m_ClickSound)
		{
			m_ClickSound->Restore();
			m_MoveSound->Restore();
			m_MoveSound->Play(false);
		}
	}
	return m_index;
}

void T_Menu::DestroyAll()
{
	gm_menuItems.clear();
	vector<MENUITEM> emptyVec;
	gm_menuItems.swap(emptyVec) ;
}