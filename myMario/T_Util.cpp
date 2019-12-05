//*******************************************************************
// TinyEngine����  
// ����: ������(WanLizhong)
// ����: www.wanlizhong.com 
// ����: 2013-08-02
// ��Ȩ���� 2007-2013 ������
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

// ��������rangeָ����Χ�ĵȶ������
// ����iArrayΪ�����������������
// ����iArray�������ȶ���ΪԪ�ظ�������range������
void T_Util::GetRandomNum(int range, int* iArray)
{
	int i;
	// ��ʼ�������
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


//������������ָ�����������x��y������ʣ�б���ƶ����ٶȣ�
//xRatio��yRatioΪ�������, �����ռ�����
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


//�������������������ָ������㡢�յ�������ƶ��ٶ�
//���Ҫ��������ֱ���ƶ��ҳ����ٶ�Ϊָ���ƶ��ٶ�
//������������̱߶�Ӧ�ķ����ٶ�
//xRatio��yRatioΪ�������, �����ռ�����
//���������Զ��жϷ���
//	xRatio��yRatioΪ��ֵ����ʾ���ҡ����ƶ�
//	xRatio��yRatioΪ��ֵ����ʾ�������ƶ�
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

// ѭ���ĸı���ԭɫ
// ������ԭɫ������������ѭ���ı���ԭɫ�е���ֵ���γ���ɫ�𽥸ı��Ч��
// red,green,blue����Ϊ�ı�����ԭɫ
// lastRed,lastGreen,lastBlue���ڼ�¼�ı�ǰ����ԭɫ
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



// ���״�ִ�иú�����ʼ������ָ��ʱ��
// ��ʱ�䵽���򷵻�true ���򷵻�false
bool T_Util::Timer(int seconds)
{
	if (beginTick == 0)
		beginTick = GetTickCount();	//��ʼ��ʱ

	if ((int)GetTickCount() - beginTick >= seconds * 1000) {
		beginTick = 0;
		return true;
	}
	else
		return false;
}