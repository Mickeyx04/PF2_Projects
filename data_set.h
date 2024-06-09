#ifndef data_set
#define data_set

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

#include "instance_counter.h"

using std::string;
using std::vector;

class DataSet : public InstanceCounter
{
    protected:
    //   col     name
    vector< string > col_names;
    //  matrix   row     cell
    vector< vector< string > > data;
    //  indxes  col-idx  pointer-to-row
    vector< vector< vector<string>* > > indices;

    public: 

    DataSet();
    virtual ~DataSet();

    void load_csv(string& filename);
    void index_data();
    int col_count();
    int row_count();
    int binary_search(string target, int col_num);
    void query(string start, string end, int column);
    void print_row(vector <string>* row);
    void print_index(int col_num);
    
};

#endif