
#include <math.h>
#include "../../common/utils/common_util.h"
#include "../../common/utils/push_relabel.h"

using namespace std;

typedef struct ErrorInfo {
	MinCutInfo minCutInfo;
	float severity;
	bool isValid;
} ErrorInfo;

typedef struct ErrorInfos {
	struct ErrorInfo* infos;
	int num;
} ErrorInfos;

struct ErrorInfo getStErrorInfo(int s, int t, int n, float** inputGraph) {
	ErrorInfo errorInfo;
	errorInfo.isValid = false;

	PushRelabel verifier(s, t, n, inputGraph);
	MinCutInfo mci = verifier.getMinCutInfo();
	if (!over(mci.totalFlow, 1.0f)) {
		errorInfo.isValid = true;
		errorInfo.minCutInfo = mci;
		errorInfo.severity = 1.0f - mci.totalFlow;
	}
    verifier.freeVars();
	return errorInfo;
}


struct ErrorInfos getNonStErrorInfos(int s, int t, int n, float** inputGraph) {
	ErrorInfos errorInfos;
	errorInfos.infos = new ErrorInfo[n];
	errorInfos.num = 0;

	if (s == t) {
	    throw runtime_error("s should not be equal to t. terminate.");
	}

	float** mergedGraph = getMergedGraph(s, t, n, inputGraph);

    for (int i = 0; i < n - 1; i++) {
    	if (i != s) {
    		PushRelabel verifier(0, i, n - 1, mergedGraph);
    		MinCutInfo mci = verifier.getMinCutInfo();
            if (!over(mci.totalFlow, 2.0f)) {
				errorInfos.infos[errorInfos.num].minCutInfo = mci;
				errorInfos.infos[errorInfos.num].severity = 2.0f - mci.totalFlow;
				errorInfos.num++;
			}
            verifier.freeVars();
    	}
    }
    //for (int i = 0; i < n; i++) {
    //    delete[] mergedGraph[i];
    //}
    //delete mergedGraph;
    //free2dArr(n, mergedGraph);
    return errorInfos;
}
/*
int main() {
	int s = 0, t = 5;
	int n = 6;
	float** arr = new float*[n];
	for(int i = 0; i < n; ++i) {
    	arr[i] = new float[n];
    	for (int j = 0; j < n; ++j) {
    		arr[i][j] = 0.0f;
    	}
	}

	arr[0][1] = 0.11f;
	arr[0][2] = 0.12f;
	arr[2][1] = 0.01f;
	arr[1][3] = 0.12f;
	arr[2][4] = 0.11f;
	arr[4][3] = 0.07f;
	arr[3][5] = 0.19f;
	arr[4][5] = 0.04f;

	ErrorInfo stErrorInfo = getStErrorInfo(s, t, n, arr);
	ErrorInfos nonStErrorInfos = getNonStErrorInfos(s, t, n, arr);

	if (stErrorInfo.isValid) {
		cout << "st error severity is : " << stErrorInfo.severity << endl;
	}
	for (int i = 0; i < nonStErrorInfos.num; i++) {
		cout << "non st error severity [" << i << "] is : " << nonStErrorInfos.infos[i].severity << endl;
	}
	
	return 0;
}*/

// int main (int argc, char **argv) {
//     const int start_node = 0, end_node = 28;
    
//     float** solution = new float*[29];
//     for(int i=0; i<29; i++) {
//         solution[i] = new float[29];
//         for (int j = 0; j < 29; ++j) {
//             solution[i][j] = 0.0f;
//         }
//     }
    
//     solution[0][27] = 1;
//     solution[1][19] = 1;
//     solution[1][20] = 1;
//     solution[2][25] = 1;
//     solution[2][28] = 1;
//     solution[3][9] = 1;
//     solution[3][14] = 1;
//     solution[4][8] = 1;
//     solution[4][20] = 1;
//     solution[5][11] = 1;
//     solution[5][27] = 1;
//     solution[6][22] = 1;
//     solution[6][24] = 1;
//     solution[7][23] = 1;
//     solution[7][26] = 1;
//     solution[8][4] = 1;
//     solution[8][11] = 1;
//     solution[9][3] = 1;
//     solution[9][19] = 1;
//     solution[10][14] = 1;
//     solution[10][21] = 1;
//     solution[11][5] = 1;
//     solution[11][8] = 1;
//     solution[12][15] = 1;
//     solution[12][23] = 1;
//     solution[13][17] = 1;
//     solution[14][3] = 1;
//     solution[14][10] = 1;
//     solution[15][12] = 1;
//     solution[15][18] = 1;
//     solution[16][17] = 1;
//     solution[16][21] = 1;
//     solution[17][13] = 1;
//     solution[17][16] = 1;
//     solution[18][15] = 1;
//     solution[18][24] = 1;
//     solution[19][1] = 1;
//     solution[19][9] = 1;
//     solution[20][1] = 1;
//     solution[20][4] = 1;
//     solution[21][10] = 1;
//     solution[21][16] = 1;
//     solution[22][6] = 1;
//     solution[22][26] = 1;
//     solution[23][7] = 1;
//     solution[23][12] = 1;
//     solution[24][6] = 1;
//     solution[24][18] = 1;
//     solution[25][2] = 1;
//     solution[25][28] = 1;
//     solution[26][7] = 1;
//     solution[26][22] = 1;
//     solution[27][0] = 1;
//     solution[27][5] = 1;
//     solution[28][2] = 1;
//     solution[28][25] = 1;
//     solution[27][0] = 1;
//     solution[19][1] = 1;
//     solution[20][1] = 1;
//     solution[25][2] = 1;
//     solution[28][2] = 1;
//     solution[9][3] = 1;
//     solution[14][3] = 1;
//     solution[8][4] = 1;
//     solution[20][4] = 1;
//     solution[11][5] = 1;
//     solution[27][5] = 1;
//     solution[22][6] = 1;
//     solution[24][6] = 1;
//     solution[23][7] = 1;
//     solution[26][7] = 1;
//     solution[4][8] = 1;
//     solution[11][8] = 1;
//     solution[3][9] = 1;
//     solution[19][9] = 1;
//     solution[14][10] = 1;
//     solution[21][10] = 1;
//     solution[5][11] = 1;
//     solution[8][11] = 1;
//     solution[15][12] = 1;
//     solution[23][12] = 1;
//     solution[17][13] = 1;
//     solution[3][14] = 1;
//     solution[10][14] = 1;
//     solution[12][15] = 1;
//     solution[18][15] = 1;
//     solution[17][16] = 1;
//     solution[21][16] = 1;
//     solution[13][17] = 1;
//     solution[16][17] = 1;
//     solution[15][18] = 1;
//     solution[24][18] = 1;
//     solution[1][19] = 1;
//     solution[9][19] = 1;
//     solution[1][20] = 1;
//     solution[4][20] = 1;
//     solution[10][21] = 1;
//     solution[16][21] = 1;
//     solution[6][22] = 1;
//     solution[26][22] = 1;
//     solution[7][23] = 1;
//     solution[12][23] = 1;
//     solution[6][24] = 1;
//     solution[18][24] = 1;
//     solution[2][25] = 1;
//     solution[28][25] = 1;
//     solution[7][26] = 1;
//     solution[22][26] = 1;
//     solution[0][27] = 1;
//     solution[5][27] = 1;
//     solution[2][28] = 1;
//     solution[25][28] = 1;
    
//     ErrorInfo stErrorInfo = getStErrorInfo(start_node, end_node, 29, solution);
//     //ErrorInfos nonStErrorInfos = getNonStErrorInfos(start_node, end_node, 29, solution);
    
//     ErrorInfo maxErrorInfo;
//     maxErrorInfo.severity = -1;
//     bool isST = false;
    
    
//     cout << endl << endl << endl << endl;
//     if (stErrorInfo.isValid) {
//         maxErrorInfo = stErrorInfo;
//         isST = true;
//         cout << "severity st: " << stErrorInfo.severity << " " << stErrorInfo.minCutInfo.size << endl;
//     }
//     /*
//     for (int i = 0; i < nonStErrorInfos.num; i++) {
//         if(nonStErrorInfos.infos[i].severity > maxErrorInfo.severity) {
//             maxErrorInfo = nonStErrorInfos.infos[i];
//             isST = false;
//             cout << "severity " << i << "st: " << nonStErrorInfos.infos[i].severity << " " << nonStErrorInfos.infos[i].minCutInfo.size << endl;
//         }
//     }
    
//     if(maxErrorInfo.severity == -1) {
//         cout << "there is no StErrorInfos" << endl;
//         return false;
//     }
    
//     cout << "maxError mincut size info" << endl;
//     cout << "serverity: " << maxErrorInfo.severity << endl << "minCutSize: " << maxErrorInfo.minCutInfo.size << endl << "totalFlow: " << maxErrorInfo.minCutInfo.totalFlow << endl;
//     */
//     return 0;
// }

int main (int argc, char **argv)
{
    const int start_node = 0, end_node = 28;
    
    float** solution = new float*[29];
    for(int i=0; i<29; i++) {
        solution[i] = new float[29];
        for (int j = 0; j < 29; ++j) {
            solution[i][j] = 0.0f;
        }
    }
    
    solution[0][27] = 1;
    solution[1][19] = 1;
    solution[1][20] = 1;
    solution[2][25] = 1;
    solution[2][28] = 1;
    solution[3][9] = 1;
    solution[3][14] = 1;
    solution[4][8] = 1;
    solution[4][20] = 1;
    solution[5][11] = 1;
    solution[5][27] = 1;
    solution[6][22] = 1;
    solution[6][24] = 1;
    solution[7][23] = 1;
    solution[7][26] = 1;
    solution[8][4] = 1;
    solution[8][11] = 1;
    solution[9][3] = 1;
    solution[9][19] = 1;
    solution[10][14] = 1;
    solution[10][21] = 1;
    solution[11][5] = 1;
    solution[11][8] = 1;
    solution[12][15] = 1;
    solution[12][23] = 1;
    solution[13][17] = 1;
    solution[14][3] = 1;
    solution[14][10] = 1;
    solution[15][12] = 1;
    solution[15][18] = 1;
    solution[16][17] = 1;
    solution[16][21] = 1;
    solution[17][13] = 1;
    solution[17][16] = 1;
    solution[18][15] = 1;
    solution[18][24] = 1;
    solution[19][1] = 1;
    solution[19][9] = 1;
    solution[20][1] = 1;
    solution[20][4] = 1;
    solution[21][10] = 1;
    solution[21][16] = 1;
    solution[22][6] = 1;
    solution[22][26] = 1;
    solution[23][7] = 1;
    solution[23][12] = 1;
    solution[24][6] = 1;
    solution[24][18] = 1;
    solution[25][2] = 1;
    solution[25][28] = 1;
    solution[26][7] = 1;
    solution[26][22] = 1;
    solution[27][0] = 1;
    solution[27][5] = 1;
    solution[28][2] = 1;
    solution[28][25] = 1;
    solution[27][0] = 1;
    solution[19][1] = 1;
    solution[20][1] = 1;
    solution[25][2] = 1;
    solution[28][2] = 1;
    solution[9][3] = 1;
    solution[14][3] = 1;
    solution[8][4] = 1;
    solution[20][4] = 1;
    solution[11][5] = 1;
    solution[27][5] = 1;
    solution[22][6] = 1;
    solution[24][6] = 1;
    solution[23][7] = 1;
    solution[26][7] = 1;
    solution[4][8] = 1;
    solution[11][8] = 1;
    solution[3][9] = 1;
    solution[19][9] = 1;
    solution[14][10] = 1;
    solution[21][10] = 1;
    solution[5][11] = 1;
    solution[8][11] = 1;
    solution[15][12] = 1;
    solution[23][12] = 1;
    solution[17][13] = 1;
    solution[3][14] = 1;
    solution[10][14] = 1;
    solution[12][15] = 1;
    solution[18][15] = 1;
    solution[17][16] = 1;
    solution[21][16] = 1;
    solution[13][17] = 1;
    solution[16][17] = 1;
    solution[15][18] = 1;
    solution[24][18] = 1;
    solution[1][19] = 1;
    solution[9][19] = 1;
    solution[1][20] = 1;
    solution[4][20] = 1;
    solution[10][21] = 1;
    solution[16][21] = 1;
    solution[6][22] = 1;
    solution[26][22] = 1;
    solution[7][23] = 1;
    solution[12][23] = 1;
    solution[6][24] = 1;
    solution[18][24] = 1;
    solution[2][25] = 1;
    solution[28][25] = 1;
    solution[7][26] = 1;
    solution[22][26] = 1;
    solution[0][27] = 1;
    solution[5][27] = 1;
    solution[2][28] = 1;
    solution[25][28] = 1;
    
    ErrorInfo stErrorInfo = getStErrorInfo(start_node, end_node, 29, solution);
    ErrorInfos nonStErrorInfos = getNonStErrorInfos(start_node, end_node, 29, solution);
    
    ErrorInfo maxErrorInfo;
    maxErrorInfo.severity = -1;
    bool isST = false;
    
    cout << endl << endl << endl << endl;
    if (stErrorInfo.isValid) {
        maxErrorInfo = stErrorInfo;
        isST = true;
        cout << "severity st: " << stErrorInfo.severity << " " << stErrorInfo.minCutInfo.size << endl;
    }
    for (int i = 0; i < nonStErrorInfos.num; i++) {
        if(nonStErrorInfos.infos[i].severity > maxErrorInfo.severity) {
            maxErrorInfo = nonStErrorInfos.infos[i];
            isST = false;
            cout << "severity " << i << "st: " << nonStErrorInfos.infos[i].severity << " " << nonStErrorInfos.infos[i].minCutInfo.size << endl;
        }
    }
    
    if(maxErrorInfo.severity == -1) {
        cout << "there is no StErrorInfos" << endl;
        return false;
    }
    
    cout << "maxError mincut size info" << endl;
    cout << "serverity: " << maxErrorInfo.severity << endl << "minCutSize: " << maxErrorInfo.minCutInfo.size << endl << "totalFlow: " << maxErrorInfo.minCutInfo.totalFlow << endl;
    
    
    cout << endl << endl << endl << endl;
    for(int i = 0; i < 29; ++i) {
        for(int j = 0; j < 29; ++j) {
            cout << solution[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl << endl << endl << endl;
    
    
    return 0;
}

