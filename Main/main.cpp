#include <iostream>
#include <fstream>
//#include <queue>
#include <algorithm>
#include "dfspath.h"
#define MaxOrder 999
#define MapSize 18
#define MaxBurden 5
#define MaxMotor 15
#define INF 0x7fffffff
#define InputFileName "sales.txt"
#define OutputFileName "output.txt" 
using namespace std;

ifstream infile(InputFileName);
ofstream outfile(OutputFileName);

int _Map[18][18]; //[0~17][0~17]
int _Time; //当前时间 
int _Money = 0; //总钱数
int _MotorQuantity = 1; //骑手数量
int _TotalOrder = 0; //总订单数
int _GetOrder = 0; //接单数
int _CompleteOrder = 0; //完成数
int _OverTimeOrder = 0; //超时数

struct Data
{
	int Number;
	int OrderTime;
	int Restaurant_x;
	int Restaurant_y;
	int Customer_x;
	int Customer_y;
}TimeTable[MaxOrder]; //时间线

struct Motor
{
	int number;
	bool enable;
	int x;
	int y;
	int time;
}MotorArray[MaxMotor];

ostream& operator << (ostream& out, const Motor& ex)
{
	out << "第" << ex.number << "位骑手的位置: " << "(" << ex.x << "," << ex.y << ")";
	return out;
}

inline bool cmp_ByOrderTime(const Data A, const Data B); 
void Output();
void init();

int main()
{
	std::ios::sync_with_stdio(false);
	init();
	
	while (_Money >= 0 && _TotalOrder > 0) //todo
	{
		//PurchaseMotor();
		//DealOrder();
		//UpdateMotor();
		Output();
		_Time++;
	}

	return 0;
}

///******///

inline bool cmp_ByOrderTime(const Data A, const Data B)
{
	return A.OrderTime < B.OrderTime;
}

void Output()
{
	outfile << "当前账户金额数: " << _Money << endl;
	for (int i = 1; i <= _MotorQuantity; i++) outfile << MotorArray[i] << endl;
	outfile << "接单数: " << _GetOrder << endl;
	outfile << "完成数: " << _CompleteOrder << endl;
	outfile << "超时数: " << _OverTimeOrder << endl;
	outfile << endl;

	return;
}

void init()
{
	while (infile >> TimeTable[++_TotalOrder].Number) //输入
	{
		infile >> TimeTable[_TotalOrder].OrderTime
			>> TimeTable[_TotalOrder].Restaurant_x >> TimeTable[_TotalOrder].Restaurant_y
			>> TimeTable[_TotalOrder].Customer_x >> TimeTable[_TotalOrder].Customer_y;
	}
	sort(TimeTable, TimeTable + _TotalOrder, cmp_ByOrderTime);

	return;
}

