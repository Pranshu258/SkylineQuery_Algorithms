// Code to generate the input data file for the Skyline Query
// Author: Pranshu Gupta

#include <iostream>
#include <random>
#include <fstream>
#include <sstream>

using namespace std;

int main (int argc, char *argv[]) {
    int N, D;
    // Construct the Random Number Generator
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> uni(0,9);
    if (argc != 3) {
        cout << "Usage: ./generate n_samples n_dims" << endl;
        exit(0);
    }

    // Read the number of Samples to be generated
    istringstream ss1(argv[1]);
    if (!(ss1 >> N)) {
        cerr << "Invalid n_samples: " << argv[1] << '\n';
        exit(0);
    }
    // Read the number of dimensions for the smples to be generated
    istringstream ss2(argv[2]);
    if (!(ss2 >> D)) {
        cerr << "Invalid n_dims: " << argv[2] << '\n';
        exit(0);
    }

    // Create the Data
    ofstream inputFile;
    inputFile.open ("data.txt");
    inputFile << N << "\t" << D << endl;
    for (int i = 0; i < N; i++) {
        inputFile << i+1 << "\t" ;
        for (int i = 0; i < D; i++) {
            inputFile << uni(rng) << "\t";
        }
        inputFile << endl;
    }
    inputFile.close();

    return 0;
}