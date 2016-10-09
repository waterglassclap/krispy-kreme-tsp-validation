#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[]){

    const int START = 3, END = 5;
    const char* INPUT_FILE_MST = "../../datas/tsp-mst/a280.txt";
    const char* INPUT_FILE_FULL = "../../datas/tsp-full/a280.txt";
    const char* OUTPUT_DIR = "../../datas/tsp-mst-odd/";
    const char* OUTPUT_FILE = "../../datas/tsp-mst-odd/odd-degree-full.txt";

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

    // calculate the num of edges in odd degree graph
    int edge_num_of_perfect_graph = 0;
    for(int i=0;i<node_num;i++){
        if(count[i])
            edge_num_of_perfect_graph++;
    }
    edge_num_of_perfect_graph = edge_num_of_perfect_graph*(edge_num_of_perfect_graph-1)/2;

    // CHECK OUTPUT DIR
    char* output_dir = strdup(OUTPUT_DIR);
    const char* comm = ("mkdir " + (string)output_dir).c_str();
    system(comm);

    cout << "writing..." << endl;

    // WRITE TO FILE - odd degree node(node_1 node_2 weight)
    ofstream out_file(OUTPUT_FILE);

    out_file << node_num <<" "<< edge_num_of_perfect_graph<<endl;
    for(int i=0;i<node_num;i++){
        for(int j=0;j<node_num;j++){
            if(i<j && count[i] && count[j]){
                out_file << i <<" "<<j <<" "<<full_weight_matrix[i][j]<<endl;
            }
        }
    }
    out_file.close();

    cout << "done" << endl;

    return 0;
}