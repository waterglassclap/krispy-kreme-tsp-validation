#include <ilcplex/ilocplex.h>
#include <cmath>
#include <limits>
#include "mincut_util.h"
#include "column_generation.h"

ILOSTLBEGIN
#define MYX(i, j) ((i) < (j) ? x[(i)][(j)] : x[(j)][(i)])

// held_karp_relaxation
double** held_karp_lp(int n, double** cost);
bool check_validation(const IloEnv& env, const IloCplex& cplex, const IloModel& model, const IloArray<IloNumVarArray>& x, std::stringstream& name, int n);
void stDivider(int s, int t, MinCutInfo* mergedInfo);
bool isSpanningTree(bool** tree, int n);
void printSpanningTree(bool** spanningTree, double** cost, int n, int iter, const char* filename);

int start_node, end_node;

int main (int argc, char **argv) {
    cout << "Held-Karp Relaxation and Column Generation" << endl;
    
    const char* filename  = argv[1];
    ifstream file(filename);
    if (!file) {
        cout << "ERROR: could not open file '" << filename
        << "' for reading" << endl;
        cout << "USAGE:   " << argv[0] << " <filename>" << endl;
        throw(-1);
    }
    
    cout << "start node: ";
    cin >> start_node;
    cout << "end node: ";
    cin >> end_node;
    
    int node_num, edge_num;
    file >> node_num >> edge_num;
    
    double** full_weight_matrix = new double*[node_num];
    for(int i=0; i<node_num; i++) {
        full_weight_matrix[i] = new double[node_num];
    }
    
    int temp_i, temp_j, temp_w;
    while(file >> temp_i >> temp_j >> temp_w) {
        full_weight_matrix[temp_i][temp_j] = temp_w;
    }
    
    file.close();
    
    double** held_karp_optimal = held_karp_lp(node_num, full_weight_matrix);
    
    if(held_karp_optimal == 0) {
        cout << "Held Karp Cplex Error" << endl;
        return 0;
    }
    
    stringstream held_karp_file;
    held_karp_file << "/Users/ttopre/Desktop/held_karp_optimal/" << start_node << "_" << end_node << "_" << filename;
    ofstream out_held_karp(held_karp_file.str().c_str());
    
    int n = node_num;
    out_held_karp << n << endl;
    out_held_karp << start_node << " " << end_node << endl;
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            if(i > j) {
                if(held_karp_optimal[i][j] > 1e-10) {
                    out_held_karp << i << " " << j << " " << held_karp_optimal[j][i] << endl;
                }
            } else {
                if(held_karp_optimal[i][j] > 1e-10) {
                    out_held_karp << i << " " << j << " " << held_karp_optimal[i][j] << endl;
                }
            }
        }
    }
    
    out_held_karp.close();
    
    cout << endl << "Column Generation Start" << endl;
    vector<bool**> spanningTrees = column_generation(node_num, full_weight_matrix, held_karp_optimal);
    
    for(int i=0; i<spanningTrees.size(); i++) {
        printSpanningTree(spanningTrees[i], full_weight_matrix, node_num, i, filename);
    }
    
    return 0;
}

double** held_karp_lp(int n, double** cost) {
    double** optimal = create2dArr<double>(n, 0.0f);
    IloEnv env;
    
    try {
        IloModel model(env);
        
        IloArray<IloNumVarArray> x(env, n); // Variables x
        IloRangeArray degree_constraints(env, n); // Degree Constraints
        std::stringstream name; // variable and constraint names
        
        // Create variables x with Variable Constraints
        for(int i = 0; i < n; ++i) {
            x[i] = IloNumVarArray(env, n);
            for(int j = i+1; j < n; ++j) {
                name << "x_" << i << "_" << j;
                x[i][j] = IloNumVar(env, 0, 1, ILOFLOAT, name.str().c_str()); // 0 <= x_i_j <= 1
                name.str(""); // clean name
            }
        }
        
        // Degree Constraints
        // sum(j, x[j][i]) == 1, For all i
        IloExpr expr(env);
        
        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < n; ++j) {
                if(i != j) {
                    expr += MYX(i, j);
                }
            }
            
            name << "degree_" << i;
            if(i == start_node || i == end_node) {
                degree_constraints[i] = IloRange(env, 1, expr, 1, name.str().c_str());
            } else {
                degree_constraints[i] = IloRange(env, 2, expr, 2, name.str().c_str());
            }
            name.str(""); // Clean name
            expr.clear(); // Clean expr
        }
        
        model.add(degree_constraints);
        
        // Objective Function
        // min sum((i,j), cost[i][j] * x[i][j])
        for(int i = 0; i < n; ++i) {
            for(int j = i+1; j < n; ++j) {
                expr += cost[i][j] * x[i][j];
            }
        }
        IloObjective obj(env, expr, IloObjective::Minimize);
        
        model.add(obj);
        
        expr.end();
        
        // LP solver object
        IloCplex cplex(model);
        
        // Solve with dual simplex
        cplex.setParam(IloCplex::RootAlg, IloCplex::Dual);
        
        int iter = 0; // for debug
        while(1) {
            // Export model to file
            cplex.exportModel("newmodel.lp");
            
            if( !cplex.solve() ) {
                cerr << "Failed to optimize Held Karp Relaxation" << endl;
                cerr << "Status: " << cplex.getStatus() << endl;
                cerr << "Solver status: " << cplex.getCplexStatus() << endl;
                return 0;
            }
            
            cout << "Held-Karp success at " << iter << " time" << endl;
            cout << "Status: " << cplex.getStatus() << endl;
            cout << "Objective value: " << cplex.getObjValue() << endl;
            if(!check_validation(env, cplex, model, x, name, n)) {
                break;
            }
            
            iter++;
        }
        
        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < n; ++j) {
                if(i != j){
                    if(cplex.getValue(MYX(i,j)) > 1e-10) {
                        optimal[i][j] = cplex.getValue(MYX(i,j));
                    }
                }
            }
        }
    } catch (IloException& e) {
        cerr << "Concert exception caught: " << e << endl;
    } catch (...) {
        cerr << "Unknown exception caught" << endl;
    }
    
    env.end();
    
    return optimal;
}

bool check_validation(const IloEnv& env, const IloCplex& cplex, const IloModel& model, const IloArray<IloNumVarArray>& x, std::stringstream& name, int n) {
    double** solution = create2dArr<double>(n, 0.0f);
    
    // Enter cplex solver result graph
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            if(i != j && cplex.getValue(MYX(i, j)) != 0) {
                solution[i][j] = cplex.getValue(MYX(i, j));
            }
        }
    }
    
    ErrorInfo stErrorInfo = MincutUtil().getStErrorInfo(start_node, end_node, n, solution);
    ErrorInfos nonStErrorInfos = MincutUtil().getNonStErrorInfos(start_node, end_node, n, solution);
    
    ErrorInfo maxErrorInfo;
    maxErrorInfo.severity = -1;
    bool isST = false;
    
    if (stErrorInfo.isValid) {
        maxErrorInfo = stErrorInfo;
        isST = true;
    }
    
    for (int i = 0; i < nonStErrorInfos.num; i++) {
        if(nonStErrorInfos.infos[i].severity > maxErrorInfo.severity) {
            maxErrorInfo = nonStErrorInfos.infos[i];
            isST = false;
        }
    }
    
    if(maxErrorInfo.severity == -1) {
        // cout << "there is no StErrorInfos" << endl;
        return false;
    }
    
    if(!isST) {
        // restore merged graph when nonSTError
        stDivider(start_node, end_node, &maxErrorInfo.minCutInfo);
    }
    
    // Subtour Elimination Constraints
    IloRangeArray subtour_constraints(env);
    IloExpr expr(env);
    
    for(int i = 0; i < maxErrorInfo.minCutInfo.size; i++) {
        expr += MYX(maxErrorInfo.minCutInfo.edgeInfos[i].source, maxErrorInfo.minCutInfo.edgeInfos[i].sink);
    }
    
    // mincut에 속하는 edge E = [e1, e2, e3] 에 대해서
    // e1 + e2 + e3 >= 1    for st
    // e1 + e2 + e3 >= 2    for non st
    
    name << "subtour";
    if(isST) {
        subtour_constraints.add(IloRange(env, 1, expr, IloInfinity, name.str().c_str()));
    } else {
        subtour_constraints.add(IloRange(env, 2, expr, IloInfinity, name.str().c_str()));
    }
    
    name.str(""); // Clean name
    expr.end(); // end expr
    
    // Add constraints to the model
    model.add(subtour_constraints);
    return true;
}

void stDivider(int s, int t, MinCutInfo* mergedInfo) {
    
    int merged_size = mergedInfo->size;
    for(int i=0; i< merged_size; i++) {
        if(mergedInfo->edgeInfos[i].source == 0 || mergedInfo->edgeInfos[i].sink == 0) {
            mergedInfo->size++;
        }
    }
    
    EdgeInfo* newEdgeInfos = new EdgeInfo[mergedInfo->size];
    
    int added_size = merged_size;
    for(int i=0; i< merged_size; i++) {
        if(mergedInfo->edgeInfos[i].source == 0) {
            newEdgeInfos[i].source = s;
            newEdgeInfos[added_size].source = t;
            
            newEdgeInfos[i].sink = mergedInfo->edgeInfos[i].sink;
            if(mergedInfo->edgeInfos[i].sink <= s && mergedInfo->edgeInfos[i].sink <= t) {
                newEdgeInfos[i].sink--;
            } else if(mergedInfo->edgeInfos[i].sink >= s && mergedInfo->edgeInfos[i].sink >= t) {
                newEdgeInfos[i].sink++;
            }
            
            newEdgeInfos[added_size].sink = newEdgeInfos[i].sink;
            added_size++;
            
        } else if(mergedInfo->edgeInfos[i].sink == 0) {
            newEdgeInfos[i].sink = s;
            newEdgeInfos[added_size].sink = t;
            
            newEdgeInfos[i].source = mergedInfo->edgeInfos[i].source;
            if(mergedInfo->edgeInfos[i].source <= s && mergedInfo->edgeInfos[i].source <= t) {
                newEdgeInfos[i].source--;
            } else if(mergedInfo->edgeInfos[i].source >= s && mergedInfo->edgeInfos[i].source >= t) {
                newEdgeInfos[i].source++;
            }
            
            newEdgeInfos[added_size].source = newEdgeInfos[i].source;
            added_size++;
            
        } else {
            newEdgeInfos[i].source = mergedInfo->edgeInfos[i].source;
            if(mergedInfo->edgeInfos[i].source <= s && mergedInfo->edgeInfos[i].source <= t) {
                newEdgeInfos[i].source--;
            } else if(mergedInfo->edgeInfos[i].source >= s && mergedInfo->edgeInfos[i].source >= t) {
                newEdgeInfos[i].source++;
            }
            
            newEdgeInfos[i].sink = mergedInfo->edgeInfos[i].sink;
            if(mergedInfo->edgeInfos[i].sink <= s && mergedInfo->edgeInfos[i].sink <= t) {
                newEdgeInfos[i].sink--;
            } else if(mergedInfo->edgeInfos[i].sink >= s && mergedInfo->edgeInfos[i].sink >= t) {
                newEdgeInfos[i].sink++;
            }
        }
    }
    
    delete [] mergedInfo->edgeInfos;
    mergedInfo->edgeInfos = newEdgeInfos;
}

bool isSpanningTree(bool** tree, int n) {
    int edge_num = 0;
    vector<int> vertex;
    
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            if(tree[i][j]) {
                vertex.push_back(i);
                vertex.push_back(j);
                edge_num++;
            }
        }
    }
    
    sort(vertex.begin(), vertex.end());
    vertex.erase(unique(vertex.begin(), vertex.end()), vertex.end());
    
    if (vertex.size() == n && edge_num == n-1) {
        return true;
    }
    return false;
}

void printSpanningTree(bool** spanningTree, double** cost, int n, int iter, const char* filename) {
    if(isSpanningTree(spanningTree, n)) {
        cout << iter << "th is spanning tree" << endl;
    } else {
        cout << iter << "th is not spanning" << endl;
        return;
    }
    
    stringstream col_generation_file;
    col_generation_file << "/Users/ttopre/Desktop/column_generation/" << start_node << "_" << end_node << "_" << iter << "_" << filename;
    ofstream out_col_generation(col_generation_file.str().c_str());
    
    out_col_generation << n << " " << n-1 << endl;
    
    for (int i=0; i<n; i++) {
        for (int j=i+1; j<n; j++) {
            if(spanningTree[i][j]) {
                out_col_generation << i << " " << j << " " << cost[i][j] << endl;
            }
        }
    }

    out_col_generation.close();
}
