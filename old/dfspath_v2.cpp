#include <iostream>
#include <cmath>
#include "dfspath_v2.h"
using namespace std;


//required

inline int Cal_Distance(Point A, Point B)
{
	return abs(A.x - B.x) + abs(A.y - B.y);
}

int Cal_ShortestDistance_DFS(int now, int which, int have, int need, bool* vis, int& ans, Point* ToSearch)
{
	if (now >= ans) return which;
	Pathtmp[have]=ToSearch[which];
	if (have == need)
	{
		for(int i=0;i<=need;i++) Path[i]=Pathtmp[i];
		ans = now;
		return which;
	}
	
	for (int i = 1; i <= need; i++)
	{
		if (vis[i])
		{
			if (ToSearch[i].isRes)
			{
				vis[i] = false;
				Cal_ShortestDistance_DFS(now + Distance[which][i], i, have + 1, need, vis, ans, ToSearch);
				vis[i] = true;
			}
			else //if customer, check if went to corresponding restaurant
			{
				if (vis[ToSearch[i].ResNum] == false)
				{
					vis[i] = false;
					Cal_ShortestDistance_DFS(now + Distance[which][i], i, have + 1, need, vis, ans, ToSearch);
					vis[i] = true;
				}
			}
		}
	}
}

int Cal_ShortestDistance(Point* ToSearch,int Quantity) //Quantity not conclude starting
{
	int ans = INF;
	bool vis[MapSize];
	for (int i = 0; i <= Quantity; i++) vis[i] = true;
	for (int i = 0; i <= Quantity; i++)
	{
		for (int j = i + 1; j <= Quantity; j++)
		{
			Distance[i][j] = Cal_Distance(ToSearch[i], ToSearch[j]);
			Distance[j][i] = Distance[i][j];
			Distance[i][i] = 0;
		}
	}
	
	Path[0]=ToSearch[0];
	
	for (int i = 1; i <= Quantity; i++)
	{
		if (ToSearch[i].isRes)
		{
			vis[i] = false;
			Cal_ShortestDistance_DFS(Distance[ToSearch[0].number][ToSearch[i].number], i, 1, Quantity, vis, ans, ToSearch);
			vis[i] = true;
		}
	}

	return ans;
}


void example()
{
	int n;
	cin >> n;

	Point ToSearch[MaxOrder];

	for (int i = 0; i <= n; i++)
	{
		cin >> ToSearch[i].number >> ToSearch[i].isRes >> ToSearch[i].ResNum >>ToSearch[i].x >> ToSearch[i].y;
	}
	
	
	cout << Cal_ShortestDistance(ToSearch, n) << endl;
	for(int i=0;i<=n;i++) cout << Path[i].x << " " << Path[i].y << endl;

	return;
}


int main()
{
	example();
	return 0;
}

