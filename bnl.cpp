// Implements the Block Nested Loop Algorithm for Skyline Queries
// Author: Pranshu Gupta

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <list>

using namespace std;

struct point {
	int* attributes;
	double timestamp;
    int index;
};

int main(int argc, char *argv[]) {
    
    clock_t begin = clock();

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

    // Read data from file and create the lst of data
    list<point> data;
    list<point> original_data;
    bool *skyline = new bool[N];

    for (int i = 0; i < N; ++i) {
        infile >> index;
        if (index == i+1) {
            // Read the dimensions now and put them in the vector
            point *p = new point;
            for (int j = 0; j < D; j++) {
                infile >> p->attributes[j];
            }
            p->timestamp = 0;
            p->index = index;
            data.push_back(*p);
            original_data.push_back(*p);
        } else {
            cout << "Invalid Data" << endl;
            exit(0);    		
        }
    }

    // Print the Data
    // int c = 1;
    // for (list<point>::iterator p = data.begin(); p != data.end(); p++) {
    //     // *p contains the current point
    //     cout << "[" << c << "]: "; 
    //     for (vector<int>::iterator v = (*p).attributes.begin(); v != (*p).attributes.end(); v++) {
    //         cout << *v << " ";
    //     }
    //     cout << endl;
    //     c++;
    // }


    // BLOCK NESTED LOOP ALGORITHM FOR SKYLINES
    //////////////////////////////////////////////////////////////////////////////
    // NOW WE CAN START FINDING THE SKYLINES
    int comparisons = 0;;
    list<point> skyline_window;
    while (!data.empty()) {
        list<point> temp_data;
        for (list<point>::iterator p = data.begin(); p != data.end(); p++) {
            bool not_skyline = false;
            for (list<point>::iterator swp = skyline_window.begin(); swp != skyline_window.end(); swp++) {
                int worse = 0;
                int better_or_equal = 0;
                for (vector<int>::iterator k = dims.begin(); k != dims.end(); ++k) {
                    if ((*p).attributes[*k - 1] > (*swp).attributes[*k - 1]) {
                        worse += 1;
                    } else {
                        better_or_equal += 1;
                    }
                }
                if (worse == dims.size()) {
                    not_skyline = true;
                }
                if (better_or_equal == dims.size()) {
                    // remove swp from the window
                    skyline_window.erase(swp);
                }
                comparisons += 1;    
            }
            // if not_skyline is still false, try to insert this point in the skyline window
            if (!not_skyline) {
                if (skyline_window.size() < win_size) {
                    // get the timestamp and push in the skyline data
                    clock_t now = clock();
                    (*p).timestamp = double(now - begin) / CLOCKS_PER_SEC;
                    skyline_window.push_back(*p);
                }
                else {
                    // get the timestamp and put in the temp data
                    clock_t now = clock();
                    (*p).timestamp = double(now - begin) / CLOCKS_PER_SEC;
                    temp_data.push_back(*p);
                }
            }
        }

        // mark the skyline points
        list<point>::iterator swp = skyline_window.begin();
        while (swp != skyline_window.end() && swp->timestamp < (temp_data.front()).timestamp) {
            skyline[(swp->index)-1] = true;
            skyline_window.erase(swp);
        }
        data = temp_data;
    }


    /////////////////////////////////////////////////////////////////////////////

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

    return 0;
}