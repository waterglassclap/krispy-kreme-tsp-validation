#include <ilcplex/ilocplex.h>
#include <cmath>
#include <limits>
#include "validation.h"

ILOSTLBEGIN
#define MYX(i, j) ((i) < (j) ? x[(i)][(j)] : x[(j)][(i)])

// held_karp_relaxation
float** held_karp_lp(int n, float** cost);
bool check_validation(const IloEnv& env, const IloCplex& cplex, const IloModel& model, const IloArray<IloNumVarArray>& x, std::stringstream& name, int n);
void print_solution(std::ostream& out, const IloCplex& cplex, const IloArray<IloNumVarArray>& x, int n); // TODO: deprecate
// column_generation
void column_generation(int n, float** cost, float** x);

const int start_node = 0, end_node = 13;

//int main (int argc, char **argv)
//{
//    const int start_node = 0, end_node = 28;
//    
//    float** solution = new float*[29];
//    for(int i=0; i<29; i++) {
//        solution[i] = new float[29];
//        for (int j = 0; j < 29; ++j) {
//            solution[i][j] = 0.0f;
//        }
//    }
//    
//    solution[0][27] = 1;
//    solution[1][19] = 1;
//    solution[1][20] = 1;
//    solution[2][25] = 1;
//    solution[2][28] = 1;
//    solution[3][9] = 1;
//    solution[3][14] = 1;
//    solution[4][8] = 1;
//    solution[4][20] = 1;
//    solution[5][11] = 1;
//    solution[5][27] = 1;
//    solution[6][22] = 1;
//    solution[6][24] = 1;
//    solution[7][23] = 1;
//    solution[7][26] = 1;
//    solution[8][4] = 1;
//    solution[8][11] = 1;
//    solution[9][3] = 1;
//    solution[9][19] = 1;
//    solution[10][14] = 1;
//    solution[10][21] = 1;
//    solution[11][5] = 1;
//    solution[11][8] = 1;
//    solution[12][15] = 1;
//    solution[12][23] = 1;
//    solution[13][17] = 1;
//    solution[14][3] = 1;
//    solution[14][10] = 1;
//    solution[15][12] = 1;
//    solution[15][18] = 1;
//    solution[16][17] = 1;
//    solution[16][21] = 1;
//    solution[17][13] = 1;
//    solution[17][16] = 1;
//    solution[18][15] = 1;
//    solution[18][24] = 1;
//    solution[19][1] = 1;
//    solution[19][9] = 1;
//    solution[20][1] = 1;
//    solution[20][4] = 1;
//    solution[21][10] = 1;
//    solution[21][16] = 1;
//    solution[22][6] = 1;
//    solution[22][26] = 1;
//    solution[23][7] = 1;
//    solution[23][12] = 1;
//    solution[24][6] = 1;
//    solution[24][18] = 1;
//    solution[25][2] = 1;
//    solution[25][28] = 1;
//    solution[26][7] = 1;
//    solution[26][22] = 1;
//    solution[27][0] = 1;
//    solution[27][5] = 1;
//    solution[28][2] = 1;
//    solution[28][25] = 1;
//    solution[27][0] = 1;
//    solution[19][1] = 1;
//    solution[20][1] = 1;
//    solution[25][2] = 1;
//    solution[28][2] = 1;
//    solution[9][3] = 1;
//    solution[14][3] = 1;
//    solution[8][4] = 1;
//    solution[20][4] = 1;
//    solution[11][5] = 1;
//    solution[27][5] = 1;
//    solution[22][6] = 1;
//    solution[24][6] = 1;
//    solution[23][7] = 1;
//    solution[26][7] = 1;
//    solution[4][8] = 1;
//    solution[11][8] = 1;
//    solution[3][9] = 1;
//    solution[19][9] = 1;
//    solution[14][10] = 1;
//    solution[21][10] = 1;
//    solution[5][11] = 1;
//    solution[8][11] = 1;
//    solution[15][12] = 1;
//    solution[23][12] = 1;
//    solution[17][13] = 1;
//    solution[3][14] = 1;
//    solution[10][14] = 1;
//    solution[12][15] = 1;
//    solution[18][15] = 1;
//    solution[17][16] = 1;
//    solution[21][16] = 1;
//    solution[13][17] = 1;
//    solution[16][17] = 1;
//    solution[15][18] = 1;
//    solution[24][18] = 1;
//    solution[1][19] = 1;
//    solution[9][19] = 1;
//    solution[1][20] = 1;
//    solution[4][20] = 1;
//    solution[10][21] = 1;
//    solution[16][21] = 1;
//    solution[6][22] = 1;
//    solution[26][22] = 1;
//    solution[7][23] = 1;
//    solution[12][23] = 1;
//    solution[6][24] = 1;
//    solution[18][24] = 1;
//    solution[2][25] = 1;
//    solution[28][25] = 1;
//    solution[7][26] = 1;
//    solution[22][26] = 1;
//    solution[0][27] = 1;
//    solution[5][27] = 1;
//    solution[2][28] = 1;
//    solution[25][28] = 1;
//    
//    ErrorInfo stErrorInfo = getStErrorInfo(start_node, end_node, 29, solution);
//    ErrorInfos nonStErrorInfos = getNonStErrorInfos(start_node, end_node, 29, solution);
//    
//    ErrorInfo maxErrorInfo;
//    maxErrorInfo.severity = -1;
//    bool isST = false;
//    
//    cout << endl << endl << endl << endl;
//    if (stErrorInfo.isValid) {
//        maxErrorInfo = stErrorInfo;
//        isST = true;
//        cout << "severity st: " << stErrorInfo.severity << " " << stErrorInfo.minCutInfo.size << endl;
//    }
//    for (int i = 0; i < nonStErrorInfos.num; i++) {
//        if(nonStErrorInfos.infos[i].severity > maxErrorInfo.severity) {
//            maxErrorInfo = nonStErrorInfos.infos[i];
//            isST = false;
//            cout << "severity " << i << "st: " << nonStErrorInfos.infos[i].severity << " " << nonStErrorInfos.infos[i].minCutInfo.size << endl;
//        }
//    }
//    
//    if(maxErrorInfo.severity == -1) {
//        cout << "there is no StErrorInfos" << endl;
//        return false;
//    }
//    
//    cout << "maxError mincut size info" << endl;
//    cout << "serverity: " << maxErrorInfo.severity << endl << "minCutSize: " << maxErrorInfo.minCutInfo.size << endl << "totalFlow: " << maxErrorInfo.minCutInfo.totalFlow << endl;
//    
//    
//    cout << endl << endl << endl << endl;
//    for(int i = 0; i < 29; ++i) {
//        for(int j = 0; j < 29; ++j) {
//            cout << solution[i][j] << " ";
//        }
//        cout << endl;
//    }
//    cout << endl << endl << endl << endl;
//    
//    
//    return 0;
//}

int main (int argc, char **argv) {
    const char* filename  = "/Users/ttopre/Downloads/bays29.txt";
    ifstream file(filename);
    if (!file) {
        cerr << "ERROR: could not open file '" << filename
        << "' for reading" << endl;
        cerr << "usage:   " << argv[0] << " <file>" << endl;
        throw(-1);
    }
    
    int node_num, edge_num;
    file >> node_num >> edge_num;
    
    float** full_weight_matrix = new float*[node_num];
    for(int i=0; i<node_num; i++) {
        full_weight_matrix[i] = new float[node_num];
    }
    
    int temp_i, temp_j, temp_w;
    while(file >> temp_i >> temp_j >> temp_w) {
        full_weight_matrix[temp_i][temp_j] = temp_w;
    }
    
    float** held_karp_optimal = held_karp_lp(node_num, full_weight_matrix);
    
    if(held_karp_optimal == 0) {
        cout << "Held Karp Cplex Error" << endl;
        return 0;
    } else {
        //column_generation(node_num, full_weight_matrix, held_karp_optimal);
    }
    
    return 0;
}

float** held_karp_lp(int n, float** cost) {
    IloEnv env;
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
    
    int iter = 0;
    while(1) {
        // Export model to file
        cplex.exportModel("newmodel.lp");
        
        bool solved = false;
        
        try {
            solved = cplex.solve();
        } catch(const IloException& e) {
            std::cerr << std::endl << std::endl;
            std::cerr << "CPLEX Raised an exception:" << std::endl;
            std::cerr << e << std::endl;
            env.end();
            throw;
        }
        
        if(solved) {
            std::cout << "Cplex success at " << iter << " time" << std::endl;
            std::cout << "\tStatus: " << cplex.getStatus() << std::endl;
            std::cout << "\tObjective value: " << cplex.getObjValue() << std::endl;
            print_solution(std::cout, cplex, x, n);
            if(!check_validation(env, cplex, model, x, name, n)) {
                break;
            }
        } else {
            std::cerr << "Cplex error!" << std::endl;
            std::cerr << "\tStatus: " << cplex.getStatus() << std::endl;
            std::cerr << "\tSolver status: " << cplex.getCplexStatus() << std::endl;
            return 0;
        }
        
        iter++;
    }
    
    // Finally, held karp optimal graph
    float** optimal = new float*[n];
    for(int i=0; i<n; i++) {
        optimal[i] = new float[n];
        for (int j = 0; j < n; ++j) {
            optimal[i][j] = 0.0f;
        }
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

    /////////프린트 테스트
    cout << endl << endl << endl << endl;
    for(int i = 0; i < n; ++i) {
        for(int j = i+1; j < n; ++j) {
            if(cplex.getValue(MYX(i,j)) > 1e-10) {
             cout << cplex.getValue(MYX(i,j)) << " : x_" << i << "_" << j << std::endl;
            }
        }
        cout << endl;
    }
    std::cout << "\tObjective value: " << cplex.getObjValue() << std::endl;
    cout << endl << endl << endl << endl;
    /////////프린트 테스트
    
    env.end();
    
    return optimal;
}

bool check_validation(const IloEnv& env, const IloCplex& cplex, const IloModel& model, const IloArray<IloNumVarArray>& x, std::stringstream& name, int n) {
    float** solution = new float*[n];
    for(int i=0; i<n; i++) {
        solution[i] = new float[n];
        for (int j = 0; j < n; ++j) {
            solution[i][j] = 0.0f;
        }
    }
    
    // Enter cplex solver result graph
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            if(i != j && cplex.getValue(MYX(i, j)) != 0) {
                solution[i][j] = cplex.getValue(MYX(i, j));
            }
        }
    }
    
    cout << endl << endl << endl << endl;
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            cout << solution[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl << endl << endl << endl;
    
    ErrorInfo stErrorInfo = getStErrorInfo(start_node, end_node, n, solution);
    ErrorInfos nonStErrorInfos = getNonStErrorInfos(start_node, end_node, n, solution);
    
    ErrorInfo maxErrorInfo;
    maxErrorInfo.severity = -1;
    bool isST = false;
    
    if (stErrorInfo.isValid) {
        maxErrorInfo = stErrorInfo;
        isST = true;
        cout << "severity st: " << stErrorInfo.severity << " " << stErrorInfo.minCutInfo.size << endl;
    }
    for (int i = 0; i < nonStErrorInfos.num; i++) {
        if(nonStErrorInfos.infos[i].severity > maxErrorInfo.severity) {
            maxErrorInfo = nonStErrorInfos.infos[i];
            isST = false;
            cout << "severity " << i << "st: " << nonStErrorInfos.infos[i].severity << " " << nonStErrorInfos.infos[i].minCutInfo.size << endl;
        }
    }
    
    if(maxErrorInfo.severity == -1) {
        return false;
    }
    
    cout << "maxError mincut size info" << endl;
    cout << maxErrorInfo.severity << endl << maxErrorInfo.minCutInfo.size << endl << maxErrorInfo.minCutInfo.totalFlow << endl;
    cout << "s t info" << endl;
    cout << maxErrorInfo.minCutInfo.edgeInfos[0].source << endl << maxErrorInfo.minCutInfo.edgeInfos[0].sink << endl << maxErrorInfo.minCutInfo.edgeInfos[0].weight;
    
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

void print_solution(std::ostream& out, const IloCplex& cplex, const IloArray<IloNumVarArray>& x, int n) {
    assert(x.getSize() == n);
    
    out << std::endl << std::endl << "Solution:" << std::endl;
    for(auto i = 0u; i < n; ++i) {
        assert(x[i].getSize() == n);
        for(auto j = 0u; j < n; ++j) {
            if(i != j){
                if(cplex.getValue(MYX(i, j)) != 0) {
                    out << cplex.getValue(MYX(i, j)) << " : x_" << i << "_" << j << std::endl;
                }
            }
        }
    }
}

void column_generation(int n, float** cost, float** x) {
    IloEnv env;
    IloModel model(env);
    
    // Slack Variables
    IloArray<IloNumVarArray> s(env, n);
    // Constraints
    IloRangeArray constraints(env);
    
    // variable and constraint names
    std::stringstream name;
    
    // Create variables s with Variable Constraints
    for(int i = 0; i < n; ++i) {
        s[i] = IloNumVarArray(env, n);
        for(int j = 0; j < n; ++j) {
            if(i != j) {
                name << "s_" << i << "_" << j;
                s[i][j] = IloNumVar(env, 0, IloInfinity, ILOFLOAT, name.str().c_str()); // 0 <= s_i_j
                name.str(""); // clean name
            }
        }
    }
    
    IloExpr expr(env);
    
    // Slack Constraints
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            constraints.add(IloRange(env, x[i][j], s[i][j], x[i][j], name.str().c_str()));
        }
        name.str(""); // Clean name
    }
    
    // Add constraints to the model
    model.add(constraints);
    
    // Objective Function
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            if (i != j){
                expr += s[i][j];
            }
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
    
    // Export model to file
    cplex.exportModel("column_generation.lp");
    
    bool solved = false;
    
    try {
        solved = cplex.solve();
    } catch(const IloException& e) {
        std::cerr << std::endl << std::endl;
        std::cerr << "CPLEX Raised an exception:" << std::endl;
        std::cerr << e << std::endl;
        env.end();
        throw;
    }
    
    if(solved) {
        // If CPLEX successfully solved the model, print the results
        std::cout << std::endl << std::endl;
        std::cout << "Cplex success!" << std::endl;
        std::cout << "\tStatus: " << cplex.getStatus() << std::endl;
        std::cout << "\tObjective value: " << cplex.getObjValue() << std::endl;
        print_solution(std::cout, cplex, s, n);
    } else {
        std::cerr << "Cplex error!" << std::endl;
        std::cerr << "\tStatus: " << cplex.getStatus() << std::endl;
        std::cerr << "\tSolver status: " << cplex.getCplexStatus() << std::endl;
        throw;
    }
    
    // dual variables
    IloNumArray duals(env);
    cplex.getDuals(duals, constraints);
    cout << "Duals      :" << endl << duals << endl;
    
    env.end();
}
