
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <vector>
#include <unordered_map>
#include <ctime>
#include "WorkerRTreeUtils.h"
//#include "Brute_Force.h"
#include "Branch_and_Bound.h"

using namespace std;

unordered_map< int,vector<int> > instance;

vector<Rectd> rects;
RTree<Worker, double, 2, float> tree;

void loadWorker(string filename) {
	freopen(filename.c_str(),"r",stdin);
	int x, y;
	while( cin >> x >> y ) {
		Worker a = Worker(Location(x,y));
		Rectd A(a.init_loc.x-a.R,a.init_loc.y-a.R,a.init_loc.x+a.R,a.init_loc.y+a.R,a);
		rects.push_back(A);
		a.id = workers.size();
		workers.push_back(a);
	}
	fclose(stdin);
}

void loadTask(string filename) {
	freopen(filename.c_str(),"r",stdin);
	int x, y, t;
	while ( cin >> x >> y >> t ) {
		Task tmp = Task(Location(x,y),t);
		if ( instance[t].empty() ) {
			vector<int> ttmp;
			instance[t] = ttmp;
		}
		instance[t].push_back(tasks.size());
		tasks.push_back(tmp);
	}
	fclose(stdin);
}

void assignTask(int id) {
	vector<int> workerCandidate;
	int x = tasks[id].loc.x;
	int y = tasks[id].loc.y;
	int r = tasks[id].expiredT;
	Rectd s(x-r,y-r,x+r,y+r,Worker(Location(0,0)));
	SearchRTree(s,tree);
	for ( int i = 0; i < searchRTreeResult.size(); i++ ) {
		int wid = searchRTreeResult[i].id;
		if ( workers[wid].taskset.size() == workers[wid].MaxTaskNum ) continue;
		int ix = workers[wid].init_loc.x;
		int iy = workers[wid].init_loc.y;
		int cx = workers[wid].cur_loc.x;
		int cy = workers[wid].cur_loc.y;
		int wr = workers[wid].R;
		if ( x >= ix - wr && x <= ix + wr && y >= iy - wr && y <= iy + wr )
			if ( abs(x-cx) + abs(y-cy) <= r )
				workerCandidate.push_back(wid);
	}
	if ( workerCandidate.empty() ) {
		FinishTaskNum++;
		return;
	}
	clock_t start, finish;
	start = clock();
	//brute_assign(workerCandidate,id);
	bnb_assign(workerCandidate,id);
	finish = clock();
	EXECUTION_TIME += (double)(finish-start);
	return;
}

void workerMove(int id) { // right & left first, up & down later
	int taskid = workers[id].taskset[0];
	if ( workers[id].cur_loc.x != tasks[taskid].loc.x ) {
		if ( workers[id].cur_loc.x > tasks[taskid].loc.x )
			(workers[id].cur_loc.x)--;
		else (workers[id].cur_loc.x)++;
	}else {
		if ( workers[id].cur_loc.y > tasks[taskid].loc.y )
			(workers[id].cur_loc.y)--;
		else (workers[id].cur_loc.y)++;
	}
	for ( int i = 0; i < workers[id].taskset.size(); i++ ) {
		int tid = workers[id].taskset[i];
		if ( workers[id].cur_loc.x == tasks[tid].loc.x && workers[id].cur_loc.y == tasks[tid].loc.y ) {
				FinishTaskNum++;
				workers[id].taskset.erase(workers[id].taskset.begin());
		}else break;
	}
}

void simulate() {
	while (1) {
		if ( FinishTaskNum == tasks.size() ) break;
		if ( !instance[GlobalTime].empty() ) {
			for ( int i = 0; i < instance[GlobalTime].size(); i++ ) {
				assignTask(instance[GlobalTime][i]);
			}
		}
		for ( int i = 0; i < workers.size(); i++ ) {
			if ( !workers[i].taskset.empty() ) {
				workerMove(i);
			}
		}
		GlobalTime++;
	}
	cout << EXECUTION_TIME << endl;
	return;
}

int main() {
	loadWorker("worker.txt");
	CreateRTree(rects,tree);
	loadTask("task.txt");
	simulate();
	return 0;
}