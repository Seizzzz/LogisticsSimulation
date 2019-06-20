#pragma once

char text_cus[] = "食客"; //临时存储 方便输出 
char text_res[] = "餐馆";

bool judge_MotorMoving() //判断骑手移动 
{
	for (int i = 0; i < _MotorQuantity; i++) //若某骑手有当前目标点 
	{
		if (!MotorVector[i].Map.empty()) return true; 
	}
	return false;
}

bool judge_Overtime() //判断超时
{
	if (_Time > DataStack.top().OrderTime + GetOrderLimit) return true; //若有订单超出接单时间后还未分配 
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
	int i = (who.Map.top().x - who.Position.x > 0) ? 1 : -1, //单位向量 
		j = (who.Map.top().y - who.Position.y > 0) ? 1 : -1;

	if (who.Map.top().x == who.Position.x)
	{
		if(who.Position.x == MapSize - 1) who.Position.x--;
		else who.Position.x++; //同行 换到另一行
		who.Position.y += j;
	}
	else if (who.Map.top().y == who.Position.y)
	{
		if(who.Position.y == MapSize - 1) who.Position.y--;
		else who.Position.y++; //同列 换到另一列
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

bool judge_Near(Motor & ex)
{
	if (ex.Map.top().x == ex.Position.x)
	{
		if (abs(ex.Map.top().y - ex.Position.y) == 1) return true; //在同行 且列坐标差为1 即在左右 
	}
	else if (ex.Map.top().y == ex.Position.y)
	{
		if (abs(ex.Map.top().x - ex.Position.x) == 1) return true; //在同列 且行坐标差为1 即在上下 
	}
	
	//if(ex.Map.top().x == ex.Position.x && ex.Map.top().y - ex.Position.y) return true; //某些情况下踩在上面 
	
	return false;
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
	cleardevice(); //清空当前动画内容 
	
	//打印地图 
	setfillcolor(EGERGB(0xFF, 0xFF, 0x0));
	for (int x = 0; x < MapLength * MapSize; x += 2 * MapLength)
		for (int y = 0; y < MapLength * MapSize; y += 2 * MapLength)
			bar(x, y, x + MapLength, y + MapLength);
	//打印骑手 
	setfillcolor(EGERGB(0xFF, 0x0, 0x0));
	for (int i = 0; i < _MotorQuantity; i++)  
	{
		setfillcolor(MotorVector[i].color);
		fillellipse(20 + MapLength * MotorVector[i].Position.x, 20 + MapLength * MotorVector[i].Position.y, Radius, Radius); 
	}
	//打印目标点 
	setfillcolor(EGERGB(0x0, 0xFF, 0x0));
	for (int i = 0; i < _MotorQuantity; i++)
	{
		if(!MotorVector[i].Map.empty())
		{
			int tmpx = MapLength * MotorVector[i].Map.top().x,
				tmpy = MapLength * MotorVector[i].Map.top().y;
			bar(tmpx, tmpy, tmpx + MapLength, tmpy + MapLength);
			
			if(MotorVector[i].Map.top()==initPoint) continue; //如果是初始点 不显示信息 
			
			//对每个目标点 在其右输出相关信息 
			setfont(20,0,"微软雅黑");
			char temp[30];
			sprintf(temp,"订单号:%d",MotorVector[i].Map.top().Number);
			
			if(MotorVector[i].Map.top().isRes) outtextxy(tmpx+40,tmpy,text_res);
			else outtextxy(tmpx+40,tmpy,text_cus);
			outtextxy(tmpx+40,tmpy+20,temp);
			setfont(50,0,"微软雅黑");
		}
	}

	for (int i = 0; i < _MotorQuantity; i++) //遍历每一个骑手 
	{
		if (!MotorVector[i].Map.empty()) //当前正在行进 
		{
			if (judge_Arrive(MotorVector[i])) //如果在过程点附近
			{
				if (MotorVector[i].Map.empty()) continue; //对应回到起始点情况 
				if (MotorVector[i].Map.top().isRes && MotorVector[i].Map.top().begin > _Time) continue; //对应未到达接单时间情况 
				if (!MotorVector[i].Map.top().isRes) //如果完成的点是顾客 
				{
					if (MotorVector[i].Map.top().deadline >= _Time) //及时赶到
					{
						outComplete.push(MotorVector[i].Map.top().Number);
						_Money += Profit;
						_CompleteOrder++;
					}
					else if (_Time > MotorVector[i].Map.top().deadline + TimeMaximum) exit(3); //(60,...
					else //(30,60]
					{
						outFail.push(MotorVector[i].Map.top().Number);
						_Money -= Punish;
						_OverTimeOrder++;
					}
				}
				MotorVector[i].Map.pop(); //当前目标点出栈 
			}
			else move_Motor(MotorVector[i]); //在过程中 更新位置 
		}
		else if (MotorVector[i].Position != initPoint) MotorVector[i].Map.push(initPoint); //无目标 但不在起点 
		//else 无目标 且在起点 忽略 
	}
	return;
}

void purchase_Motor() //购买骑手 
{
	_Money -= MotorPrice; //扣除费用 
	MotorVector[_MotorQuantity].number = _MotorQuantity; //分配骑手序号 
	MotorVector[_MotorQuantity].enable = true; //标记已购买 
	MotorVector[_MotorQuantity].Position = initPoint; //移动至初始位置 
	MotorVector[_MotorQuantity].color = (rand() * 65530) % 0xFFFFFF; //分配颜色用于绘制 
	_MotorQuantity++; //全局骑手数增加 
	return;
}
