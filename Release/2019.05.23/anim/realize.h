#pragma once

struct Point
{
	int x;
	int y;
	int begin; //��ʼʱ�� 
	int deadline; //������ʱ�� 
	bool isRes; //�Ƿ����̼� 
	int Number; //������ 
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
stack<int> outComplete;
stack<int> outFail;

#include "dfs.h"
#include "act.h"
