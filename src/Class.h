#ifndef Class_H
#define Class_H

#include <vector>

#define ExpiredTime 5
#define Region 3
#define MaxNum 4
 
using namespace std;

int GlobalTime = 0;
int FinishTaskNum = 0;
double EXECUTION_TIME = 0;

class Location {
public:
	int x;
	int y;
	Location() {}
	Location(int x, int y) : x(x), y(y) {}
};

class Task {
public:
	Location loc;
	int launchT;
	int expiredT;
	Task(Location loc, int lT, int eT = ExpiredTime) : loc(loc), launchT(lT), expiredT(eT) {}
};

class Worker {
public:
	int id;
	int R;
	int dis;
	Location init_loc;
	Location cur_loc;
	bool active;
	int MaxTaskNum;
	vector<int> taskset;
	Worker() {}
	Worker(Location loc, int r = Region, int n = MaxNum) : init_loc(loc), cur_loc(loc), active(true), R(r), MaxTaskNum(n), dis(0) {}
};

vector<Task> tasks;
vector<Worker> workers;

#endif