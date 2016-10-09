#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <vector>

using namespace std;

int main(int argc, char* argv[]){

    const int START = 3, END = 5;
    const char* INPUT_FILE_MST = "../../datas/tsp-mst/a280.txt";
    const char* INPUT_FILE_FULL = "../../datas/tsp-full/a280.txt";
    const char* MIDDLE_ODD_FILE_NAME = "odd-degree-full.txt";
    const char* OUTPUT_DIR = "../../datas/perfect_match/";
    const char* OUTPUT_FILE = "../../datas/perfect_match/perfect_match.txt";
    string output_blossom_file_name = "blossom_output.txt";

    int node_num, mst_edge_num, temp1, temp2, temp3;

    // OPEN MST FILE
    ifstream in_file_mst(INPUT_FILE_MST);
    if(!in_file_mst.is_open()){
        cout<<"tsp-mst file open error" << endl;
        return 1;
    }

    cout << "tsp-mst file is opened, start reading" << endl;

    // READ MST FILE
    in_file_mst >> node_num >> mst_edge_num;
    bool *count = new bool[node_num];
    fill(count, count+node_num, false);
    count[START] = count[END] = true;

    while(in_file_mst >> temp1 >> temp2 >> temp3){
        count[temp1] = !count[temp1];
        count[temp2] = !count[temp2];
    }
    in_file_mst.close();
    // END OF READING MST FILE

    cout << "tsp-mst file is closed" << endl;

    int** full_weight_matrix = new int*[node_num];
    for(int i=0;i<node_num;i++){
        full_weight_matrix[i] = new int[node_num];
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

    int num_odd_node=0, edge_num_of_perfect_graph;
    vector<int> odd_node_list;
    for(int i=0, j=0;i<node_num;i++){
        if(count[i]) {
            odd_node_list.push_back(i);
            num_odd_node++;
        }
    }
    edge_num_of_perfect_graph = num_odd_node*(num_odd_node-1)/2;

    cout << "writing odd degree node graph" << endl;

    // WRITE MIDDLE FILE - odd degree node(node_1 node_2 weight)
    char* middle_odd_file_name = strdup(MIDDLE_ODD_FILE_NAME);
    const char* middle_odd_file = ("./" + (string)middle_odd_file_name).c_str();
    ofstream out_file(middle_odd_file);

    out_file << num_odd_node <<" "<< edge_num_of_perfect_graph<<endl;
    for(int i=0;i<odd_node_list.size();i++){
        for(int j=0;j<odd_node_list.size();j++){
            if(i<j){
                out_file << i <<" "<<j<<" "<<full_weight_matrix[odd_node_list[i]][odd_node_list[j]]<<endl;
            }
        }
    }
    out_file.close();

    cout << "selecting odd node is done, execute blossom v" << endl;

    // EXECUTE BLOSSOM V
    char* absol_current_dir = realpath(".", NULL);
    const char* comm_blossom = ((string)absol_current_dir + "/blossom5 -w "
                               + (string)absol_current_dir + "/" + output_blossom_file_name + " -e "
                               + (string)absol_current_dir + "/" + (string)middle_odd_file_name).c_str();
    free(absol_current_dir);
    system(comm_blossom);

    cout << "blossom v is done, map the fake node num to real node num.." << endl;


    // CHECK OUTPUT DIR
    char* output_dir = strdup(OUTPUT_DIR);
    const char* comm = ("mkdir " + (string)output_dir).c_str();
    system(comm);

    // READ BLOSSOM V OUTPUT FILE

    const char* output_blossom_file = ("./" + output_blossom_file_name).c_str();
    ifstream in_file_blossom(output_blossom_file);

    if(!in_file_blossom.is_open()){
        cout<<"blossom output file open error" << endl;
        return 1;
    }

    ofstream out_file_blossom(OUTPUT_FILE);
    int num_blossom_edge;
    in_file_blossom >> num_odd_node >> num_blossom_edge;
    out_file_blossom << num_odd_node << " " << num_blossom_edge << endl;
    while(in_file_blossom >> temp1 >> temp2){
        out_file_blossom << odd_node_list[temp1] << " " << odd_node_list[temp2] << " "
                         << full_weight_matrix[odd_node_list[temp1]][odd_node_list[temp2]] << endl;
    }
    in_file_blossom.close();
    out_file_blossom.close();
    // END OF READING BLOSSOM V OUTPUT FILE

    cout << "remove middle files" << endl;

    // REMOVE ODD NODE GRAPH FILE AND BLOSSOM V OUTPUT FILE
    const char* comm_remove = ("rm " + output_blossom_file_name + " " + MIDDLE_ODD_FILE_NAME ).c_str();
    system(comm_remove);

    cout << "done" << endl;

    return 0;
}