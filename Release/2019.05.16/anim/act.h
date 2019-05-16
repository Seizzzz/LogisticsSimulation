#pragma once

bool judge_MotorMoving()
{
	for (int i = 0; i < _MotorQuantity; i++)
	{
		if (!MotorVector[i].Map.empty()) return true;
	}
	return false;
}

bool judge_Overtime() //todo
{
	if (_Time > DataStack.top().OrderTime + 3) return true;
	return false;
}

int exist_Motor() //completed
{
	for (int i = 0; i < _MotorQuantity; i++)
	{
		//如果骑手无任务且返回到了起点 优先使用  因此处起点位置全局较优 
		if (MotorVector[i].enable && //骑手已购买 
			MotorVector[i].Map.empty() && //骑手当前无任务 
			MotorVector[i].Position == initPoint) return i; //返回可用骑手编号

		 //其次使用未返回起点但无任务的骑手 
		if (MotorVector[i].enable && //骑手已购买 
			MotorVector[i].Map.size() == 1 && //骑手当前目标只剩一个点了 
			MotorVector[i].Map.top() == initPoint) //当前只需返回起点 
		{
			MotorVector[i].Map.pop(); //取消返回起点的计划 准备接受任务指派 
			return i;
		}
	}
	return -1; //未找到 
}

void move_Motor(Motor & who) //completed
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
			who.Position.y += 2 * j;
		else if (abs(who.Map.top().y - who.Position.y) == 1) //列方向上到位
			who.Position.x += 2 * i;
	}

	return;
}

bool judge_Arrive(Motor & ex) //completed
{
	if (ex.Map.size() == 1 && ex.Map.top() == initPoint) //因初始点不在店家类位置 此处需单独操作判断 
	{
		if (abs(ex.Position.x - initPoint.x) == 1 && abs(ex.Position.y - initPoint.y) == 1)
		{
			ex.Position = initPoint; //回到初始点 
			while (!ex.Map.empty()) ex.Map.pop(); //清空 
			return true;
		}
		return false;
	}

	if (ex.Map.top().x == ex.Position.x)
	{
		if (abs(ex.Map.top().y - ex.Position.y) == 1) return true; //在同行 且列坐标差为1 即在左右 
	}
	else if (ex.Map.top().y == ex.Position.y)
	{
		if (abs(ex.Map.top().x - ex.Position.x) == 1) return true; //在同列 且行坐标差为1 即在上下 
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
	cleardevice();
	setfillcolor(EGERGB(0xFF, 0xFF, 0x0));
	for (int x = 0; x < MapLength * MapSize; x += 2 * MapLength)
		for (int y = 0; y < MapLength * MapSize; y += 2 * MapLength)
			bar(x, y, x + MapLength, y + MapLength);
	setfillcolor(EGERGB(0xFF, 0x0, 0x0));
	for (int i = 0; i < _MotorQuantity; i++)
		fillellipse(20 + MapLength * MotorVector[i].Position.x, 20 + MapLength * MotorVector[i].Position.y, Radius, Radius);

	for (int i = 0; i < _MotorQuantity; i++) //遍历每一个骑手 
	{
		if (!MotorVector[i].Map.empty()) //当前正在行进 
		{
			if (judge_Arrive(MotorVector[i])) //如果在过程点附近
			{
				if (MotorVector[i].Map.empty()) continue; //对应回到起始点情况 
				if (!MotorVector[i].Map.top().isRes) //如果完成的点是顾客 
				{
					if (MotorVector[i].Map.top().deadline >= _Time) //及时赶到
					{
						_Money += Profit;
						_CompleteOrder++;
					}
					else if (_Time > MotorVector[i].Map.top().deadline + LimitTime) exit(3); //(60,...
					else //(30,60]
					{
						_Money -= Punish;
						_OverTimeOrder++;
					}
				}
				MotorVector[i].Map.pop();
			}
			else move_Motor(MotorVector[i]); //在过程中 更新位置 
		}
		else if (MotorVector[i].Position != initPoint) MotorVector[i].Map.push(initPoint); //无目标 但不在起点 
		//else 无目标 且在起点 忽略 
	}
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
