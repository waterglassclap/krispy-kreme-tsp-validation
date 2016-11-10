#ifndef RANDOMIZED_PPR_H
#define RANDOMIZED_PPR_H

#include "ppr_ds.h"
#include "abs_ppr.h"

using namespace std;

class RandomizedPpr : public AbsPpr {
public:
	RandomizedPpr(struct ConstrSet bM, struct FracSol bS) : AbsPpr(bM, bS) {};
	virtual pair<struct FracSol, struct ConstrSet> getNewFsCsPair(pair<struct FracSol, struct ConstrSet> aFsCsPair,
                                                                  pair<struct FracSol, struct ConstrSet> bFsCsPair);
};

#endif
