#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include "Graph.h"

using namespace std;

int main(int argc, char* argv[]){

    const int START = 3, END = 5;
    const char* INPUT_FILE_MST = "../../datas/tsp-mst/a280.txt";
    const char* INPUT_FILE_FULL = "../../datas/tsp-full/a280.txt";
    const char* MIDDLE_FILE_NAME_ODD = "odd-degree-full.txt";
    const char* MIDDLE_FILE_NAME_BLOSSOM = "blossom_output.txt";
    const char* MIDDLE_FILE_PERFECT = "../../datas/perfect_match/perfect_match.txt";
    const char* MIDDLE_DIR_PERFECT = "../../datas/perfect_match/";
    const char* OUTPUT_FILE_EULER = "../../datas/euler/euler-path.txt";

    int num_node_of_full, num_edge_of_mst, temp1, temp2, temp3;

    // OPEN MST FILE
    ifstream in_file_mst(INPUT_FILE_MST);
    if(!in_file_mst.is_open()){
        cout<<"tsp-mst file open error" << endl;
        return 1;
    }

    cout << "tsp-mst file is opened, start reading" << endl;

    // READ MST FILE
    in_file_mst >> num_node_of_full >> num_edge_of_mst;

    Graph combined_graph(num_node_of_full);
    bool *count = new bool[num_node_of_full];
    fill(count, count+num_node_of_full, false);
    count[START] = count[END] = true;

    while(in_file_mst >> temp1 >> temp2 >> temp3){
        count[temp1] = !count[temp1];
        count[temp2] = !count[temp2];
        combined_graph.addEdge(temp1, temp2);
    }
    in_file_mst.close();
    // END OF READING MST FILE

    cout << "tsp-mst file is closed" << endl;

    int** full_weight_matrix = new int*[num_node_of_full];
    for(int i=0;i<num_node_of_full;i++){
        full_weight_matrix[i] = new int[num_node_of_full];
    }

    // OPEN FULL FILE
    ifstream in_file_full(INPUT_FILE_FULL);
    if(!in_file_full.is_open()){
        cout<<"tsp-full file open error" << endl;
        return 1;
    }

    cout << "tsp-full file is opened, start reading" << endl;

    // READ FULL FILE - read weight info
    in_file_full >> temp1 >> temp2;

    while(in_file_full >> temp1 >> temp2 >> temp3) {
        full_weight_matrix[temp1][temp2] = temp3;
    }
    in_file_full.close();
    // END OF READING FULL FILE

    cout << "tsp-full file is closed" << endl;

    // calculate the num of edges and nodes in odd degree graph

    int num_odd_node=0, num_edge_of_odd;
    vector<int> odd_node_list;
    for(int i=0, j=0;i<num_node_of_full;i++){
        if(count[i]) {
            odd_node_list.push_back(i);
            num_odd_node++;
        }
    }
    num_edge_of_odd = num_odd_node*(num_odd_node-1)/2;

    cout << "writing odd degree node graph" << endl;

    // WRITE MIDDLE FILE - odd degree node(node_1 node_2 weight)
    char* middle_file_name_odd = strdup(MIDDLE_FILE_NAME_ODD);
    const char* middle_file_odd = ("./" + (string)middle_file_name_odd).c_str();
    ofstream out_file_odd(middle_file_odd);

    out_file_odd << num_odd_node <<" "<< num_edge_of_odd<<endl;
    for(int i=0;i<odd_node_list.size();i++){
        for(int j=0;j<odd_node_list.size();j++){
            if(i<j){
                out_file_odd << i <<" "<<j<<" "<<full_weight_matrix[odd_node_list[i]][odd_node_list[j]]<<endl;
            }
        }
    }
    out_file_odd.close();

    cout << "selecting odd node is done, execute blossom v" << endl;

    // EXECUTE BLOSSOM V
    char* absol_current_dir = realpath(".", NULL);
    char* middle_file_name_blossom = strdup(MIDDLE_FILE_NAME_BLOSSOM);
    const char* comm_blossom = ((string)absol_current_dir + "/blossom5 -w "
                                + (string)absol_current_dir + "/" + (string)middle_file_name_blossom + " -e "
                                + (string)absol_current_dir + "/" + (string)middle_file_name_odd).c_str();
    free(absol_current_dir);
    system(comm_blossom);

    cout << "blossom v is done, map the fake node num to real node num.." << endl;


    // CHECK OUTPUT DIR
    char* middle_dir_perfect = strdup(MIDDLE_DIR_PERFECT);
    const char* comm_mk_perfect = ("mkdir " + (string)middle_dir_perfect).c_str();
    system(comm_mk_perfect);

    // READ BLOSSOM V OUTPUT FILE

    const char* output_blossom_file = ("./" + (string)middle_file_name_blossom).c_str();
    ifstream in_file_blossom(output_blossom_file);

    if(!in_file_blossom.is_open()){
        cout<<"blossom output file open error" << endl;
        return 1;
    }

    ofstream out_file_blossom(MIDDLE_FILE_PERFECT);
    int num_edge_of_blossom;
    in_file_blossom >> num_odd_node >> num_edge_of_blossom;
    out_file_blossom << num_odd_node << " " << num_edge_of_blossom << endl;
    while(in_file_blossom >> temp1 >> temp2){
        out_file_blossom << odd_node_list[temp1] << " " << odd_node_list[temp2] << " "
                         << full_weight_matrix[odd_node_list[temp1]][odd_node_list[temp2]] << endl;
        combined_graph.addEdge(odd_node_list[temp1], odd_node_list[temp2]);
    }
    in_file_blossom.close();
    out_file_blossom.close();
    // END OF READING BLOSSOM V OUTPUT FILE

    cout << "remove middle files" << endl;

    // REMOVE ODD NODE GRAPH FILE AND BLOSSOM V OUTPUT FILE
    if( remove( MIDDLE_FILE_NAME_ODD ) != 0  || remove(MIDDLE_FILE_NAME_BLOSSOM) != 0)
        perror( "error deleting file" );

    cout << "perfect matching is done, find euler path" << endl;


    // Find Euler Path
    ofstream out_file_euler(OUTPUT_FILE_EULER);
    combined_graph.printEulerTour(out_file_euler);
    out_file_euler.close();

    cout <<"done" <<endl;

    return 0;
}