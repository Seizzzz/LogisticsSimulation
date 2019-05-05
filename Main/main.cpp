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
		//<< "(" << (ex.Position.x >> 1) << "," << (ex.Position.y >> 1) << ")";
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
void dfs_Path(int have, int need, int nowAt, int dis, vector<Point>& nowOrder);
void search_Path(Motor& ex, vector<Point>& nowOrder, int quantity);
void move_Motor(Motor& who);

int main()
{
	std::ios::sync_with_stdio(false);
	init();

	while (_Money >= 0) //当 
	{
		//if(_CompleteOrder + _OverTimeOrder == _TotalOrder) exit(0);
		if(DataStack.empty() && !judge_MotorMoving()) break;
		update_Motor();
		
		while(!DataStack.empty() && _Time >= DataStack.top().OrderTime) //当前时间及前 还有未处理的订单
		{
			if(exist_Motor() == -1) //如果不存在可用骑手 
			{
				if(_Money >= MotorPrice)
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
		//如果骑手无任务且返回到了起点 优先使用  因此处起点位置全局较优 
		if(MotorVector[i].enable && //骑手已购买 
		   MotorVector[i].Map.empty() && //骑手当前无任务 
		   MotorVector[i].Position == initPoint) return i; //返回可用骑手编号
		    
		//其次使用未返回起点但无任务的骑手 
		if(MotorVector[i].enable && //骑手已购买 
		   MotorVector[i].Map.size() == 1 && //骑手当前目标只剩一个点了 
		   MotorVector[i].Map.top() == initPoint) //当前只需返回起点 
			{
				MotorVector[i].Map.pop(); //取消返回起点的计划 准备接受任务指派 
				return i;
			}
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
			who.Position.y += 2*j;
		else if (abs(who.Map.top().y - who.Position.y) == 1) //列方向上到位
			who.Position.x += 2*i;
	}

	return;
}

bool judge_Arrive(Motor& ex) //completed
{
	if(ex.Map.size() == 1 && ex.Map.top() == initPoint) //因初始点不在店家类位置 此处需单独操作判断 
	{
		if(abs(ex.Position.x - initPoint.x) == 1 && abs(ex.Position.y - initPoint.y) == 1)
		{
			ex.Position = initPoint; //回到初始点 
			while(!ex.Map.empty()) ex.Map.pop(); //清空 
			return true;
		}
		return false;
	} 
	
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
		/*test
		stack<Point> tmp;
		if(MotorVector[i].Map.empty()) cout << "骑手" << i << "为空";
		else
		{
			cout << "骑手" << i << endl;
			while(!MotorVector[i].Map.empty())
			{
				tmp.push(MotorVector[i].Map.top());
				MotorVector[i].Map.pop();
			}	
		} 
		while(!tmp.empty())
		{
			cout << "(" << tmp.top().x << "," << tmp.top().y << ") "; 
			MotorVector[i].Map.push(tmp.top());
			tmp.pop();
		}
		cout << endl;
		*/
		
		if(!MotorVector[i].Map.empty()) //当前正在行进 
		{
			if(judge_Arrive(MotorVector[i])) //如果在过程点附近
			{
				if(MotorVector[i].Map.empty()) continue; //对应回到起始点情况 
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
	return abs((A.x - B.x) >> 1) + abs((A.y - B.y) >> 1);
}

vector<vector<int>> Distance(MaxBurden, vector<int>(MaxBurden, 0));
vector<bool> vis(MaxBurden);
vector<int> sv_Path(MaxBurden);
vector<int> sv_Path_Min(MaxBurden);
int sv_Dis_Min = INF;

void dfs_Path(int have, int need, int nowAt, int dis, vector<Point>& nowOrder) //已有点个数 需要点个数 访问数组 现在路程
{
	if (dis >= sv_Dis_Min) return;

	sv_Path[have] = nowAt;
	if (have == need)
	{
		sv_Dis_Min = dis;
		for (int i = 1; i <= need + 1; i++) sv_Path_Min[i] = sv_Path[i];
		return;
	}

	for (int i = 1; i <= need; i++)
	{
		if (vis[i]) //这个点没到过 
		{
			if (!nowOrder[i].isRes) //如果没去过且该点是顾客
			{
				if (vis[i + 1] == false) //到过这个点对应的商家
				{
					vis[i] = false;
					dfs_Path(have + 1, need, i, dis + Distance[nowAt][i], nowOrder);
					vis[i] = true;
				}
			}
			else //该点是店家
			{
				vis[i] = false;
				dfs_Path(have + 1, need, i, dis + Distance[nowAt][i], nowOrder);
				vis[i] = true;
			}
		}
	}
	return;
}

void search_Path(Motor& ex, vector<Point>& nowOrder, int quantity) //todo
{
	sv_Dis_Min = INF;
	for (int i = 0; i <= quantity; i++) vis[i] = true;
	//初始化 

	for (int i = 0; i <= quantity; i++)
	{
		Distance[i][i] = 0;
		for (int j = i + 1; j <= quantity; j++)
		{
			Distance[i][j] = cal_Distance(nowOrder[i], nowOrder[j]);
			Distance[j][i] = Distance[i][j];
		}
	}
	//计算距离 

	for (int i = 1; i <= quantity; i++)
	{
		if (nowOrder[i].isRes) //选择一个商家的点开始 
		{
			vis[i] = false;
			dfs_Path(1, quantity, i, Distance[0][i], nowOrder);
			vis[i] = true;
		}
	}
	
	if(sv_Dis_Min > EndMergeTime) return; //如果添加后路径过长 
	//else
	while (!ex.Map.empty()) ex.Map.pop();
	for (int i = 1; i <= quantity; i++) ex.Map.push(nowOrder[sv_Path_Min[i]]);
	return;
}

bool able_Order(Motor& ex, stack<Point>& nowOrder) //todo
{
	vector<Point> nowOrder_vec; //传入dfs的结构体数组
	stack<Point> sv_nowOrder; //保存传入的order栈
	//int sv_Map_size = ex.Map.size();
	int quantity=nowOrder.size();
	
	nowOrder_vec.push_back(ex.Position);
	while (!nowOrder.empty()) //将stack转化为vector 方便操作
	{
		nowOrder_vec.push_back(nowOrder.top());
		sv_nowOrder.push(nowOrder.top());
		nowOrder.pop();
	}
	
	search_Path(ex, nowOrder_vec, quantity); //准备分配

	while (!sv_nowOrder.empty()) //将stack内容还愿
	{
		nowOrder.push(sv_nowOrder.top());
		sv_nowOrder.pop();
	}
	
	//reverse_Stack(ex.Map); //将倒序点反转 
	if (sv_Dis_Min <= EndMergeTime) return true; //如果分配后时间允许 即分配成功
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
		if(DataStack.top().OrderTime <= _Time + PreMergeTime)
		{
			sv.push(DataStack.top());
			nowOrder.push(DataStack.top().Restaurant);
			nowOrder.top().isRes = true;
			nowOrder.top().deadline = INF;
			nowOrder.push(DataStack.top().Customer);
			nowOrder.top().isRes = false;
			nowOrder.top().deadline = DataStack.top().OrderTime + LimitTime;
			
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
		else break;
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
	MotorVector[_MotorQuantity].number = _MotorQuantity;
	MotorVector[_MotorQuantity].enable = true;
	MotorVector[_MotorQuantity].Position = initPoint;
	_MotorQuantity++;
	return;
}

void output() //completed
{
	outfile << "时间: " << _Time << endl;
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
	if(initPoint.x % 2){ if(initPoint.y % 2) initPoint.x++; } //x,y同为奇数 
	else{ if(initPoint.y % 2 == 0) initPoint.y++; } //x,y同为偶数 
	
	reverse_Stack(DataStack);
	return;
}
