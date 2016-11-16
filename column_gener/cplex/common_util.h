#ifndef COMMON_UTIL_H
#define COMMON_UTIL_H

#include <iostream>

const float ERROR_TOLERANCE = 1e-9;

bool over(float value, float boundary);
bool exceed(float value, float boundary);
// merged node will be 0
float** getMergedGraph(int s, int t, int n, float** inputGraph);

#endif
