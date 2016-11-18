#include "randomized_ppr.h"

void RandomizedPpr::updateSol(PprAns* targetAns, PprAns* incAns, PprAns* decAns) {
	PprAns *selectedAns, *notSelectedAns;
	float probNumerator = incAns->delta;
	float probDenominator = decAns->delta + incAns->delta;
	
	if (probDenominator == 0) {
		throw runtime_error("no delta. terminate."); // TODO : is this correct?
	}

	// get probability p by vector diff sizes
	float p = probNumerator / probDenominator;
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); // 0.0 to 1.0

	// with probability p, select decFsCsPair or incFsCsPair
	if (r <= p) {
		selectedAns = decAns;
		notSelectedAns = incAns;
	} else {
		selectedAns = incAns;
		notSelectedAns = decAns;
	}

	free2dArr(notSelectedAns->n, notSelectedAns->sol);
	free2dArr(notSelectedAns->n, notSelectedAns->T);
	delete notSelectedAns;	
	
	// y <- y'
	free2dArr(targetAns->n, targetAns->sol);
	targetAns->sol = selectedAns->sol;
	// T <- T /\ A'
	// TODO : intersect T
}
