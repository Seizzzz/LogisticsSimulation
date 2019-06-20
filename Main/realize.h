#pragma once

struct Point
{
	int x; //横坐标 
	int y; //纵坐标 
	int begin; //开始时间 
	int deadline; //最晚到达时间 
	bool isRes; //是否是商家 
	int Number; //订单号 
}; //地图点 
struct Data
{
	int Number; //订单序号 
	int OrderTime; //下单时间 
	Point Restaurant; //餐馆位置 
	Point Customer; //食客位置 
}; //数据 
struct Motor
{
	int number; //骑手序号 
	bool enable; //骑手购买状态 
	Point Position; //骑手位置 
	stack<Point> Map; //骑手目标点 
	long long color; //0xFFFFFF
}; //骑手 

ifstream intest(TestFileName);
ifstream infile(InputFileName);
ofstream outfile(OutputFileName);
Point initPoint; //骑手初始位置 
stack<Data> DataStack; //订单栈 
vector<Motor> MotorVector(MaxMotor); //骑手vector 
stack<int> outComplete; //存储完成单号 
stack<int> outFail; //存储超时单号 

#include "dfs.h"
#include "act.h"
