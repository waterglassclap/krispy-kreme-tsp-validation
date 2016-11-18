#include "validation.h"
#include <math.h>

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
    verifier.freeVars();
    return errorInfo;
}


struct ErrorInfos getNonStErrorInfos(int s, int t, int n, float** inputGraph) {
    ErrorInfos errorInfos;
    errorInfos.infos = new ErrorInfo[n];
    errorInfos.num = 0;
    
    if (s == t) {
        //throw runtime_error("s should not be equal to t. terminate.");
        cout << "s should not be equal to t. terminate.";
        exit(0);
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
            verifier.freeVars();
        }
    }
    //for (int i = 0; i < n; i++) {
    //    delete[] mergedGraph[i];
    //}
    //delete mergedGraph;
    //free2dArr(n, mergedGraph);
    return errorInfos;
}
