// Implements the Brute Force algorithm for Skyline Queries
// Author: Pranshu Gupta

// Will be used to benchmark the correctness of other implementations

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
	// Input file: data.txt
	ifstream infile("../data/data.txt");
	int N, D, index;
	infile >> N >> D;

	// Read the Query from the query.txt file
	string line1;
	ifstream infile1("../data/query.txt");
	getline(infile1, line1);
	stringstream ss1(line1);
	vector<int> dims;
	for(int i = 0; ss1 >> i; ) {
		dims.push_back(i);
	}

	// Create a Matrix for the data
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

	// NAIVE ALGORITHM FOR SKYLINES
	////////////////////////////////////////////////////////////////////////////////////////
	// Now let's find out the skylines in the dataset
	int number_skylines = 0;
	for (int i = 0; i < N; i++) {
		// Compare data[i] with all the other datapoints
		not_skyline[i] = false;
		for (int j = 0; j < N; j++) {
			int worse = 0, equalorworse = 0;
			for (vector<int>::iterator k = dims.begin(); k != dims.end(); ++k) {
				if (data[i][*k - 1] > data[j][*k - 1]) {
					worse += 1;
				}
				if (data[i][*k - 1] >= data[j][*k - 1]) {
					equalorworse += 1;
				}
			}
			if (equalorworse == dims.size() && worse > 0) {
				not_skyline[i] = true;
				// cout << "[" << i+1 << "] beaten by [" << j+1 << "]" << endl;
				break;
			}
		}
		if (!not_skyline[i]) {
			// This point was not dominated by any other point, so it is a skyline
			// cout << "[" << i+1 << "] is a Skyline" << endl; 
			number_skylines += 1;  
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////

	cout << "Skyline Points: " << endl;
	int printed = 0;
	for (int i = 0; i < N; i++) {
		if (!not_skyline[i]) {
			cout << i+1 << "\t";
			printed += 1;
			if (printed % 10 == 0)
				cout << endl;
		}
	}
	cout << endl;
	cout << "Number of skyline points: " << printed << endl;
	cout << "Number of comparisons: " << N*N << endl;
	return 0;
}