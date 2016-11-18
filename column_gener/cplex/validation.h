#ifndef validation_h
#define validation_h

#include "push_relabel.h"
#include "common_util.h"

// for Validation Check
typedef struct ErrorInfo {
    MinCutInfo minCutInfo;
    float severity;
    bool isValid;
} ErrorInfo;

typedef struct ErrorInfos {
    struct ErrorInfo* infos;
    int num;
} ErrorInfos;

// validation check
struct ErrorInfo getStErrorInfo(int s, int t, int n, float** inputGraph);
struct ErrorInfos getNonStErrorInfos(int s, int t, int n, float** inputGraph);

#endif
