using namespace std;

int JudgeMotorEmpty(Motor* MotorArray)
{
	for(int i=0;i<_MotorQuantity;i++)
	{
		if(MotorArray[i].enable && MotorArray[i].time>=0) return i;
	}
	return -1;
}

void GivetoMotor(Motor* ThisMotor,int Quantity)
{
	;
}

int sch(Data* TimeTable,int nowOrder)
{
	Point nowPoint[MaxBurden];
	for(int i=0;i<MaxBurden;i++)
	{
		nowPoint[i]=TimeTable[nowOrder+i];
	}
	
	int Quantity;
	for(Quantity=1;Quantity<MaxBurden;Quantity++)
	{
		int thisTime=Cal_ShortestDistance(nowPoint,Quantity);
		if(thisTime>BufTime) return Quantity-1;
	}
	return Quantity;
}
