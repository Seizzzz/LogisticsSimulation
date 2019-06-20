#pragma once

ostream& operator << (ostream& out, Motor& ex) //����<< �������������Ϣ 
{
	out << "��" << ex.number << "λ���ֵ�λ��: "
		//<< "(" << (ex.Position.x >> 1) << "," << (ex.Position.y >> 1) << ")";
		<< "(" << ex.Position.x << "," << ex.Position.y << ")"
		<< " ͣ��: ";
	
	if(!ex.Map.empty() && judge_Near(ex))
	{
		if(ex.Map.top().isRes) out << "ʳ��: ";
		else out << "�͹�: ";
		out << "(" << ex.Map.top().x << "," << ex.Map.top().y << ")";
	}
	out << ";";
	
	return out;
}
ostream& operator << (ostream& out, const Data& ex) //����<< �������������Ϣ
{
	out << ex.Number << " " << ex.OrderTime << " "
		<< ex.Restaurant.x << " " << ex.Restaurant.y << " "
		<< ex.Customer.x << " " << ex.Customer.y;
	return out;
}
bool operator == (const Point& left, const Point& right) //����== �ж�ͬλ�õ� 
{
	if (left.x == right.x && left.y == right.y) return true;
	return false;
}
bool operator != (const Point & left, const Point & right) //����== �ж�ͬλ�õ� 
{
	return !(left == right);
}

template<typename T> void reverse_Stack(stack<T> & s) //��תջ 
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

void output() //��� 
{
	//��Ļ��� 
	cout << "ʱ��: " << _Time << endl;
	cout << "Ǯ: " << _Money << endl;
	for (int i = 0; i < _MotorQuantity; i++)
		cout << MotorVector[i] << endl;
	cout << "�ӵ���: " << _GetOrder << endl;

	//�ļ���� 
	outfile << "ʱ��: " << _Time << endl;
	outfile << "Ǯ: " << _Money << endl;
	for (int i = 0; i < _MotorQuantity; i++)
		outfile << MotorVector[i] << endl;
	outfile << "�ӵ���: " << _GetOrder << endl;
	
	//������� 
	cout << "�����: " << _CompleteOrder << " ";
	outfile << "�����: " << _CompleteOrder << " ";
	cout << "�ᵥ��";
	outfile << "�ᵥ��";
		while(!outComplete.empty())
		{
			cout << outComplete.top() << "; ";
			outfile << outComplete.top() << "; ";
			outComplete.pop();
		}
	cout << endl;
	outfile << endl;
		
	cout << "��ʱ��: " << _OverTimeOrder << "; ";
	outfile << "��ʱ��: " << _OverTimeOrder << "; ";
	cout << "������";
	outfile << "������";
		while(!outFail.empty())
		{
			cout << outFail.top() << " ";
			outfile << outFail.top() << " ";
			outFail.pop();
		}
	cout << endl;
	outfile << endl;
	
	//�������
	cout << "��ǰ����: " << _Money + _MotorQuantity*MotorPrice - initMoney << endl;
	outfile << "��ǰ����: " << _Money + _MotorQuantity*MotorPrice - initMoney << endl;
	
	return;
}

void input_File() //���ļ����� 
{
	Data tmp;
	while (infile >> tmp.Number) //����
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

void input_Keyboard() //�Ӽ���/����̨���� 
{
	Data tmp;
	while (cin >> tmp.Number) //����
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

void init() //��������graph��ȫ�ֱ����� 
{
	initgraph(MapLength * MapSize + DealLength, MapLength * MapSize); //��ʼ��graph��С 
	setbkcolor(EGERGB(0xFF, 0xFF, 0xFF)); //���ñ���ɫ 
	setfillcolor(EGERGB(0xFF, 0xFF, 0x0)); //�������ɫ 
	for (int x = 0; x < MapLength * MapSize; x += 2 * MapLength)
		for (int y = 0; y < MapLength * MapSize; y += 2 * MapLength)
			bar(x, y, x + MapLength, y + MapLength); //���Ƶ�ͼ 
	
	initPoint = Point{ 0,0 };
	_Money = initMoney;
	if (infile) input_File(); //�ɹ����ļ� ���ļ����� 
	else input_Keyboard(); //���ļ� �Ӽ��̶��� 
	//���벢�����ʼ�� 

	initPoint.x /= _TotalOrder;
	initPoint.y /= _TotalOrder;
	if (initPoint.x % 2) { if (initPoint.y % 2) initPoint.x++; } //x,yͬΪ���� 
	else { if (initPoint.y % 2 == 0) initPoint.y++; } //x,yͬΪż�� 
	
	initPoint.x = 8; //�����汾Ĭ�ϳ�ʼλ�� 
	initPoint.y = 9; //�����汾Ĭ�ϳ�ʼλ�� 

	reverse_Stack(DataStack);
	return;
}
