#include "randomized_ppr.h"


void substituteTargetAns(PprAns* targetAns, PprAns* newAns) {
    // y <- y'
    free2dArr<double>(targetAns->n, targetAns->sol);
    targetAns->sol = newAns->sol;
    // T <- T /\ A'
    for (int i = 0; i <targetAns->n; i++) {
        if (targetAns->minsetA[i] && newAns->minsetA[i]) {
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


void RandomizedPpr::updateSol(PprAns* targetAns, PprAns* incAns, PprAns* decAns) {
    double probNumerator = incAns->delta;
    double probDenominator = decAns->delta + incAns->delta;
    
    if (probDenominator == 0) {
        throw runtime_error("no delta. terminate."); // TODO : is this correct?
    }

    // get probability p by vector diff sizes
    double p = probNumerator / probDenominator;
    srand(time(NULL));
    double r = (double) (rand() % 1000) / 1000; // 0.0 to 1.0
    //double r = 0.5f;

    // with probability p, select decFsCsPair or incFsCsPair
    if (r <= p) {
        free2dArr<double>(incAns->n, incAns->sol);
        free2dArr<bool>(incAns->n, incAns->T);
        delete incAns->minsetA;
        delete incAns;

        substituteTargetAns(targetAns, decAns);
    
    }

    else {
        free2dArr<double>(decAns->n, decAns->sol);
        free2dArr<bool>(decAns->n, decAns->T);
        delete decAns->minsetA;
        delete decAns;

        substituteTargetAns(targetAns, incAns);

    }
}
