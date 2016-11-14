
#include <math.h>
#include "../../common/utils/common_util.h"
#include "../../common/utils/push_relabel.h"

using namespace std;

typedef struct ErrorInfo {
	MinCutInfo minCutInfo;
	float severity;
	bool isValid;
} ErrorInfo;

typedef struct ErrorInfos {
	struct ErrorInfo* infos;
	int num;
} ErrorInfos;

struct ErrorInfo getStErrorInfo(int s, int t, int n, float** inputGraph) {
	ErrorInfo errorInfo;
	errorInfo.isValid = false;

	PushRelabel verifier(s, t, n, inputGraph);
	MinCutInfo mci = verifier.getMinCutInfo();
	if (!over(mci.totalFlow, 1.0f)) {
		errorInfo.isValid = true;
		errorInfo.minCutInfo = mci;
		errorInfo.severity = 1.0f - mci.totalFlow;
	}
	return errorInfo;
}


struct ErrorInfos getNonStErrorInfos(int s, int t, int n, float** inputGraph) {
	ErrorInfos errorInfos;
	errorInfos.infos = new ErrorInfo[n];
	errorInfos.num = 0;

	if (s == t) {
	    throw runtime_error("s should not be equal to t. terminate.");
	}

	float** mergedGraph = getMergedGraph(s, t, n, inputGraph);

    for (int i = 0; i < n - 1; i++) {
    	if (i != s) {
    		PushRelabel verifier(0, i, n - 1, mergedGraph);
    		MinCutInfo mci = verifier.getMinCutInfo();
			if (!over(mci.totalFlow, 2.0f)) {
				errorInfos.infos[errorInfos.num].minCutInfo = mci;
				errorInfos.infos[errorInfos.num].severity = 2.0f - mci.totalFlow;
				errorInfos.num++;
			}
    	}
    }
    // TODO : free mergedGraph
    return errorInfos;
}

int main() {
	int s = 0, t = 5;
	int n = 6;
	float** arr = new float*[n];
	for(int i = 0; i < n; ++i) {
    	arr[i] = new float[n];
    	for (int j = 0; j < n; ++j) {
    		arr[i][j] = 0.0f;
    	}
	}

	arr[0][1] = 0.11f;
	arr[0][2] = 0.12f;
	arr[2][1] = 0.01f;
	arr[1][3] = 0.12f;
	arr[2][4] = 0.11f;
	arr[4][3] = 0.07f;
	arr[3][5] = 0.19f;
	arr[4][5] = 0.04f;

	ErrorInfo stErrorInfo = getStErrorInfo(s, t, n, arr);
	ErrorInfos nonStErrorInfos = getNonStErrorInfos(s, t, n, arr);

	if (stErrorInfo.isValid) {
		cout << "st error severity is : " << stErrorInfo.severity << endl;
	}
	for (int i = 0; i < nonStErrorInfos.num; i++) {
		cout << "non st error severity [" << i << "] is : " << nonStErrorInfos.infos[i].severity << endl;
	}
	
	return 0;
}
