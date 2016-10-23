#include <iostream>
#include <fstream>
#include <vector>
#include "Graph/Graph.h"
#include "Blossom/PerfectMatching.h"

using namespace std;

// functions
Graph* get_mst_edges_and_combine_graph_mst(int& num_node_of_full, vector<bool>& count);
void get_weight_info(int num_node_of_full);
PerfectMatching* blossom_v(int& num_odd_node, PerfectMatching::Options options, vector<int>& odd_node_list, int num_node_of_full, vector<bool>& count);
void combine_graph_blossom_v(Graph &combined_graph, PerfectMatching* &pm, int num_odd_node, vector<int>& odd_node_list);

const int START = 3, END = 5;
const char* INPUT_FILE_MST = "../../datas/tsp-mst/a280.txt";
const char* INPUT_FILE_FULL = "../../datas/tsp-full/a280.txt";
const char* OUTPUT_FILE_EULER = "../../datas/euler/euler-path.txt";

int** full_weight_matrix;

int main(int argc, char* argv[]){

    int num_node_of_full;
    Graph* combined_graph;
    vector<bool> count;
    try {
        combined_graph = get_mst_edges_and_combine_graph_mst(num_node_of_full, count);
        get_weight_info(num_node_of_full);
    }catch(int i){
        if(i==1)
            cout<<"ERR: tsp-mst file open error" << endl;
        else
            cout<<"ERR: tsp-full file open error" << endl;
        return 1;
    }

    int num_odd_node;
    vector<int> odd_node_list;
    struct PerfectMatching::Options options;

    PerfectMatching* pm = blossom_v(num_odd_node, options, odd_node_list, num_node_of_full, count);
    combine_graph_blossom_v(*combined_graph, pm, num_odd_node, odd_node_list);
    delete pm;

    // Find Euler Path
    ofstream out_file_euler(OUTPUT_FILE_EULER);
    combined_graph->printEulerTour(out_file_euler);
    out_file_euler.close();
    delete combined_graph;


    for(int i=0;i<num_node_of_full;i++){
        delete[] full_weight_matrix[i];
    }
    delete full_weight_matrix;


    cout <<"INFO: done" <<endl;
    return 0;
}





/*
 * count degree of each node and save it in 'count'
 * add mst's edges to 'combined_graph'
 */
Graph* get_mst_edges_and_combine_graph_mst(int& num_node_of_full, vector<bool>& count){
    int num_edge_of_mst, temp1, temp2, temp3;

    ifstream in_file_mst(INPUT_FILE_MST);
    if(!in_file_mst.is_open()){
        throw 1;
    }
    cout << "INFO: tsp-mst file is opened, start reading" << endl;

    in_file_mst >> num_node_of_full >> num_edge_of_mst;
    Graph* combined_graph = new Graph(num_node_of_full);

    count.resize(num_node_of_full);
    fill(count.begin(), count.end(), false);
    count[START] = count[END] = true;
    while(in_file_mst >> temp1 >> temp2 >> temp3){
        count[temp1] = !count[temp1];
        count[temp2] = !count[temp2];
        combined_graph->addEdge(temp1, temp2);
    }
    in_file_mst.close();

    cout << "INFO: tsp-mst file is closed" << endl;
    return combined_graph;
}


/*
 * get each of the edges' weight and save it in 'full_weight_matrix'
 */
void get_weight_info(int num_node_of_full){
    int temp1, temp2, temp3;
    full_weight_matrix = new int*[num_node_of_full];
    for(int i=0;i<num_node_of_full;i++){
        full_weight_matrix[i] = new int[num_node_of_full];
    }

    ifstream in_file_full(INPUT_FILE_FULL);
    if(!in_file_full.is_open()){
        throw 2;
    }

    cout << "INFO: tsp-full file is opened, start reading" << endl;

    in_file_full >> temp1 >> temp2;

    while(in_file_full >> temp1 >> temp2 >> temp3) {
        full_weight_matrix[temp1][temp2] = temp3;
    }
    in_file_full.close();

    cout << "INFO: tsp-full file is closed" << endl;
}

/*
 * set 'num_odd_node' as the number of odd-degree nodes
 * run blossom v and save the result in 'pm'
 * pm's node numbering is different, so keep this transition info in 'odd_node_list'
 */
PerfectMatching* blossom_v(int& num_odd_node, PerfectMatching::Options options,
               vector<int>& odd_node_list, int num_node_of_full, vector<bool>& count){

    //prepare data
    num_odd_node=0;
    for(int i=0;i<num_node_of_full;i++){
        if(count[i]) {
            odd_node_list.push_back(i); // compress for blossomv
            num_odd_node++;
        }
    }
    int num_edge_of_odd = num_odd_node*(num_odd_node-1)/2;

    int* edges = new int[2*num_edge_of_odd];
    int* weights = new int[num_edge_of_odd];

    int e = 0;
    for(int i=0;i<odd_node_list.size();i++){
        for(int j=0;j<odd_node_list.size();j++){
            if(i<j){
                edges[2*e] = i;
                edges[2*e+1] = j;
                weights[e] = full_weight_matrix[odd_node_list[i]][odd_node_list[j]];
                e++;
            }
        }
    }
    if(e!=num_edge_of_odd) cout << "WARN: e!=num_edge_of_odd"<<endl;

    cout << "INFO: execute blossom v" << endl;
    PerfectMatching* pm = new PerfectMatching(num_odd_node, num_edge_of_odd);
    for (e=0; e<num_edge_of_odd; e++) pm->AddEdge(edges[2*e], edges[2*e+1], weights[e]);
    pm->options = options;
    pm->Solve();
    double cost = ComputePerfectMatchingCost(num_odd_node, num_edge_of_odd, edges, weights, pm);
    printf("INFO: cost = %.1f\n", cost);

    delete [] edges;
    delete [] weights;
    return pm;
}

/*
 * add perfect matching graph's edges to 'combined_graph'
 */
void combine_graph_blossom_v(Graph &combined_graph, PerfectMatching* &pm, int num_odd_node, vector<int>& odd_node_list){
    for(int i=0;i<num_odd_node;i++){
        int j = pm->GetMatch(i);
        if(i<j)
            combined_graph.addEdge(odd_node_list[i], odd_node_list[j]);
    }
}