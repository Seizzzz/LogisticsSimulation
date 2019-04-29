#ifndef DFSPATH_H
#define DFSPATH_H

#include "main.h"
#define MaxBurden 5

int Distance[10][10];
struct Point
{
	int number;
	bool isRes;
	int ResNum; //need if not res
	int x;
	int y;
};
Point Pathtmp[2*MaxBurden];
Point Path[2*MaxBurden];

inline int Cal_Distance(Point A, Point B);
int Cal_ShortestDistance_DFS(int now, int which, int have, int need, bool* vis, int& ans, Point* ToSearch);
int Cal_ShortestDistance(Point* ToSearch,int Quantity);

#endif
