//*******************************************************************
// TinyEngine引擎  
// 作者: 万立中(WanLizhong)
// 博客: www.wanlizhong.com 
// 日期: 2013-08-02
// 版权所有 2007-2013 万立中
// (C) 2007-2013 WanLizhong All Rights Reserved
//*******************************************************************

#pragma once
#include "T_Config.h"

static int beginTick = 0;	//用于定时

class T_Util
{
private:
public:

	static HWND GetHWnd()
	{
		return FindWindow(WIN_CLASS, NULL);
	}

	static HINSTANCE GetHInst()
	{
		return GetModuleHandle(NULL);
	}

	static wchar_t * int_to_wstring(int i);

	// 产生参数range指定范围的等额随机数
	// 参数iArray为输出随机数的输出参数
	// 参数iArray必须首先定义为元素个数等于range的数组
	static void GetRandomNum(int range, int* iArray);


	// 计算startPT到destPT之间
	// 斜角移动时X轴与Y轴的分量值
	// xRatio和yRatio为输出参数
	static void GetBevelSpeed(
		IN POINT startPT, IN POINT destPT, 
		OUT int& xRatio, OUT int& yRatio
	);

	// 根据输入的三个参数所指定的起点、终点坐标和移动速度
	// 如果要保持两点直线移动且长边速度为指定移动速度
	// 本函数计算出短边对应的分量速度
	// xRatio与yRatio为输出参数, 即最终计算结果
	// 本函数会自动判断方向：
	//	xRatio、yRatio为正值，表示朝右、下移动
	//	xRatio、yRatio为负值，表示朝左、上移动
	static void GetBevelSpeed(
		IN POINT startPT, IN POINT destPT, IN int Speed, 
		OUT int& xRatio, OUT int& yRatio
	);

	// 循环的改变三原色
	// 给定三原色，将根据索引循环改变三原色中的数值，形成颜色逐渐改变的效果
	// red,green,blue用于为改变后的三原色
	// lastRed,lastGreen,lastBlue用于记录改变前的三原色
	static void ChangeRGB(double *red, double *green, double *blue, double *lastRed, double *lastGreen, double *lastBlue, int *changeIndex);
	static bool Timer(int seconds);	// 运行指定时间

};