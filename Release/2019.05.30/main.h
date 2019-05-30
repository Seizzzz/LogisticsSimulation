#ifndef _MAIN_H
#define _MAIN_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

#define initMoney 1000
#define PreserveMoney 50
#define Profit 10
#define Punish 50
#define TimeMaximum 60
#define LimitTime 5
#define MaxOrder 999
#define MapSize 17
#define MapLength 40
#define Radius 10
#define DelayTime 150
#define MaxBurden 20 //即最多五组订单 
#define MaxMotor 15
#define MotorPrice 300
#define GetOrderLimit 3 //接单时间限制 
#define INF 0x7fffffff
#define InputFileName  "sales.txt"
#define OutputFileName "output.txt" 
#define TestFileName "test.txt" 

/* exit value
0 --- 正常结束 
1 --- 吊销营业执照 
2 --- 破产 
3 --- 被判定恶意废单 
*/

int PreMergeTime = 5;  //经验时间 接收多长时间内的单 
int EndMergeTime = 25; //经验时间 最大订单路径长度 

int _Map[18][18]; //[0~17][0~17] 
int _Time = 0; //当前时间 
int _Money; //总钱数 
int _MotorQuantity = 0; //骑手数量 
int _TotalOrder = 0; //总订单数 
int _GetOrder = 0; //接单数 
int _CompleteOrder = 0; //完成数 
int _OverTimeOrder = 0; //超时数 

#endif
