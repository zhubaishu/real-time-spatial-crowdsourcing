#ifndef Brute_Force_H
#define Brute_Force_H

#include <iostream>
#include <cmath>
#include <vector>
#include <unordered_map>
#include "Class.h"

using namespace std;

struct Best_assign {
	int id;
	int dis;
	vector<int> taskset;
	Best_assign() : id(-1) {}
};

int overall_dis(vector<int>& route, int wid) {
	int sum = 0;
	int init_x = workers[wid].cur_loc.x;
	int init_y = workers[wid].cur_loc.y;
	for ( int i = 0; i < route.size(); i++ ) {
		if ( !i ) sum += abs(tasks[route[i]].loc.x - init_x) + abs(tasks[route[i]].loc.y - init_y);
		else {
			int x1 = tasks[route[i-1]].loc.x;
			int y1 = tasks[route[i-1]].loc.y;
			int x2 = tasks[route[i]].loc.x;
			int y2 = tasks[route[i]].loc.y;
			sum += abs(x1-x2) + abs(y1-y2);
		}
	}
	return sum;
}

int minT;
vector<int> new_route;

void findRoute(vector<int>& route, unordered_map<int,bool>& contain, vector<int>& tmp, int T) {
	if ( tmp.size() == route.size() ) {
		if ( T < minT ) {
			minT = T;
			new_route = tmp;
		}
		return;
	}
	for ( int i = 0; i < route.size(); i++ ) {
		int x = tasks[tmp.back()].loc.x;
		int y = tasks[tmp.back()].loc.y;
		if ( !contain[route[i]] ) {
			int xx = tasks[route[i]].loc.x;
			int yy = tasks[route[i]].loc.y;
			int TT = T + abs(x-xx) + abs(y-yy);
			int eT = tasks[route[i]].launchT + tasks[route[i]].expiredT;
			if ( TT <= eT ) {
				tmp.push_back(route[i]);
				contain[route[i]] = true;
				findRoute(route,contain,tmp,TT);
				contain[route[i]] = false;
				tmp.pop_back();
			}
		}
	}
}

vector<int> best_route(int tid, int wid) {
	int init_x = workers[wid].cur_loc.x;
	int init_y = workers[wid].cur_loc.y;
	vector<int>& route = workers[wid].taskset;
	route.push_back(tid);
	new_route.clear();
	minT = INT_MAX;
	unordered_map<int,bool> contain;
	for ( int i = 0; i < route.size(); i++ )
		contain[route[i]] = false;
	for ( int i = 0; i < route.size(); i++ ) {
		int x = tasks[route[i]].loc.x;
		int y = tasks[route[i]].loc.y;
		int eT = tasks[route[i]].launchT + tasks[route[i]].expiredT;
		int T = GlobalTime + abs(x-init_x) + abs(y-init_y);
		if ( T <= eT ) {
			vector<int> tmp;
			tmp.push_back(route[i]);
			contain[route[i]] = true;
			findRoute(route,contain,tmp,T);
			contain[route[i]] = false;
		}
	}
	route.pop_back();
	return new_route;
}

void brute_assign(vector<int>& candidate, int tid) {
	int min = INT_MAX;
	Best_assign assign = Best_assign();
	for ( int i = 0; i < candidate.size(); i++ ) {
		vector<int> tmp = best_route(tid,candidate[i]);
		if ( tmp.empty() ) continue;
		int dis = overall_dis(tmp,candidate[i]);
		int delta_dis =  dis - workers[candidate[i]].dis;
		if ( delta_dis < min ) {
			min = delta_dis;
			assign.id = candidate[i];
			assign.taskset = tmp;
			assign.dis = dis;
		} 
	}
	if ( assign.id == -1 ) FinishTaskNum++;
	else {
		workers[assign.id].taskset = assign.taskset;
		workers[assign.id].dis = assign.dis;
	}
	return;
}

#endif 