#pragma once

#define DealLength 300
#define Tmpsize 20

PIMAGE img[5]; //ͼƬ���� 
mouse_msg msg = {0};

void ani_init()
{
	img[0]=newimage(); //Ǯ��ͼƬ 
	img[1]=newimage(); //����ͼƬ 
	img[2]=newimage(); //��ʱͼƬ 
	img[3]=newimage(); //ʱ��ͼƬ 
	img[4]=newimage(); //�˳�ͼƬ 
	getimage(img[0],"p_money.jpg",0,0);
	getimage(img[1],"p_order.jpg",0,0);
	getimage(img[2],"p_over.jpg",0,0);
	getimage(img[3],"p_time.jpg",0,0);
	getimage(img[4],"p_exit.jpg",0,0);
	setfont(50,0,"΢���ź�"); //������������С 
	return;
}

void ani_update()
{
	char tmp[Tmpsize]; //��ʱ�洢������� 
	putimage(MapLength * MapSize + 50, 50, img[0]); //Ǯ 
	sprintf(tmp,"%d",_Money);
	outtextxy(MapLength * MapSize + 170, 80, tmp);
	putimage(MapLength * MapSize + 30, 150, img[1]); //������ 
	sprintf(tmp,"%d",_CompleteOrder);
	outtextxy(MapLength * MapSize + 170, 180, tmp);
	putimage(MapLength * MapSize + 30, 250, img[2]); //��ʱ�� 
	sprintf(tmp,"%d",_OverTimeOrder);
	outtextxy(MapLength * MapSize + 170, 280, tmp);
	putimage(MapLength * MapSize + 30, 350, img[3]); //ʱ�� 
	sprintf(tmp,"%d",_Time);
	outtextxy(MapLength * MapSize + 170, 380, tmp);
	
	putimage(MapLength * MapSize + 180, 550, img[4]); //�˳���ť
	return;
}

void ani_input()
{
	while(mousemsg()) msg = getmouse(); //��ȡ�����Ϣ 

	if(msg.is_down()) //�ж���갴�� 
	{
		char temp[30]; //��ʱ�洢������� 
		int st_x=msg.x,st_y=msg.y; //�洢��ʼ�����λ�� 
		
		if(st_x > MapLength * MapSize + 180 && st_y > 550) exit(0); //�ж��˳���ť 
		
		setfont(30,0,"΢���ź�"); //���������С
		
		//�����ȡ����ʼλ�� 
		sprintf(temp,"�ѻ�ȡ�͹�(%d,%d)",st_x / MapLength,st_y / MapLength);
		outtextxy(MapLength * MapSize + 20, 500 , temp);
		setfillcolor(EGERGB(0x0, 0x0, 0xFF));
		//�������λ�� 
		bar((st_x / MapLength) * MapLength,(st_y / MapLength) * MapLength
			,(st_x / MapLength) * MapLength + MapLength,(st_y / MapLength) * MapLength + MapLength);
		
		//�жϵڶ��ε�� 
		while(msg.is_down()) msg = getmouse();
		while(!msg.is_down()) msg = getmouse();
		
		//�����ȡ���յ�λ�� 
		int ed_x=msg.x,ed_y=msg.y;
		sprintf(temp,"�ѻ�ȡʳ��(%d,%d)",ed_x / MapLength,ed_y / MapLength);
		outtextxy(MapLength * MapSize + 20, 550, temp);
		
		//�����յ�λ�� 
		bar((ed_x / MapLength) * MapLength,(ed_y / MapLength) * MapLength
			,(ed_x / MapLength) * MapLength + MapLength,(ed_y / MapLength) * MapLength + MapLength);
		
		Data tmp; //��ʱ�洢������Ϣ 
		tmp.Number = _TotalOrder + 600; //��¼������� 
		sprintf(temp,"�Ѵ�������%d",tmp.Number); //֪ͨ��ɶ������� 
		outtextxy(MapLength * MapSize + 20, 600, temp);
		Sleep(250); //���ڹ۲� 
		setfont(50,0,"΢���ź�");
		
		++_TotalOrder;
		tmp.OrderTime = _Time;
		tmp.Restaurant.x = ed_x / MapLength;
		tmp.Restaurant.y = ed_y / MapLength;
		tmp.Customer.x = st_x / MapLength;
		tmp.Customer.y = st_y / MapLength;
		
		DataStack.push(tmp); //���¶���ѹ��ջ�� 
	}
}
