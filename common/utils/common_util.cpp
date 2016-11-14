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

	int newRow = 0;
	for (int originalRow = 0; originalRow < n; originalRow++) {
		int newCol = 0, currentRow, currentCol;
		if (originalRow == t) {
			currentRow = s;
		} else {
			currentRow = newRow;
			newRow++;
		}
		for (int originalCol = 0; originalCol < n; originalCol++) {
			if (originalCol == t) {
				currentCol = s;
			} else {
				currentCol = newCol;
				newCol++;
			}
			mergedGraph[currentRow][currentCol] += inputGraph[originalRow][originalCol];
		}
	}
	
	return mergedGraph;
}
