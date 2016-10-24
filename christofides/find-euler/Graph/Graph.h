#ifndef EULER_GRAPH_H
#define EULER_GRAPH_H

#include <iostream>
#include <list>
#include <fstream> //file I/O library

using namespace std;

class Graph {
private:
    int V;
    list<int> *adj;
public:
    list<int> *euler_path;
    Graph(int V);
    void addEdge(int u, int v);
    void rmvEdge(int u, int v);

    // Methods to print Eulerian tour
    void printEulerTour(ofstream &outstream);
    void printEulerUtil(int s, ofstream &outstream);

    // This function returns count of vertices reachable from v. It does DFS
    int DFSCount(int v, bool visited[]);

    // Utility function to check if edge u-v is a valid next edge in
    // Eulerian trail or circuit
    bool isValidNextEdge(int u, int v);
};

#endif