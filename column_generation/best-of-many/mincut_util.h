#ifndef MINCUT_UTIL_H
#define MINCUT_UTIL_H

#include <math.h>
#include "common_util.h"
#include "edge_info.h"
#include "push_relabel.h"

typedef struct MinCutInfo {
	int* heights;
	int cutHeight;
	int nodeNum;

	struct EdgeInfo* edgeInfos;
	int size;
	double totalFlow;
} MinCutInfo;

typedef struct ErrorInfo {
    MinCutInfo minCutInfo;
    double severity;
    bool isValid;
} ErrorInfo;

typedef struct ErrorInfos {
    struct ErrorInfo* infos;
    int num;
} ErrorInfos;

class MincutUtil {
public:
	MincutUtil(); // TODO : specify type / lib to use
	struct MinCutInfo getMinCutInfo(int s, int t, int n, double** inputGraph);
	struct ErrorInfo getStErrorInfo(int s, int t, int n, double** inputGraph);
	struct ErrorInfos getNonStErrorInfos(int s, int t, int n, double** inputGraph);
};

#endif
