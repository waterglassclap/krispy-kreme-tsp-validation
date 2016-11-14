#ifndef RANDOMIZED_PPR_H
#define RANDOMIZED_PPR_H

#include "ppr_ds.h"
#include "abs_ppr.h"

using namespace std;

class RandomizedPpr : public AbsPpr {
private:
	float getSolDiffSize(PprAns* first, PprAns* second);
public:
	RandomizedPpr() : AbsPpr() {};
    virtual void updateSol(PprAns* targetAns, PprAns* incAns, PprAns* decAns);
};


#endif
