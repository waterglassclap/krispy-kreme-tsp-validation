#include "randomized_ppr.h"

using namespace std;

int main() {
    PprAns* targetAns = new PprAns;
    targetAns->n = 6;
    targetAns->delta = 0.0f;
    targetAns->sol = create2dArr(targetAns->n, 0.6f);

    for (int i = 0; i < targetAns->n; i++) {
        targetAns->sol[i][i] = 0.0f;
    }

    targetAns->T = create2dArr(targetAns->n, false);

    RandomizedPpr rppr = RandomizedPpr();
    rppr.pipageRound(targetAns);
    return 0;
}
