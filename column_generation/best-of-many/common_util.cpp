#include "common_util.h"

bool over(double value, double boundary) {
	return (value + ERROR_TOLERANCE >= boundary);
}


bool exceed(double value, double boundary) {
	return (value + ERROR_TOLERANCE > boundary);
}

// TODO : check if it is correct
double** getMergedGraph(int s, int t, int n, double** inputGraph) {
	double** mergedGraph = create2dArr<double>(n - 1, 0.0f);

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
			if (currentRow != currentCol) {
				mergedGraph[currentRow][currentCol] += inputGraph[originalRow][originalCol];
			}
		}
	}
	
	return mergedGraph;
}

