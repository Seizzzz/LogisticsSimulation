#include <bits/stdc++.h>
#define INF 0x7fffffff
using namespace std;

int Distance[10][10];
struct Point
{
	int number;
	int x;
	int y;
};
//required

inline int Cal_Distance(Point A, Point B)
{
	return abs(A.x - B.x) + abs(A.y - B.y);
}

void Cal_ShortestDistance_DFS(int now, int which, int have, int need, bool* vis, int& ans)
{
	if (now >= ans) return;
	if (have == need)
	{
		ans = now;
		return;
	}

	for (int i = 1; i <= need; i++)
	{
		if (vis[i])
		{
			vis[i] = false;
			Cal_ShortestDistance_DFS(now + Distance[which][i], i, have + 1, need, vis, ans);
			vis[i] = true;
		}
	}
}

int Cal_ShortestDistance(Point* ToSearch,int Quantity)
{
	int ans = INF;
	bool vis[Quantity+1];
	for (int i = 0; i <= Quantity; i++) vis[i] = true;

	for (int i = 1; i <= Quantity; i++)
	{
		vis[i] = false;
		Cal_ShortestDistance_DFS(Distance[ToSearch[0].number][ToSearch[i].number], i, 1, Quantity, vis, ans);
		vis[i] = true;
	}

	return ans;
}

void example()
{
	int n;
	cin >> n;

	Point ToSearch[n+1];
	ToSearch[0].number = 0;
	ToSearch[0].x = 0;
	ToSearch[0].y = 0;
	for (int i = 1; i <= n; i++)
	{
		ToSearch[i].number = i;
		cin >> ToSearch[i].x >> ToSearch[i].y;
	}

	for (int i = 1; i <= n; i++)
	{
		for (int j = i + 1; j <= n; j++)
		{
			Distance[i][j] = Cal_Distance(ToSearch[i], ToSearch[j]);
			Distance[j][i] = Distance[i][j];
			Distance[i][i] = 0;
		}
	}

	cout << Cal_ShortestDistance(ToSearch, n) << endl;

	return;
}


int main()
{
	example();
	return 0;
}
