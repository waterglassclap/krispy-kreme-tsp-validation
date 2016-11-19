#ifndef ABS_PPR_H
#define ABS_PPR_H

#include <utility>
#include <iostream>
#include <deque>
#include <stack>

#include <climits>
#include <float.h>
#include <algorithm>

#include "../common/model/edge_info.h"
#include "../common/model/two_d_graph.h"
#include "../common/utils/common_util.h"
//#include "../common/utils/push_relabel.h"
#include "../common/utils/max-flow-min-cut-graph-cut/src/mincut_util.h"
#include "ppr_ds.h"
#include "indice_diff.h"

using namespace std;

class AbsPpr {
public:
    AbsPpr();
    bool isIntegral(PprAns* ans);
    void initializeTightSet(PprAns* ans);
    EdgeInfo* pullEdge(PprAns* ans);
    bool* getMinsetA(PprAns* prevAns, EdgeInfo* i, EdgeInfo* j);
    float getConstraintDelta(bool* minsetA, PprAns* prevAns);
    PprAns* hitConstraint(PprAns* prevAns, EdgeInfo* i, EdgeInfo* j);
    virtual void updateSol(PprAns* targetAns, PprAns* incAns, PprAns* decAns) = 0;
    TwoDGraph<float>* getValidationGraph(PprAns* prevAns, EdgeInfo* i, EdgeInfo* j);
    void pipageRound(PprAns* targetAns);
};

#endif
