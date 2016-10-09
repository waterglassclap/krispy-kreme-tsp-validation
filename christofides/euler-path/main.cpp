#include "Graph.h"

int main() {

    const char* INPUT_FILE_MST = "../../datas/tsp-mst/a280.txt";
    const char* INPUT_FILE_PERFECT = "../../datas/tsp-mst-odd/odd-degree-full.txt";
    const char* OUTPUT_FILE = "../../datas/euler/euler-path.txt";

    ifstream in_file_mst(INPUT_FILE_MST); //open input file
    if(!in_file_mst.is_open()){
        cout<<"tsp-mst file open error" << endl;
        return 1;
    }
    ifstream in_file_perfect(INPUT_FILE_PERFECT); //open input file
    if(!in_file_perfect.is_open()){
        cout<<"tsp-perfect file open error" << endl;
        return 1;
    }
    ofstream out_file(OUTPUT_FILE);

    int num_v, num_e;
    int mst_v, mst_e, perfect_v, perfect_e;

    in_file_mst >> mst_v >> mst_e;
    in_file_mst.ignore(1000, '\n');

    in_file_perfect >> perfect_v >> perfect_e;
    in_file_perfect.ignore(1000, '\n');

    if(mst_v != perfect_v) {
        cout<<"mst와 perfect matching의 노드 수가 다름" << endl;
        return 1;
    }

    num_v = mst_v;
    num_e = mst_e + perfect_e;

    out_file << num_v << ' ' << num_e << '\n';

    Graph new_graph(num_v);

    for (int i = 0; i < mst_e; i++) {
        int s, t, w;
        in_file_mst >> s >> t >> w;
        in_file_mst.ignore(1000, '\n');
        new_graph.addEdge(s, t);
    }

    for (int i = 0; i < perfect_e; i++) {
        int s, t, w;
        in_file_perfect >> s >> t >> w;
        in_file_perfect.ignore(1000, '\n');
        new_graph.addEdge(s, t);
    }

    new_graph.printEulerTour(out_file);

    in_file_mst.close();
    in_file_perfect.close();
    out_file.close(); //close file

    return 0;
}