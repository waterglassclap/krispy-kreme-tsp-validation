#ifndef PPR_DS_H
#define PPR_DS_H

typedef struct FracSol {
	float** sol;
	int size;
} FracSol;

typedef struct ConstrSet {
	float** set;
	int rowNum;
	int colNum;
} ConstrSet;

#endif
