#include "abs_ppr.h"

// TODO : float error, 1e-9 handling
// TODO : check if indiceDiff works correctly

void printPprAns(PprAns* pprAns) {
	/* test code */
			cout << "====target y===="<< endl;
			for (int i = 0; i < pprAns->n; i++) {
				for (int j = 0; j <pprAns->n; j++) {
					cout << pprAns->sol[i][j] << " ";
				}
				cout << endl;
			}
			cout << "====target T===="<< endl;
			for (int i = 0; i < pprAns->n; i++) {
				for (int j = 0; j < pprAns->n; j++) {
					cout << pprAns->T[i][j] << " ";
				}
				cout << endl;
			}
	cin.ignore();
	/*************/

}

TwoDGraph<float>* getTwoDGraphCopy(TwoDGraph<float>* inputGraph) {
	TwoDGraph<float>* newGraph = new TwoDGraph<float>;
	newGraph->n = inputGraph->n;
	newGraph->source = inputGraph->source;
	newGraph->sink = inputGraph->sink;
	newGraph->set = new float*[newGraph->n];
	for (int i = 0; i < newGraph->n; i++) {
		newGraph->set[i] = new float[newGraph->n];
		for (int j = 0; j < newGraph->n; j++) {
			newGraph->set[i][j] = inputGraph->set[i][j];
		}
	}
	return newGraph;
}

IndiceDiff* getIndiceDiffCopy(IndiceDiff* inputIndiceDiff) {
	IndiceDiff* newIndiceDiff = new IndiceDiff;
	newIndiceDiff->n = inputIndiceDiff->n;
	newIndiceDiff->diffHistories = new deque<int>[newIndiceDiff->n];
	for (int i = 0; i < newIndiceDiff->n; i++) {
		newIndiceDiff->diffHistories[i] = inputIndiceDiff->diffHistories[i]; // TODO : is this correct?
	}
	return newIndiceDiff;
}

// TODO : this seems to result in segfault, check!
void mergeGraph(TwoDGraph<float>* targetGraph, IndiceDiff* indiceDiff, int firstTarget, int secondTarget) {
	int n = targetGraph->n;
	float** mergedSet = create2dArr<float>(n - 1, 0.0f);
	IndiceDiff* tempIndiceDiff = getIndiceDiffCopy(indiceDiff);
	// merge set, merged node would be node 0
	int newRow = 1;
	for (int originalRow = 0; originalRow < n; originalRow++) {
		int newCol = 1, currentRow, currentCol;
		if (originalRow == firstTarget || originalRow == secondTarget) {
			currentRow = 0;
		} else {
			currentRow = newRow;
			newRow++;
		}
		// update indice diff history
		for (int i = 0; i < indiceDiff->n; i++) {
			int lastDiffHist = i;
			if (tempIndiceDiff->diffHistories[i].size() != 0) {
				lastDiffHist = tempIndiceDiff->diffHistories[i].back();
			}
			if (lastDiffHist == originalRow) {
				indiceDiff->diffHistories[i].push_back(currentRow);
			}
		}

		for (int originalCol = 0; originalCol < n; originalCol++) {
			if (originalCol == firstTarget || originalCol == secondTarget) {
				currentCol = 0;
			} else {
				currentCol = newCol;
				newCol++;
			}
			mergedSet[currentRow][currentCol] += targetGraph->set[originalRow][originalCol];
		}
	}
	// update target Graph set
	free2dArr<float>(n, targetGraph->set);
	targetGraph->set = mergedSet;
	targetGraph->n = n - 1;
	// update indice history
	delete[] tempIndiceDiff->diffHistories;
	delete tempIndiceDiff;
}

AbsPpr::AbsPpr() {
	// Do nothing
}

bool AbsPpr::isIntegral(PprAns* ans) {
	for (int i = 0; i < ans->n; i++) {
		for (int j = 0; j < ans->n; j++) {
			if (static_cast<int>(ans->sol[i][j]) != ans->sol[i][j]) {
				return false;
			}
		}
	}
	return true;
}


void AbsPpr::initializeTightSet(PprAns* ans) {
	delete ans->T;
	ans->T = create2dArr<bool>(ans->n, true);
	for (int i = 0; i < ans->n; i++) {
		ans->T[i][i] = false;
	}
}

// s is nodes[-2], t is nodes[-1]
TwoDGraph<float>* AbsPpr::getValidationGraph(PprAns* prevAns) {
	int n = prevAns->n + 2;
	TwoDGraph<float>* validationGraph = new TwoDGraph<float>;
	validationGraph->n = n;
	validationGraph->source = n - 2;
	validationGraph->sink = n - 1;
	validationGraph->set = create2dArr<float>(n, 0.0f);
	//get wFromS: s -(wFromS)-> vi, which is 1/2 SIGMA(Ye, e E V) : TODO : is this correct?
	float wFromS = 0.0f;
	for (int i = 0; i < prevAns->n; i++) {
		for (int j = i; j < prevAns->n; j++) {
			wFromS += prevAns->sol[i][j];
		}
	}
	// 1. s -> vi to wFromS
	// 2. vi -> t to 1
	for (int i = 0; i < n; i++) {
		validationGraph->set[n - 2][i] = wFromS;
		validationGraph->set[i][n - 1] = 1.0f;
	}
	// 3. fill the rest of the graph with prevAns weights;
	for (int i = 0; i < n - 2; i++) {
		for (int j = 0; j < n - 2; j++) {
			if (j > i) {
				validationGraph->set[i][j] = prevAns->sol[i][j];
				validationGraph->set[j][i] = validationGraph->set[i][j];
			}
		}
	}
	return validationGraph;
}

EdgeInfo* AbsPpr::pullEdge(PprAns* ans) {
	EdgeInfo* edge = new EdgeInfo;
	edge->source = -1;
	edge->sink = -1;
	edge->weight = -1;

	for (int i = 0; i < ans->n; i++) {
		for (int j = 0; j < ans->n; j++) {
			if (ans->T[i][j] && ans->T[j][i]
				&& (static_cast<int>(ans->sol[i][j]) != ans->sol[i][j])) { // TODO : is this correct?
				edge->source = i;
				edge->sink = j;
				edge->weight = ans->sol[i][j];
				// mark T
				ans->T[i][j] = false;
				ans->T[j][i] = false;
				return edge;
			}
		}
	}
	return edge;
}

bool** AbsPpr::getMinsetA(PprAns* prevAns, EdgeInfo* i, EdgeInfo* j) {
	TwoDGraph<float>* validationGraph = getValidationGraph(prevAns);
	MinCutInfo mci;
	
	IndiceDiff* indiceDiff = new IndiceDiff;
	indiceDiff->n = validationGraph->n;
	indiceDiff->diffHistories = new deque<int>[validationGraph->n];
	// merge source with i.source and i.sink

	mergeGraph(validationGraph, indiceDiff, i->source, i->sink);
	mergeGraph(validationGraph, indiceDiff,
		indiceDiff->diffHistories[i->source].back(),
		indiceDiff->diffHistories[validationGraph->source].back());

	// if i and j has no common node, compare:
	// 1) s <- merge(merge(i), s),		t <- merge(j.node[0], t)
	// 2) s <- merge(merge(i), s),		t <- merge(j.node[1], t)
	if (!(j->source == i->source || j->source == i->sink ||
		j->sink == i->source || j->sink == i->sink)) {
		TwoDGraph<float>* tempGraph1 = getTwoDGraphCopy(validationGraph);
		TwoDGraph<float>* tempGraph2 = getTwoDGraphCopy(validationGraph);
		IndiceDiff* tempIndiceDiff1 = getIndiceDiffCopy(indiceDiff);
		IndiceDiff* tempIndiceDiff2 = getIndiceDiffCopy(indiceDiff);

		mergeGraph(tempGraph1, tempIndiceDiff1,
			tempIndiceDiff1->diffHistories[j->source].back(),
			tempIndiceDiff1->diffHistories[tempGraph1->sink].back());
		mergeGraph(tempGraph2, tempIndiceDiff2,
			tempIndiceDiff2->diffHistories[j->sink].back(),
			tempIndiceDiff2->diffHistories[tempGraph2->sink].back());

		PushRelabel verifier1(
			tempIndiceDiff1->diffHistories[tempGraph1->source].back(),
			tempIndiceDiff1->diffHistories[tempGraph1->sink].back(),
			tempGraph1->n, tempGraph1->set);
		PushRelabel verifier2(
			tempIndiceDiff2->diffHistories[tempGraph2->source].back(),
			tempIndiceDiff2->diffHistories[tempGraph2->sink].back(),
			tempGraph2->n, tempGraph2->set);

		MinCutInfo tempMinCutInfo1 = verifier1.getMinCutInfo();
		MinCutInfo tempMinCutInfo2 = verifier2.getMinCutInfo();

		delete[] indiceDiff->diffHistories;
		delete indiceDiff;

		if(tempMinCutInfo1.totalFlow < tempMinCutInfo2.totalFlow) {
			mci = tempMinCutInfo1;
			indiceDiff = tempIndiceDiff1;
			delete[] tempIndiceDiff2->diffHistories;
			delete tempIndiceDiff2;
		} else {
			mci = tempMinCutInfo2;
			indiceDiff = tempIndiceDiff2;
			delete[] tempIndiceDiff1->diffHistories;
			delete tempIndiceDiff1;
		}
		free2dArr<float>(tempGraph1->n, tempGraph1->set);
		free2dArr<float>(tempGraph2->n, tempGraph2->set);
		delete tempGraph1;
		delete tempGraph2;
	}
	// if i and j has common node, s <- merge(merge(i), s), t <- merge(i.nodes - s, t)
	else {
		if (j->source == i->source || j->source == i->sink) {
			mergeGraph(validationGraph, indiceDiff,
				indiceDiff->diffHistories[j->sink].back(),
				indiceDiff->diffHistories[validationGraph->sink].back());
		}
		else if (j->sink == i->source || j->sink == i->sink) {
			mergeGraph(validationGraph, indiceDiff,
				indiceDiff->diffHistories[j->source].back(),
				indiceDiff->diffHistories[validationGraph->sink].back());
		}
		// get min cut of validation graph
		PushRelabel verifier(
			indiceDiff->diffHistories[validationGraph->source].back(),
			indiceDiff->diffHistories[validationGraph->sink].back(),
			validationGraph->n, validationGraph->set);
    	mci = verifier.getMinCutInfo();
	}

	bool *nodeMarker = new bool[prevAns->n], *tempNodeMarker = new bool[prevAns->n];
	bool** minsetMarker = create2dArr<bool>(prevAns->n, false);

	// separate nodes to 2 with min-cut
	for (int i = 0; i < mci.nodeNum; i++) {
		if (mci.heights[i] > mci.cutHeight) {
			tempNodeMarker[i] = true;
		}
	}
	// reverse nodeMarker if true-set does not contain source
	if (!tempNodeMarker[indiceDiff->diffHistories[i->source].back()]) {
		for (int i = 0; i < mci.nodeNum; i++) {
			tempNodeMarker[i] = !tempNodeMarker[i];
		}
	}
	// reverse minset marker with previous indices
	for (int i = 0; i <prevAns->n; i++) {
		if (tempNodeMarker[indiceDiff->diffHistories[i].back()]) {
			nodeMarker[i] = true;
		}
	}

	// mark minsetMarker
	for (int i = 0; i < prevAns->n; i++) {
		for (int j = 0; j < prevAns->n; j++) {
			if (nodeMarker[i] && nodeMarker[j]) {
				minsetMarker[i][j] = true;
			}
		}
	}

	free2dArr<float>(validationGraph->n, validationGraph->set);
	delete validationGraph;
	delete[] indiceDiff->diffHistories;
	delete indiceDiff;
	delete nodeMarker;
	
	return minsetMarker;
}

float AbsPpr::getConstraintDelta(bool** minsetA, PprAns* prevAns) {
	int absA = 0;
	float sumAEdges = 0.0f;
	for (int i = 0; i < prevAns->n; i++) {
		for (int j = i; j < prevAns->n; j++) {
			if (minsetA[i][j]) {
				absA++;
				sumAEdges += prevAns->sol[i][j]; // TODO : is this correct?
			}
		}
	}
	return ((float)absA) - 1.0f - sumAEdges;
}

PprAns* AbsPpr::hitConstraint(PprAns* prevAns, EdgeInfo* i, EdgeInfo* j) {
	PprAns* newAns = new PprAns;
	bool** minsetA = getMinsetA(prevAns, i, j);
	float constraintDelta = getConstraintDelta(minsetA, prevAns); 
	cout << "constraintDelta : " << constraintDelta << endl;
	
	if (constraintDelta < 0.0f) {
		throw runtime_error("constraintDelta is smaller than 0, something's wrong! Terminate.");
	}

	if (prevAns->sol[j->source][j->sink] < constraintDelta) {
		// TODO : just terminate?
		minsetA = create2dArr<bool>(prevAns->n, false);
		constraintDelta = prevAns->sol[j->source][j->sink];
	}

	newAns->n = prevAns->n;
	newAns->T = minsetA;
	newAns->delta = constraintDelta;

	// copy prevAns solution
	newAns->sol = new float*[newAns->n];
	for (int i = 0; i < newAns->n; i++) {
		newAns->sol[i] = new float[newAns->n];
		for (int j = 0; j < newAns->n; j++) {
			newAns->sol[i][j] = prevAns->sol[i][j];
		}
	}
	// yi = yi + delta
	newAns->sol[i->source][i->sink] += constraintDelta;
	newAns->sol[i->sink][i->source] += constraintDelta;
	// yj = yj - delta
	newAns->sol[j->source][j->sink] -= constraintDelta;
	newAns->sol[j->sink][j->source] -= constraintDelta;
	
	
	return newAns;
}

void AbsPpr::pipageRound(PprAns* targetAns) {
	while (!isIntegral(targetAns)) {
		initializeTightSet(targetAns);
		while (true) {
			cout << endl;
			cout << "iteration start." << endl;
			EdgeInfo* i = pullEdge(targetAns);
			EdgeInfo* j = pullEdge(targetAns);
			// invalid edge
			if (i->source < 0 || j->source < 0) {
				cout << "invalid edge. break" << endl;
				printPprAns(targetAns);
				break;
			}
			PprAns* incAns = hitConstraint(targetAns, i, j);
			PprAns* decAns = hitConstraint(targetAns, j, i);
			updateSol(targetAns, incAns, decAns);
			printPprAns(targetAns);
		}
	}
}
