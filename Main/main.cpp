#include "main.h"
#include <windows.h>
#include "graphics.h"
using namespace std;

struct Point; //Point结构体 记录基本位置 
struct Data; //Data结构体 记录数据 
struct Motor; //Motor结构体 记录骑手 
void output(); //输出函数 
void init(); //初始化函数 
void update_Motor(); //更新骑手函数 
void purchase_Motor(); //购买骑手函数 
void deal_DataStack(); //处理订单函数 
bool judge_Overtime(); //判断超时函数 
bool judge_MotorMoving(); //判断移动函数 
int exist_Motor(); //查找可用骑手函数 
int cal_Distance(const Point & A, const Point & B); //计算Point结构体间距离 
void dfs_Path(int have, int need, int nowAt, int dis, vector<Point> & nowOrder); //深度优先搜索 
void search_Path(Motor & ex, vector<Point> & nowOrder, int quantity); //查找路径函数 
void move_Motor(Motor & who); //移动骑手函数 

ostream& operator << (ostream& out, const Motor& ex); //重载<< 方便输出骑手信息 
ostream& operator << (ostream& out, const Data& ex); //重载<< 方便输出订单信息 
bool operator == (const Point& left, const Point& right); //重载== 判断同位置点 
bool operator != (const Point & left, const Point & right); //重载== 判断同位置点 
template<typename T> void reverse_Stack(stack<T> & s); //翻转栈 

#include "realize.h"
#include "animate.h"
#include "in_out.h"

int main()
{
	std::ios::sync_with_stdio(false);
	srand((unsigned)time(NULL)); //初始化随机数 
	intest >> PreMergeTime >> EndMergeTime; //输入配置值 
	
	init(); //创建动画graph、全局变量等 
	ani_init(); //初始化动画配置 
	
	while (_Money >= 0) //当未破产时循环 
	{
		Sleep(DelayTime); //动画延时 
		//if (DataStack.empty() && !judge_MotorMoving()) break; //判断是否处理完所有订单 
		
		update_Motor(); //更新骑手 
		ani_update(); //动画更新 
		thread ani(ani_input); //创建动画输入线程 
		ani.join(); //等待进程结束 

		while (!DataStack.empty() && _Time >= DataStack.top().OrderTime) //当前时间及前 还有未处理的订单
		{
			if (exist_Motor() == -1) //如果不存在可用骑手 
			{
				if (_Money - PreserveMoney >= MotorPrice) //如果当前减去预留钱款后能购买骑手 
				{
					while(_Money - PreserveMoney >= MotorPrice) purchase_Motor(); //则一直买 
					deal_DataStack(); //购买后处理订单 
				} 
				else break; //钱不够 等下一时间节点 
			}
			else deal_DataStack(); //如果存在骑手 则分配订单 
		}
		
		/*
		if (!DataStack.empty() && judge_Overtime()) //判断是否订单超时未分配 
		{
			DataStack.pop(); 
			exit(1); //return 1 吊销营业执照
		}
		*/

		output(); //输出 
		_Time++; //时间单位更新 
	}

	if (_Money < 0) exit(2); //破产退出 返回2 
	
	return 0;
}
