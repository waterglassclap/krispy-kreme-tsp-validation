#ifndef INDICE_DIFF_H
#define INDICE_DIFF_H

#include <deque>

using namespace std;

typedef struct IndiceDiff {
	int n;
	deque<int>* diffHistories; // TODO : is this deque array?
} IndiceDiff;

#endif
