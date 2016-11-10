#ifndef ABS_PPR_H
#define ABS_PPR_H

#include <utility>
#include <iostream>
#include <deque>

#include <climits>
#include <float.h>
#include <algorithm>

#include "../common_ds/edge_info.h"
#include "../utils/push_relabel.h"
#include "ppr_ds.h"

using namespace std;

class AbsPpr {
private:
	struct ConstrSet baseMatroid;
	struct FracSol baseSol;
public:
	AbsPpr(struct ConstrSet, struct FracSol);
	virtual pair<struct FracSol, struct ConstrSet> getNewFsCsPair(pair<struct FracSol, struct ConstrSet> aFsCsPair,
                                                                  pair<struct FracSol, struct ConstrSet> bFsCsPair) = 0;
	pair<struct FracSol, struct ConstrSet> hitConstraint(struct FracSol, struct EdgeInfo, struct EdgeInfo);
	struct FracSol pipageRound();
};

#endif
