#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "Graph/Graph.h"
#include "Blossom/PerfectMatching.h"

using namespace std;
typedef void(*pt2func)(list<int> *euler_path, int num_node_of_full);

// functions
pt2func set_comm_arg(int n, char* arg[]);
void take_first(list<int> *euler_path, int num_node_of_full);
void take_second(list<int> *euler_path, int num_node_of_full);
void take_random(list<int> *euler_path, int num_node_of_full);
void greedy(list<int> *euler_path, int num_node_of_full);
void greedy2(list<int> *euler_path, int num_node_of_full);
int total_cost(list<int> *euler_path);
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
    count.clear(); //free memory
    count.shrink_to_fit();
    odd_node_list.clear();
    odd_node_list.shrink_to_fit();
    delete pm;

    // Find an Euler Path
    list<int> *euler_path = new list<int>;
    combined_graph->euler_path = euler_path;
    ofstream out_file_euler(OUTPUT_FILE_EULER);
    combined_graph->printEulerTour(out_file_euler);
    out_file_euler.close();
    delete combined_graph;

    pt2func short_cutting_func = set_comm_arg(argc, argv);
    cout <<"INFO: short cutting by ";
    short_cutting_func(euler_path, num_node_of_full);
    int final_cost = total_cost(euler_path);
    cout << "INFO: total cost of s-t path TSP is "<<final_cost<<endl;

    //test short cutting
    set<int> test_set(euler_path->begin(), euler_path->end());
    cout <<"total: "<< num_node_of_full <<endl
         <<"list: "<<euler_path->size()<<endl
         <<"set: "<<test_set.size()<<endl;
    cout <<"start: "<<*euler_path->begin()<<" end: "<<*prev(euler_path->end())<<endl;
    cout << "it should be (total=list=set) or short cutting has an error"<<endl;

    delete euler_path;
    for(int i=0;i<num_node_of_full;i++){
        delete[] full_weight_matrix[i];
    }
    delete full_weight_matrix;

    return 0;
}





pt2func set_comm_arg(int n, char* arg[]){
    map<string, int> comm={
            {"take_first",0},
            {"greedy", 1},
            {"take_second", 2},
            {"greedy2", 3},
            {"take_random", 4}
    };
    switch (comm[arg[1]]){
        case 0:
            return &take_first;
        case 1:
            return &greedy;
        case 2:
            return &take_second;
        case 3:
            return &greedy2;
        case 4:
            return &take_random;
        default:
            cout<<"ERR: unknown command line argument"<<endl;
            throw 1;
    }
}

void take_first(list<int> *euler_path, int num_node_of_full){
    cout << "'take_first'"<<endl;
    vector<bool> visited(num_node_of_full, false);
    list<int>::iterator iter = euler_path->begin();
    while(iter!=euler_path->end()){
        if(visited[*iter]){
            iter = euler_path->erase(iter);
        }else if(*iter==END && iter!=prev(euler_path->end())){
            iter = euler_path->erase(iter);
        }else{
            visited[*iter] = true;
            iter++;
        }
    }
    visited.clear();
    visited.shrink_to_fit();
}

void take_second(list<int> *euler_path, int num_node_of_full){
    cout << "'take_second'"<<endl;
    vector< list<int>::iterator > visited(num_node_of_full, euler_path->end());
    list<int>::iterator iter = euler_path->begin();
    while(iter!=euler_path->end()){
        if(visited[*iter] != euler_path->end()){
            if(*iter==START){
                iter = euler_path->erase(iter);
            }else{
                euler_path->erase(visited[*iter]);
                visited[*iter] = iter;
                iter++;
            }
        }else{
            visited[*iter] = iter;
            iter++;
        }
    }
    visited.clear();
    visited.shrink_to_fit();
}


void take_random(list<int> *euler_path, int num_node_of_full){
    cout << "'take_random'"<<endl;
    srand (time(NULL));
    vector< list<int>::iterator > visited(num_node_of_full, euler_path->end());
    list<int>::iterator iter = euler_path->begin();

    while(iter!=prev(euler_path->end())){
        if(*iter==END){
            iter = euler_path->erase(iter);
        }else if(visited[*iter] != euler_path->end()){ // second visit
            if(*iter==START) {
                iter = euler_path->erase(iter);
            }else{
                int prob = rand()%2;
                if(prob==0){
                    iter = euler_path->erase(iter);
                }else{
                    euler_path->erase(visited[*iter]);
                    visited[*iter] = iter;
                    iter++;
                }
            }
        }else{ // first visit
            visited[*iter] = iter;
            iter++;
        }
    }
    visited.clear();
    visited.shrink_to_fit();

}


void greedy2(list<int> *euler_path, int num_node_of_full){
    cout << "'greedy2'"<<endl;
    list<int>::iterator
            junction_A=euler_path->end(),
            junction_B= euler_path->end(),
            junction_C=euler_path->end();
    vector< list<int>::iterator > visited(num_node_of_full, euler_path->end());
    list<int>::iterator iter = euler_path->begin();

    // first iteration: remove duplicated visit of first or last node
    iter++;
    while(iter!=prev(euler_path->end())){
        if(*iter==START or *iter==END){
            iter = euler_path->erase(iter);
        }
        iter++;
    }

    // second iteration: set junctions -> compare costs -> short cutting
    iter = euler_path->begin();
    while(iter!=euler_path->end()){
        if(visited[*iter] != euler_path->end()){ // duplicated visit
            set<int> dup_visit;
            dup_visit.insert(*iter);

            // set junction A, B, C
            junction_A = prev(visited[*iter]);
            junction_B = prev(iter);
            iter++;
            while(visited[*iter]!=euler_path->end()){ //iterate until visiting a new node
                long old_a = distance(euler_path->begin(), junction_A);
                long prob_a = distance(euler_path->begin(), prev(visited[*iter]));
                if(old_a > prob_a) {
                    junction_A = prev(visited[*iter]);
                }
                if(*iter != *junction_B)
                    dup_visit.insert(*iter);
                iter++;
            }
            junction_C = iter;
            visited[*iter] = iter;
            iter++;


            // calculate cost
            int cost_prev=0, cost_curr=0;
            set<int> copy_dup_visit = dup_visit;
            // prev_cost
            list<int>::iterator temp_iter = junction_A;
            while (temp_iter!=junction_B){
                cost_prev+=full_weight_matrix[*temp_iter][*next(temp_iter)];
                temp_iter++;
            }
            cost_prev+=full_weight_matrix[*temp_iter][*junction_C];
            // curr_cost
            temp_iter = junction_A;
            list<int>::iterator next_temp_iter = next(temp_iter);
            while(next_temp_iter!=junction_B){
                if(copy_dup_visit.find(*next_temp_iter) == copy_dup_visit.end())
                    cost_curr+=full_weight_matrix[*temp_iter][*next_temp_iter];
                temp_iter++;
                next_temp_iter++;
            }
            cost_curr+=full_weight_matrix[*temp_iter][*next_temp_iter];
            temp_iter++;// = junction_B
            while (temp_iter!=junction_C){
                if(copy_dup_visit.find(*next(temp_iter))!= copy_dup_visit.end()){
                    cost_curr += full_weight_matrix[*temp_iter][*next(temp_iter)];
                    copy_dup_visit.erase(*next(temp_iter));
                }
                temp_iter++;
            }


            //short cutting
            if(cost_curr > cost_prev){
                iter = junction_B;
                iter++;
                while(iter!=junction_C){
                    iter = euler_path->erase(iter);
                }
                visited[*iter] = iter;
            }else{
                iter = junction_A;
                iter++;
                while(iter!=junction_B){
                    if(dup_visit.find(*iter)!=dup_visit.end()){
                        iter = euler_path->erase(iter);
                    }else{
                        iter++;
                    }
                }
                iter++;
                while(iter!=junction_C){
                    if(*iter!=*junction_B) {
                        visited[*iter] = iter;
                        iter++;
                    }else {
                        iter = euler_path->erase(iter);
                    }
                }
                visited[*iter] = iter;
            }
            iter++;
        }else{ // first visit
            visited[*iter] = iter;
            iter++;
        }

    }
    visited.clear();
    visited.shrink_to_fit();
}


void greedy(list<int> *euler_path, int num_node_of_full){
    cout << "'greedy'"<<endl;
    vector< list<int>::iterator > visited(num_node_of_full, euler_path->end());
    list<int>::iterator iter = euler_path->begin();
    while(iter!=euler_path->end()){
        if(visited[*iter] != euler_path->end() && visited[*iter] != euler_path->begin() ){
            int cost_prev = 0, cost_curr = 0;
            list<int>::iterator prev_iter = visited[*iter];
            cost_curr+=full_weight_matrix[*prev(iter)][*iter];
            cost_curr+=full_weight_matrix[*iter][*next(iter)];
            cost_curr-=full_weight_matrix[*prev(iter)][*next(iter)];

            cost_prev+=full_weight_matrix[*prev(prev_iter)][*prev_iter];
            cost_prev+=full_weight_matrix[*prev_iter][*next(prev_iter)];
            cost_prev-=full_weight_matrix[*prev(prev_iter)][*next(prev_iter)];

            if(cost_curr > cost_prev){
                iter = euler_path->erase(iter);
            }else{
                visited[*iter] = iter;
                euler_path->erase(prev_iter);
                iter++;
            }
        }else if(*iter==END && iter!=prev(euler_path->end())){
            iter = euler_path->erase(iter);
        }else if(*iter==START && iter!=euler_path->begin()){
            iter = euler_path->erase(iter);
        }else{
            visited[*iter] = iter;
            iter++;
        }
    }
    visited.clear();
    visited.shrink_to_fit();
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
        full_weight_matrix[temp2][temp1] = temp3;
    }
    in_file_full.close();

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
    int perfect_match_cost = ComputePerfectMatchingCost(num_odd_node, num_edge_of_odd, edges, weights, pm);
    cout <<"INFO: perfect matching cost = " <<perfect_match_cost<<endl;

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

/*
 * return: cost of given s-t path TSP
 */
int total_cost(list<int> *euler_path){
    int cost=0;
    list<int>::iterator iter = euler_path->begin(), iter_last = prev(euler_path->end());
    if(euler_path->empty()){
        cout << "WARN: euler path is empty"<<endl;
        return 0;
    }
    while(true){
        cost+=full_weight_matrix[*iter][*next(iter)];
        iter++;
        if(iter == iter_last) break;
    }
    return cost;
}