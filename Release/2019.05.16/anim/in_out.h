#pragma once

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
	if (left.x == right.x && left.y == right.y) return true;
	return false;
}
bool operator != (const Point & left, const Point & right)
{
	return !(left == right);
}

template<typename T> void reverse_Stack(stack<T> & s)
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

void output() //completed
{
	//屏幕输出 
	cout << "时间: " << _Time << endl;
	cout << "当前账户金额数: " << _Money << endl;
	for (int i = 0; i < _MotorQuantity; i++)
		cout << MotorVector[i] << endl;
	cout << "接单数: " << _GetOrder << endl;
	cout << "完成数: " << _CompleteOrder << endl;
	cout << "超时数: " << _OverTimeOrder << endl;
	cout << endl;

	//文件输出 
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

void input_File()
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

void input_Keyboard()
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

void init() //todo
{
	initgraph(MapLength * MapSize, MapLength * MapSize);
	setbkcolor(EGERGB(0x0, 0x0, 0x0));
	setfillcolor(EGERGB(0xFF, 0xFF, 0x0));
	for (int x = 0; x < MapLength * MapSize; x += 2 * MapLength)
		for (int y = 0; y < MapLength * MapSize; y += 2 * MapLength)
			bar(x, y, x + MapLength, y + MapLength);
	
	initPoint = Point{ 0,0 };
	_Money = initMoney;
	if (infile) input_File(); //成功打开文件 从文件读入 
	else input_Keyboard(); //无文件 从键盘读入 
	//输入并计算初始点 

	initPoint.x /= _TotalOrder;
	initPoint.y /= _TotalOrder;
	if (initPoint.x % 2) { if (initPoint.y % 2) initPoint.x++; } //x,y同为奇数 
	else { if (initPoint.y % 2 == 0) initPoint.y++; } //x,y同为偶数 

	reverse_Stack(DataStack);
	return;
}
