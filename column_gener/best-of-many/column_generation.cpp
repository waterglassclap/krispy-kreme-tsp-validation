#include "column_generation.h"

vector<bool**> column_generation(int n, double** cost, double** x) {
    vector<bool**> spanningTrees;
    vector<bool**> returnTrees;
    IloEnv env;
    
    try {
        
        IloModel model(env);
        
        IloArray<IloNumVarArray> s(env, n); // Slack Variables
        IloNumVarArray y(env); // Possibility of each spanning tree (convex combination)
        IloArray<IloRangeArray> constraints(env, n*(n-1)/2); // Constraints
        std::stringstream name; // variable and constraint names
        
        // Create variables s with Variable Constraints
        for(int i = 0; i < n; ++i) {
            s[i] = IloNumVarArray(env, n);
            for(int j = i+1; j < n; ++j) {
                name << "s_" << i << "_" << j;
                s[i][j] = IloNumVar(env, 0, IloInfinity, ILOFLOAT, name.str().c_str()); // 0 <= s_i_j
                name.str(""); // clean name
            }
        }
        
        // Slack Constraints
        for(int i = 0; i < n; ++i) {
            constraints[i] = IloRangeArray(env, n);
            for(int j = i+1; j < n; ++j) {
                name << "slack_" << i << "_" << j;
                constraints[i][j] = IloRange(env, x[i][j], s[i][j], x[i][j], name.str().c_str());
                name.str("");
            }
            name.str(""); // Clean name
            
            model.add(constraints[i]);
        }
        
        // Objective Function
        IloExpr expr(env);
        
        for(int i = 0; i < n; ++i) {
            for(int j = i+1; j < n; ++j) {
                expr += s[i][j];
            }
        }
        IloObjective obj(env, expr, IloObjective::Minimize);
        
        // Add object function to model
        model.add(obj);
        
        expr.end();

        // LP solver object
        IloCplex cplex(model);
        
        // Solve with dual simplex
        cplex.setParam(IloCplex::RootAlg, IloCplex::Primal);
        
        int iter = 0; // for debug
        
        while(1) {
            // Export model to file
            cplex.exportModel("column_generation.lp");
            
            if( !cplex.solve() ) {
                cerr << "Failed to optimize Column Generation" << endl;
                cerr << "Status: " << cplex.getStatus() << endl;
                cerr << "Solver status: " << cplex.getCplexStatus() << endl;
                throw;
            }
            
            // dual graph to get Maximum Spanning Tree
            double** dualGraph = create2dArr<double>(n, 0);
            bool** maxSpan = create2dArr<bool>(n, false);
            
            // get weight by -dual
            IloNum dual;
            for(int i=0; i<n; i++) {
                for(int j=i+1; j<n; j++){
                    dual = cplex.getDual(constraints[i][j]);
                    
                    // symmetric matrix to get MST
                    dualGraph[i][j] = -dual;
                    dualGraph[j][i] = -dual;
                }
            }
            
            if(getMST(dualGraph, maxSpan, n) >= -1e-9) {
                std::cout << "Column Generation success at " << iter << " time" << std::endl;
                std::cout << "Status: " << cplex.getStatus() << std::endl;
                std::cout << "Objective value: " << cplex.getObjValue() << std::endl;
                break;
            } else {
                IloNumColumn col = obj(0);
                
                for(int i=0; i<n; i++) {
                    for(int j=i+1; j<n; j++){
                        if(maxSpan[i][j])
                            col += constraints[i][j](1);
                    }
                }
                name << "column_y_" << iter;
                y.add(IloNumVar(col, 0, IloInfinity, ILOFLOAT, name.str().c_str()));
                name.str("");
                col.end();
                
                spanningTrees.push_back(maxSpan);
            }
            
            iter++;
        }
        
        for (int i=0; i<iter; i++) {
            if(cplex.getValue(y[i]) > 0) {
                cout << "probability: " << cplex.getValue(y[i]) << endl;
                returnTrees.push_back(spanningTrees[i]);
            }
        }
        
    } catch (IloException& e) {
        cerr << "Concert exception caught: " << e << endl;
    } catch (...) {
        cerr << "Unknown exception caught" << endl;
    }
    
    env.end();
    
    return returnTrees;
}

double getMST(double** graph, bool** maxSpan, int n) {
    // Get MST using Prim Algorithm
    int parent[n]; // constructed MST vertex
    double key[n];   // Picked Edge's weight
    bool mstSet[n];  // included vertice
    
    // Initialize as INFINITE
    for (int i = 0; i < n; i++)
        key[i] = MAXFLOAT, mstSet[i] = false;
    
    key[0] = 0;     // include first vertex in MST
    parent[0] = -1; // First node is always root of MST
    
    for (int i = 0; i < n-1; i++) {
        // Pick minimum key vertex that not yet include in mstSet
        double min = MAXFLOAT;
        int min_index = 0;
        
        for (int v = 0; v < n; v++)
            if (mstSet[v] == false && key[v] < min)
                min = key[v], min_index = v;
        
        mstSet[min_index] = true;
        
        // set key value of vertex which is adjacent min_index
        for (int v = 0; v < n; v++)
            if (graph[min_index][v] != 0 && mstSet[v] == false && graph[min_index][v] < key[v])
                parent[v]  = min_index, key[v] = graph[min_index][v];
    }
    
    double sum_weight = 0;
    
    for (int i = 1; i < n; i++){
        sum_weight += graph[i][parent[i]];
        i < parent[i] ? maxSpan[i][parent[i]] = true : maxSpan[parent[i]][i] = true;
    }
    return sum_weight;
}
