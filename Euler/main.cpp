#include "Graph.h"

int main() {
    ifstream instream("/Users/ttopre/Desktop/input.txt"); //open input file
    if (instream.fail()) { //checking file open success
        cout << "File instream is failed" << endl;
        exit(1);
    }
    ofstream outstream("/Users/ttopre/Desktop/output.txt"); //open output file

    int num_v, num_e;

    instream >> num_v >> num_e;
    instream.ignore(1000, '\n');

    Graph g1(num_v);

    for (int i = 0; i < num_e; i++) {
        int s, t;
        instream >> s >> t;
        instream.ignore(1000, '\n');
        g1.addEdge(s, t);
    }

    g1.printEulerTour(outstream);

    instream.close();
    outstream.close(); //close file

    return 0;
}