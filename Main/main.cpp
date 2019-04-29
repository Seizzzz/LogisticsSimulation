#include "main.h"
using namespace std;

ifstream infile(InputFileName);
ofstream outfile(OutputFileName);

struct Point
{
	int x;
	int y;
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

Point initPoint;
stack<Data> DataStack;
vector<Motor> MotorVector;

ostream& operator << (ostream& out, const Motor& ex)
{
	out << "第" << ex.number << "位骑手的位置: "
		<< "(" << ex.Position.x << "," << ex.Position.y << ")";
	return out;
}
ostream& operator << (ostream& out, const Data& ex)
{
	out << ex.Number << " " << ex.OrderTime << " "
		<< ex.Restaurant.x << " " << ex.Restaurant.y << " "
		<< ex.Customer.x << " " << ex.Customer.y;
	return out;
}
bool operator == (const Point& left, const Point& right)
{
	if(left.x == right.x && left.y == right.y) return true;
	return false;
}
bool operator != (const Point& left, const Point& right)
{
	return !(left == right);
}

template<typename T> void reverse_Stack(stack<T>& s)
{
	stack<T> tmp1;
	stack<T> tmp2;
	while(!s.empty())
	{
		tmp1.push(s.top());
		s.pop();
	}
	while(!tmp1.empty())
	{
		tmp2.push(tmp1.top());
		tmp1.pop();
	}
	while(!tmp2.empty())
	{
		s.push(tmp2.top());
		tmp2.pop();
	}
	return;
}

void output();
void init();
void update_Motor();
void purchase_Motor();
void deal_DataStack();
bool judge_Overtime();
int exist_Motor();
int cal_Distance(const Point& A, const Point& B);
Point move_Motor(Motor& who, Point st, Point ed);

int main()
{
	std::ios::sync_with_stdio(false);
	init();
	
	/*
	while(!DataStack.empty())
	{
		outfile << DataStack.top() << endl;
		DataStack.pop();
	}
	*/
	
	while (_Money >= 0)
	{
		update_Motor();
		
		while(_Time >= DataStack.top().OrderTime) //当前时间及前 还有未处理的订单
		{
			if(exist_Motor() == -1) //如果不存在骑手 
			{
				if(_Money >= MotorPrice) purchase_Motor();
				else break;
			}
			else deal_DataStack(); //如果存在骑手 则分配订单 
		} 
		
		if (judge_Overtime())
		{
			DataStack.pop();
			exit(1); //return 1 吊销营业执照
		}
		
		
		output();
		_Time++;
	}

	return 0;
}

///******///
bool judge_Overtime() //todo
{
	if(_Time > DataStack.top().OrderTime + 3) return true;
	return false;
}

int exist_Motor() //completed
{
	for(int i = 0; i < _MotorQuantity; i++)
	{
		if(MotorVector[i].enable && //骑手已购买 
		   MotorVector[i].Map.empty() && //骑手当前无任务 
		   MotorVector[i].Position == initPoint) return i; //返回可用骑手编号 
	}
	return -1; //未找到 
}

Point move_Motor(Motor& who, Point st, Point ed) //todo
{
	/*
	Point next;
	...
	return next;
	
	Point next = who.Position;
	if(who.Position.x == who.Map.top().x + 1 || who.Position.x == who.Map.top().x - 1) //如果行方向上 骑手已到位 
	{
		if(who.Position.x < who.Map.top().x + 2) next.x += 2;
		else if(who.Position.x < who.Map.top().x - 2) next.x -= 2;
		else next.x
	}// emmm有点小问题 
	
	who.Position = next;
	return;
	*/
	return initPoint;
}

void update_Motor() //completed
{
	/*
	每秒执行一次
	对每个正在移动的骑手，调用move，更新位置
	若到当前目标点旁，当前点出列
	当为空且不在初始点时，返回初始点
	return;
	*/
	
	for(int i = 0; i < _MotorQuantity; i++) //遍历每一个骑手 
	{
		if(!MotorVector[i].Map.empty()) //当前正在行进 
		{
			if(abs(MotorVector[i].Position.x - MotorVector[i].Map.top().x) <= 1 &&
			   abs(MotorVector[i].Position.y - MotorVector[i].Map.top().y) <= 1) MotorVector[i].Map.pop(); //如果在过程点附近 
			else move_Motor(MotorVector[i], MotorVector[i].Position, MotorVector[i].Map.top()); //在过程中 更新位置 
		}
		else if(MotorVector[i].Position != initPoint) MotorVector[i].Map.push(initPoint); //无目标 但不在起点 
		//else 无目标 且在起点 忽略 
	}
	return;
}

int cal_Distance(const Point& A, const Point& B) //todo
{
	//not exact value
	//return ...
	return 0;
}

void dfs_Path(Motor& ex, vector<Point>& Res, vector<Point>& Cus) //todo
{
	vector<bool> visRes;

	
	
}

bool able_Order(Motor& ex, stack<Point>& inRes, stack<Point>& inCus) //todo
{
	vector<Point> Res;
	vector<Point> Cus;
	stack<Point> sv_Res;
	stack<Point> sv_Cus;
	
	while(!inRes.empty())
	{
		Res.push_back(inRes.top());
		sv_Res.push(inRes.top());
		inRes.pop(); 
	}
	while(!inCus.empty())
	{
		Cus.push_back(inCus.top());
		sv_Cus.push(inCus.top());
		inCus.pop();
	}
	
	dfs_Path(ex,Res,Cus);
	
	while(!sv_Res.empty())
	{
		inRes.push(sv_Res.top());
		sv_Res.pop(); 
	}
	while(!sv_Cus.empty())
	{
		inCus.push(sv_Cus.top());
		sv_Cus.pop();
	}
	
	if (!ex.Map.empty()) return true;
	//else
	return false;
} 

void merge_Order(Motor& ex)
{
	stack<Point> nowOrder_Restaurant; //保存要去的店家位置 
	stack<Point> nowOrder_Customer; //保存要去的顾客位置 
	
	stack<Data> sv; //保存输入数据 失败时push回 
	
	while(true) //不断判断是否能添加新的订单 
	{	
		sv.push(DataStack.top());
		nowOrder_Restaurant.push(DataStack.top().Restaurant);
		nowOrder_Customer.push(DataStack.top().Customer);
		DataStack.pop();
		
		if(able_Order(ex,nowOrder_Restaurant,nowOrder_Customer));
		else
		{
			DataStack.push(sv.top());
			break;
		} 
	}
	return;
}

void deal_DataStack() //completed
{
	int able = exist_Motor(); //记录可用骑手的编号 
	merge_Order(MotorVector[able]); //计算 并将过程点放入骑手Map中 
	return;
}

void purchase_Motor() //completed
{	
	_Money -= MotorPrice;
	MotorVector[_MotorQuantity].enable=true;
	MotorVector[_MotorQuantity].Position=initPoint;
	_MotorQuantity++;
	return;
}

void output() //completed
{
	outfile << "当前账户金额数: " << _Money << endl;
	for (int i = 1; i <= _MotorQuantity; i++)
		outfile << MotorVector[i] << endl;
	outfile << "接单数: " << _GetOrder << endl;
	outfile << "完成数: " << _CompleteOrder << endl;
	outfile << "超时数: " << _OverTimeOrder << endl;
	outfile << endl;
	return;
}

void init() //todo
{
	//输入并计算初始点 
	Data tmp;
	while (infile >> tmp.Number) //输入
	{
		infile >> tmp.OrderTime
			   >> tmp.Restaurant.x >> tmp.Restaurant.y
			   >> tmp.Customer.x >> tmp.Customer.y;
		tmp.Restaurant.x <<= 1;
		tmp.Restaurant.y <<= 1;
		tmp.Customer.x <<= 1;
		tmp.Customer.y <<= 1;
		DataStack.push(tmp);
		_TotalOrder++;
	}
	reverse_Stack(DataStack);
	return;
}

