#pragma once

#define DealLength 300
#define Tmpsize 20

PIMAGE img[5]; //图片数组 
mouse_msg msg = {0};

void ani_init()
{
	img[0]=newimage(); //钱包图片 
	img[1]=newimage(); //订单图片 
	img[2]=newimage(); //超时图片 
	img[3]=newimage(); //时间图片 
	img[4]=newimage(); //退出图片 
	getimage(img[0],"p_money.jpg",0,0);
	getimage(img[1],"p_order.jpg",0,0);
	getimage(img[2],"p_over.jpg",0,0);
	getimage(img[3],"p_time.jpg",0,0);
	getimage(img[4],"p_exit.jpg",0,0);
	setfont(50,0,"微软雅黑"); //设置输出字体大小 
	return;
}

void ani_update()
{
	char tmp[Tmpsize]; //临时存储输出内容 
	putimage(MapLength * MapSize + 50, 50, img[0]); //钱 
	sprintf(tmp,"%d",_Money);
	outtextxy(MapLength * MapSize + 170, 80, tmp);
	putimage(MapLength * MapSize + 30, 150, img[1]); //订单数 
	sprintf(tmp,"%d",_CompleteOrder);
	outtextxy(MapLength * MapSize + 170, 180, tmp);
	putimage(MapLength * MapSize + 30, 250, img[2]); //超时数 
	sprintf(tmp,"%d",_OverTimeOrder);
	outtextxy(MapLength * MapSize + 170, 280, tmp);
	putimage(MapLength * MapSize + 30, 350, img[3]); //时间 
	sprintf(tmp,"%d",_Time);
	outtextxy(MapLength * MapSize + 170, 380, tmp);
	
	putimage(MapLength * MapSize + 180, 550, img[4]); //退出按钮
	return;
}

void ani_input()
{
	while(mousemsg()) msg = getmouse(); //获取鼠标信息 

	if(msg.is_down()) //判断鼠标按下 
	{
		char temp[30]; //临时存储输出内容 
		int st_x=msg.x,st_y=msg.y; //存储初始点鼠标位置 
		
		if(st_x > MapLength * MapSize + 180 && st_y > 550) exit(0); //判断退出按钮 
		
		setfont(30,0,"微软雅黑"); //设置字体大小
		
		//输出获取的起始位置 
		sprintf(temp,"已获取餐馆(%d,%d)",st_x / MapLength,st_y / MapLength);
		outtextxy(MapLength * MapSize + 20, 500 , temp);
		setfillcolor(EGERGB(0x0, 0x0, 0xFF));
		//高亮起点位置 
		bar((st_x / MapLength) * MapLength,(st_y / MapLength) * MapLength
			,(st_x / MapLength) * MapLength + MapLength,(st_y / MapLength) * MapLength + MapLength);
		
		//判断第二次点击 
		while(msg.is_down()) msg = getmouse();
		while(!msg.is_down()) msg = getmouse();
		
		//输出获取的终点位置 
		int ed_x=msg.x,ed_y=msg.y;
		sprintf(temp,"已获取食客(%d,%d)",ed_x / MapLength,ed_y / MapLength);
		outtextxy(MapLength * MapSize + 20, 550, temp);
		
		//高亮终点位置 
		bar((ed_x / MapLength) * MapLength,(ed_y / MapLength) * MapLength
			,(ed_x / MapLength) * MapLength + MapLength,(ed_y / MapLength) * MapLength + MapLength);
		
		Data tmp; //临时存储订单信息 
		tmp.Number = _TotalOrder + 600; //记录订单编号 
		sprintf(temp,"已创建订单%d",tmp.Number); //通知完成订单创建 
		outtextxy(MapLength * MapSize + 20, 600, temp);
		Sleep(250); //用于观察 
		setfont(50,0,"微软雅黑");
		
		++_TotalOrder;
		tmp.OrderTime = _Time;
		tmp.Restaurant.x = ed_x / MapLength;
		tmp.Restaurant.y = ed_y / MapLength;
		tmp.Customer.x = st_x / MapLength;
		tmp.Customer.y = st_y / MapLength;
		
		DataStack.push(tmp); //将新订单压入栈中 
	}
}
