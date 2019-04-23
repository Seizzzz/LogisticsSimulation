#include <iostream>
#include <fstream>
#include <algorithm>
#include "dfspath.h" 

#define MaxOrder 999
#define MapSize 18
#define MaxBurden 5
#define INF 0x7fffffff
#define InputFileName "sales.txt"
#define OutputFileName "output.txt"
using namespace std;

ifstream infile(InputFileName);
ofstream outfile(OutputFileName);

struct Data
{
	int Number;
	int OrderTime;
	int Restaurant_x;
	int Restaurant_y;
	int Customer_x;
	int Customer_y;
}TimeTable[MaxOrder]; //时间线

int _Map[18][18]; //[0~17][0~17]
int _Money = 0; //总钱数
int _MotorQuantity = 0; //骑手数量
int _TotalOrder = 0; //总订单数
int _GetOrder = 0; //接单数
int _CompleteOrder = 0; //完成数
int _OverTimeOrder = 0; //超时数

inline bool cmp_ByOrderTime(const Data A, const Data B)
{
	return A.OrderTime < B.OrderTime;
}

void Output()
{
	outfile << _Money << endl;
	for (int i = 1; i <= _MotorQuantity; i++)
	{
		;//todo
	}
	outfile << _GetOrder << endl;
	outfile << _CompleteOrder << endl;
	outfile << _OverTimeOrder << endl;

	return;
}

int main()
{
	std::ios::sync_with_stdio(false);
	
	while (infile >> TimeTable[_TotalOrder].Number) //输入
	{
		_TotalOrder++;
		infile >> TimeTable[_TotalOrder].OrderTime
				>> TimeTable[_TotalOrder].Restaurant_x >> TimeTable[_TotalOrder].Restaurant_y
				>> TimeTable[_TotalOrder].Customer_x >> TimeTable[_TotalOrder].Customer_y;
	}
	sort(TimeTable, TimeTable + _TotalOrder, cmp_ByOrderTime);
	
	while (_Money >= 0)
	{
		
		//Output();
	}

	return 0;
}
