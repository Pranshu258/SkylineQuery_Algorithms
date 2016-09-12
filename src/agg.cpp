// Implementation of Aggregation like Algorithm for Skyline Queries
// Author: Pranshu Gupta

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <cmath>
#include <list>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Some functions and global data
vector<int> dims;
int N, D, curr_dim;

struct point {
	int* attributes;
	double timestamp;
	int index;
};

void stupid_print (list<point> data) {
	for (list<point>::iterator p = data.begin(); p != data.end(); p++) {	  
		cout << (*p).index << " ";
	}
	cout << endl;
	return;
}

// implement the custom sort function here
bool compare_points(const point &p1, const point &p2) {
	return p1.attributes[curr_dim] < p2.attributes[curr_dim];
}

// Here goes the main program
int main(int argc, char *argv[]) {

	// Input file: data.txt
	ifstream infile("../data/data.txt");
	int index, win_size;
	infile >> N >> D;

	ifstream infile1("../data/query.txt");
	string line1, line2;

	// Read the Query Dimensions from the query.txt file
	getline(infile1, line1);
	stringstream ss1(line1);

	for(int i = 0; ss1 >> i; ) {
		dims.push_back(i);
	}
	// Read the Window size from the query.txt file
	getline(infile1, line2);
	stringstream ss2(line2);
	ss2 >> win_size;

	// Read data from file and create the lst of data
	list<point> data;
	list<point> original_data;
	bool *skyline = new bool[N];

	for (int i = 0; i < N; ++i) {
		infile >> index;
		if (index == i+1) {
			// Read the dimensions now and put them in the vector
			point *p = new point;
			p->attributes = new int[D];
			for (int j = 0; j < D; j++) {
				int val;
				infile >> val;
				p->attributes[j] = val;
			}
			p->timestamp = N*N;
			p->index = index;
			data.push_back(*p);
			original_data.push_back(*p);
		} else {
			cout << "Invalid Data" << endl;
			exit(0);			
		}
	}

	// All the input data has been read and is there in original_data and the data lists
	// Let's start the aggregation like process
	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	// Get the dimension wise sorted lists
	list<list<point>> cycle_data;
	for (vector<int>::iterator k = dims.begin(); k != dims.end(); ++k) {
		curr_dim = *k - 1;
		data.sort(&compare_points);
		cycle_data.push_back(data);
		data = original_data;
	}

	list<point> bnl_data;
	point finisher;
	int * seen = new int[N];
	// Cycle through the cycle_data list
	bool none_fully_seen = true;
	while (none_fully_seen) {
		for (list<list<point>>::iterator dim_list = cycle_data.begin(); dim_list != cycle_data.end(); ++dim_list) {
			// Get the first element and increment its seen count
			point p = (*dim_list).front();
			(*dim_list).pop_front();
			int ind = p.index;
			seen[ind-1] += 1;
			// If seeing for the first time, insert the point in the bnl_data
			if (seen[ind-1] == 1) {
				bnl_data.push_back(p);
			} else if (seen[ind-1] == dims.size()) {
				// We have fully seen a point, points that have still not been inserted in bnl_data are irrelevalent
				finisher = p;
				none_fully_seen = false;
				break;
			}
		}
	}


	list<point> additional;
	// Find other points that are equal to the points in bnl_data to take care of non-dvc cases
	for (list<point>::iterator q = original_data.begin(); q != original_data.end(); q++) {
		if (finisher.index != q->index) {
			int equal = 0;
			for (vector<int>::iterator k = dims.begin(); k != dims.end(); ++k) {
				if (finisher.attributes[*k - 1] == q->attributes[*k - 1]) {
					equal += 1;
				}
			}
			if (equal == dims.size()) {
				additional.push_back(*q);
			}
		}
	}
	for (list<point>::iterator p = additional.begin(); p != additional.end(); p++) {
		bnl_data.push_back(*p);
	}

	// Now we can apply BNL on bnl_data list
	// BLOCK NESTED LOOP ALGORITHM FOR SKYLINES
	//////////////////////////////////////////////////////////////////////////////
	// NOW WE CAN START FINDING THE SKYLINES
	int comparisons = 0;;
	list<point> skyline_window;
	while (!bnl_data.empty()) {
		list<point> temp_data;

		//cout << "Data: " << endl;
		//stupid_print(data);

		for (list<point>::iterator p = bnl_data.begin(); p != bnl_data.end(); p++) {

			bool not_skyline = false;
			list<point>::iterator swp = skyline_window.begin();
			while (swp != skyline_window.end()) {
				int equalorworse = 0, worse = 0;
				int equalorbetter = 0, better = 0;
				for (vector<int>::iterator k = dims.begin(); k != dims.end(); ++k) {
					if ((*p).attributes[*k - 1] > (*swp).attributes[*k - 1]) {
						worse += 1;
					} else if ((*p).attributes[*k - 1] == (*swp).attributes[*k - 1]) {
						equalorworse += 1;
						equalorbetter += 1;
					} else if ((*p).attributes[*k - 1] < (*swp).attributes[*k - 1]) {
						better += 1;
					}
				}
				if (equalorworse + worse == dims.size() && worse > 0) {
					not_skyline = true;
				}
				if (equalorbetter + better == dims.size() && better > 0) {
					// remove swp from the window
					//cout << "Removed " << (*swp).index << " from Skyline window, dominated by: " << (*p).index << endl;
					swp = skyline_window.erase(swp);
					comparisons += 1;
					continue;
				}
				comparisons += 1; 
				swp++;
			}   
			// if not_skyline is still false, try to insert this point in the skyline window
			if (!not_skyline) {
				if (skyline_window.size() < win_size) {
					// get the timestamp and push in the skyline data
					(*p).timestamp = comparisons;
					skyline_window.push_back(*p);
					//cout << "Inserted " << (*p).index << " in Skyline window. Time: " << (*p).timestamp << endl;
				}
				else {
					// get the timestamp and put in the temp data
					(*p).timestamp = comparisons;
					temp_data.push_back(*p);
					//cout << "Inserted " << (*p).index << " in temp_data. Time: " << (*p).timestamp << endl;
				}
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////

		// mark the skyline points
		list<point>::iterator swp = skyline_window.begin();
		while (swp != skyline_window.end()) {
			//cout << "Size of Skyline Window: " << skyline_window.size() << endl;
			if (skyline_window.size() > 0) {
				if ((*swp).timestamp < (temp_data.front()).timestamp || temp_data.size() == 0) {
					//cout << "Removing " << swp->index << " from window as valid skyline" << endl;
					if (swp->index <= N) {
						skyline[(swp->index)-1] = true;
						swp = skyline_window.erase(swp);
						continue;
					} else {
						cout << "Something went wrong" << endl;
					}
				} else {
					break;
				}
			} else {
				break;
			}
			swp++;
		}		

		//cout << "-------------------------------------------" << endl;

		bnl_data = temp_data;
		temp_data.clear();
	}


	/////////////////////////////////////////////////////////////////////////////

	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

	cout << "Skyline Points: " << endl;
	int printed = 0;
	for (int i = 0; i < N; i++) {
		if (skyline[i]) {
			cout << i+1 << "\t";
			printed += 1;
			if (printed % 10 == 0)
				cout << endl;
		}
	}
	cout << endl;
	cout << "Number of skyline points: " << printed << endl;
	cout << "Number of comparisons: " << comparisons << endl;
	cout << "Time taken: " << time_span.count() << " seconds" << endl;

	return 0;
}