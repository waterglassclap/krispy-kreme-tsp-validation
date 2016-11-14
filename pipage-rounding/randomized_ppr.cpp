#include "randomized_ppr.h"

float RandomizedPpr::getSolDiffSize(PprAns* first, PprAns* second) {
	// TODO : return only positive
	return 1.0f;
}


void RandomizedPpr::updateSol(PprAns* targetAns, PprAns* incAns, PprAns* decAns) {
	PprAns *selectedAns, *notSelectedAns;
	float firstSolDiffSize = getSolDiffSize(targetAns, incAns);
	float secondSolDiffSize = getSolDiffSize(incAns, decAns);
	
	if (secondSolDiffSize == 0) {
		throw runtime_error("no difference between first and second. terminate."); // TODO : is this correct?
	}

	// get probability p by vector diff sizes
	float p = firstSolDiffSize / secondSolDiffSize;
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); // 0.0 to 1.0

	if (p > 1.0f) {
		throw runtime_error("probability is bigger than 1. terminate."); // TODO : is this correct?
	}

	// with probability p, select decFsCsPair or incFsCsPair
	if (r <= p) {
		selectedAns = decAns;
		notSelectedAns = incAns;
	} else {
		selectedAns = incAns;
		notSelectedAns = decAns;
	}

	delete notSelectedAns->sol;  // TODO : delete each rows
	delete notSelectedAns->T;  // TODO : delete each rows
	delete notSelectedAns;
	
	// y <- y'
	delete targetAns->sol; // TODO : delete each rows
	targetAns->sol = selectedAns->sol;
	// T <- T /\ A'
	// TODO : intersect T
}
