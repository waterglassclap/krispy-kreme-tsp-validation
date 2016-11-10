#include "abs_ppr.h"

AbsPpr::AbsPpr(struct ConstrSet bM, struct FracSol bS) {
	baseMatroid = bM;
	baseSol = bS;
}

pair<struct FracSol, struct ConstrSet> AbsPpr::hitConstraint(struct FracSol y, struct EdgeInfo i, struct EdgeInfo j) {
    pair <struct FracSol, struct ConstrSet> newFsCsPair;
    
	// TODO
    return newFsCsPair;
}

struct FracSol pipageRound() {
	FracSol y;
	// TODO
	return y;
}
