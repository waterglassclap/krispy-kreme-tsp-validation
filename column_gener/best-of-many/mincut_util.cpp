#include "mincut_util.h"

MincutUtil::MincutUtil() {
    // Do nothing
}

MinCutInfo MincutUtil::getMinCutInfo(int s, int t, int n, double** inputGraph) {
    Graph g;
    // get min cut
    for(int i = 0; i < n; i++){
        addVertex(g);
    }
    initVertices(g);
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            if (i != j) {
                addSingleEdge(g, i, j, inputGraph[i][j], inputGraph[j][i]);
            }
        }
    }
    double max_flow = pushRelabel(g, s, t);
    vector<Edge> min_cut = getMinCut(g, s);
    
    // build mincut info
    MinCutInfo minCutInfo;
    minCutInfo.nodeNum = n;
    minCutInfo.heights = new int[n];
    for (int i = 0; i < n; i++) {
        minCutInfo.heights[i] = getHeight(g, i);
    }
    for (int i = 0; i < n; i++) {
        bool isPresent = false;
        for (int j = 0; j < n; j++) {
            if (minCutInfo.heights[j] == i) {
                isPresent = true;
                break;
            }
        }
        if (!isPresent) {
            minCutInfo.cutHeight = i;
            break;
        }
    }
    minCutInfo.size = (int)min_cut.size();
    minCutInfo.totalFlow = max_flow;
    minCutInfo.edgeInfos = new EdgeInfo[minCutInfo.size];
    for (int i = 0; i < minCutInfo.size; i++) {
        minCutInfo.edgeInfos[i].source = min_cut[i]._from;
        minCutInfo.edgeInfos[i].sink = min_cut[i]._to;
        minCutInfo.edgeInfos[i].weight = inputGraph[min_cut[i]._from][min_cut[i]._to];
    }
    for (int i = 0; i < g.vertices.size(); i++) {
        g.vertices[i].adj_edges.clear();
    }
    g.vertices.clear();
    g.edges.clear();
    return minCutInfo;
}

ErrorInfo MincutUtil::getStErrorInfo(int s, int t, int n, double** inputGraph) {
    ErrorInfo errorInfo;
    errorInfo.isValid = false;
    
    MinCutInfo mci = getMinCutInfo(s, t, n, inputGraph);
    
    if (!over(mci.totalFlow, 1.0f)) {
        errorInfo.isValid = true;
        errorInfo.minCutInfo = mci;
        errorInfo.severity = 1.0f - mci.totalFlow;
    }
    
    return errorInfo;
}

struct ErrorInfos MincutUtil::getNonStErrorInfos(int s, int t, int n, double** inputGraph) {
    ErrorInfos errorInfos;
    errorInfos.infos = new ErrorInfo[n];
    errorInfos.num = 0;
    
    if (s == t) {
        // throw runtime_error("s should not be equal to t. terminate.");
        cout << "s should not be equal to t. terminate.";
        exit(0);
    }
    
    double** mergedGraph = getMergedGraph(s, t, n, inputGraph);
    
    for (int i = 1; i < n - 1; i++) {
        MinCutInfo mci = getMinCutInfo(0, i, n - 1, mergedGraph);
        if (!over(mci.totalFlow, 2.0f)) {
            errorInfos.infos[errorInfos.num].minCutInfo = mci;
            errorInfos.infos[errorInfos.num].severity = 2.0f - mci.totalFlow;
            errorInfos.num++;
        }
    }
    
    return errorInfos;
}
