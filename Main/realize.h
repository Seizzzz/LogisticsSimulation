#pragma once

struct Point
{
	int x;
	int y;
	int begin; //开始时间 
	int deadline; //最晚到达时间 
	bool isRes; //是否是商家 
	int Number; //订单号 
}; //地图点 
struct Data
{
	int Number;
	int OrderTime;
	Point Restaurant;
	Point Customer;
}; //数据 
struct Motor
{
	int number;
	bool enable;
	Point Position;
	stack<Point> Map;
}; //骑手 

ifstream intest(TestFileName);
ifstream infile(InputFileName);
ofstream outfile(OutputFileName);
Point initPoint;
stack<Data> DataStack;
vector<Motor> MotorVector(MaxMotor);
stack<int> outComplete;
stack<int> outFail;

#include "dfs.h"
#include "act.h"
