#include "data_set.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <set>
// including so much bc idk why the sub server is yelling at me


using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

//current collumn
int g_activeColumn = 0;

DataSet::DataSet()
{

}

DataSet::~DataSet()
{

}

int DataSet::col_count() //width
{
    return this->col_names.size();
}

int DataSet::row_count() //height
{
    return this->data.size();
}

void DataSet::load_csv(string& filename)
{
    // Clear any existing data
    this->data.clear();

    // Open the file
    std::ifstream stream;
    stream.open(filename);
    if (!stream.is_open()) {
        throw std::system_error(errno, std::generic_category(), filename);
    }

    // Read the file
    while (!stream.eof())
    {
        // Read a line
        string s;
        if (!std::getline(stream, s))
            break;
        vector<string> empty;
        this->data.push_back(empty);
        vector<string>& current_row = this->data.back();

        // Break up the line into cells
        while (true)
        {
            // Find the next comma
            size_t pos = s.find(",");
            if (pos == std::string::npos)
            {
                // This is the last cell in the row
                current_row.push_back(s);
                break;
            }
            else
            {
                // This is not the last cell in the row
                current_row.push_back(s.substr(0, pos));
                s.erase(0, pos + 1); // erase the cell and the comma
            }
        }

        // Ensure all rows have the same size
        if (current_row.size() < 1)
        {
            // This row is empty, so just drop it
            this->data.pop_back();
        }
        else if (current_row.size() != this->data[0].size())
        {
            // Uh oh, the row is the wrong size!

            // note: had to change this from what the prof had bc it was yelling errors at me about appending 
            // and yada yada
            cout << "Error, row " << this->data.size()<< " has " << current_row.size() 
            << " elements. Expected " << this->data[0].size();
            throw std::runtime_error("ur a buffoon!");
        }
    }

    // Move the first row into this->col_names
    this->col_names = this->data[0];
    this->data.erase(this->data.begin());
}

string strip(const string value)
{
    string s = value;
    while (s.size() > 0 && std::isspace(s[s.size() - 1]))
        s.erase(s.end() - 1);
    while (s.size() > 0 && std::isspace(s[0]))
        s.erase(s.begin());
    return s;
}

//named smartie bc it was yelling at me. not any diff than reg smart compare in LinkedList.cpp
int smartie_compare(std::string& a, std::string& b) 
{

    // Skip the parts that are the same
    unsigned int i = 0;
    while(i < a.size() && i < b.size() && a[i] == b[i]) {
        i++;
    }
    if (i >= a.size()) {
        if (i >= b.size()) {
            return 0;
        } else {
            return -1;
        }
    } else if (i >= b.size()) {
        return 1;
    }

    // Skip zeros
    unsigned int a_start = i;
    unsigned int b_start = i;
    while (a_start < a.size() && a[a_start] == '0')
        a_start++;
    while (b_start < b.size() && b[b_start] == '0')
        b_start++;

    // Count digits
    unsigned int a_digits = a_start;
    while (a_digits < a.size() && a[a_digits] >= '0' && a[a_digits] <= '9')
        a_digits++;
    unsigned int b_digits = b_start;
    while (b_digits < b.size() && b[b_digits] >= '0' && b[b_digits] <= '9')
        b_digits++;
    if (a_digits > 0 && a_digits < b_digits) {
        return -1; // a comes first because its number is shorter
    } else if (b_digits > 0 && b_digits < a_digits) {
        return 1; // b comes first because its number is shorter
    } else {
        // The numbers are the same length, so compare alphabetically
        return strcmp(a.c_str() + a_start, b.c_str() + b_start);
    }
}


bool custom_less(vector<string>* a, vector<string>* b)
{
    string a_val = (*a)[g_activeColumn];
    string b_val = (*b)[g_activeColumn];

    a_val = strip(a_val);
    b_val = strip(b_val);
   
    int compare = smartie_compare(a_val,b_val); 
    if (compare < 0) {
        return true;
    }
    return false;
}


void DataSet::index_data()
{
    // Index the data
    indices.clear();
    indices.resize(this->col_count());
    for (int i = 0; i < this->col_count(); i++)
    {
        g_activeColumn = i;

        // Build an index for column i
        vector< vector<string>* >& index = indices[i];
        for (int j = 0; j < this->row_count(); j++)
        {
            index.push_back(&this->data[j]);
        }
        // Sort the index in column i using smart_compare
        // *** you have to write this part ***
        std::sort(index.begin(), index.end(), custom_less); // end is one past the actual end.
        indices[i] = index;
    }
}

int DataSet::binary_search(string target, int col_num)
{
    // Quickly find the approximate row_num
    int floor = 0;
    int ceil = this->row_count() - 1;
    int row_num = 0;
    while(true) 
    {
        row_num = (floor + ceil) / 2;
        if (row_num == floor)
            break;
        string hello = strip((*indices[col_num][row_num])[col_num]);
        int cmp = smartie_compare(hello, target);
        if (cmp < 0)
            floor = row_num;
        else
            ceil = row_num;
    }
    // fine-tune the row_num

    string hello = strip((*indices[col_num][row_num])[col_num]);
    int cmp = smartie_compare(hello, target);
    while ((row_num > 0) && (cmp > 0))
    {
        row_num--;
        hello = (*indices[col_num][row_num])[col_num];
        hello = strip(hello);
        cmp = smartie_compare(hello, target);
    }
    while ((row_num < this->row_count()) && (cmp < 0))
    {
        row_num++;
        hello = (*indices[col_num][row_num])[col_num];
        hello = strip(hello);
        cmp = smartie_compare(hello, target);
    }
    return row_num;
}

void DataSet::print_row(vector <string>* row) {
    
    for (int i = 0; i < this->col_count(); i++)
    {
        cout << (*row)[i];
        if (i < this->col_count() - 1)
        {
            cout << ", ";
        }
    }
    cout << endl;
}

void DataSet::query(string start, string end, int column)
{
    int row = this->binary_search(start, column);
    this->print_row(indices[column][row]);
    while (true)
    {
        row++; 
        string hello = (*indices[column][row])[column];
        hello = strip(hello);

        int wompwomp = smartie_compare(hello, end);

        if (wompwomp < 0)
        {
            this->print_row(indices[column][row]);
        }
        else
        {
            break;
        }
    }
}

void DataSet::print_index(int col_num)
{
    cout << "Sorted by column " << col_num << ":" << endl;
    cout << "-------------------" << endl;
    vector< vector<string>*> index = this->indices[col_num];
    for (size_t i = 0; i < index.size(); i++)
    {
        vector<string>& row = *index[i];
        for (size_t j = 0; j < row.size(); j++)
        {
            if (j > 0)
                cout << ", ";
            cout << row[j];
        }
        cout << endl;
    }
}


