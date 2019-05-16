#pragma once

struct Point
{
	int x;
	int y;
	int deadline; //������ʱ�� 
	bool isRes; //�Ƿ����̼� 
}; //��ͼ�� 
struct Data
{
	int Number;
	int OrderTime;
	Point Restaurant;
	Point Customer;
}; //���� 
struct Motor
{
	int number;
	bool enable;
	Point Position;
	stack<Point> Map;
}; //���� 

ifstream intest(TestFileName);
ifstream infile(InputFileName);
ofstream outfile(OutputFileName);
Point initPoint;
stack<Data> DataStack;
vector<Motor> MotorVector(MaxMotor);

#include "dfs.h"
#include "act.h"
