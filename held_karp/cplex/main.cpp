#include <ilcplex/ilocplex.h>
#include <cmath>
#include <limits>
ILOSTLBEGIN

void held_karp_lp(int n, int** cost);
void print_solution(std::ostream& out, const IloCplex& cplex, const IloArray<IloNumVarArray>& x, int n);
const int start_node = 3, end_node = 5;

int main (int argc, char **argv)
{
    const char* filename  = "/Users/ttopre/Downloads/d1655.txt";
    ifstream file(filename);
    if (!file) {
        cerr << "ERROR: could not open file '" << filename
        << "' for reading" << endl;
        cerr << "usage:   " << argv[0] << " <file>" << endl;
        throw(-1);
    }
    
    int node_num, edge_num;
    file >> node_num >> edge_num;
    
    int** full_weight_matrix = new int*[node_num];
    for(int i=0; i<node_num; i++) {
        full_weight_matrix[i] = new int[node_num];
    }
    
    int temp_i, temp_j, temp_w;
    while(file >> temp_i >> temp_j >> temp_w) {
        full_weight_matrix[temp_i][temp_j] = temp_w;
    }
    
    held_karp_lp(node_num, full_weight_matrix);
    
    return 0;
}

void held_karp_lp(int n, int** cost) {
    IloEnv env;
    IloModel model(env);
    
    // Variables
    IloArray<IloNumVarArray> x(env, n);
    
    // Degree Constraints
    IloRangeArray degree_constraints(env, n);  // Constraints 1)
    
    // variable and constraint names
    std::stringstream name;

    // Create variables x with Variable Constraints
    for(int i = 0; i < n; ++i) {
        x[i] = IloNumVarArray(env, n);
        for(int j = 0; j < n; ++j) {
            if(i != j) {
                name << "x_" << i << "_" << j;
                x[i][j] = IloNumVar(env, 0, 1, ILOFLOAT, name.str().c_str()); // 0 <= x_i_j <= 1
                name.str(""); // clean name
            }
        }
    }
    
    IloExpr expr(env);
    
    // Degree Constraints
    // sum(j, x[j][i]) == 1, For all i
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            if(i != j) {
                expr += x[i][j];
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
    
    // Add constraints to the model
    model.add(degree_constraints);
    
    // Objective Function
    // min sum((i,j), cost[i][j] * x[i][j])
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            if (i != j){
                expr += cost[i][j] * x[i][j];
            }
        }
    }
    IloObjective obj(env, expr, IloObjective::Minimize);
    
    // Add object function to model
    model.add(obj);
    
    expr.end();
    
    // LP solver object
    IloCplex cplex(model);
    
    // Export model to file
    cplex.exportModel("model.lp");
    
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
        print_solution(std::cout, cplex, x, n);
    } else {
        std::cerr << "Cplex error!" << std::endl;
        std::cerr << "\tStatus: " << cplex.getStatus() << std::endl;
        std::cerr << "\tSolver status: " << cplex.getCplexStatus() << std::endl;
    }
    
    env.end();
}

void print_solution(std::ostream& out, const IloCplex& cplex, const IloArray<IloNumVarArray>& x, int n) {
    assert(x.getSize() == n);
    
    out << std::endl << std::endl << "Solution:" << std::endl;
    for(auto i = 0u; i < n; ++i) {
        assert(x[i].getSize() == n);
        for(auto j = 0u; j < n; ++j) {
            if(i != j){
                if(cplex.getValue(x[i][j]) != 0) {
                    out << cplex.getValue(x[i][j]) << " : x_" << i << "_" << j << std::endl;
                }
            }
        }
    }
}
