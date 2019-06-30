#ifndef _MAIN_H
#define _MAIN_H

#include <iostream>
#include <stdlib.h> 
#include <fstream>
#include <cmath>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <thread>

#define initMoney 3000 //��ʼ�ʽ� 
#define PreserveMoney 50 //Ԥ���ʽ� 
#define Profit 10 //�������� 
#define Punish 50 //���ʷ��� 
#define TimeMaximum 60 //�ʱ�� 
#define LimitTime 30 //����ʱ�� 
#define MaxOrder 999 //��󶩵��� 
#define MapSize 17 //��ͼ���� 
#define Radius 10 //���ֱ�ǰ뾶 
#define DelayTime 300 //���¼�� 
#define MapLength 40 //��λ���������� 
#define MaxBurden 20 //��������鶩�� 
#define MaxMotor 30 //��������� 
#define MotorPrice 300 //���ּ۸� 
#define GetOrderLimit 3 //�ӵ�ʱ������ 
#define INF 0x7fffffff //��׼���ֵ 
#define InputFileName  "sales.txt"
#define OutputFileName "output.txt" 
#define TestFileName "test.txt" 

/* exit value
0 --- �������� 
1 --- ����Ӫҵִ�� 
2 --- �Ʋ� 
3 --- ���ж�����ϵ� 
*/

int PreMergeTime = 5;  //����ʱ�� ���ն೤ʱ���ڵĵ� 
int EndMergeTime = 25; //����ʱ�� ��󶩵�·������ 

int _Map[18][18]; //[0~17][0~17] 
int _Time = 0; //��ǰʱ�� 
int _Money; //��Ǯ�� 
int _MotorQuantity = 0; //�������� 
int _TotalOrder = 0; //�ܶ����� 
int _GetOrder = 0; //�ӵ��� 
int _CompleteOrder = 0; //����� 
int _OverTimeOrder = 0; //��ʱ�� 

#endif
