// Implements the Brute Force algorithm for Skyline Queries
// Author: Pranshu Gupta

// Will be used to benchmark the correctness of other implementations

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]) {
    // Input file: data.txt
    ifstream infile("data.txt");
    int N, D, index;
    infile >> N >> D;

    // Create a Matrix for the data
    int **data = new int*[N];

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

    // Print the entire dataset on the console
    // for (int i = 0; i < N; i++) {
    //     for (int j = 0; j < D; j++) {
    //         cout << data[i][j] << " ";
    //     }
    //     cout << endl;
    // } 

    // Now let's find out the skylines in the dataset
    for (int i = 0; i < N; i++) {
        // Compare data[i] with all the other datapoints
        bool not_skyline = false;
        for (int j = 0; j < N; j++) {
            int worse = 0;
            for (int k = 0; k < D; k++) {
                if (data[i][k] > data[j][k]) {
                    worse += 1;
                }
            }
            if (worse == D) {
                not_skyline = true;
                cout << "[" << i+1 << "] beaten by [" << j+1 << "]" << endl;
                break;
            }
        }
        if (!not_skyline) {
            // This point was not dominated by any other point, so it is a skyline
            cout << "[" << i+1 << "] is a Skyline" << endl;   
        }
    }

    


    return 0;
}