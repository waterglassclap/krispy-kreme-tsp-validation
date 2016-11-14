#ifndef ABS_PPR_H
#define ABS_PPR_H

#include <utility>
#include <iostream>
#include <deque>

#include <climits>
#include <float.h>
#include <algorithm>

#include "../common/model/edge_info.h"
#include "../common/utils/common_util.h"
#include "../common/utils/push_relabel.h"
#include "ppr_ds.h"

using namespace std;

class AbsPpr {
public:
	AbsPpr();
	bool isIntegral(PprAns* ans);
	void initializeTightSet(PprAns* ans);
	EdgeInfo* pullEdge(PprAns* ans);
	PprAns* hitConstraint(PprAns* prevAns, EdgeInfo* i, EdgeInfo* j);
	virtual void updateSol(PprAns* targetAns, PprAns* incAns, PprAns* decAns) = 0;
	void pipageRound(PprAns* targetAns);
};

#endif
