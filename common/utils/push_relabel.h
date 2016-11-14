#ifndef PUSH_RELABEL_H
#define PUSH_RELABEL_H

#include <iostream>
#include <deque>

#include <climits>
#include <float.h>
#include <algorithm>

#include "../model/edge_info.h"
#include "common_util.h"

using namespace std;

typedef struct MinCutInfo {
	struct EdgeInfo* edgeInfos;
	int size;
	float totalFlow;
} MinCutInfo;

class PushRelabel {
private:
	//capacity matrix
	float ** cMatrix;
	//flow
	float ** fMatrix;
	//residual flow
	float ** rMatrix;
	//the excess of each node
	float * excess;
	//height function
	int * height;
	deque<float> excessNode;
	int nodeNumber;
	int source;
	int sink;

public:
	PushRelabel(int, int, int, float**);
	void printInfo();
	void residualBuilder();
	int checkNode(int);
	bool push(int, int);
	bool relable(int);
	void mineMaxFlow();
	struct MinCutInfo getMinCutInfo();
};

#endif
