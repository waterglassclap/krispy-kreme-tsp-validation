#ifndef PPR_DS_H
#define PPR_DS_H

typedef struct PprAns {
    int n;
    float delta;
    float** sol;
    bool** T;
} PprAns;

#endif
