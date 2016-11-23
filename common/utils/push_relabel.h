#ifndef push_relabel_h
#define push_relabel_h

#include <iostream>
#include <stack>
#include <set>
#include <vector>
#include "../model/edge_info.h"
using namespace std;

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

// To represent a flow network
class Graph
{
    int NODES;
    int source;
    int sink;
    
    double *excess;
    int *height, *list, *seen;
    
    double** C, **F;
    
public:
    Graph(int V, int S, int T, double** C);  // Constructor
    
    void push(int u, int v);
    void relabel(int u);
    void discharge(int u);
    void moveToFront(int i, int *A);
    double pushRelabel();
    vector<EdgeInfo> getMinCut(int s);
    int* getHeight();
    void freeGraph();
};

#endif /* push_relabel_h */
