#include <fstream>
#include <string.h>
#include <glob.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <regex>
#include "randomized_ppr.h"

using namespace std;

int targetIteration = 500;


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

void writePprAnsToFile(PprAns* targetAns, string outputFilename) {
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
    
    glob_t inputList;
    glob("inputs/*", GLOB_TILDE, NULL, &inputList);
    regex pathRe("(.*/)(.*)\\.txt");
    smatch m;
    string str;
    
    for(unsigned int i = 0; i < inputList.gl_pathc; ++i){
        // parse input file name
        str = inputList.gl_pathv[i];
        regex_search (str, m, pathRe);
        string inputFilename = m[m.size() - 1].str();
        string outputDirname = "outputs/" + inputFilename;
        // make dir for output
        mkdir(outputDirname.c_str(), S_IRWXU);
        
        cout << ">>>> run pipage for file : " << inputFilename << "<<<<" << endl;
        for (unsigned int j = 0; j < targetIteration; j++) {
            cout << "iteration " << j << "..." << endl;
            PprAns* targetAns = readInputFromFile(m[0].str());
            RandomizedPpr rppr = RandomizedPpr();
            rppr.pipageRound(targetAns);
            string outputFilename = outputDirname + "/" + to_string(j) + ".txt";
            writePprAnsToFile(targetAns, outputFilename);
        }
    }
    return 0;
}
