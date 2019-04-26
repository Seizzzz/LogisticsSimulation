#include "main.h"
using namespace std;

ifstream infile(InputFileName);
ofstream outfile(OutputFileName);

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
	while (infile >> TimeTable[_TotalOrder++].Number) //输入
	{
		infile >> TimeTable[_TotalOrder].OrderTime
			>> TimeTable[_TotalOrder].Restaurant_x >> TimeTable[_TotalOrder].Restaurant_y
			>> TimeTable[_TotalOrder].Customer_x >> TimeTable[_TotalOrder].Customer_y;
	}
	sort(TimeTable, TimeTable + _TotalOrder, cmp_ByOrderTime);

	return;
}

