#include "push_relabel.h"

Graph::Graph(int V, int S, int T, double** C)
{
    this->NODES = V;
    this->source = S;
    this->sink = T;
    
    this->C = C;
    
    this->F = (double **) calloc(NODES, sizeof(double*));
    for (int i = 0; i < this->NODES; i++) {
        this->F[i] = (double *) calloc(NODES, sizeof(double));
    }
    
    this->excess = (double *) calloc(NODES, sizeof(double));
    this->height = (int *) calloc(NODES, sizeof(int));
    this->seen = (int *) calloc(NODES, sizeof(int));
    this->list = (int *) calloc((NODES-2), sizeof(int));
}

void Graph::push(int u, int v) {
    double send = MIN(excess[u], C[u][v] - F[u][v]);
    F[u][v] += send;
    F[v][u] -= send;
    excess[u] -= send;
    excess[v] += send;
}

void Graph::relabel(int u) {
    int v;
    int min_height = std::numeric_limits<int>::max();
    for (v = 0; v < NODES; v++) {
        if (C[u][v] - F[u][v] > 0) {
            min_height = MIN(min_height, height[v]);
            height[u] = min_height + 1;
        }
    }
};

void Graph::discharge(int u) {
    while (excess[u] > 0) {
        if (seen[u] < NODES) {
            int v = seen[u];
            if ((C[u][v] - F[u][v] > 0) && (height[u] > height[v])){
                push(u, v);
            }
            else
                seen[u] += 1;
        } else {
            relabel(u);
            seen[u] = 0;
        }
    }
}

void Graph::moveToFront(int i, int *A) {
    int temp = A[i];
    int n;
    for (n = i; n > 0; n--){
        A[n] = A[n-1];
    }
    A[0] = temp;
}

double Graph::pushRelabel() {
    int i, p;
    
    // source와 sink 제외하고 나머지를 list에
    for (i = 0, p = 0; i < NODES; i++){
        if((i != source) && (i != sink)) {
            list[p] = i;
            p++;
        }
    }
    
    height[source] = NODES;
    excess[source] = std::numeric_limits<double>::max();
    for (i = 0; i < NODES; i++)
        push(source, i);
    
    p = 0;
    while (p < NODES - 2) {
        int u = list[p];
        int old_height = height[u];
        discharge(u);
        if (height[u] > old_height) {
            moveToFront(p,list);
            p=0;
        }
        else
            p += 1;
    }
    double maxflow = 0;
    for (i = 0; i < NODES; i++)
        maxflow += F[source][i];

    return maxflow;
}

//// get mincut from maxflow
vector<EdgeInfo> Graph::getMinCut(int s) {
    bool* marked = (bool *) calloc(NODES, sizeof(bool));
    
    // reset marks
    for(int i = 0; i < NODES; i++)
        marked[i] = false;
    
    // dfs along edges in the residual network, and mark
    // all vertices that can be reached this way.
    set<int> visited;
    stack<int> S;
    S.push(s);
    
    while(!S.empty()) {
        int u = S.top();
        S.pop();
        
        for (int v = 0; v < NODES; v++) {
            if (C[u][v] - F[u][v] > 0) {
                if(visited.count(v) == 0) S.push(v);
            }
        }
        
        visited.insert(u);
        marked[u] = true;
    }
    
    vector<EdgeInfo> res;
    
    //edge 처음부터 끝까지 from to가 marked 된거만 푸시백
    for (int i=0; i<NODES; i++) {
        for(int j=0; j<NODES; j++) {
            if (marked[i] && !marked[j]) {
                EdgeInfo cut;
                cut.sink = i;
                cut.source = j;
                cut.weight = C[i][j];
                res.push_back(cut);
            }
        }
    }
    
    return res;
}

int* Graph::getHeight() {
    return height;
}

void Graph::freeGraph() {
    free(list);
    free(seen);
    free(excess);
}
