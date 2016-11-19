#include "abs_ppr.h"

// TODO : float error, 1e-9 handling
// TODO : minset 찾는거 잘 되나 확인해보기
// TODO : 왜 할때마다 변함

void printPprAns(PprAns* pprAns) {
    cout << "====target y===="<< endl;
    for (int i = 0; i < pprAns->n; i++) {
        for (int j = 0; j <pprAns->n; j++) {
            cout << pprAns->sol[i][j] << "\t";
        }
        cout << endl;
    }
    cout << "====target T===="<< endl;
    for (int i = 0; i < pprAns->n; i++) {
        for (int j = 0; j < pprAns->n; j++) {
            cout << pprAns->T[i][j] << "\t";
        }
        cout << endl;
    }
    cin.ignore();
}

TwoDGraph<float>* getTwoDGraphCopy(TwoDGraph<float>* inputGraph) {
    TwoDGraph<float>* newGraph = new TwoDGraph<float>;
    newGraph->n = inputGraph->n;
    newGraph->source = inputGraph->source;
    newGraph->sink = inputGraph->sink;
    newGraph->set = new float*[newGraph->n];
    for (int i = 0; i < newGraph->n; i++) {
        newGraph->set[i] = new float[newGraph->n];
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

void mergeGraph(TwoDGraph<float>* targetGraph, IndiceDiff* indiceDiff, int firstTarget, int secondTarget) {
    int n = targetGraph->n;
    float** mergedSet = create2dArr<float>(n - 1, 0.0f);
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
    free2dArr<float>(n, targetGraph->set);
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
            if (static_cast<int>(ans->sol[i][j]) != ans->sol[i][j]) {
                return false;
            }
        }
    }
    return true;
}


void AbsPpr::initializeTightSet(PprAns* ans) {
    delete ans->T;
    ans->T = create2dArr<bool>(ans->n, true);
    for (int i = 0; i < ans->n; i++) {
        ans->T[i][i] = false;
    }
}

// s is nodes[-2], t is nodes[-1]
TwoDGraph<float>* AbsPpr::getValidationGraph(PprAns* prevAns, EdgeInfo* firstEdge, EdgeInfo* secondEdge) {
    int n = prevAns->n + 2;
    TwoDGraph<float>* validationGraph = new TwoDGraph<float>;
    validationGraph->n = n;
    validationGraph->source = n - 2;
    validationGraph->sink = n - 1;
    validationGraph->set = create2dArr<float>(n, 0.0f);

    validationGraph->set[n - 2][n - 1] = 0.0f;
    validationGraph->set[n - 1][n - 2] = 0.0f;
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
    for (int i = 0; i < n; i++) {
        //get wFromS: s -(wFromS)-> vi, which is 1/2 SIGMA(Ye, e E V) : TODO : is this correct?
        float wFromS = 0.0f;
        for (int j = 0; j < prevAns->n; j++) {
            wFromS += validationGraph->set[i][j];
        }
        if (i != n - 2) {
            validationGraph->set[n - 2][i] = wFromS;
        }
        if (i != n - 1) {
            validationGraph->set[i][n - 1] = 1.0f;
        }
    }
    return validationGraph;
}

EdgeInfo* AbsPpr::pullEdge(PprAns* ans) {
    EdgeInfo* edge = new EdgeInfo;
    edge->source = -1;
    edge->sink = -1;
    edge->weight = -1;

    for (int i = 0; i < ans->n; i++) {
        for (int j = 0; j < ans->n; j++) {
            if (ans->T[i][j] && ans->T[j][i]
                && (static_cast<int>(ans->sol[i][j]) != ans->sol[i][j])) { // TODO : is this correct?
                edge->source = i;
                edge->sink = j;
                edge->weight = ans->sol[i][j];
                // mark T
                ans->T[i][j] = false;
                ans->T[j][i] = false;
                return edge;
            }
        }
    }
    return edge;
}

bool* AbsPpr::getMinsetA(PprAns* prevAns, EdgeInfo* i, EdgeInfo* j) {
    TwoDGraph<float>* validationGraph = getValidationGraph(prevAns, i, j);
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
        TwoDGraph<float>* tempGraph1 = getTwoDGraphCopy(validationGraph);
        TwoDGraph<float>* tempGraph2 = getTwoDGraphCopy(validationGraph);
        IndiceDiff* tempIndiceDiff1 = getIndiceDiffCopy(indiceDiff);
        IndiceDiff* tempIndiceDiff2 = getIndiceDiffCopy(indiceDiff);

        mergeGraph(tempGraph1, tempIndiceDiff1,
            tempIndiceDiff1->diffInfo[j->source],
            tempIndiceDiff1->diffInfo[tempGraph1->sink]);
        mergeGraph(tempGraph2, tempIndiceDiff2,
            tempIndiceDiff2->diffInfo[j->sink],
            tempIndiceDiff2->diffInfo[tempGraph2->sink]);

        PushRelabel verifier1(
            tempIndiceDiff1->diffInfo[tempGraph1->source],
            tempIndiceDiff1->diffInfo[tempGraph1->sink],
            tempGraph1->n, tempGraph1->set);
        PushRelabel verifier2(
            tempIndiceDiff2->diffInfo[tempGraph2->source],
            tempIndiceDiff2->diffInfo[tempGraph2->sink],
            tempGraph2->n, tempGraph2->set);

        MinCutInfo tempMinCutInfo1 = verifier1.getMinCutInfo();
        MinCutInfo tempMinCutInfo2 = verifier2.getMinCutInfo();

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
        free2dArr<float>(tempGraph1->n, tempGraph1->set);
        free2dArr<float>(tempGraph2->n, tempGraph2->set);
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

        // get min cut of validation graph
        PushRelabel verifier(
            indiceDiff->diffInfo[validationGraph->source],
            indiceDiff->diffInfo[validationGraph->sink],
            validationGraph->n, validationGraph->set);
        mci = verifier.getMinCutInfo();
    }

    bool *minsetMarker = new bool[prevAns->n], *tempMinsetMarker = new bool[prevAns->n];
    
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

    free2dArr<float>(validationGraph->n, validationGraph->set);
    delete validationGraph;
    delete[] indiceDiff->diffInfo;
    delete indiceDiff;
    delete tempMinsetMarker;
    
    return minsetMarker;
}

float AbsPpr::getConstraintDelta(bool* minsetA, PprAns* prevAns) {
    int absA = 0;
    float sumAEdges = 0.0f;

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
    return ((float)absA) - 1.0f - sumAEdges;
}

PprAns* AbsPpr::hitConstraint(PprAns* prevAns, EdgeInfo* i, EdgeInfo* j) {
    PprAns* newAns = new PprAns;
    newAns->n = prevAns->n;
    // 1. build newAns->minsetA newAns->T
    newAns->minsetA = getMinsetA(prevAns, i, j);
    // get T from minset
    newAns->T = create2dArr<bool>(prevAns->n, false);
    for (int i = 0; i < prevAns->n; i++) {
        for (int j = 0; j < prevAns->n; j++) {
            if (newAns->minsetA[i] && newAns->minsetA[j] && i != j) {
                newAns->T[i][j] = true;
            }
        }
    }
    // 2. build newAns->delta
    newAns->delta = getConstraintDelta(newAns->minsetA, prevAns);
    if (newAns->delta < 0.0f) {
        throw runtime_error("constraintDelta is smaller than 0, something's wrong! Terminate.");
    }
    // check if delta meet lower bound. If so, update
    if (prevAns->sol[j->source][j->sink] < newAns->delta) {
        cout << " delta met lower bound. j weight : " << prevAns->sol[j->source][j->sink] << ", delta" << newAns->delta << endl;
        free2dArr<bool>(prevAns->n, newAns->T);
        newAns->T = create2dArr<bool>(prevAns->n, false);
        newAns->delta = prevAns->sol[j->source][j->sink];
    }

    // 3. build newAns->sol
    newAns->sol = new float*[newAns->n];
    for (int i = 0; i < newAns->n; i++) {
        newAns->sol[i] = new float[newAns->n];
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
            EdgeInfo* j = pullEdge(targetAns);
            // invalid edge
            if (i->source < 0 || j->source < 0) {
                cout << "invalid edge. break" << endl;
                break;
            }

            PprAns* incAns = hitConstraint(targetAns, i, j);
            PprAns* decAns = hitConstraint(targetAns, j, i);
            updateSol(targetAns, incAns, decAns);            
        }
    }
}
