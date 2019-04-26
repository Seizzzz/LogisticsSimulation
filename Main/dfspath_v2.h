#ifndef DFSPATH_H
#define DFSPATH_H

#define MaxOrder 999
#define MaxBurden 5
#define MapSize 18
#define INF 0x7fffffff


int Distance[10][10];
struct Point
{
	int number;
	bool isRes;
	int ResNum; //need if not res
	int x;
	int y;
};
Point Path[2*MaxBurden];

inline int Cal_Distance(Point A, Point B);
int Cal_ShortestDistance_DFS(int now, int which, int have, int need, bool* vis, int& ans, Point* ToSearch);
int Cal_ShortestDistance(Point* ToSearch,int Quantity);

#endif
