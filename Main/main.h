#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <fstream>
//#include <queue>
#include <algorithm>
#include "dfspath_v2.h"
#define MaxOrder 999
#define MapSize 18
#define MaxBurden 5
#define MaxMotor 15
#define INF 0x7fffffff
#define InputFileName "sales.txt"
#define OutputFileName "output.txt" 

struct Data
{
	int Number;
	int OrderTime;
	int Restaurant_x;
	int Restaurant_y;
	int Customer_x;
	int Customer_y;
}TimeTable[MaxOrder]; //ʱ����

struct Motor
{
	int number;
	bool enable;
	int x;
	int y;
	int time;
}MotorArray[MaxMotor];

int _Map[18][18]; //[0~17][0~17]
int _Time; //��ǰʱ�� 
int _Money = 0; //��Ǯ��
int _MotorQuantity = 1; //��������
int _TotalOrder = 0; //�ܶ�����
int _GetOrder = 0; //�ӵ���
int _CompleteOrder = 0; //�����
int _OverTimeOrder = 0; //��ʱ��

#endif
