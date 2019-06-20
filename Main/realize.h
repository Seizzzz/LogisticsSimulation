#pragma once

struct Point
{
	int x; //������ 
	int y; //������ 
	int begin; //��ʼʱ�� 
	int deadline; //������ʱ�� 
	bool isRes; //�Ƿ����̼� 
	int Number; //������ 
}; //��ͼ�� 
struct Data
{
	int Number; //������� 
	int OrderTime; //�µ�ʱ�� 
	Point Restaurant; //�͹�λ�� 
	Point Customer; //ʳ��λ�� 
}; //���� 
struct Motor
{
	int number; //������� 
	bool enable; //���ֹ���״̬ 
	Point Position; //����λ�� 
	stack<Point> Map; //����Ŀ��� 
	long long color; //0xFFFFFF
}; //���� 

ifstream intest(TestFileName);
ifstream infile(InputFileName);
ofstream outfile(OutputFileName);
Point initPoint; //���ֳ�ʼλ�� 
stack<Data> DataStack; //����ջ 
vector<Motor> MotorVector(MaxMotor); //����vector 
stack<int> outComplete; //�洢��ɵ��� 
stack<int> outFail; //�洢��ʱ���� 

#include "dfs.h"
#include "act.h"
