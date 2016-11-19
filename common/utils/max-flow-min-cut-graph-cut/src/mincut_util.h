#ifndef MINCUT_UTIL_H
#define MINCUT_UTIL_H

#include <math.h>
#include "../../common_util.h"
#include "../../../model/edge_info.h"
#include "general_settings.h"

typedef struct MinCutInfo {
	int* heights;
	int cutHeight;
	int nodeNum;

	struct EdgeInfo* edgeInfos;
	int size;
	float totalFlow;
} MinCutInfo;

typedef struct ErrorInfo {
    MinCutInfo minCutInfo;
    float severity;
    bool isValid;
} ErrorInfo;

typedef struct ErrorInfos {
    struct ErrorInfo* infos;
    int num;
} ErrorInfos;

class MincutUtil {
public:
	MincutUtil(); // TODO : specify type / lib to use
	struct MinCutInfo getMinCutInfo(int s, int t, int n, float** inputGraph);
	struct ErrorInfo getStErrorInfo(int s, int t, int n, float** inputGraph);
	struct ErrorInfos getNonStErrorInfos(int s, int t, int n, float** inputGraph);
};

#endif
