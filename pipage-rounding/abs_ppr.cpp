#include "abs_ppr.h"

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
	ans->T = new bool*[ans->n];
	for (int i = 0; i < ans->n; i++) {
		ans->T[i] = new bool[ans->n];
		for (int j = 0; j < ans->n; j++) {
			ans->T[i][j] = true;
		}
	}
}


EdgeInfo* AbsPpr::pullEdge(PprAns* ans) {
	EdgeInfo* edge = new EdgeInfo;
	edge->source = -1;
	edge->sink = -1;
	edge->weight = -1;

	for (int i = 0; i < ans->n; i++) {
		for (int j = 0; j < ans->n; j++) {
			if (ans->T[i][j] && ans->T[j][i]) { // TODO : is this correct?
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

// TODO : What if edge has common node?
PprAns* AbsPpr::hitConstraint(PprAns* prevAns, EdgeInfo* i, EdgeInfo* j) {
	PprAns* newAns = new PprAns;

	// TODO : calculate 
	// TODO : free
	float** iMergedGraph = getMergedGraph(i->source, i->sink, prevAns->n, prevAns->sol);
	float** mergedGraph = getMergedGraph(i->source, i->sink, prevAns->n, prevAns->sol);
	int newSource = j->source;
	int newSink = j->sink;
	int newN = n - 1;
	mergedGraph = getMergedGraph(j->source, i->sink, prevAns->n, mergedGraph);

	PushRelabel verifier(s, t, n, inputGraph);
	MinCutInfo mci = verifier.getMinCutInfo();

	// TODO
    return newAns;
}

void AbsPpr::pipageRound(PprAns* targetAns) {
	while (!isIntegral(targetAns)) {
		initializeTightSet(targetAns);
		while (true) {
			EdgeInfo* i = pullEdge(targetAns);
			EdgeInfo* j = pullEdge(targetAns);
			// invalid edge
			if (i->source < 0 || j->source < 0) {
				break;
			}
			PprAns* incAns = hitConstraint(targetAns, i, j);
			PprAns* decAns = hitConstraint(targetAns, j, i);
			updateSol(targetAns, incAns, decAns);
		}
	}
}
