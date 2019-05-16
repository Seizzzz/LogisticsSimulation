#include "main.h"
#include <windows.h>
#include "graphics.h"
using namespace std;

struct Point;
struct Data;
struct Motor;
void output();
void init();
void update_Motor();
void purchase_Motor();
void deal_DataStack();
bool judge_Overtime();
bool judge_MotorMoving();
int exist_Motor();
int cal_Distance(const Point & A, const Point & B);
void dfs_Path(int have, int need, int nowAt, int dis, vector<Point> & nowOrder);
void search_Path(Motor & ex, vector<Point> & nowOrder, int quantity);
void move_Motor(Motor & who);

ostream& operator << (ostream& out, const Motor& ex);
ostream& operator << (ostream& out, const Data& ex);
bool operator == (const Point& left, const Point& right);
bool operator != (const Point & left, const Point & right);
template<typename T> void reverse_Stack(stack<T> & s);

#include "realize.h"
#include "in_out.h"

int main()
{
	std::ios::sync_with_stdio(false);
	intest >> PreMergeTime >> EndMergeTime;
	
	init();

	while (_Money >= 0) //当 
	{
		Sleep(DelayTime);
		if (DataStack.empty() && !judge_MotorMoving()) break;
		update_Motor();

		while (!DataStack.empty() && _Time >= DataStack.top().OrderTime) //当前时间及前 还有未处理的订单
		{
			if (exist_Motor() == -1) //如果不存在可用骑手 
			{
				if (_Money >= MotorPrice)
				{
					purchase_Motor();
					deal_DataStack();
				}
				else break;
			}
			else deal_DataStack(); //如果存在骑手 则分配订单 
		}

		if (!DataStack.empty() && judge_Overtime())
		{
			DataStack.pop();
			exit(1); //return 1 吊销营业执照
		}

		output();
		_Time++;
	}

	if (_Money < 0) exit(2);
	
	return 0;
}
