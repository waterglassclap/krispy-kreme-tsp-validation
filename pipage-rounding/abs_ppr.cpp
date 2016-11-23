#include "abs_ppr.h"

void printPprAns(PprAns* pprAns) {
    int edgeCounter = 0;
    cout << "====target y====" << endl;
    for (int i = 0; i < pprAns->n; i++) {
        for (int j = 0; j <pprAns->n; j++) {
            cout << pprAns->sol[i][j] << " ";
            if (pprAns->sol[i][j] > 0.0f) {
                edgeCounter++;
            }
        }
        cout << endl;
    }
    cout << "edges : " << edgeCounter << endl;
    // cout << "====target T===="<< endl;
    // for (int i = 0; i < pprAns->n; i++) {
    //     for (int j = 0; j < pprAns->n; j++) {
    //         cout << pprAns->T[i][j] << " ";
    //     }
    //     cout << endl;
    // }
}

void printDouble2DGraph(TwoDGraph<double>* inputGraph) {
    for (int i = 0; i < inputGraph->n; i++) {
        for (int j = 0; j <inputGraph->n; j++) {
            cout << inputGraph->set[i][j] << " ";
        }
        cout << endl;
    }
}

void printDouble2DArray(int n, double** inputGraph) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << inputGraph[i][j] << "\t";
        }
        cout << endl;
    }
}

TwoDGraph<double>* getTwoDGraphCopy(TwoDGraph<double>* inputGraph) {
    TwoDGraph<double>* newGraph = new TwoDGraph<double>;
    newGraph->n = inputGraph->n;
    newGraph->source = inputGraph->source;
    newGraph->sink = inputGraph->sink;
    newGraph->set = new double*[newGraph->n];
    for (int i = 0; i < newGraph->n; i++) {
        newGraph->set[i] = new double[newGraph->n];
        for (int j = 0; j < newGraph->n; j++) {
            newGraph->set[i][j] = inputGraph->set[i][j];
        }
    }
    return newGraph;
}

IndiceDiff* getIndiceDiffCopy(IndiceDiff* inputIndiceDiff) {
    IndiceDiff* newIndiceDiff = new IndiceDiff;
    newIndiceDiff->n = inputIndiceDiff->n;
    newIndiceDiff->diffInfo = new int[newIndiceDiff->n];
    for (int i = 0; i < inputIndiceDiff->n; i++) {
        newIndiceDiff->diffInfo[i] = inputIndiceDiff->diffInfo[i];
    }
    return newIndiceDiff;
}

void mergeGraph(TwoDGraph<double>* targetGraph, IndiceDiff* indiceDiff, int firstTarget, int secondTarget) {
    int n = targetGraph->n;
    double** mergedSet = create2dArr<double>(n - 1, 0.0f);
    IndiceDiff* tempIndiceDiff = getIndiceDiffCopy(indiceDiff);
    // merge set, merged node would be node 0
    int newRow = 1;
    for (int originalRow = 0; originalRow < n; originalRow++) {
        int newCol = 1, currentRow, currentCol;
        if (originalRow == firstTarget || originalRow == secondTarget) {
            currentRow = 0;
        } else {
            currentRow = newRow;
            newRow++;
        }
        // update indice diff history
        for (int i = 0; i < indiceDiff->n; i++) {
            if (tempIndiceDiff->diffInfo[i] == originalRow) {
                indiceDiff->diffInfo[i] = currentRow;
            }
        }
        for (int originalCol = 0; originalCol < n; originalCol++) {
            if (originalCol == firstTarget || originalCol == secondTarget) {
                currentCol = 0;
            } else {
                currentCol = newCol;
                newCol++;
            }
            if (currentRow != currentCol) {
                mergedSet[currentRow][currentCol] += targetGraph->set[originalRow][originalCol];
            }
        }
    }

    // update target Graph set
    free2dArr<double>(n, targetGraph->set);
    targetGraph->set = mergedSet;
    targetGraph->n = n - 1;

    // update indice history
    delete[] tempIndiceDiff->diffInfo;
    delete tempIndiceDiff;
}

AbsPpr::AbsPpr() {
    // Do nothing
}

bool AbsPpr::isIntegral(PprAns* ans) {
    for (int i = 0; i < ans->n; i++) {
        for (int j = 0; j < ans->n; j++) {
            if (!roughly_equal(static_cast<int>(ans->sol[i][j]), ans->sol[i][j])) {
                return false;
            }
        }
    }
    return true;
}


void AbsPpr::initializeTightSet(PprAns* ans) {
    free2dArr<bool>(ans->n, ans->T);
    ans->T = create2dArr<bool>(ans->n, false);
    for (int i = 0; i < ans->n; i++) {
        for (int j = 0; j < ans->n; j++) {
            if (i != j) {
            //if (i != j && !roughly_equal(0.0f, ans->sol[i][j])) {
                ans->T[i][j] = true;
                ans->T[j][i] = true;
            }
        }
    }
}


// s is nodes[-2], t is nodes[-1]
TwoDGraph<double>* AbsPpr::getValidationGraph(PprAns* prevAns) {
    // cout << "==== make validationGraph ====" << endl;
    // cout << ">>>> before built <<<<" << endl;
    // printDouble2DArray(prevAns->n, prevAns->sol);
    int n = prevAns->n + 2;
    TwoDGraph<double>* validationGraph = new TwoDGraph<double>;
    validationGraph->n = n;
    validationGraph->source = n - 2;
    validationGraph->sink = n - 1;
    validationGraph->set = create2dArr<double>(n, 0.0f);
    // 3. fill the rest of the graph with prevAns weights;
    for (int i = 0; i < n - 2; i++) {
        for (int j = 0; j < n - 2; j++) {
            if (j > i) {
                validationGraph->set[i][j] = 0.5f * prevAns->sol[i][j];
                validationGraph->set[j][i] = validationGraph->set[i][j];
            }
        }
    }
    // 1. s -> vi to wFromS
    // 2. vi -> t to 1
    for (int i = 0; i < n - 2; i++) {
        //get wFromS: s -(wFromS)-> vi, which is 1/2 SIGMA(Ye, e E V) : TODO : is this correct?
        double wFromS = 0.0f;
        for (int j = 0; j < n - 2; j++) {
            if (i != j) {
                wFromS += validationGraph->set[i][j];
            }
        }
        if (i < n - 2) {
            validationGraph->set[n - 2][i] = wFromS;
            validationGraph->set[i][n - 1] = 1.0f;
        }
    }
    // cout << ">>>> after built <<<<" << endl;
    // printDouble2DArray(validationGraph->n, validationGraph->set);
    // cout << endl;
    return validationGraph;
}

bool isDuplicate(int firsti, int firstj, int secondi, int secondj) {
    return (firsti == secondi && firstj == secondj) ||
            (firstj == secondi && firsti == secondj);
}

EdgeInfo* AbsPpr::pullEdge(PprAns* ans) {
    EdgeInfo* edge = new EdgeInfo;
    edge->source = -1;
    edge->sink = -1;
    edge->weight = -1;

    for (int i = 0; i < ans->n; i++) {
        for (int j = 0; j < ans->n; j++) {
            if (i != j &&
                ans->T[i][j] &&
                ans->T[j][i] &&
                !roughly_equal(static_cast<int>(ans->sol[i][j]), ans->sol[i][j])) { // TODO : is this correct?
                edge->source = i;
                edge->sink = j;
                edge->weight = ans->sol[i][j];
                return edge;
            }
        }
    }
    return edge;
}

EdgeInfo* AbsPpr::pullEdge(PprAns* ans, EdgeInfo* noDupEdge) {
    EdgeInfo* edge = new EdgeInfo;
    edge->source = -1;
    edge->sink = -1;
    edge->weight = -1;

    for (int i = 0; i < ans->n; i++) {
        for (int j = 0; j < ans->n; j++) {
            if (i != j &&
                ans->T[i][j] &&
                ans->T[j][i] &&
                !roughly_equal(static_cast<int>(ans->sol[i][j]), ans->sol[i][j]) &&
                !isDuplicate(noDupEdge->source, noDupEdge->sink, i, j)) { // TODO : is this correct?
                edge->source = i;
                edge->sink = j;
                edge->weight = ans->sol[i][j];
                return edge;
            }
        }
    }
    return edge;
}

bool* AbsPpr::getMinsetA(PprAns* prevAns, EdgeInfo* i, EdgeInfo* j) {
    TwoDGraph<double>* validationGraph = getValidationGraph(prevAns);
    MinCutInfo mci;
    
    IndiceDiff* indiceDiff = new IndiceDiff;
    indiceDiff->n = validationGraph->n;
    indiceDiff->diffInfo = new int[validationGraph->n];
    for (int i = 0; i < validationGraph->n; i++) {
        indiceDiff->diffInfo[i] = i;
    }
    // merge source with i.source and i.sink
    mergeGraph(validationGraph, indiceDiff, i->source, i->sink);
    mergeGraph(validationGraph, indiceDiff,
        indiceDiff->diffInfo[i->source],
        indiceDiff->diffInfo[validationGraph->source]);
    
    // if i and j has no common node, compare:
    // 1) s <- merge(merge(i), s),      t <- merge(j.node[0], t)
    // 2) s <- merge(merge(i), s),      t <- merge(j.node[1], t)
    if (!(j->source == i->source || j->source == i->sink ||
        j->sink == i->source || j->sink == i->sink)) {
        TwoDGraph<double>* tempGraph1 = getTwoDGraphCopy(validationGraph);
        TwoDGraph<double>* tempGraph2 = getTwoDGraphCopy(validationGraph);
        IndiceDiff* tempIndiceDiff1 = getIndiceDiffCopy(indiceDiff);
        IndiceDiff* tempIndiceDiff2 = getIndiceDiffCopy(indiceDiff);

        mergeGraph(tempGraph1, tempIndiceDiff1,
            tempIndiceDiff1->diffInfo[j->source],
            tempIndiceDiff1->diffInfo[tempGraph1->sink]);
        mergeGraph(tempGraph2, tempIndiceDiff2,
            tempIndiceDiff2->diffInfo[j->sink],
            tempIndiceDiff2->diffInfo[tempGraph2->sink]);

        MinCutInfo tempMinCutInfo1 = MincutUtil().getMinCutInfo(
            tempIndiceDiff1->diffInfo[tempGraph1->source],
            tempIndiceDiff1->diffInfo[tempGraph1->sink],
            tempGraph1->n, tempGraph1->set);
        MinCutInfo tempMinCutInfo2 = MincutUtil().getMinCutInfo(
            tempIndiceDiff2->diffInfo[tempGraph2->source],
            tempIndiceDiff2->diffInfo[tempGraph2->sink],
            tempGraph2->n, tempGraph2->set);

        delete[] indiceDiff->diffInfo;
        delete indiceDiff;

        if(tempMinCutInfo1.totalFlow < tempMinCutInfo2.totalFlow) {
            mci = tempMinCutInfo1;
            indiceDiff = tempIndiceDiff1;
            delete[] tempIndiceDiff2->diffInfo;
            delete tempIndiceDiff2;
        } else {
            mci = tempMinCutInfo2;
            indiceDiff = tempIndiceDiff2;
            delete[] tempIndiceDiff1->diffInfo;
            delete tempIndiceDiff1;
        }
        free2dArr<double>(tempGraph1->n, tempGraph1->set);
        free2dArr<double>(tempGraph2->n, tempGraph2->set);
        delete tempGraph1;
        delete tempGraph2;
    }
    // if i and j has common node, s <- merge(merge(i), s), t <- merge(i.nodes - s, t)
    else {
        if (j->source == i->source || j->source == i->sink) {
            mergeGraph(validationGraph, indiceDiff,
                indiceDiff->diffInfo[j->sink],
                indiceDiff->diffInfo[validationGraph->sink]);
        }
        else if (j->sink == i->source || j->sink == i->sink) {
            mergeGraph(validationGraph, indiceDiff,
                indiceDiff->diffInfo[j->source],
                indiceDiff->diffInfo[validationGraph->sink]);
        }

        mci = MincutUtil().getMinCutInfo(
            indiceDiff->diffInfo[validationGraph->source],
            indiceDiff->diffInfo[validationGraph->sink],
            validationGraph->n, validationGraph->set);
    }

    bool* minsetMarker = createArr(prevAns->n, false);
    bool* tempMinsetMarker = createArr(mci.nodeNum, false);

    // separate nodes to 2 with min-cut
    for (int i = 0; i < mci.nodeNum; i++) {
        if (mci.heights[i] > mci.cutHeight) {
            tempMinsetMarker[i] = true;
        }
    }

    // reverse nodeMarker if true-set does not contain source
    if (!tempMinsetMarker[indiceDiff->diffInfo[i->source]]) {
        for (int i = 0; i < mci.nodeNum; i++) {
            tempMinsetMarker[i] = !tempMinsetMarker[i];
        }
    }
    
    // reverse minset marker with previous indices
    for (int i = 0; i <prevAns->n; i++) {
        if (tempMinsetMarker[indiceDiff->diffInfo[i]]) {
            minsetMarker[i] = true;
        }
    }

    free2dArr<double>(validationGraph->n, validationGraph->set);
    delete validationGraph;
    delete[] indiceDiff->diffInfo;
    delete indiceDiff;
    delete tempMinsetMarker;
    
    return minsetMarker;
}

double AbsPpr::getConstraintDelta(bool* minsetA, PprAns* prevAns) {
    int absA = 0;
    double sumAEdges = 0.0f;

    for (int i = 0; i < prevAns->n; i++) {
        if (minsetA[i]) {
            absA++;
        }
        for (int j = i; j < prevAns->n; j++) {            
            if (minsetA[i] && minsetA[j] && i != j) {
                sumAEdges += prevAns->sol[i][j]; // TODO : is this correct?
            }
        }
    }
    return ((double)absA) - 1.0f - sumAEdges;
}

PprAns* AbsPpr::hitConstraint(PprAns* prevAns, EdgeInfo* i, EdgeInfo* j) {
    PprAns* newAns = new PprAns;
    newAns->n = prevAns->n;
    // 1. build newAns->minsetA newAns->T
    bool* minsetA = getMinsetA(prevAns, i, j);
    // get T from minset
    newAns->T = create2dArr<bool>(prevAns->n, false);
    for (int i = 0; i < prevAns->n; i++) {
        for (int j = i; j < prevAns->n; j++) {
            if (minsetA[i] && minsetA[j] && i != j) {
                newAns->T[i][j] = true;
                newAns->T[j][i] = true;
            }
        }
    }
    // 2. build newAns->delta
    newAns->delta = getConstraintDelta(minsetA, prevAns);
    if (newAns->delta < 0.0f) {
        throw runtime_error("constraintDelta is smaller than 0, something's wrong! Terminate.");
    }
    // check if delta meet lower bound. If so, update
    if (prevAns->sol[j->source][j->sink] < newAns->delta) {
        //cout << "==== meet lower bound! ====" << endl;        
        free2dArr<bool>(prevAns->n, newAns->T);
        newAns->T = create2dArr<bool>(prevAns->n, false);
        newAns->delta = prevAns->sol[j->source][j->sink];

        newAns->T[j->source][j->sink] = true;
        newAns->T[j->sink][j->source] = true;
    }    
    // 3. build newAns->sol
    newAns->sol = new double*[newAns->n];
    for (int i = 0; i < newAns->n; i++) {
        newAns->sol[i] = new double[newAns->n];
        for (int j = 0; j < newAns->n; j++) {
            newAns->sol[i][j] = prevAns->sol[i][j];
        }
    }
    // yi = yi + delta
    newAns->sol[i->source][i->sink] += newAns->delta;
    newAns->sol[i->sink][i->source] += newAns->delta;
    // yj = yj - delta
    newAns->sol[j->source][j->sink] -= newAns->delta;
    newAns->sol[j->sink][j->source] -= newAns->delta;

    return newAns;
}

void AbsPpr::pipageRound(PprAns* targetAns) {
    while (!isIntegral(targetAns)) {
        initializeTightSet(targetAns);
        while (true) {
            EdgeInfo* i = pullEdge(targetAns);
            EdgeInfo* j = pullEdge(targetAns, i);
            // invalid edge
            if (i->source < 0 || j->source < 0) {
                //cout << "invalid edge. break" << endl;
                break;
            }
            PprAns* incAns = hitConstraint(targetAns, i, j);
            PprAns* decAns = hitConstraint(targetAns, j, i);
            updateSol(targetAns, incAns, decAns);
        }
    }
    printPprAns(targetAns);
}
