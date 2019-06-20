#pragma once

char text_cus[] = "ʳ��"; //��ʱ�洢 ������� 
char text_res[] = "�͹�";

bool judge_MotorMoving() //�ж������ƶ� 
{
	for (int i = 0; i < _MotorQuantity; i++) //��ĳ�����е�ǰĿ��� 
	{
		if (!MotorVector[i].Map.empty()) return true; 
	}
	return false;
}

bool judge_Overtime() //�жϳ�ʱ
{
	if (_Time > DataStack.top().OrderTime + GetOrderLimit) return true; //���ж��������ӵ�ʱ���δ���� 
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
	int i = (who.Map.top().x - who.Position.x > 0) ? 1 : -1, //��λ���� 
		j = (who.Map.top().y - who.Position.y > 0) ? 1 : -1;

	if (who.Map.top().x == who.Position.x)
	{
		if(who.Position.x == MapSize - 1) who.Position.x--;
		else who.Position.x++; //ͬ�� ������һ��
		who.Position.y += j;
	}
	else if (who.Map.top().y == who.Position.y)
	{
		if(who.Position.y == MapSize - 1) who.Position.y--;
		else who.Position.y++; //ͬ�� ������һ��
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

bool judge_Near(Motor & ex)
{
	if (ex.Map.top().x == ex.Position.x)
	{
		if (abs(ex.Map.top().y - ex.Position.y) == 1) return true; //��ͬ�� ���������Ϊ1 �������� 
	}
	else if (ex.Map.top().y == ex.Position.y)
	{
		if (abs(ex.Map.top().x - ex.Position.x) == 1) return true; //��ͬ�� ���������Ϊ1 �������� 
	}
	
	//if(ex.Map.top().x == ex.Position.x && ex.Map.top().y - ex.Position.y) return true; //ĳЩ����²������� 
	
	return false;
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
	cleardevice(); //��յ�ǰ�������� 
	
	//��ӡ��ͼ 
	setfillcolor(EGERGB(0xFF, 0xFF, 0x0));
	for (int x = 0; x < MapLength * MapSize; x += 2 * MapLength)
		for (int y = 0; y < MapLength * MapSize; y += 2 * MapLength)
			bar(x, y, x + MapLength, y + MapLength);
	//��ӡ���� 
	setfillcolor(EGERGB(0xFF, 0x0, 0x0));
	for (int i = 0; i < _MotorQuantity; i++)  
	{
		setfillcolor(MotorVector[i].color);
		fillellipse(20 + MapLength * MotorVector[i].Position.x, 20 + MapLength * MotorVector[i].Position.y, Radius, Radius); 
	}
	//��ӡĿ��� 
	setfillcolor(EGERGB(0x0, 0xFF, 0x0));
	for (int i = 0; i < _MotorQuantity; i++)
	{
		if(!MotorVector[i].Map.empty())
		{
			int tmpx = MapLength * MotorVector[i].Map.top().x,
				tmpy = MapLength * MotorVector[i].Map.top().y;
			bar(tmpx, tmpy, tmpx + MapLength, tmpy + MapLength);
			
			if(MotorVector[i].Map.top()==initPoint) continue; //����ǳ�ʼ�� ����ʾ��Ϣ 
			
			//��ÿ��Ŀ��� ��������������Ϣ 
			setfont(20,0,"΢���ź�");
			char temp[30];
			sprintf(temp,"������:%d",MotorVector[i].Map.top().Number);
			
			if(MotorVector[i].Map.top().isRes) outtextxy(tmpx+40,tmpy,text_res);
			else outtextxy(tmpx+40,tmpy,text_cus);
			outtextxy(tmpx+40,tmpy+20,temp);
			setfont(50,0,"΢���ź�");
		}
	}

	for (int i = 0; i < _MotorQuantity; i++) //����ÿһ������ 
	{
		if (!MotorVector[i].Map.empty()) //��ǰ�����н� 
		{
			if (judge_Arrive(MotorVector[i])) //����ڹ��̵㸽��
			{
				if (MotorVector[i].Map.empty()) continue; //��Ӧ�ص���ʼ����� 
				if (MotorVector[i].Map.top().isRes && MotorVector[i].Map.top().begin > _Time) continue; //��Ӧδ����ӵ�ʱ����� 
				if (!MotorVector[i].Map.top().isRes) //�����ɵĵ��ǹ˿� 
				{
					if (MotorVector[i].Map.top().deadline >= _Time) //��ʱ�ϵ�
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
				MotorVector[i].Map.pop(); //��ǰĿ����ջ 
			}
			else move_Motor(MotorVector[i]); //�ڹ����� ����λ�� 
		}
		else if (MotorVector[i].Position != initPoint) MotorVector[i].Map.push(initPoint); //��Ŀ�� ��������� 
		//else ��Ŀ�� ������� ���� 
	}
	return;
}

void purchase_Motor() //�������� 
{
	_Money -= MotorPrice; //�۳����� 
	MotorVector[_MotorQuantity].number = _MotorQuantity; //����������� 
	MotorVector[_MotorQuantity].enable = true; //����ѹ��� 
	MotorVector[_MotorQuantity].Position = initPoint; //�ƶ�����ʼλ�� 
	MotorVector[_MotorQuantity].color = (rand() * 65530) % 0xFFFFFF; //������ɫ���ڻ��� 
	_MotorQuantity++; //ȫ������������ 
	return;
}
