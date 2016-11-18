#include "randomized_ppr.h"

using namespace std;

int main() {
    PprAns* targetAns = new PprAns;
    targetAns->n = 12;
    targetAns->delta = 0.0f;
    targetAns->sol = create2dArr(targetAns->n, 0.0f);
    targetAns->sol[0][1] = 0.5f;
    targetAns->sol[1][0] = 0.5f;
    targetAns->sol[0][2] = 0.5f;
    targetAns->sol[2][0] = 0.5f;
    targetAns->sol[2][1] = 0.5f;
    targetAns->sol[1][2] = 0.5f;
    
    targetAns->sol[1][3] = 1.0f;
    targetAns->sol[3][1] = 1.0f;
    targetAns->sol[3][5] = 1.0f;
    targetAns->sol[5][3] = 1.0f;
    targetAns->sol[5][7] = 1.0f;
    targetAns->sol[7][5] = 1.0f;
    targetAns->sol[7][9] = 1.0f;
    targetAns->sol[9][7] = 1.0f;

    targetAns->sol[2][4] = 1.0f;
    targetAns->sol[4][2] = 1.0f;
    targetAns->sol[4][6] = 1.0f;
    targetAns->sol[6][4] = 1.0f;
    targetAns->sol[6][8] = 1.0f;
    targetAns->sol[8][6] = 1.0f;
    targetAns->sol[8][10] = 1.0f;
    targetAns->sol[10][8] = 1.0f;

    targetAns->sol[9][11] = 0.5f;
    targetAns->sol[11][9] = 0.5f;    
    targetAns->sol[9][10] = 0.5f;
    targetAns->sol[10][9] = 0.5f;
    targetAns->sol[10][11] = 0.5f;
    targetAns->sol[11][10] = 0.5f;

    
    // targetAns->n = 4;
    // targetAns->delta = 0.0f;
    // targetAns->sol = create2dArr(targetAns->n, 0.5f);

     for (int i = 0; i < targetAns->n; i++) {
        targetAns->sol[i][i] = 0.0f;
    }
    // targetAns->sol[0][3] = 0.0f;
    // targetAns->sol[3][0] = 0.0f;
    // targetAns->sol[1][2] = 1.0f;
    // targetAns->sol[2][1] = 1.0f;

    for (int i = 0; i < targetAns->n; i++) {
        for (int j = 0; j < targetAns->n; j++) {
            cout << targetAns->sol[i][j] << " ";
        }
        cout << endl;
    }

    targetAns->T = create2dArr(targetAns->n, false);

    RandomizedPpr rppr = RandomizedPpr();
    rppr.pipageRound(targetAns);
    return 0;
}
