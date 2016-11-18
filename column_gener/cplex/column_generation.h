#ifndef column_generation_h
#define column_generation_h

#include <ilcplex/ilocplex.h>
#include <cmath>
#include <iostream>

using namespace std;

// column_generation
void column_generation(int n, float** cost, float** x);
float getMST(float** graph, bool** maxSpan, int n);

#endif /* column_generation_h */
