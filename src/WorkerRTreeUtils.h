#ifndef WorkerRTree_H
#define WorkerRTree_H

#include <stdio.h>
#include <vector>
#include <cstring>
#include "RTree.h"
#include "Class.h"

using namespace std;

struct Rectd {
	Rectd() {
	}
	Rectd(double a_minX, double a_minY, double a_maxX, double a_maxY, Worker v) {
		min[0] = a_minX;
		min[1] = a_minY;
		max[0] = a_maxX;
		max[1] = a_maxY;
		vid = v;
	}
	double min[2];
	double max[2];
	Worker vid;
};

RTree<Worker, double, 2, float>::Iterator it;
vector<Worker> searchRTreeResult;

bool MySearchCallback(Worker id, void* arg) {
	searchRTreeResult.push_back(id);
	return true; 
}

void CreateRTree(vector<struct Rectd> rects, RTree<Worker, double, 2, float> &tree) {
	int nrects = rects.size();
	for ( int i = 0; i < nrects; i++ ) {
		tree.Insert(rects[i].min, rects[i].max, rects[i].vid); 
	}
}

void SearchRTree(struct Rectd search_rect, RTree<Worker, double, 2, float> &tree) {
	searchRTreeResult.clear();
	int nhits = tree.Search(search_rect.min, search_rect.max, MySearchCallback, &search_rect.vid);
}

void TraverseRTree(RTree<Worker, double, 2, float> &tree) {
	int count = 0;
	Worker value;
	for ( tree.GetFirst(it); !tree.IsNull(it); tree.GetNext(it) ) {
		value = tree.GetAt(it);
		double boundsMin[2] = {0, 0};
		double boundsMax[2] = {0, 0};
		it.GetBounds(boundsMin, boundsMax);
		count++;
	}
}

#endif
