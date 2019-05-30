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

void dfs_Path(int have, int need, int nowAt, int dis, vector<Point> & nowOrder) //���е���� ��Ҫ����� �������� ����·��
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

void search_Path(Motor & ex, vector<Point> & nowOrder, int quantity) //todo
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

	if (sv_Dis_Min > EndMergeTime) return; //�����Ӻ�·������ 
	//else
	while (!ex.Map.empty()) ex.Map.pop();
	for (int i = 1; i <= quantity; i++) ex.Map.push(nowOrder[sv_Path_Min[i]]);
	return;
}

bool able_Order(Motor & ex, stack<Point> & nowOrder) //todo
{
	vector<Point> nowOrder_vec; //����dfs�Ľṹ������
	stack<Point> sv_nowOrder; //���洫���orderջ
	//int sv_Map_size = ex.Map.size();
	int quantity = nowOrder.size();

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

void merge_Order(Motor & ex)
{
	stack<Point> nowOrder; //����Ҫȥ�ĵ�Һ͹˿͵�λ�� 
	stack<Data> sv; //������������ ʧ��ʱpush�� 
	int quantity = 0; //��¼��ӵĶ������� 

	while (!DataStack.empty()) //�����ж��Ƿ�������µĶ��� 
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
