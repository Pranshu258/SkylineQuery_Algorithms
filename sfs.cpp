// Implements the Block Nested Loop Algorithm for Skyline Queries
// Author: Pranshu Gupta

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <cmath>
#include <list>

using namespace std;

vector<int> dims;
int N, D;

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
    // find the entropy of both the functions
    double e1 = 0, e2 = 0;
    for (int k = 0; k < D; k++) {
        e1 += log(p1.attributes[k] + 1);
        e2 += log(p2.attributes[k] + 1);
    }
    //cout << e1 << " " << e2 << endl;
    return e1 < e2;
}

int main(int argc, char *argv[]) {
    
    clock_t begin = clock();

    // Input file: data.txt
    ifstream infile("data.txt");
    int index, win_size;
    infile >> N >> D;

    ifstream infile1("query.txt");
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

    //stupid_print(data);
    data.sort(&compare_points);
    //stupid_print(data);

    // BLOCK NESTED LOOP ALGORITHM FOR SKYLINES
    //////////////////////////////////////////////////////////////////////////////
    // NOW WE CAN START FINDING THE SKYLINES
    int comparisons = 0;;
    list<point> skyline_window;
    while (!data.empty()) {
        list<point> temp_data;

        //cout << "Data: " << endl;
        //stupid_print(data);

        for (list<point>::iterator p = data.begin(); p != data.end(); p++) {

            //cout << (*p).index << ". Skyline Window: " << endl;
            //stupid_print(skyline_window);

            bool not_skyline = false;
            list<point>::iterator swp = skyline_window.begin();
            while (swp != skyline_window.end()) {
                int worse = 0;
                int better = 0;
                for (vector<int>::iterator k = dims.begin(); k != dims.end(); ++k) {
                    if ((*p).attributes[*k - 1] > (*swp).attributes[*k - 1]) {
                        worse += 1;
                    } else if ((*p).attributes[*k - 1] < (*swp).attributes[*k - 1]){
                        better += 1;
                    }
                }
                if (worse == dims.size()) {
                    not_skyline = true;
                }
                if (better == dims.size()) {
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

        data = temp_data;
        temp_data.clear();
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
    cout << "Number of skyline points: " << printed << endl;
    cout << "Number of comparisons: " << comparisons << endl;

    return 0;
}