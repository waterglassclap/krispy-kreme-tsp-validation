#ifndef COMMON_UTIL_H
#define COMMON_UTIL_H

#include <iostream>

const float ERROR_TOLERANCE = 1e-9;

bool over(float value, float boundary);
bool exceed(float value, float boundary);
// merged node will be 0
float** getMergedGraph(int s, int t, int n, float** inputGraph);
template <typename T> void free2dArr(int n, T** inputGraph) {
	for (int i = 0; i < n; i++) {
		delete inputGraph[i];
	}
	delete inputGraph;
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
