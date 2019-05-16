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
		//��������������ҷ��ص������ ����ʹ��  ��˴����λ��ȫ�ֽ��� 
		if (MotorVector[i].enable && //�����ѹ��� 
			MotorVector[i].Map.empty() && //���ֵ�ǰ������ 
			MotorVector[i].Position == initPoint) return i; //���ؿ������ֱ��

		 //���ʹ��δ������㵫����������� 
		if (MotorVector[i].enable && //�����ѹ��� 
			MotorVector[i].Map.size() == 1 && //���ֵ�ǰĿ��ֻʣһ������ 
			MotorVector[i].Map.top() == initPoint) //��ǰֻ�践����� 
		{
			MotorVector[i].Map.pop(); //ȡ���������ļƻ� ׼����������ָ�� 
			return i;
		}
	}
	return -1; //δ�ҵ� 
}

void move_Motor(Motor & who) //completed
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
			who.Position.y += 2 * j;
		else if (abs(who.Map.top().y - who.Position.y) == 1) //�з����ϵ�λ
			who.Position.x += 2 * i;
	}

	return;
}

bool judge_Arrive(Motor & ex) //completed
{
	if (ex.Map.size() == 1 && ex.Map.top() == initPoint) //���ʼ�㲻�ڵ����λ�� �˴��赥�������ж� 
	{
		if (abs(ex.Position.x - initPoint.x) == 1 && abs(ex.Position.y - initPoint.y) == 1)
		{
			ex.Position = initPoint; //�ص���ʼ�� 
			while (!ex.Map.empty()) ex.Map.pop(); //��� 
			return true;
		}
		return false;
	}

	if (ex.Map.top().x == ex.Position.x)
	{
		if (abs(ex.Map.top().y - ex.Position.y) == 1) return true; //��ͬ�� ���������Ϊ1 �������� 
	}
	else if (ex.Map.top().y == ex.Position.y)
	{
		if (abs(ex.Map.top().x - ex.Position.x) == 1) return true; //��ͬ�� ���������Ϊ1 �������� 
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
	cleardevice();
	setfillcolor(EGERGB(0xFF, 0xFF, 0x0));
	for (int x = 0; x < MapLength * MapSize; x += 2 * MapLength)
		for (int y = 0; y < MapLength * MapSize; y += 2 * MapLength)
			bar(x, y, x + MapLength, y + MapLength);
	setfillcolor(EGERGB(0xFF, 0x0, 0x0));
	for (int i = 0; i < _MotorQuantity; i++)
		fillellipse(20 + MapLength * MotorVector[i].Position.x, 20 + MapLength * MotorVector[i].Position.y, Radius, Radius);

	for (int i = 0; i < _MotorQuantity; i++) //����ÿһ������ 
	{
		if (!MotorVector[i].Map.empty()) //��ǰ�����н� 
		{
			if (judge_Arrive(MotorVector[i])) //����ڹ��̵㸽��
			{
				if (MotorVector[i].Map.empty()) continue; //��Ӧ�ص���ʼ����� 
				if (!MotorVector[i].Map.top().isRes) //�����ɵĵ��ǹ˿� 
				{
					if (MotorVector[i].Map.top().deadline >= _Time) //��ʱ�ϵ�
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
			else move_Motor(MotorVector[i]); //�ڹ����� ����λ�� 
		}
		else if (MotorVector[i].Position != initPoint) MotorVector[i].Map.push(initPoint); //��Ŀ�� ��������� 
		//else ��Ŀ�� ������� ���� 
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
