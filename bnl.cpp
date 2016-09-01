// Implements the Block Nested Loop Algorithm for Skyline Queries
// Author: Pranshu Gupta

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
    // Input file: data.txt
    ifstream infile("data.txt");
    int N, D, index, win_size;
    infile >> N >> D;

    ifstream infile1("query.txt");
    string line1, line2;

    // Read the Query Dimensions from the query.txt file
    getline(infile1, line1);
    stringstream ss1(line1);
    vector<int> dims;
    for(int i = 0; ss1 >> i; ) {
        dims.push_back(i);
    }
    // Read the Window size from the query.txt file
    getline(infile1, line2);
    stringstream ss2(line2);
    ss2 >> win_size;

    // Read data from file and put it in an array
    int **data = new int*[N];
    bool *not_skyline = new bool[N];

    for (int i = 0; i < N; i++) {
        infile >> index;
        if (index == i+1) {
            data[i] = new int[D];
            for (int j = 0; j < D; j++) {
                infile >> data[i][j];
            }
        } else {
            cout << "Invalid Data" << endl;
            exit(0);
        }
    }

    // BLOCK NESTED LOOP ALGORITHM FOR SKYLINES
    //////////////////////////////////////////////////////////////////////////////
    // NOW WE CAN START FINDING THE SKYLINES
    


    /////////////////////////////////////////////////////////////////////////////

    return 0;
}