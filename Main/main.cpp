#include "main.h"
#include <windows.h>
#include "graphics.h"
using namespace std;

struct Point; //Point�ṹ�� ��¼����λ�� 
struct Data; //Data�ṹ�� ��¼���� 
struct Motor; //Motor�ṹ�� ��¼���� 
void output(); //������� 
void init(); //��ʼ������ 
void update_Motor(); //�������ֺ��� 
void purchase_Motor(); //�������ֺ��� 
void deal_DataStack(); //���������� 
bool judge_Overtime(); //�жϳ�ʱ���� 
bool judge_MotorMoving(); //�ж��ƶ����� 
int exist_Motor(); //���ҿ������ֺ��� 
int cal_Distance(const Point & A, const Point & B); //����Point�ṹ������ 
void dfs_Path(int have, int need, int nowAt, int dis, vector<Point> & nowOrder); //����������� 
void search_Path(Motor & ex, vector<Point> & nowOrder, int quantity); //����·������ 
void move_Motor(Motor & who); //�ƶ����ֺ��� 

ostream& operator << (ostream& out, const Motor& ex); //����<< �������������Ϣ 
ostream& operator << (ostream& out, const Data& ex); //����<< �������������Ϣ 
bool operator == (const Point& left, const Point& right); //����== �ж�ͬλ�õ� 
bool operator != (const Point & left, const Point & right); //����== �ж�ͬλ�õ� 
template<typename T> void reverse_Stack(stack<T> & s); //��תջ 

#include "realize.h"
#include "animate.h"
#include "in_out.h"

int main()
{
	std::ios::sync_with_stdio(false);
	srand((unsigned)time(NULL)); //��ʼ������� 
	intest >> PreMergeTime >> EndMergeTime; //��������ֵ 
	
	init(); //��������graph��ȫ�ֱ����� 
	ani_init(); //��ʼ���������� 
	
	while (_Money >= 0) //��δ�Ʋ�ʱѭ�� 
	{
		Sleep(DelayTime); //������ʱ 
		//if (DataStack.empty() && !judge_MotorMoving()) break; //�ж��Ƿ��������ж��� 
		
		update_Motor(); //�������� 
		ani_update(); //�������� 
		thread ani(ani_input); //�������������߳� 
		ani.join(); //�ȴ����̽��� 

		while (!DataStack.empty() && _Time >= DataStack.top().OrderTime) //��ǰʱ�估ǰ ����δ����Ķ���
		{
			if (exist_Motor() == -1) //��������ڿ������� 
			{
				if (_Money - PreserveMoney >= MotorPrice) //�����ǰ��ȥԤ��Ǯ����ܹ������� 
				{
					while(_Money - PreserveMoney >= MotorPrice) purchase_Motor(); //��һֱ�� 
					deal_DataStack(); //��������� 
				} 
				else break; //Ǯ���� ����һʱ��ڵ� 
			}
			else deal_DataStack(); //����������� ����䶩�� 
		}
		
		/*
		if (!DataStack.empty() && judge_Overtime()) //�ж��Ƿ񶩵���ʱδ���� 
		{
			DataStack.pop(); 
			exit(1); //return 1 ����Ӫҵִ��
		}
		*/

		output(); //��� 
		_Time++; //ʱ�䵥λ���� 
	}

	if (_Money < 0) exit(2); //�Ʋ��˳� ����2 
	
	return 0;
}
