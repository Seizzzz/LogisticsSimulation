#include "main.h"
using namespace std;

ifstream infile(InputFileName);
ofstream outfile(OutputFileName);

struct Point
{
	int x;
	int y;
	int deadline; //������ʱ�� 
	bool isRes; //�Ƿ����̼� 
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
vector<Motor> MotorVector(MaxMotor);

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
	
	while (_Money >= 0) //�� 
	{
		if(DataStack.empty() && !judge_MotorMoving()) break;
		update_Motor();
		
		while(!DataStack.empty() && _Time >= DataStack.top().OrderTime) //��ǰʱ�估ǰ ����δ����Ķ���
		{
			if(exist_Motor() == -1) //��������ڿ������� 
			{
				if(_Money >= MotorPrice) purchase_Motor();
				else break;
			}
			else deal_DataStack(); //����������� ����䶩�� 
		} 
		
		if (!DataStack.empty() && judge_Overtime())
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
		if(MotorVector[i].enable && //�����ѹ��� 
		   MotorVector[i].Map.empty() && //���ֵ�ǰ������ 
		   MotorVector[i].Position == initPoint) return i; //���ؿ������ֱ�� 
	}
	return -1; //δ�ҵ� 
}

void move_Motor(Motor& who) //completed
{
	int i = (who.Map.top().x - who.Position.x > 0) ? 1 : -1,
		j = (who.Map.top().y - who.Position.y > 0) ? 1 : -1;

	if (who.Map.top().x == who.Position.x)
	{
		who.Position.x++; //ͬ�� ������һ��
		who.Position.y += j;
	}
	else if (who.Map.top().y == who.Position.y)
	{
		who.Position.y++; //ͬ�� ������һ��
		who.Position.x += i;
	}
	else
	{
		if (abs(who.Map.top().x - who.Position.x) > 1 && abs(who.Map.top().y - who.Position.y) > 1) //ƽ��λ��
		{
			who.Position.x += i;
			who.Position.y += j;
		}
		else if (abs(who.Map.top().x - who.Position.x) == 1) //�з����ϵ�λ
			who.Position.y += j;
		else if (abs(who.Map.top().y - who.Position.y) == 1) //�з����ϵ�λ
			who.Position.x += i;
	}

	return;
}

bool judge_Arrive(Motor& ex) //completed
{
	if(ex.Map.top().x == ex.Position.x)
	{
		if(abs(ex.Map.top().y - ex.Position.y) == 1) return true; //��ͬ�� ���������Ϊ1 �������� 
	}
	else if(ex.Map.top().y == ex.Position.y)
	{
		if(abs(ex.Map.top().x - ex.Position.x) == 1) return true; //��ͬ�� ���������Ϊ1 �������� 
	}
	return false; 
} 

void update_Motor() //todo  ��Ҫ��������ȸ��� 
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
			if(judge_Arrive(MotorVector[i])) //����ڹ��̵㸽��
			{
				if(!MotorVector[i].Map.top().isRes) //�����ɵĵ��ǹ˿� 
				{
					if(MotorVector[i].Map.top().deadline >= _Time) //��ʱ�ϵ�
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
			else move_Motor(MotorVector[i]); //�ڹ����� ����λ�� 
		}
		else if(MotorVector[i].Position != initPoint) MotorVector[i].Map.push(initPoint); //��Ŀ�� ��������� 
		//else ��Ŀ�� ������� ���� 
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

void dfs_Path(int have, int need, vector<bool> visRes, int dis) //���е���� ��Ҫ����� �������� ����·��
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
	vector<bool> visRes(nowOrder.size() , true); //����һ��size��С��true���� ���ڱ����Ƿ�֮ǰ���ʹ���Ӧ���̼�
	//vector<Point> sv;
	//dfs_Path(0, nowOrder.size(), visRes, 0);

	/*
	�˴�dfsδ��� ��ֱ�ӷ���ԭʼ�� ���Ž�
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
	vector<Point> nowOrder_vec; //����dfs�Ľṹ������
	stack<Point> sv_nowOrder; //���洫���orderջ
	int sv_Map_size = ex.Map.size();

	while (!nowOrder.empty()) //��stackת��Ϊvector �������
	{
		nowOrder_vec.push_back(nowOrder.top());
		sv_nowOrder.push(nowOrder.top());
		nowOrder.pop();
	}
	
	search_Path(ex, nowOrder_vec); //׼������

	while (!sv_nowOrder.empty()) //��stack���ݻ�Ը
	{
		nowOrder.push(sv_nowOrder.top());
		sv_nowOrder.pop();
	}
	
	if (ex.Map.size() > sv_Map_size) return true; //�������������Map���� ������ɹ�
	//else
	return false;
} 

void merge_Order(Motor& ex)
{
	stack<Point> nowOrder; //����Ҫȥ�ĵ�Һ͹˿͵�λ�� 
	stack<Data> sv; //������������ ʧ��ʱpush�� 
	int quantity = 0; //��¼��ӵĶ������� 
	
	while (!DataStack.empty()) //�����ж��Ƿ�������µĶ��� 
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
			DataStack.push(sv.top()); //��ԭ��һ��pop��Ԫ��
			break; //�½�return
		} 
	}
	
	_GetOrder += quantity;
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
	for (int i = 0; i < _MotorQuantity; i++)
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
	initPoint = Point{0,0};
	_Money = initMoney;
	while (infile >> tmp.Number) //����
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

