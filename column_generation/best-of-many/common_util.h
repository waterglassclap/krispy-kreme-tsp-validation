#ifndef COMMON_UTIL_H
#define COMMON_UTIL_H

#include <iostream>

const double ERROR_TOLERANCE = 1e-9;

bool over(double value, double boundary);
bool exceed(double value, double boundary);
// merged node will be 0
double** getMergedGraph(int s, int t, int n, double** inputGraph);
template <typename T> void free2dArr(int n, T** inputGraph) {
	for (int i = 0; i < n; i++) {
		delete inputGraph[i];
	}
	delete[] inputGraph;
}
template <typename T> T** create2dArr(int n, T initValue) {
	T** arr = new T*[n];
	for (int i = 0; i < n; i++) {
		arr[i] = new T[n];
		for (int j = 0; j < n; j++) {
			arr[i][j] = initValue;
		}
	}
	return arr;
}


#endif
