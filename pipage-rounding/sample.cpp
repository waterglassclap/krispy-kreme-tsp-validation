#include "randomized_ppr.h"

using namespace std;

int main() {
    srand(time(NULL));

    PprAns* targetAns = new PprAns;

    targetAns->n = 6;
    targetAns->delta = 0.0f;
    targetAns->sol = create2dArr<double>(targetAns->n, 0.0f);

    targetAns->sol[0][1] = 0.5f;
    targetAns->sol[0][4] = 0.25f;
    targetAns->sol[0][5] = 1.0f;
    targetAns->sol[1][2] = 0.5f;
    targetAns->sol[1][3] = 0.25f;
    targetAns->sol[1][4] = 0.5f;
    targetAns->sol[1][5] = 0.25f;
    targetAns->sol[2][3] = 1.0f;
    targetAns->sol[2][4] = 0.25f;
    targetAns->sol[4][5] = 0.5f;

    targetAns->sol[1][0] = 0.5f;
    targetAns->sol[4][0] = 0.25f;
    targetAns->sol[5][0] = 1.0f;
    targetAns->sol[2][1] = 0.5f;
    targetAns->sol[3][1] = 0.25f;
    targetAns->sol[4][1] = 0.5f;
    targetAns->sol[5][1] = 0.25f;
    targetAns->sol[3][2] = 1.0f;
    targetAns->sol[4][2] = 0.25f;
    targetAns->sol[5][4] = 0.5f;


//     targetAns->n = 29;
//     targetAns->delta = 0.0f;
//     targetAns->sol = create2dArr<double>(targetAns->n, 0.0f);

// targetAns->sol[0][27] = 1;
// targetAns->sol[1][2] = 1;
// targetAns->sol[1][20] = 1;
// targetAns->sol[2][28] = 1;
// targetAns->sol[3][9] = 0.5;
// targetAns->sol[3][14] = 1;
// targetAns->sol[3][19] = 0.5;
// targetAns->sol[4][8] = 1;
// targetAns->sol[4][25] = 1;
// targetAns->sol[5][11] = 1;
// targetAns->sol[5][27] = 1;
// targetAns->sol[6][22] = 1;
// targetAns->sol[6][24] = 1;
// targetAns->sol[7][23] = 1;
// targetAns->sol[7][26] = 1;
// targetAns->sol[8][11] = 1;
// targetAns->sol[9][12] = 0.5;
// targetAns->sol[9][19] = 1;
// targetAns->sol[10][18] = 0.5;
// targetAns->sol[10][21] = 1;
// targetAns->sol[10][24] = 0.5;
// targetAns->sol[12][15] = 1;
// targetAns->sol[12][20] = 0.5;
// targetAns->sol[13][16] = 1;
// targetAns->sol[13][21] = 1;
// targetAns->sol[14][17] = 1;
// targetAns->sol[15][18] = 1;
// targetAns->sol[16][17] = 1;
// targetAns->sol[18][24] = 0.5;
// targetAns->sol[19][20] = 0.5;
// targetAns->sol[22][26] = 1;
// targetAns->sol[25][28] = 1;


// targetAns->sol[27][0] = 1;
// targetAns->sol[2][1] = 1;
// targetAns->sol[20][1] = 1;
// targetAns->sol[28][2] = 1;
// targetAns->sol[9][3] = 0.5;
// targetAns->sol[14][3] = 1;
// targetAns->sol[19][3] = 0.5;
// targetAns->sol[8][4] = 1;
// targetAns->sol[25][4] = 1;
// targetAns->sol[11][5] = 1;
// targetAns->sol[27][5] = 1;
// targetAns->sol[22][6] = 1;
// targetAns->sol[24][6] = 1;
// targetAns->sol[23][7] = 1;
// targetAns->sol[26][7] = 1;
// targetAns->sol[11][8] = 1;
// targetAns->sol[12][9] = 0.5;
// targetAns->sol[19][9] = 1;
// targetAns->sol[18][10] = 0.5;
// targetAns->sol[21][10] = 1;
// targetAns->sol[24][10] = 0.5;
// targetAns->sol[15][12] = 1;
// targetAns->sol[20][12] = 0.5;
// targetAns->sol[16][13] = 1;
// targetAns->sol[21][13] = 1;
// targetAns->sol[17][14] = 1;
// targetAns->sol[18][15] = 1;
// targetAns->sol[17][16] = 1;
// targetAns->sol[24][18] = 0.5;
// targetAns->sol[20][19] = 0.5;
// targetAns->sol[26][22] = 1;
// targetAns->sol[28][25] = 1;

     for (int i = 0; i < targetAns->n; i++) {
        targetAns->sol[i][i] = 0.0f;
    }
    targetAns->T = create2dArr<bool>(targetAns->n, true);

    RandomizedPpr rppr = RandomizedPpr();
    rppr.pipageRound(targetAns);
    return 0;
}
