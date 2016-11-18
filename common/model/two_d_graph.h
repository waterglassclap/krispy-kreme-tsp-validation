#ifndef TWO_D_GRAPH_H
#define TWO_D_GRAPH_H

template <typename T>
struct TwoDGraph {
	int source;
	int sink;
	int n;
	T** set;
};

#endif
