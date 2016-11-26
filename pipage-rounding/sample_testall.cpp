#include <fstream>
#include <string.h>
#include <glob.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <regex>
#include "randomized_ppr.h"

using namespace std;

typedef struct FullyConnInfo {
    int n;
    int** set;
} FullyConnInfo;

int targetIteration = 500;

string FULL_DATA_DIR = "../datas/tsp-full/";

PprAns* readInputFromFile(string inputFilename) {
    int s, t, a, b;
    double w;
    PprAns* targetAns = new PprAns;
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

void writePprAnsToFile(PprAns* targetAns, string outputFilename, FullyConnInfo fullyConnInfo) {
    ofstream outfile(outputFilename);

    outfile << targetAns->n << " " << targetAns->n - 1;
    for (int i = 0; i < targetAns->n; i++) {
        for (int j = i; j <targetAns->n; j++) {
            if (targetAns->sol[i][j] > 0.0f) {
            //outfile << endl << i << " " << j << " " << targetAns->sol[i][j];
            outfile << endl << i << " " << j << " " << fullyConnInfo.set[i][j];
            }
        }
    }
    outfile.close();
}

FullyConnInfo getFullyConnInfo(string fullDataFilename) {
    int nodeNum, edgeNum;
    int a, b, w;
    FullyConnInfo fullyConnInfo;
    ifstream infile(fullDataFilename.c_str());
    infile >> nodeNum >> edgeNum;

    fullyConnInfo.n = nodeNum;
    fullyConnInfo.set = create2dArr<int>(nodeNum, 0);
    while (infile >> a >> b >> w) {
        fullyConnInfo.set[a][b] = w;
    }
    infile.close();

    return fullyConnInfo;
}

void runPipageMultiple(string inputFilepath, string inputFilename, string outputDirname) {
    string fullDataFilename = FULL_DATA_DIR + "rat195.txt";
    FullyConnInfo fullyConnInfo = getFullyConnInfo(fullDataFilename);

    cout << ">>>> run pipage for file : " << inputFilename << "<<<<" << endl;
    for (unsigned int j = 0; j < targetIteration; j++) {
        PprAns* targetAns = readInputFromFile(inputFilepath);
        RandomizedPpr rppr = RandomizedPpr();
        rppr.pipageRound(targetAns);
        string outputFilename = outputDirname + "/" + to_string(j) + ".txt";
        writePprAnsToFile(targetAns, outputFilename, fullyConnInfo);
    }
}

int main() {
    srand(time(NULL));

    string inputDirname = "inputs";
    string outputRootDirname = "outputs";

    // get input file list from dir
    glob_t inputList;
    glob((inputDirname + "/*").c_str(), GLOB_TILDE, NULL, &inputList);
    regex pathRe("(.*/)(.*)\\.txt");
    smatch m;
    string str;
    
    for(unsigned int i = 0; i < inputList.gl_pathc; ++i){
        // parse input file name
        str = inputList.gl_pathv[i];
        regex_search (str, m, pathRe);
        string inputFilename = m[m.size() - 1].str();
        string outputDirname = outputRootDirname + "/" + inputFilename;

        // make dir for output
        mkdir(outputDirname.c_str(), S_IRWXU);

        runPipageMultiple(m[0].str(), inputFilename, outputDirname);

        //cout << "Press Enter to Continue ....";
        //cin.ignore();

    }
    return 0;
}
