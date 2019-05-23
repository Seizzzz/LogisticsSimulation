#pragma once

vector<vector<int>> Distance(MaxBurden, vector<int>(MaxBurden, 0));
vector<bool> vis(MaxBurden);
vector<int> sv_Path(MaxBurden);
vector<int> sv_Path_Min(MaxBurden);
int sv_Dis_Min = INF;

int cal_Distance(const Point & A, const Point & B) //todo
{
	return abs((A.x - B.x) >> 1) + abs((A.y - B.y) >> 1);
}

void dfs_Path(int have, int need, int nowAt, int dis, vector<Point> & nowOrder) //已有点个数 需要点个数 访问数组 现在路程
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

void search_Path(Motor & ex, vector<Point> & nowOrder, int quantity) //todo
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

	if (sv_Dis_Min > EndMergeTime) return; //如果添加后路径过长 
	//else
	while (!ex.Map.empty()) ex.Map.pop();
	for (int i = 1; i <= quantity; i++) ex.Map.push(nowOrder[sv_Path_Min[i]]);
	return;
}

bool able_Order(Motor & ex, stack<Point> & nowOrder) //todo
{
	vector<Point> nowOrder_vec; //传入dfs的结构体数组
	stack<Point> sv_nowOrder; //保存传入的order栈
	//int sv_Map_size = ex.Map.size();
	int quantity = nowOrder.size();

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

void merge_Order(Motor & ex)
{
	stack<Point> nowOrder; //保存要去的店家和顾客的位置 
	stack<Data> sv; //保存输入数据 失败时push回 
	int quantity = 0; //记录添加的订单数量 

	while (!DataStack.empty()) //不断判断是否能添加新的订单 
	{
		if (DataStack.top().OrderTime <= _Time + PreMergeTime)
		{
			sv.push(DataStack.top());
			nowOrder.push(DataStack.top().Restaurant);
			nowOrder.top().begin = DataStack.top().OrderTime;
			nowOrder.top().isRes = true;
			nowOrder.top().deadline = INF;
			nowOrder.push(DataStack.top().Customer);
			nowOrder.top().isRes = false;
			nowOrder.top().Number = DataStack.top().Number;
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
