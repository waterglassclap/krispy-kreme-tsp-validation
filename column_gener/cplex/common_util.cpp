#include "common_util.h"

bool over(float value, float boundary) {
	return (value + ERROR_TOLERANCE >= boundary);
}


bool exceed(float value, float boundary) {
	return (value + ERROR_TOLERANCE > boundary);
}


float** getMergedGraph(int s, int t, int n, float** inputGraph) {
	// size will be n - 1
	float** mergedGraph = new float*[n - 1];
	for (int i = 0; i < n - 1; i++) {
		mergedGraph[i] = new float[n - 1];
	}

	int newRow = 1;
	for (int originalRow = 0; originalRow < n; originalRow++) {
		int newCol = 1, currentRow, currentCol;
		if (originalRow == s || originalRow == t) {
			currentRow = 0;
		} else {
			currentRow = newRow;
			newRow++;
		}
		for (int originalCol = 0; originalCol < n; originalCol++) {
			if (originalCol == s || originalCol == t) {
				currentCol = 0;
			} else {
				currentCol = newCol;
				newCol++;
			}
			mergedGraph[currentRow][currentCol] += inputGraph[originalRow][originalCol];
		}
	}
	
	return mergedGraph;
}
