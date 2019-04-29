#include "main.h"
using namespace std;

ifstream infile(InputFileName);
ofstream outfile(OutputFileName);

struct Point
{
	int x;
	int y;
}; //��ͼ�� 
struct Data
{
	int Number;
	int OrderTime;
	Point Restaurant;
	Point Customer;
}; //���� 
struct Motor
{
	int number;
	bool enable;
	Point Position;
	stack<Point> Map;
}; //���� 

Point initPoint;
stack<Data> DataStack;
vector<Motor> MotorVector;

ostream& operator << (ostream& out, const Motor& ex)
{
	out << "��" << ex.number << "λ���ֵ�λ��: "
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
		
		while(_Time >= DataStack.top().OrderTime) //��ǰʱ�估ǰ ����δ����Ķ���
		{
			if(exist_Motor() == -1) //������������� 
			{
				if(_Money >= MotorPrice) purchase_Motor();
				else break;
			}
			else deal_DataStack(); //����������� ����䶩�� 
		} 
		
		if (judge_Overtime())
		{
			DataStack.pop();
			exit(1); //return 1 ����Ӫҵִ��
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
		if(MotorVector[i].enable && //�����ѹ��� 
		   MotorVector[i].Map.empty() && //���ֵ�ǰ������ 
		   MotorVector[i].Position == initPoint) return i; //���ؿ������ֱ�� 
	}
	return -1; //δ�ҵ� 
}

Point move_Motor(Motor& who, Point st, Point ed) //todo
{
	/*
	Point next;
	...
	return next;
	
	Point next = who.Position;
	if(who.Position.x == who.Map.top().x + 1 || who.Position.x == who.Map.top().x - 1) //����з����� �����ѵ�λ 
	{
		if(who.Position.x < who.Map.top().x + 2) next.x += 2;
		else if(who.Position.x < who.Map.top().x - 2) next.x -= 2;
		else next.x
	}// emmm�е�С���� 
	
	who.Position = next;
	return;
	*/
	return initPoint;
}

void update_Motor() //completed
{
	/*
	ÿ��ִ��һ��
	��ÿ�������ƶ������֣�����move������λ��
	������ǰĿ����ԣ���ǰ�����
	��Ϊ���Ҳ��ڳ�ʼ��ʱ�����س�ʼ��
	return;
	*/
	
	for(int i = 0; i < _MotorQuantity; i++) //����ÿһ������ 
	{
		if(!MotorVector[i].Map.empty()) //��ǰ�����н� 
		{
			if(abs(MotorVector[i].Position.x - MotorVector[i].Map.top().x) <= 1 &&
			   abs(MotorVector[i].Position.y - MotorVector[i].Map.top().y) <= 1) MotorVector[i].Map.pop(); //����ڹ��̵㸽�� 
			else move_Motor(MotorVector[i], MotorVector[i].Position, MotorVector[i].Map.top()); //�ڹ����� ����λ�� 
		}
		else if(MotorVector[i].Position != initPoint) MotorVector[i].Map.push(initPoint); //��Ŀ�� ��������� 
		//else ��Ŀ�� ������� ���� 
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
	stack<Point> nowOrder_Restaurant; //����Ҫȥ�ĵ��λ�� 
	stack<Point> nowOrder_Customer; //����Ҫȥ�Ĺ˿�λ�� 
	
	stack<Data> sv; //������������ ʧ��ʱpush�� 
	
	while(true) //�����ж��Ƿ�������µĶ��� 
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
	int able = exist_Motor(); //��¼�������ֵı�� 
	merge_Order(MotorVector[able]); //���� �������̵��������Map�� 
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
	outfile << "��ǰ�˻������: " << _Money << endl;
	for (int i = 1; i <= _MotorQuantity; i++)
		outfile << MotorVector[i] << endl;
	outfile << "�ӵ���: " << _GetOrder << endl;
	outfile << "�����: " << _CompleteOrder << endl;
	outfile << "��ʱ��: " << _OverTimeOrder << endl;
	outfile << endl;
	return;
}

void init() //todo
{
	//���벢�����ʼ�� 
	Data tmp;
	while (infile >> tmp.Number) //����
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

