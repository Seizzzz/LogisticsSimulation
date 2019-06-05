#pragma once

#define DealLength 400
#define Tmpsize 20

PIMAGE img[4];
mouse_msg msg = {0};

void ani_init()
{
	img[0]=newimage();
	img[1]=newimage();
	img[2]=newimage();
	img[3]=newimage();
	getimage(img[0],"p_money.jpg",0,0);
	getimage(img[1],"p_order.jpg",0,0);
	getimage(img[2],"p_over.jpg",0,0);
	getimage(img[3],"p_time.jpg",0,0);
	setfont(50,0,"微软雅黑");
	return;
}

void ani_update()
{
	char tmp[Tmpsize];
	putimage(MapLength * MapSize + 20, 50, img[0]); //钱 
	sprintf(tmp,"%d",_Money);
	outtextxy(MapLength * MapSize + 150, 80, tmp);
	putimage(MapLength * MapSize, 150, img[1]); //订单数 
	sprintf(tmp,"%d",_CompleteOrder);
	outtextxy(MapLength * MapSize + 150, 180, tmp);
	putimage(MapLength * MapSize, 250, img[2]); //超时数 
	sprintf(tmp,"%d",_OverTimeOrder);
	outtextxy(MapLength * MapSize + 150, 280, tmp);
	putimage(MapLength * MapSize, 350, img[3]); //时间 
	sprintf(tmp,"%d",_Time);
	outtextxy(MapLength * MapSize + 150, 380, tmp);
	return;
}

void ani_input()
{
	while(mousemsg()) msg = getmouse();

	if(msg.is_down())
	{
		int st_x=msg.x,st_y=msg.y;
		while(msg.is_down()) msg = getmouse();
		while(!msg.is_down()) msg = getmouse();
		int ed_x=msg.x,ed_y=msg.y;
		
		Data tmp;
		
		tmp.Number = _TotalOrder + 600;
		tmp.OrderTime = _Time;
		tmp.Restaurant.x = ed_x / MapLength;
		tmp.Restaurant.y = ed_y / MapLength;
		tmp.Customer.x = st_x / MapLength;
		tmp.Customer.y = st_y / MapLength;
		
		DataStack.push(tmp);
	}
}
