#pragma once

ostream& operator << (ostream& out, Motor& ex) //重载<< 方便输出骑手信息 
{
	out << "第" << ex.number << "位骑手的位置: "
		//<< "(" << (ex.Position.x >> 1) << "," << (ex.Position.y >> 1) << ")";
		<< "(" << ex.Position.x << "," << ex.Position.y << ")"
		<< " 停靠: ";
	
	if(!ex.Map.empty() && judge_Near(ex))
	{
		if(ex.Map.top().isRes) out << "食客: ";
		else out << "餐馆: ";
		out << "(" << ex.Map.top().x << "," << ex.Map.top().y << ")";
	}
	out << ";";
	
	return out;
}
ostream& operator << (ostream& out, const Data& ex) //重载<< 方便输出订单信息
{
	out << ex.Number << " " << ex.OrderTime << " "
		<< ex.Restaurant.x << " " << ex.Restaurant.y << " "
		<< ex.Customer.x << " " << ex.Customer.y;
	return out;
}
bool operator == (const Point& left, const Point& right) //重载== 判断同位置点 
{
	if (left.x == right.x && left.y == right.y) return true;
	return false;
}
bool operator != (const Point & left, const Point & right) //重载== 判断同位置点 
{
	return !(left == right);
}

template<typename T> void reverse_Stack(stack<T> & s) //翻转栈 
{
	stack<T> tmp1;
	stack<T> tmp2;
	while (!s.empty())
	{
		tmp1.push(s.top());
		s.pop();
	}
	while (!tmp1.empty())
	{
		tmp2.push(tmp1.top());
		tmp1.pop();
	}
	while (!tmp2.empty())
	{
		s.push(tmp2.top());
		tmp2.pop();
	}
	return;
}

void output() //输出 
{
	//屏幕输出 
	cout << "时间: " << _Time << endl;
	cout << "钱: " << _Money << endl;
	for (int i = 0; i < _MotorQuantity; i++)
		cout << MotorVector[i] << endl;
	cout << "接单数: " << _GetOrder << endl;

	//文件输出 
	outfile << "时间: " << _Time << endl;
	outfile << "钱: " << _Money << endl;
	for (int i = 0; i < _MotorQuantity; i++)
		outfile << MotorVector[i] << endl;
	outfile << "接单数: " << _GetOrder << endl;
	
	//补充输出 
	cout << "完成数: " << _CompleteOrder << " ";
	outfile << "完成数: " << _CompleteOrder << " ";
	cout << "结单：";
	outfile << "结单：";
		while(!outComplete.empty())
		{
			cout << outComplete.top() << "; ";
			outfile << outComplete.top() << "; ";
			outComplete.pop();
		}
	cout << endl;
	outfile << endl;
		
	cout << "超时数: " << _OverTimeOrder << "; ";
	outfile << "超时数: " << _OverTimeOrder << "; ";
	cout << "罚单：";
	outfile << "罚单：";
		while(!outFail.empty())
		{
			cout << outFail.top() << " ";
			outfile << outFail.top() << " ";
			outFail.pop();
		}
	cout << endl;
	outfile << endl;
	
	//利润输出
	cout << "当前利润: " << _Money + _MotorQuantity*MotorPrice - initMoney << endl;
	outfile << "当前利润: " << _Money + _MotorQuantity*MotorPrice - initMoney << endl;
	
	return;
}

void input_File() //从文件输入 
{
	Data tmp;
	while (infile >> tmp.Number) //输入
	{
		infile >> tmp.OrderTime
			>> tmp.Restaurant.x >> tmp.Restaurant.y
			>> tmp.Customer.x >> tmp.Customer.y;
		/*
		tmp.Restaurant.x <<= 1;
		tmp.Restaurant.y <<= 1;
		tmp.Customer.x <<= 1;
		tmp.Customer.y <<= 1;
		*/
		DataStack.push(tmp);

		initPoint.x += tmp.Restaurant.x;
		initPoint.y += tmp.Restaurant.y;
		_TotalOrder++;
	}
	return;
}

void input_Keyboard() //从键盘/控制台输入 
{
	Data tmp;
	while (cin >> tmp.Number) //输入
	{
		cin >> tmp.OrderTime
			>> tmp.Restaurant.x >> tmp.Restaurant.y
			>> tmp.Customer.x >> tmp.Customer.y;
		/*
		tmp.Restaurant.x <<= 1;
		tmp.Restaurant.y <<= 1;
		tmp.Customer.x <<= 1;
		tmp.Customer.y <<= 1;
		*/
		DataStack.push(tmp);

		initPoint.x += tmp.Restaurant.x;
		initPoint.y += tmp.Restaurant.y;
		_TotalOrder++;
	}
	return;
}

void init() //创建动画graph、全局变量等 
{
	initgraph(MapLength * MapSize + DealLength, MapLength * MapSize); //初始化graph大小 
	setbkcolor(EGERGB(0xFF, 0xFF, 0xFF)); //设置背景色 
	setfillcolor(EGERGB(0xFF, 0xFF, 0x0)); //设置填充色 
	for (int x = 0; x < MapLength * MapSize; x += 2 * MapLength)
		for (int y = 0; y < MapLength * MapSize; y += 2 * MapLength)
			bar(x, y, x + MapLength, y + MapLength); //绘制地图 
	
	initPoint = Point{ 0,0 };
	_Money = initMoney;
	if (infile) input_File(); //成功打开文件 从文件读入 
	else input_Keyboard(); //无文件 从键盘读入 
	//输入并计算初始点 

	initPoint.x /= _TotalOrder;
	initPoint.y /= _TotalOrder;
	if (initPoint.x % 2) { if (initPoint.y % 2) initPoint.x++; } //x,y同为奇数 
	else { if (initPoint.y % 2 == 0) initPoint.y++; } //x,y同为偶数 
	
	initPoint.x = 8; //动画版本默认初始位置 
	initPoint.y = 9; //动画版本默认初始位置 

	reverse_Stack(DataStack);
	return;
}
