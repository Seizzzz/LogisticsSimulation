#include "main.h"
using namespace std;

ifstream infile(InputFileName);
ofstream outfile(OutputFileName);

struct Point
{
	int x;
	int y;
	int deadline; //最晚到达时间 
	bool isRes; //是否是商家 
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
vector<Motor> MotorVector(MaxMotor);

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
bool judge_MotorMoving();
int exist_Motor();
int cal_Distance(const Point& A, const Point& B);
void search_Path(Motor& ex, vector<Point>& Res, vector<Point>& Cus);
void move_Motor(Motor& who);

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
	
	while (_Money >= 0) //当 
	{
		if(DataStack.empty() && !judge_MotorMoving()) break;
		update_Motor();
		
		while(!DataStack.empty() && _Time >= DataStack.top().OrderTime) //当前时间及前 还有未处理的订单
		{
			if(exist_Motor() == -1) //如果不存在可用骑手 
			{
				if(_Money >= MotorPrice) purchase_Motor();
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

	return 0;
}

///******///
bool judge_MotorMoving()
{
	for(int i = 0; i < _MotorQuantity; i++)
	{
		if(!MotorVector[i].Map.empty()) return true;
	}
	return false;
}

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

void move_Motor(Motor& who) //completed
{
	int i = (who.Map.top().x - who.Position.x > 0) ? 1 : -1,
		j = (who.Map.top().y - who.Position.y > 0) ? 1 : -1;

	if (who.Map.top().x == who.Position.x)
	{
		who.Position.x++; //同行 换到另一行
		who.Position.y += j;
	}
	else if (who.Map.top().y == who.Position.y)
	{
		who.Position.y++; //同列 换到另一列
		who.Position.x += i;
	}
	else
	{
		if (abs(who.Map.top().x - who.Position.x) > 1 && abs(who.Map.top().y - who.Position.y) > 1) //平凡位置
		{
			who.Position.x += i;
			who.Position.y += j;
		}
		else if (abs(who.Map.top().x - who.Position.x) == 1) //行方向上到位
			who.Position.y += j;
		else if (abs(who.Map.top().y - who.Position.y) == 1) //列方向上到位
			who.Position.x += i;
	}

	return;
}

bool judge_Arrive(Motor& ex) //completed
{
	if(ex.Map.top().x == ex.Position.x)
	{
		if(abs(ex.Map.top().y - ex.Position.y) == 1) return true; //在同行 且列坐标差为1 即在左右 
	}
	else if(ex.Map.top().y == ex.Position.y)
	{
		if(abs(ex.Map.top().x - ex.Position.x) == 1) return true; //在同列 且行坐标差为1 即在上下 
	}
	return false; 
} 

void update_Motor() //todo  需要添加数量等更新 
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
			if(judge_Arrive(MotorVector[i])) //如果在过程点附近
			{
				if(!MotorVector[i].Map.top().isRes) //如果完成的点是顾客 
				{
					if(MotorVector[i].Map.top().deadline >= _Time) //及时赶到
					{
						_Money += Profit;
						_CompleteOrder++;
					} 
					else
					{
						_Money -= Punish;
						_OverTimeOrder++;
					}
				} 
		   		MotorVector[i].Map.pop();
			} 
			else move_Motor(MotorVector[i]); //在过程中 更新位置 
		}
		else if(MotorVector[i].Position != initPoint) MotorVector[i].Map.push(initPoint); //无目标 但不在起点 
		//else 无目标 且在起点 忽略 
	}
	return;
}

int cal_Distance(const Point& A, const Point& B) //todo
{
	return ((A.x - B.x) >> 1) + ((A.y - B.y) >> 1);
}

vector<Point> sv_Path(MaxBurden);
vector<Point> sv_Path_Min(MaxBurden);
int sv_Dis_Min = INF;

void dfs_Path(int have, int need, vector<bool> visRes, int dis) //已有点个数 需要点个数 访问数组 现在路程
{
	if (dis >= sv_Dis_Min) return;
	if (have == need)
	{
		sv_Dis_Min = dis;
		sv_Path_Min.assign(sv_Path.begin(), sv_Path.end());
	}

	//for(int i=)
	return;
}

void search_Path(Motor& ex, vector<Point>& nowOrder) //todo
{
	vector<bool> visRes(nowOrder.size() , true); //创建一个size大小的true数组 用于保存是否之前访问过对应的商家
	//vector<Point> sv;
	//dfs_Path(0, nowOrder.size(), visRes, 0);

	/*
	此处dfs未完成 先直接返回原始点 非优解
	*/

	for (int i = 0; i < nowOrder.size(); i++)
	{
		sv_Path_Min[i] = nowOrder[i];
	}

	//
	
	for (int i = 0; i < nowOrder.size(); i++)
	{
		ex.Map.push(sv_Path_Min[i]);
	}

	return;
}

bool able_Order(Motor& ex, stack<Point>& nowOrder) //todo
{
	vector<Point> nowOrder_vec; //传入dfs的结构体数组
	stack<Point> sv_nowOrder; //保存传入的order栈
	int sv_Map_size = ex.Map.size();

	while (!nowOrder.empty()) //将stack转化为vector 方便操作
	{
		nowOrder_vec.push_back(nowOrder.top());
		sv_nowOrder.push(nowOrder.top());
		nowOrder.pop();
	}
	
	search_Path(ex, nowOrder_vec); //准备分配

	while (!sv_nowOrder.empty()) //将stack内容还愿
	{
		nowOrder.push(sv_nowOrder.top());
		sv_nowOrder.pop();
	}
	
	if (ex.Map.size() > sv_Map_size) return true; //如果分配后该骑手Map增大 即分配成功
	//else
	return false;
} 

void merge_Order(Motor& ex)
{
	stack<Point> nowOrder; //保存要去的店家和顾客的位置 
	stack<Data> sv; //保存输入数据 失败时push回 
	int quantity = 0; //记录添加的订单数量 
	
	while (!DataStack.empty()) //不断判断是否能添加新的订单 
	{	
		sv.push(DataStack.top());
		nowOrder.push(DataStack.top().Restaurant);
		nowOrder.top().isRes = true;
		nowOrder.push(DataStack.top().Customer);
		nowOrder.top().isRes = false;
		DataStack.pop();
		
		if (able_Order(ex, nowOrder))
		{
			quantity++;
			continue;
		}
		else
		{
			DataStack.push(sv.top()); //还原上一个pop的元素
			break; //下接return
		} 
	}
	
	_GetOrder += quantity;
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
	for (int i = 0; i < _MotorQuantity; i++)
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
	initPoint = Point{0,0};
	_Money = initMoney;
	while (infile >> tmp.Number) //输入
	{
		infile >> tmp.OrderTime
			   >> tmp.Restaurant.x >> tmp.Restaurant.y
			   >> tmp.Customer.x   >> tmp.Customer.y;
		tmp.Restaurant.x <<= 1;
		tmp.Restaurant.y <<= 1;
		tmp.Customer.x <<= 1;
		tmp.Customer.y <<= 1;
		DataStack.push(tmp);
		
		initPoint.x += tmp.Restaurant.x;
		initPoint.y += tmp.Restaurant.y;
		_TotalOrder++;
	}
	
	initPoint.x /= _TotalOrder;
	initPoint.y /= _TotalOrder;
	initPoint.x = (initPoint.x % 2) ? 
						initPoint.x : (initPoint.x < (MapSize >> 1)) ?
													  initPoint.x + 1:initPoint.x - 1;
	initPoint.y = (initPoint.y % 2) ? 
						initPoint.y : (initPoint.y < (MapSize >> 1)) ?
													  initPoint.y + 1:initPoint.y - 1;
	
	reverse_Stack(DataStack);
	return;
}

