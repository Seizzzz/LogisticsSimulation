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

	while (_Money >= 0) //�� 
	{
		//if(_CompleteOrder + _OverTimeOrder == _TotalOrder) exit(0);
		if(DataStack.empty() && !judge_MotorMoving()) break;
		update_Motor();
		
		while(!DataStack.empty() && _Time >= DataStack.top().OrderTime) //��ǰʱ�估ǰ ����δ����Ķ���
		{
			if(exist_Motor() == -1) //��������ڿ������� 
			{
				if(_Money >= MotorPrice)
				{
					purchase_Motor();
					deal_DataStack();
				}
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
		//��������������ҷ��ص������ ����ʹ��  ��˴����λ��ȫ�ֽ��� 
		if(MotorVector[i].enable && //�����ѹ��� 
		   MotorVector[i].Map.empty() && //���ֵ�ǰ������ 
		   MotorVector[i].Position == initPoint) return i; //���ؿ������ֱ��
		    
		//���ʹ��δ������㵫����������� 
		if(MotorVector[i].enable && //�����ѹ��� 
		   MotorVector[i].Map.size() == 1 && //���ֵ�ǰĿ��ֻʣһ������ 
		   MotorVector[i].Map.top() == initPoint) //��ǰֻ�践����� 
			{
				MotorVector[i].Map.pop(); //ȡ���������ļƻ� ׼����������ָ�� 
				return i;
			}
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
			who.Position.y += 2*j;
		else if (abs(who.Map.top().y - who.Position.y) == 1) //�з����ϵ�λ
			who.Position.x += 2*i;
	}

	return;
}

bool judge_Arrive(Motor& ex) //completed
{
	if(ex.Map.size() == 1 && ex.Map.top() == initPoint) //���ʼ�㲻�ڵ����λ�� �˴��赥�������ж� 
	{
		if(abs(ex.Position.x - initPoint.x) == 1 && abs(ex.Position.y - initPoint.y) == 1)
		{
			ex.Position = initPoint; //�ص���ʼ�� 
			while(!ex.Map.empty()) ex.Map.pop(); //��� 
			return true;
		}
		return false;
	} 
	
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
		/*test
		stack<Point> tmp;
		if(MotorVector[i].Map.empty()) cout << "����" << i << "Ϊ��";
		else
		{
			cout << "����" << i << endl;
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
		
		if(!MotorVector[i].Map.empty()) //��ǰ�����н� 
		{
			if(judge_Arrive(MotorVector[i])) //����ڹ��̵㸽��
			{
				if(MotorVector[i].Map.empty()) continue; //��Ӧ�ص���ʼ����� 
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
	return abs((A.x - B.x) >> 1) + abs((A.y - B.y) >> 1);
}

vector<vector<int>> Distance(MaxBurden, vector<int>(MaxBurden, 0));
vector<bool> vis(MaxBurden);
vector<int> sv_Path(MaxBurden);
vector<int> sv_Path_Min(MaxBurden);
int sv_Dis_Min = INF;

void dfs_Path(int have, int need, int nowAt, int dis, vector<Point>& nowOrder) //���е���� ��Ҫ����� �������� ����·��
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
		if (vis[i]) //�����û���� 
		{
			if (!nowOrder[i].isRes) //���ûȥ���Ҹõ��ǹ˿�
			{
				if (vis[i + 1] == false) //����������Ӧ���̼�
				{
					vis[i] = false;
					dfs_Path(have + 1, need, i, dis + Distance[nowAt][i], nowOrder);
					vis[i] = true;
				}
			}
			else //�õ��ǵ��
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
	//��ʼ�� 

	for (int i = 0; i <= quantity; i++)
	{
		Distance[i][i] = 0;
		for (int j = i + 1; j <= quantity; j++)
		{
			Distance[i][j] = cal_Distance(nowOrder[i], nowOrder[j]);
			Distance[j][i] = Distance[i][j];
		}
	}
	//������� 

	for (int i = 1; i <= quantity; i++)
	{
		if (nowOrder[i].isRes) //ѡ��һ���̼ҵĵ㿪ʼ 
		{
			vis[i] = false;
			dfs_Path(1, quantity, i, Distance[0][i], nowOrder);
			vis[i] = true;
		}
	}
	
	if(sv_Dis_Min > EndMergeTime) return; //�����Ӻ�·������ 
	//else
	while (!ex.Map.empty()) ex.Map.pop();
	for (int i = 1; i <= quantity; i++) ex.Map.push(nowOrder[sv_Path_Min[i]]);
	return;
}

bool able_Order(Motor& ex, stack<Point>& nowOrder) //todo
{
	vector<Point> nowOrder_vec; //����dfs�Ľṹ������
	stack<Point> sv_nowOrder; //���洫���orderջ
	//int sv_Map_size = ex.Map.size();
	int quantity=nowOrder.size();
	
	nowOrder_vec.push_back(ex.Position);
	while (!nowOrder.empty()) //��stackת��Ϊvector �������
	{
		nowOrder_vec.push_back(nowOrder.top());
		sv_nowOrder.push(nowOrder.top());
		nowOrder.pop();
	}
	
	search_Path(ex, nowOrder_vec, quantity); //׼������

	while (!sv_nowOrder.empty()) //��stack���ݻ�Ը
	{
		nowOrder.push(sv_nowOrder.top());
		sv_nowOrder.pop();
	}
	
	//reverse_Stack(ex.Map); //������㷴ת 
	if (sv_Dis_Min <= EndMergeTime) return true; //��������ʱ������ ������ɹ�
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
				DataStack.push(sv.top()); //��ԭ��һ��pop��Ԫ��
				break; //�½�return
			} 
		}
		else break;
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
	MotorVector[_MotorQuantity].number = _MotorQuantity;
	MotorVector[_MotorQuantity].enable = true;
	MotorVector[_MotorQuantity].Position = initPoint;
	_MotorQuantity++;
	return;
}

void output() //completed
{
	outfile << "ʱ��: " << _Time << endl;
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
	if(initPoint.x % 2){ if(initPoint.y % 2) initPoint.x++; } //x,yͬΪ���� 
	else{ if(initPoint.y % 2 == 0) initPoint.y++; } //x,yͬΪż�� 
	
	reverse_Stack(DataStack);
	return;
}
