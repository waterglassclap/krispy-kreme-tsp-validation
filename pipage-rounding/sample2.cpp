#include <fstream>
#include <string.h>
#include "randomized_ppr.h"

using namespace std;

char FILENAME[50] = "280.txt";

PprAns* readInputFromFile() {
    int s, t, a, b;
    double w;
    PprAns* targetAns = new PprAns;
    
    char inputFilename[50] = "inputs/";
    strcat(inputFilename, FILENAME);
    ifstream infile(inputFilename);

    infile >> targetAns->n;
    infile >> s >> t;
    targetAns->sol = create2dArr<double>(targetAns->n, 0.0f);
    while (infile >> a >> b >> w) {
        targetAns->sol[a][b] = w;
    }
    infile.close();

    for (int i = 0; i < targetAns->n; i++) {
        targetAns->sol[i][i] = 0.0f;
    }

    targetAns->T = create2dArr<bool>(targetAns->n, true);

    return targetAns;
}

void writePprAnsToFile(PprAns* targetAns) {
    char outputFilename[50] = "outputs/";
    strcat(outputFilename, FILENAME);
    ofstream outfile(outputFilename);

    for (int i = 0; i < targetAns->n; i++) {
        for (int j = 0; j <targetAns->n; j++) {
            if (targetAns->sol[i][j] > 0.0f) {
            outfile << i << " " << j << " " << targetAns->sol[i][j] << endl;
            }
        }
    }

    outfile.close();
}

int main() {
    srand(time(NULL));

    PprAns* targetAns = readInputFromFile();
    RandomizedPpr rppr = RandomizedPpr();
    rppr.pipageRound(targetAns);
    writePprAnsToFile(targetAns);

    return 0;
}
