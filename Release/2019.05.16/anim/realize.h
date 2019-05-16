#pragma once

struct Point
{
	int x;
	int y;
	int deadline; //最晚到达时间 
	bool isRes; //是否是商家 
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

#include "dfs.h"
#include "act.h"
