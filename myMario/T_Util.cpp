//*******************************************************************
// TinyEngine引擎  
// 作者: 万立中(WanLizhong)
// 博客: www.wanlizhong.com 
// 日期: 2013-08-02
// 版权所有 2007-2013 万立中
// (C) 2007-2013 WanLizhong All Rights Reserved
//*******************************************************************

#include "T_Util.h"

wchar_t * T_Util::int_to_wstring(int i)
{
	wchar_t *w_str;
	w_str = new wchar_t[20];
	if(!w_str)
	{
		delete []w_str;
		return NULL;
	}
	swprintf(w_str, 20, L"%d ", i);
	return w_str;
}

// 产生参数range指定范围的等额随机数
// 参数iArray为输出随机数的输出参数
// 参数iArray必须首先定义为元素个数等于range的数组
void T_Util::GetRandomNum(int range, int* iArray)
{
	int i;
	// 初始化随机数
	srand((unsigned)time(NULL));	
	for(i=0; i<range; ++i) 
	{
		iArray[i]=i;
	}
	for(i=range; i>0; --i) 
	{
		int r = rand()%i;
		swap(iArray[i-1], iArray[r]);
	}
}


//根据两个参数指定的坐标计算x、y轴的速率（斜角移动的速度）
//xRatio与yRatio为输出参数, 即最终计算结果
void T_Util::GetBevelSpeed(IN POINT startPT, IN POINT destPT, 
									OUT int& xRatio, OUT int& yRatio)
{
	float aX, bY, Bias;
	if(destPT.x > startPT.x)
	{
		aX = (float)(destPT.x - startPT.x);
	}
	else
	{
		aX = (float)(startPT.x - destPT.x);
	}

	if(destPT.y > startPT.y)
	{
		bY = (float)(destPT.y - startPT.y);	
	}
	else
	{
		bY = (float)(startPT.y - destPT.y);	
	}

	if(aX==0 || bY==0)
	{
		xRatio = 0;
		yRatio = 0;
		return;
	}

	Bias = sqrt(aX*aX+bY*bY);
	xRatio = (int)(aX/Bias);
	yRatio = (int)(bY/Bias);

	if(startPT.x-destPT.x == 0) xRatio = 0;
	if(startPT.y-destPT.y == 0) yRatio = 0;
	if(startPT.x-destPT.x > 0) xRatio = -xRatio;
	if(startPT.y-destPT.y > 0) yRatio = -yRatio;
}


//根据输入的三个参数所指定的起点、终点坐标和移动速度
//如果要保持两点直线移动且长边速度为指定移动速度
//本函数计算出短边对应的分量速度
//xRatio与yRatio为输出参数, 即最终计算结果
//本函数会自动判断方向：
//	xRatio、yRatio为正值，表示朝右、下移动
//	xRatio、yRatio为负值，表示朝左、上移动
void T_Util::GetBevelSpeed(IN POINT startPT, IN POINT destPT, IN int Speed, 
									 OUT int& xRatio, OUT int& yRatio)
{
	float aX,bY;
	if(destPT.x > startPT.x)
	{
		aX = (float)(destPT.x - startPT.x);
	}
	else
	{
		aX = (float)(startPT.x - destPT.x);
	}

	if(destPT.y > startPT.y)
	{
		bY = (float)(destPT.y - startPT.y);	
	}
	else
	{
		bY = (float)(startPT.y - destPT.y);	
	}

	if(aX==0 || bY==0)
	{
		xRatio = 0;
		yRatio = 0;
		return;
	}

	if(bY>aX)
	{
		yRatio = Speed;
		xRatio = (int)((Speed*aX)/bY);
	}
	else
	{
		xRatio = Speed;
		yRatio = (int)((Speed*bY)/aX);
	}
	if(startPT.x-destPT.x == 0) xRatio = 0;
	if(startPT.y-destPT.y == 0) yRatio = 0;
	if(startPT.x-destPT.x > 0) xRatio = -xRatio;
	if(startPT.y-destPT.y > 0) yRatio = -yRatio;
}

// 循环的改变三原色
// 给定三原色，将根据索引循环改变三原色中的数值，形成颜色逐渐改变的效果
// red,green,blue用于为改变后的三原色
// lastRed,lastGreen,lastBlue用于记录改变前的三原色
void T_Util::ChangeRGB(double *red, double *green, double *blue, double *lastRed, double *lastGreen, double *lastBlue, int *changeIndex)
{
	switch (*changeIndex) {
	case 0:
		if (*red <= 10) {
			*changeIndex = 1;
			*green = 11;
			*lastGreen = 10;
		}
		else if ((*lastRed < *red && *red <= 230)) {
			*lastRed = *red;
			*red += 0.4;
		}
		else if (*lastRed > *red || *red > 230) {
			*lastRed = *red;
			*red -= 0.4;
		}
		break;
	case 1:
		if (*green <= 10) {
			*changeIndex = 2;
			*blue = 11;
			*lastBlue = 10;
		}
		else if ((*lastGreen < *green && *green <= 230)) {
			*lastGreen = *green;
			*green += 0.4;
		}
		else if (*lastGreen > *green || *green > 230) {
			*lastGreen = *green;
			*green -= 0.4;
		}
		break;
	case 2:
		if (*blue <= 10) {
			*changeIndex = 0;
			*red = 11;
			*lastRed = 10;
		}
		else if ((*lastBlue < *blue && *blue <= 230)) {
			*lastBlue = *blue;
			*blue += 0.4;
		}
		else if (*lastBlue > *blue || *blue > 230) {
			*lastBlue = *blue;
			*blue -= 0.4;
		}
		break;
	}
}



// 从首次执行该函数开始，运行指定时间
// 若时间到，则返回true 否则返回false
bool T_Util::Timer(int seconds)
{
	if (beginTick == 0)
		beginTick = GetTickCount();	//开始计时

	if ((int)GetTickCount() - beginTick >= seconds * 1000) {
		beginTick = 0;
		return true;
	}
	else
		return false;
}