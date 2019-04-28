#ifndef _MAIN_H
#define _MAIN_H

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

#define MaxOrder 999
#define MapSize 18
#define MaxBurden 10
#define MaxMotor 15
#define MotorPrice 300
#define GetOrderLimit 3 //�ӵ�ʱ������ 
#define INF 0x7fffffff
#define InputFileName  "sales.txt"
#define OutputFileName "output.txt" 

int _Map[18][18]; //[0~17][0~17] 
int _Time = 0; //��ǰʱ�� 
int _Money = 0; //��Ǯ�� 
int _MotorQuantity = 0; //�������� 
int _TotalOrder = 0; //�ܶ����� 
int _GetOrder = 0; //�ӵ��� 
int _CompleteOrder = 0; //����� 
int _OverTimeOrder = 0; //��ʱ�� 

#endif
