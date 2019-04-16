#include <bits/stdc++.h>
#define MaxOrder 999
#define INF 0x7fffffff
#define InputFileName sales.txt
#define OutputFileName output.txt
using namespace std;

struct Point
{
	int number;
	int x;
	int y;
};

struct Data
{
	int Number;
	int OrderTime;
	int Restaurant_x;
	int Restaurant_y;
	int Customer_x;
	int Customer_y;
}TimeTable[MaxOrder]; //时间线

ifstream infile(InputFileName);
ofstream outfile(OutputFileName);

int _Map[18][18]; //[0~17][0~17]
int _Money = 0; //总钱数
int _MotorQuantity = 0; //骑手数量
int _TotalOrder = 0; //总订单数
int _GetOrder = 0; //接单数
int _CompleteOrder = 0; //完成数
int _OverTimeOrder = 0; //超时数

inline int Cal_Distance(Point A, Point B)
{
	return abs(A.x - B.x) + abs(A.y - B.y);
}
void Cal_ShortestDistance_DFS(int now, int which, int have, int need, bool* vis, int& ans)
{
	if (now >= ans) return;
	if (have == need)
	{
		ans = now;
		return;
	}

	for (int i = 1; i <= need; i++)
	{
		if (vis[i])
		{
			vis[i] = false;
			Cal_ShortestDistance_DFS(now + Distance[which][i], i, have + 1, need, vis, ans);
			vis[i] = true;
		}
	}
}
int Cal_ShortestDistance(Point * ToSearch, int Quantity)
{
	int ans = INF;
	bool vis[Quantity + 1];
	for (int i = 0; i <= Quantity; i++) vis[i] = true;

	for (int i = 1; i <= Quantity; i++)
	{
		vis[i] = false;
		Cal_ShortestDistance_DFS(Distance[ToSearch[0].number][ToSearch[i].number], i, 1, Quantity, vis, ans);
		vis[i] = true;
	}

	return ans;
}

int cmp_ByOrderTime(Data A, Data B)
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

	while (ifstream >> TimeTable[_TotalOrder].Number) //输入
	{
		_TotalOrder++;
		ifstream >> TimeTable[_TotalOrder].OrderTime
				>> TimeTable[_TotalOrder].Restaurant_x >> TimeTable[_TotalOrder].Restaurant_y
				>> TimeTable[_TotalOrder].Customer_x >> TimeTable[_TotalOrder].Customer_y;
	}
	sort(TimeTable, _TotalOrder, cmp_ByOrderTime);

	while (_Money >= 0)
	{


		Output();
	}

	return 0;
}
