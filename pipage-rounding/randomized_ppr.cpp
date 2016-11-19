#include "randomized_ppr.h"

void RandomizedPpr::updateSol(PprAns* targetAns, PprAns* incAns, PprAns* decAns) {
    PprAns *selectedAns, *notSelectedAns;
    float probNumerator = incAns->delta;
    float probDenominator = decAns->delta + incAns->delta;
    
    if (probDenominator == 0) {
        throw runtime_error("no delta. terminate."); // TODO : is this correct?
    }

    srand((unsigned int)time(NULL));
    // get probability p by vector diff sizes
    float p = probNumerator / probDenominator;
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); // 0.0 to 1.0
    //float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); // 0.0 to 1.0
    //float r = 0.5f;

    // with probability p, select decFsCsPair or incFsCsPair
    if (r <= p) {
        selectedAns = decAns;
        notSelectedAns = incAns;
    } else {
        selectedAns = incAns;
        notSelectedAns = decAns;
    }

    free2dArr<float>(notSelectedAns->n, notSelectedAns->sol);
    free2dArr<bool>(notSelectedAns->n, notSelectedAns->T);
    delete notSelectedAns;  
    
    // y <- y'
    free2dArr<float>(targetAns->n, targetAns->sol);
    targetAns->sol = selectedAns->sol;
    // T <- T /\ A'
    for (int i = 0; i <targetAns->n; i++) {
        if (targetAns->minsetA[i] && selectedAns->minsetA[i]) {
            targetAns->minsetA[i] = true;
        } else {
            targetAns->minsetA[i] = false;
        }
    }
    for (int i = 0; i <targetAns->n; i++) {
        for (int j = 0; j < targetAns->n; j++) {
            if (targetAns->minsetA[i] && targetAns->minsetA[j] && i != j) {
                targetAns->T[i][j] = true;
            } else {
               targetAns->T[i][j] = false;
            }
        }
    }
}
