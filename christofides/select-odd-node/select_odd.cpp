#include <iostream>
#include <fstream>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[]){

    const int START = 3, END = 5;
    const char* INPUT_FILE_MST = "../tsp-data-handling/datas/tsp-mst/a280.txt";
    const char* INPUT_FILE_FULL = "../tsp-data-handling/datas/tsp-full/a280.txt";
    const char* OUTPUT_FILE = "./odd-degree-full.txt";

    // OPEN MST FILE
    int node_num, edge_num, temp1, temp2, temp3;
    ifstream in_file_mst(INPUT_FILE_MST);
    if(!in_file_mst.is_open()){
        cout<<"tsp-mst file open error" << endl;
        return 1;
    }

    cout << "tsp-mst file is opened, start reading" << endl;

    // READ MST FILE
    in_file_mst >> node_num >> edge_num;
    bool *count = new bool[node_num];
    fill(count, count+node_num, false);
    count[START] = count[END] = true;
    float *distance = new float[node_num*(node_num-1)/2];

    while(in_file_mst >> temp1 >> temp2 >> temp3){
        count[temp1] = !count[temp1];
        count[temp2] = !count[temp2];
        if(temp1<temp2){
            distance[temp1]=temp3;
        }else{

        }
    }
    in_file_mst.close();
    // END OF READING MST FILE


    // OPEN FULL FILE
    ifstream in_file_full(INPUT_FILE_FULL);
    if(!in_file_full.is_open()){
        cout<<"tsp-full file open error" << endl;
        return 1;
    }

    cout << "tsp-full file is opened, start reading" << endl;

    // READ FULL FILE
    int full_edge_num;
    in_file_full >> temp1 >> full_edge_num;
    int** full_weight_matrix = new int*[node_num];
    for(int i=0;i<node_num;i++){
        full_weight_matrix[i] = new int[node_num];
    }

    for(int i=0;i<full_edge_num;i++) {
        in_file_full >> temp1 >> temp2 >> temp3;
        full_weight_matrix[temp1][temp2] = temp3;
    }
    in_file_full.close();

    // END OF READING FULL FILE


    // WRITE FILE
    int edge_num_of_perfect_graph = 0;
    for(int i=0;i<node_num;i++){
        if(count[i])
            edge_num_of_perfect_graph++;
    }
    edge_num_of_perfect_graph = edge_num_of_perfect_graph*(edge_num_of_perfect_graph-1)/2;


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

    return 0;
}