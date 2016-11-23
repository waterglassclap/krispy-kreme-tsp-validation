#ifndef PPR_DS_H
#define PPR_DS_H

typedef struct PprAns {
    int n;
    double delta;
    double** sol;
    bool** T;
} PprAns;

#endif
