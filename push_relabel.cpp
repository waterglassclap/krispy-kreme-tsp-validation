#include <cstdlib>
#include <cstdio>
#include <limits>

#define NODES 280
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define SOURCE 8
#define SINK 42

void push(const float * const * C, float ** F, float *excess, int u, int v) {
  float send = MIN(excess[u], C[u][v] - F[u][v]);
  F[u][v] += send;
  F[v][u] -= send;
  excess[u] -= send;
  excess[v] += send;
}

void relabel(const float * const * C, const float * const * F, int *height, int u) {
  int v;
  int min_height = std::numeric_limits<int>::max();
  for (v = 0; v < NODES; v++) {
    if (C[u][v] - F[u][v] > 0) {
      min_height = MIN(min_height, height[v]);
      height[u] = min_height + 1;
    }
  }
};

void discharge(const float * const * C, float ** F, float *excess, int *height, int *seen, int u) {
  while (excess[u] > 0) {
    if (seen[u] < NODES) {
      int v = seen[u];
      if ((C[u][v] - F[u][v] > 0) && (height[u] > height[v])){
    push(C, F, excess, u, v);
      }
      else
    seen[u] += 1;
    } else {
      relabel(C, F, height, u);
      seen[u] = 0;
    }
  }
}

void moveToFront(int i, int *A) {
  int temp = A[i];
  int n;
  for (n = i; n > 0; n--){
    A[n] = A[n-1];
  }
  A[0] = temp;
}

int pushRelabel(const float * const * C, float ** F, int source, int sink) {
  float *excess;
  int *height, *list, *seen, i, p;

  excess = (float *) calloc(NODES, sizeof(float));
  height = (int *) calloc(NODES, sizeof(int));
  seen = (int *) calloc(NODES, sizeof(int));

  list = (int *) calloc((NODES-2), sizeof(int));

  for (i = 0, p = 0; i < NODES; i++){
    if((i != source) && (i != sink)) {
      list[p] = i;
      p++;
    }
  }

  height[source] = NODES;
  excess[source] = std::numeric_limits<float>::max();
  for (i = 0; i < NODES; i++)
    push(C, F, excess, source, i);

  p = 0;
  while (p < NODES - 2) {
    int u = list[p];
    int old_height = height[u];
    discharge(C, F, excess, height, seen, u);
    if (height[u] > old_height) {
      moveToFront(p,list);
      p=0;
    }
    else
      p += 1;
  }
  float maxflow = 0;
  for (i = 0; i < NODES; i++)
    maxflow += F[source][i];


  printf("%s\n", "Heights:");
  for(i=0;i<NODES;i++){
    printf("%d ", height[i]);
  }
  printf("\n");



  free(list);

  free(seen);
  free(height);
  free(excess);

  

  return maxflow;
}

void printMatrix(const float * const * M) {
  int i,j;
  for (i = 0; i < NODES; i++) {
    for (j = 0; j < NODES; j++)
      printf("%f\t",M[i][j]);
    printf("\n");
  }
}

int main(void) {
  float **flow, **inputGraph;
  int i;
  flow = (float **) calloc(NODES, sizeof(float*));
  inputGraph = (float **) calloc(NODES, sizeof(float*));
  for (i = 0; i < NODES; i++) {
    flow[i] = (float *) calloc(NODES, sizeof(float));
    inputGraph[i] = (float *) calloc(NODES, sizeof(float));
  }

  //Sample graph
  inputGraph[0][1] = 1;
  inputGraph[0][2] = 0.5;
  inputGraph[0][279] = 0.5;
  inputGraph[1][0] = 1;
  inputGraph[1][279] = 1;
  inputGraph[2][0] = 0.5;
  inputGraph[2][278] = 1;
  inputGraph[2][279] = 0.5;
  inputGraph[3][4] = 1;
  inputGraph[3][276] = 1;
  inputGraph[4][3] = 1;
  inputGraph[4][5] = 1;
  inputGraph[5][4] = 1;
  inputGraph[5][6] = 1;
  inputGraph[6][5] = 1;
  inputGraph[6][7] = 0.5;
  inputGraph[6][8] = 0.5;
  inputGraph[7][6] = 0.5;
  inputGraph[7][8] = 0.5;
  inputGraph[7][9] = 1;
  inputGraph[8][6] = 0.5;
  inputGraph[8][7] = 0.5;
  inputGraph[9][7] = 1;
  inputGraph[9][10] = 1;
  inputGraph[10][9] = 1;
  inputGraph[10][11] = 1;
  inputGraph[11][10] = 1;
  inputGraph[11][12] = 1;
  inputGraph[12][11] = 1;
  inputGraph[12][14] = 1;
  inputGraph[13][14] = 1;
  inputGraph[13][23] = 1;
  inputGraph[14][12] = 1;
  inputGraph[14][13] = 1;
  inputGraph[15][16] = 1;
  inputGraph[15][270] = 1;
  inputGraph[16][15] = 1;
  inputGraph[16][17] = 1;
  inputGraph[17][16] = 1;
  inputGraph[17][132] = 1;
  inputGraph[18][19] = 0.5;
  inputGraph[18][131] = 1;
  inputGraph[18][132] = 0.5;
  inputGraph[19][18] = 0.5;
  inputGraph[19][20] = 1;
  inputGraph[19][130] = 0.5;
  inputGraph[20][19] = 1;
  inputGraph[20][127] = 1;
  inputGraph[21][24] = 1;
  inputGraph[21][25] = 1;
  inputGraph[22][23] = 1;
  inputGraph[22][24] = 1;
  inputGraph[23][13] = 1;
  inputGraph[23][22] = 1;
  inputGraph[24][21] = 1;
  inputGraph[24][22] = 1;
  inputGraph[25][21] = 1;
  inputGraph[25][26] = 1;
  inputGraph[26][25] = 1;
  inputGraph[26][27] = 1;
  inputGraph[27][26] = 1;
  inputGraph[27][28] = 1;
  inputGraph[28][27] = 1;
  inputGraph[28][125] = 1;
  inputGraph[29][30] = 1;
  inputGraph[29][124] = 1;
  inputGraph[30][29] = 1;
  inputGraph[30][31] = 1;
  inputGraph[31][30] = 1;
  inputGraph[31][32] = 1;
  inputGraph[32][31] = 1;
  inputGraph[32][33] = 1;
  inputGraph[33][32] = 1;
  inputGraph[33][34] = 1;
  inputGraph[34][33] = 1;
  inputGraph[34][38] = 1;
  inputGraph[35][36] = 1;
  inputGraph[35][37] = 1;
  inputGraph[36][35] = 1;
  inputGraph[36][37] = 1;
  inputGraph[37][35] = 1;
  inputGraph[37][36] = 1;
  inputGraph[38][34] = 1;
  inputGraph[38][39] = 1;
  inputGraph[39][38] = 1;
  inputGraph[39][40] = 1;
  inputGraph[40][39] = 1;
  inputGraph[40][41] = 1;
  inputGraph[41][40] = 1;
  inputGraph[41][42] = 0.5;
  inputGraph[41][59] = 0.5;
  inputGraph[42][41] = 0.5;
  inputGraph[42][59] = 0.5;
  inputGraph[43][56] = 1;
  inputGraph[43][58] = 1;
  inputGraph[44][45] = 1;
  inputGraph[44][55] = 1;
  inputGraph[45][44] = 1;
  inputGraph[45][54] = 1;
  inputGraph[46][47] = 1;
  inputGraph[46][53] = 1;
  inputGraph[47][46] = 1;
  inputGraph[47][52] = 1;
  inputGraph[48][49] = 1;
  inputGraph[48][51] = 1;
  inputGraph[49][48] = 1;
  inputGraph[49][50] = 1;
  inputGraph[50][49] = 1;
  inputGraph[50][51] = 1;
  inputGraph[51][48] = 1;
  inputGraph[51][50] = 1;
  inputGraph[52][47] = 1;
  inputGraph[52][53] = 1;
  inputGraph[53][46] = 1;
  inputGraph[53][52] = 1;
  inputGraph[54][45] = 1;
  inputGraph[54][55] = 1;
  inputGraph[55][44] = 1;
  inputGraph[55][54] = 1;
  inputGraph[56][43] = 1;
  inputGraph[56][57] = 1;
  inputGraph[57][56] = 1;
  inputGraph[57][58] = 1;
  inputGraph[58][43] = 1;
  inputGraph[58][57] = 1;
  inputGraph[59][41] = 0.5;
  inputGraph[59][42] = 0.5;
  inputGraph[59][60] = 1;
  inputGraph[60][59] = 1;
  inputGraph[60][61] = 0.5;
  inputGraph[60][117] = 0.5;
  inputGraph[61][60] = 0.5;
  inputGraph[61][62] = 1;
  inputGraph[61][117] = 0.5;
  inputGraph[62][61] = 1;
  inputGraph[62][63] = 1;
  inputGraph[63][62] = 1;
  inputGraph[63][64] = 1;
  inputGraph[64][63] = 1;
  inputGraph[64][65] = 1;
  inputGraph[65][64] = 1;
  inputGraph[65][66] = 1;
  inputGraph[66][65] = 1;
  inputGraph[66][67] = 1;
  inputGraph[67][66] = 1;
  inputGraph[67][68] = 1;
  inputGraph[68][67] = 1;
  inputGraph[68][69] = 1;
  inputGraph[69][68] = 1;
  inputGraph[69][70] = 1;
  inputGraph[70][69] = 1;
  inputGraph[70][71] = 1;
  inputGraph[71][70] = 1;
  inputGraph[71][72] = 1;
  inputGraph[72][71] = 1;
  inputGraph[72][73] = 1;
  inputGraph[73][72] = 1;
  inputGraph[73][75] = 1;
  inputGraph[74][75] = 1;
  inputGraph[74][76] = 1;
  inputGraph[75][73] = 1;
  inputGraph[75][74] = 1;
  inputGraph[76][74] = 1;
  inputGraph[76][77] = 1;
  inputGraph[77][76] = 1;
  inputGraph[77][78] = 1;
  inputGraph[78][77] = 1;
  inputGraph[78][79] = 1;
  inputGraph[79][78] = 1;
  inputGraph[79][80] = 1;
  inputGraph[80][79] = 1;
  inputGraph[80][81] = 1;
  inputGraph[81][80] = 1;
  inputGraph[81][82] = 1;
  inputGraph[82][81] = 1;
  inputGraph[82][87] = 1;
  inputGraph[83][84] = 1;
  inputGraph[83][86] = 1;
  inputGraph[84][83] = 1;
  inputGraph[84][85] = 1;
  inputGraph[85][84] = 1;
  inputGraph[85][115] = 1;
  inputGraph[86][83] = 1;
  inputGraph[86][112] = 1;
  inputGraph[87][82] = 1;
  inputGraph[87][111] = 1;
  inputGraph[88][89] = 1;
  inputGraph[88][108] = 1;
  inputGraph[89][88] = 1;
  inputGraph[89][90] = 1;
  inputGraph[90][89] = 1;
  inputGraph[90][91] = 1;
  inputGraph[91][90] = 1;
  inputGraph[91][98] = 1;
  inputGraph[92][93] = 1;
  inputGraph[92][97] = 1;
  inputGraph[93][92] = 1;
  inputGraph[93][94] = 1;
  inputGraph[94][93] = 1;
  inputGraph[94][95] = 1;
  inputGraph[95][94] = 1;
  inputGraph[95][96] = 1;
  inputGraph[96][95] = 1;
  inputGraph[96][97] = 1;
  inputGraph[97][92] = 1;
  inputGraph[97][96] = 1;
  inputGraph[98][91] = 1;
  inputGraph[98][99] = 1;
  inputGraph[99][98] = 1;
  inputGraph[99][100] = 1;
  inputGraph[100][99] = 1;
  inputGraph[100][101] = 0.5;
  inputGraph[100][168] = 0.5;
  inputGraph[101][100] = 0.5;
  inputGraph[101][102] = 1;
  inputGraph[101][168] = 0.5;
  inputGraph[102][101] = 1;
  inputGraph[102][103] = 1;
  inputGraph[103][102] = 1;
  inputGraph[103][104] = 1;
  inputGraph[104][103] = 1;
  inputGraph[104][105] = 0.5;
  inputGraph[104][106] = 0.5;
  inputGraph[105][104] = 0.5;
  inputGraph[105][106] = 0.5;
  inputGraph[105][172] = 1;
  inputGraph[106][104] = 0.5;
  inputGraph[106][105] = 0.5;
  inputGraph[106][173] = 1;
  inputGraph[107][108] = 1;
  inputGraph[107][109] = 1;
  inputGraph[108][88] = 1;
  inputGraph[108][107] = 1;
  inputGraph[109][107] = 1;
  inputGraph[109][110] = 1;
  inputGraph[110][109] = 1;
  inputGraph[110][111] = 1;
  inputGraph[111][87] = 1;
  inputGraph[111][110] = 1;
  inputGraph[112][86] = 1;
  inputGraph[112][113] = 1;
  inputGraph[113][112] = 1;
  inputGraph[113][114] = 1;
  inputGraph[114][113] = 1;
  inputGraph[114][115] = 0.5;
  inputGraph[114][116] = 0.5;
  inputGraph[115][85] = 1;
  inputGraph[115][114] = 0.5;
  inputGraph[115][116] = 0.5;
  inputGraph[116][114] = 0.5;
  inputGraph[116][115] = 0.5;
  inputGraph[116][117] = 1;
  inputGraph[117][60] = 0.5;
  inputGraph[117][61] = 0.5;
  inputGraph[117][116] = 1;
  inputGraph[118][119] = 1;
  inputGraph[118][156] = 1;
  inputGraph[119][118] = 1;
  inputGraph[119][120] = 1;
  inputGraph[120][119] = 1;
  inputGraph[120][121] = 1;
  inputGraph[121][120] = 1;
  inputGraph[121][122] = 1;
  inputGraph[122][121] = 1;
  inputGraph[122][123] = 1;
  inputGraph[123][122] = 1;
  inputGraph[123][124] = 1;
  inputGraph[124][29] = 1;
  inputGraph[124][123] = 1;
  inputGraph[125][28] = 1;
  inputGraph[125][126] = 1;
  inputGraph[126][125] = 1;
  inputGraph[126][127] = 1;
  inputGraph[127][20] = 1;
  inputGraph[127][126] = 1;
  inputGraph[128][129] = 1;
  inputGraph[128][153] = 1;
  inputGraph[129][128] = 1;
  inputGraph[129][130] = 1;
  inputGraph[130][19] = 0.5;
  inputGraph[130][129] = 1;
  inputGraph[130][131] = 0.5;
  inputGraph[131][18] = 1;
  inputGraph[131][130] = 0.5;
  inputGraph[131][132] = 0.5;
  inputGraph[132][17] = 1;
  inputGraph[132][18] = 0.5;
  inputGraph[132][131] = 0.5;
  inputGraph[133][134] = 1;
  inputGraph[133][269] = 1;
  inputGraph[134][133] = 1;
  inputGraph[134][268] = 1;
  inputGraph[135][136] = 1;
  inputGraph[135][267] = 1;
  inputGraph[136][135] = 1;
  inputGraph[136][137] = 1;
  inputGraph[137][136] = 1;
  inputGraph[137][138] = 1;
  inputGraph[138][137] = 1;
  inputGraph[138][139] = 1;
  inputGraph[139][138] = 1;
  inputGraph[139][140] = 1;
  inputGraph[140][139] = 1;
  inputGraph[140][141] = 1;
  inputGraph[141][140] = 1;
  inputGraph[141][146] = 1;
  inputGraph[142][143] = 1;
  inputGraph[142][145] = 1;
  inputGraph[143][142] = 1;
  inputGraph[143][199] = 1;
  inputGraph[144][145] = 1;
  inputGraph[144][198] = 1;
  inputGraph[145][142] = 1;
  inputGraph[145][144] = 1;
  inputGraph[146][141] = 1;
  inputGraph[146][147] = 1;
  inputGraph[147][146] = 1;
  inputGraph[147][148] = 1;
  inputGraph[148][147] = 1;
  inputGraph[148][149] = 1;
  inputGraph[149][148] = 1;
  inputGraph[149][178] = 1;
  inputGraph[150][151] = 1;
  inputGraph[150][177] = 1;
  inputGraph[151][150] = 1;
  inputGraph[151][155] = 1;
  inputGraph[152][154] = 1;
  inputGraph[152][155] = 1;
  inputGraph[153][128] = 1;
  inputGraph[153][154] = 1;
  inputGraph[154][152] = 1;
  inputGraph[154][153] = 1;
  inputGraph[155][151] = 1;
  inputGraph[155][152] = 1;
  inputGraph[156][118] = 1;
  inputGraph[156][157] = 1;
  inputGraph[157][156] = 1;
  inputGraph[157][158] = 1;
  inputGraph[158][157] = 1;
  inputGraph[158][159] = 1;
  inputGraph[159][158] = 1;
  inputGraph[159][174] = 1;
  inputGraph[160][161] = 1;
  inputGraph[160][174] = 1;
  inputGraph[161][160] = 1;
  inputGraph[161][162] = 1;
  inputGraph[162][161] = 1;
  inputGraph[162][163] = 1;
  inputGraph[163][162] = 1;
  inputGraph[163][164] = 1;
  inputGraph[164][163] = 1;
  inputGraph[164][165] = 1;
  inputGraph[165][164] = 1;
  inputGraph[165][166] = 1;
  inputGraph[166][165] = 1;
  inputGraph[166][167] = 1;
  inputGraph[167][166] = 1;
  inputGraph[167][168] = 1;
  inputGraph[168][100] = 0.5;
  inputGraph[168][101] = 0.5;
  inputGraph[168][167] = 1;
  inputGraph[169][170] = 1;
  inputGraph[169][171] = 1;
  inputGraph[170][169] = 1;
  inputGraph[170][171] = 1;
  inputGraph[171][169] = 1;
  inputGraph[171][170] = 1;
  inputGraph[172][105] = 1;
  inputGraph[172][173] = 1;
  inputGraph[173][106] = 1;
  inputGraph[173][172] = 1;
  inputGraph[174][159] = 1;
  inputGraph[174][160] = 1;
  inputGraph[175][176] = 1;
  inputGraph[175][180] = 1;
  inputGraph[176][175] = 1;
  inputGraph[176][177] = 1;
  inputGraph[177][150] = 1;
  inputGraph[177][176] = 1;
  inputGraph[178][149] = 1;
  inputGraph[178][179] = 1;
  inputGraph[179][178] = 1;
  inputGraph[179][180] = 0.5;
  inputGraph[179][181] = 0.5;
  inputGraph[180][175] = 1;
  inputGraph[180][179] = 0.5;
  inputGraph[180][181] = 0.5;
  inputGraph[181][179] = 0.5;
  inputGraph[181][180] = 0.5;
  inputGraph[181][182] = 1;
  inputGraph[182][181] = 1;
  inputGraph[182][183] = 1;
  inputGraph[183][182] = 1;
  inputGraph[183][184] = 1;
  inputGraph[184][183] = 1;
  inputGraph[184][185] = 0.5;
  inputGraph[184][186] = 0.5;
  inputGraph[185][184] = 0.5;
  inputGraph[185][186] = 1;
  inputGraph[185][189] = 0.5;
  inputGraph[186][184] = 0.5;
  inputGraph[186][185] = 1;
  inputGraph[186][187] = 0.5;
  inputGraph[187][186] = 0.5;
  inputGraph[187][188] = 1;
  inputGraph[187][189] = 0.5;
  inputGraph[188][187] = 1;
  inputGraph[188][189] = 1;
  inputGraph[189][185] = 0.5;
  inputGraph[189][187] = 0.5;
  inputGraph[189][188] = 1;
  inputGraph[190][191] = 1;
  inputGraph[190][192] = 1;
  inputGraph[191][190] = 1;
  inputGraph[191][192] = 1;
  inputGraph[192][190] = 1;
  inputGraph[192][191] = 1;
  inputGraph[193][194] = 1;
  inputGraph[193][196] = 1;
  inputGraph[194][193] = 1;
  inputGraph[194][195] = 1;
  inputGraph[195][194] = 1;
  inputGraph[195][200] = 1;
  inputGraph[196][193] = 1;
  inputGraph[196][197] = 1;
  inputGraph[197][196] = 1;
  inputGraph[197][198] = 1;
  inputGraph[198][144] = 1;
  inputGraph[198][197] = 1;
  inputGraph[199][143] = 1;
  inputGraph[199][200] = 0.5;
  inputGraph[199][201] = 0.5;
  inputGraph[200][195] = 1;
  inputGraph[200][199] = 0.5;
  inputGraph[200][201] = 0.5;
  inputGraph[201][199] = 0.5;
  inputGraph[201][200] = 0.5;
  inputGraph[201][202] = 1;
  inputGraph[202][201] = 1;
  inputGraph[202][203] = 1;
  inputGraph[203][202] = 1;
  inputGraph[203][204] = 1;
  inputGraph[204][203] = 1;
  inputGraph[204][205] = 1;
  inputGraph[205][204] = 1;
  inputGraph[205][206] = 0.5;
  inputGraph[205][207] = 0.5;
  inputGraph[206][205] = 0.5;
  inputGraph[206][207] = 0.5;
  inputGraph[206][209] = 1;
  inputGraph[207][205] = 0.5;
  inputGraph[207][206] = 0.5;
  inputGraph[207][208] = 1;
  inputGraph[208][207] = 1;
  inputGraph[208][251] = 1;
  inputGraph[209][206] = 1;
  inputGraph[209][210] = 1;
  inputGraph[210][209] = 1;
  inputGraph[210][211] = 1;
  inputGraph[211][210] = 1;
  inputGraph[211][212] = 1;
  inputGraph[212][211] = 1;
  inputGraph[212][213] = 1;
  inputGraph[213][212] = 1;
  inputGraph[213][214] = 1;
  inputGraph[214][213] = 1;
  inputGraph[214][215] = 1;
  inputGraph[215][214] = 1;
  inputGraph[215][216] = 1;
  inputGraph[216][215] = 1;
  inputGraph[216][217] = 1;
  inputGraph[217][216] = 1;
  inputGraph[217][218] = 1;
  inputGraph[218][217] = 1;
  inputGraph[218][219] = 1;
  inputGraph[219][218] = 1;
  inputGraph[219][220] = 1;
  inputGraph[220][219] = 1;
  inputGraph[220][221] = 1;
  inputGraph[221][220] = 1;
  inputGraph[221][222] = 1;
  inputGraph[222][221] = 1;
  inputGraph[222][223] = 1;
  inputGraph[223][222] = 1;
  inputGraph[223][224] = 1;
  inputGraph[224][223] = 1;
  inputGraph[224][225] = 1;
  inputGraph[225][224] = 1;
  inputGraph[225][226] = 1;
  inputGraph[226][225] = 1;
  inputGraph[226][227] = 1;
  inputGraph[227][226] = 1;
  inputGraph[227][228] = 1;
  inputGraph[228][227] = 1;
  inputGraph[228][229] = 1;
  inputGraph[229][228] = 1;
  inputGraph[229][250] = 1;
  inputGraph[230][231] = 1;
  inputGraph[230][237] = 1;
  inputGraph[231][230] = 1;
  inputGraph[231][232] = 1;
  inputGraph[232][231] = 1;
  inputGraph[232][233] = 1;
  inputGraph[233][232] = 1;
  inputGraph[233][234] = 1;
  inputGraph[234][233] = 1;
  inputGraph[234][235] = 1;
  inputGraph[235][234] = 1;
  inputGraph[235][236] = 1;
  inputGraph[236][235] = 1;
  inputGraph[236][237] = 1;
  inputGraph[237][230] = 1;
  inputGraph[237][236] = 1;
  inputGraph[238][239] = 1;
  inputGraph[238][245] = 1;
  inputGraph[239][238] = 1;
  inputGraph[239][240] = 1;
  inputGraph[240][239] = 1;
  inputGraph[240][241] = 1;
  inputGraph[241][240] = 1;
  inputGraph[241][242] = 1;
  inputGraph[242][241] = 1;
  inputGraph[242][243] = 1;
  inputGraph[243][242] = 1;
  inputGraph[243][246] = 1;
  inputGraph[244][245] = 1;
  inputGraph[244][246] = 1;
  inputGraph[245][238] = 1;
  inputGraph[245][244] = 1;
  inputGraph[246][243] = 1;
  inputGraph[246][244] = 1;
  inputGraph[247][248] = 1;
  inputGraph[247][277] = 1;
  inputGraph[248][247] = 1;
  inputGraph[248][249] = 1;
  inputGraph[249][248] = 1;
  inputGraph[249][250] = 1;
  inputGraph[250][229] = 1;
  inputGraph[250][249] = 1;
  inputGraph[251][208] = 1;
  inputGraph[251][252] = 1;
  inputGraph[252][251] = 1;
  inputGraph[252][253] = 1;
  inputGraph[253][252] = 1;
  inputGraph[253][254] = 1;
  inputGraph[254][253] = 1;
  inputGraph[254][255] = 1;
  inputGraph[255][254] = 1;
  inputGraph[255][256] = 1;
  inputGraph[256][255] = 1;
  inputGraph[256][257] = 1;
  inputGraph[257][256] = 1;
  inputGraph[257][258] = 1;
  inputGraph[258][257] = 1;
  inputGraph[258][259] = 1;
  inputGraph[259][258] = 1;
  inputGraph[259][260] = 1;
  inputGraph[260][259] = 1;
  inputGraph[260][261] = 1;
  inputGraph[261][260] = 1;
  inputGraph[261][262] = 1;
  inputGraph[262][261] = 1;
  inputGraph[262][263] = 1;
  inputGraph[263][262] = 1;
  inputGraph[263][264] = 1;
  inputGraph[264][263] = 1;
  inputGraph[264][265] = 1;
  inputGraph[265][264] = 1;
  inputGraph[265][266] = 1;
  inputGraph[266][265] = 1;
  inputGraph[266][267] = 1;
  inputGraph[267][135] = 1;
  inputGraph[267][266] = 1;
  inputGraph[268][134] = 1;
  inputGraph[268][269] = 1;
  inputGraph[269][133] = 1;
  inputGraph[269][268] = 1;
  inputGraph[270][15] = 1;
  inputGraph[270][271] = 1;
  inputGraph[271][270] = 1;
  inputGraph[271][272] = 1;
  inputGraph[272][271] = 1;
  inputGraph[272][273] = 1;
  inputGraph[273][272] = 1;
  inputGraph[273][274] = 1;
  inputGraph[274][273] = 1;
  inputGraph[274][275] = 1;
  inputGraph[275][274] = 1;
  inputGraph[275][276] = 1;
  inputGraph[276][3] = 1;
  inputGraph[276][275] = 1;
  inputGraph[277][247] = 1;
  inputGraph[277][278] = 1;
  inputGraph[278][2] = 1;
  inputGraph[278][277] = 1;
  inputGraph[279][0] = 0.5;
  inputGraph[279][1] = 1;
  inputGraph[279][2] = 0.5;

  printf("Capacity:\n");
  printMatrix(inputGraph);

  printf("Max Flow:\n%d\n", pushRelabel(inputGraph, flow, SOURCE, SINK));

  printf("Flows:\n");
  printMatrix(flow);

  return 0;
}