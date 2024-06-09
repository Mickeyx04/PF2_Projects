#include <iostream>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <set>
#include <fstream>

#include "charmatrix.h"
#include "linked_list.h"
#include "config.h"
#include "data_set.h"
#include "instance_counter.h"
#include "priority_queue.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

vector<string> g_lex;
CharMatrix g_grid;
DataSet g_dataset;
int g_instantiations = 0;
int g_deletions = 0;
Heap g_priority_queue;


string word;

//vector<string> fuvk;
CharMatrix jjj;
DataSet g_dat;

//lexicon

void fill_lexicon()
{

while (true)
{
    if (!config.quiet)
    {
    for (int i = 0; i < 5; i++)
    {
        cout << endl;
    }
    cout << "Please enter another word" << endl;
    cout << "> ";
    }

    getline(cin, word);
    if (word.size() == 0)
    {
        break;
    }

    if (word == "-")
    {
        g_lex.pop_back();
    }
    else 
    {
    g_lex.push_back(word);
    }

    if (!config.quiet)
    {
    cout << "So far, the words you have entered are:" << endl;
    for (unsigned int i = 0; i < g_lex.size(); i++)
    {
        cout << i << ". " << g_lex[i] << endl;
    }
    }
}
}

void tear_down()
{
    unsigned int bookie = 0;
    while (bookie < g_lex.size())
    {
        cout << g_lex.back() << endl;
        g_lex.pop_back();
    }
}

// char matrix

void load_char_matrix()
{
    if (!config.quiet)
    {
        cout << "Please enter a grid of characters." << endl;
        cout << "All rows should have the same length." << endl;
        cout << "When you are done, just press Enter." << endl;
    }
    vector<string> grid;
    while(true)
    {
        string row;
        getline(cin, row);
        if (row.compare("") == 0)
            break;
        if (grid.size() > 0 && row.size() != grid[0].size())
            throw std::runtime_error("Rows in a CharMatrix must all have the same size!");
        grid.push_back(row);
    }
    int height = grid.size();
    int width = 0;
    if (height > 0)
        width = grid[0].size();
    g_grid.resize(width, height);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            g_grid.put(x, y, grid[y][x]);
        }
    }
}

void print_char_matrix()
{
    for (int y = 0; y < g_grid.height(); y++)
    {
        for (int x = 0; x < g_grid.width(); x++)
        {
            cout << g_grid.get(x, y);
        }
        cout << endl;
    }
}

void fill(int x, int y, char c, int max_depth)
{
    if (config.debug)
    {
    usleep(100000);
    for (int i = 0; i < 20; i++)
        cout << endl;
    print_char_matrix();
    cout.flush();
    }

    char bef = g_grid.get(x, y);
    if (bef == c)
        return;
    if (max_depth == 0)
        return;
    g_grid.put(x, y, c);
    if (x > 0 && g_grid.get(x - 1, y) == bef)
        fill (x - 1, y, c, max_depth - 1);
    if (y > 0 && g_grid.get(x, y - 1) == bef)
        fill (x, y - 1, c, max_depth - 1);
    if (x + 1 < g_grid.width() && g_grid.get(x + 1, y) == bef)
        fill (x + 1, y, c, max_depth - 1);
    if (y + 1 < g_grid.height() && g_grid.get(x, y + 1) == bef)
        fill (x, y + 1, c, max_depth - 1);
}

void flood_fill()
{
    string word;
    cout << "Please enter a starting column:" << endl;
    cout << "> ";
    getline(cin, word);
    int x = stoi(word);
    cout << "Please enter a starting row:" << endl;
    cout << "> ";
    getline(cin, word);
    int y = stoi(word);
    cout << "Please enter a fill character:" << endl;
    cout << "> ";
    getline(cin, word);
    int c = word[0];
    cout << "Please enter the max fill depth:" << endl;
    cout << "> ";
    getline(cin, word);
    int max_depth = stoi(word);
    fill(x, y, c, max_depth);
}

// boggle

string getString(char x) 
{
    string s(1, x);

    return s;
}

void checkBog (int x, int y, string&current_word, string& letter_matrix, string& letter_lex, CharMatrix& here)
{   
    char matrix = g_grid.get(x,y);
    letter_matrix = getString(matrix);
    here.put(x,y,'1');
    current_word.erase(0,1);
    letter_lex = current_word.substr(0,1);

    if (config.debug) {
        cout << "after checkBog:" << endl;
        cout << "x is " << x << endl;
        cout << "y is " << y << endl;
        cout << "letter_matrix is " << letter_matrix << endl;
        cout << "letter_lex is " << letter_lex << endl;
    }

}

bool recursive_boggle(int x, int y, string& current_word, string& letter_lex, string& letter_matrix, CharMatrix& here) 
{
    char matrix = g_grid.get(x,y);
    letter_matrix = getString(matrix);

    if (config.debug) {
        cout << "in recursive_boggle:" << endl;
        cout << "x is " << x << endl;
        cout << "y is " << y << endl;
        cout << "letter_matrix is " << letter_matrix << endl;
        cout << "letter_lex is " << letter_lex << endl;
        cout << here.get(x,y) << endl;
    }

    if (x >= 0 || y >= 0) {
        if (y > 0) 
        {
            y = y - 1; 
            if (here.get(x,y) != '1')
            {
                matrix = g_grid.get(x, y);
                letter_matrix = getString(matrix); 
        
                if (config.debug) {
                    cout << "1:" << endl;
                    cout << "letter_matrix is " << letter_matrix << endl;
                    cout << "letter_lex is " << letter_lex << endl; }
                
                if (letter_matrix == letter_lex)
                {
                    checkBog(x,y,current_word,letter_matrix,letter_lex,here);
                    recursive_boggle(x,y,current_word,letter_lex,letter_matrix,here);
                }
            }
            y = y + 1;
        }
        if (y + 1 < here.height())
        {
            y++;
            if (here.get(x,y) != '1')
            {
                matrix = g_grid.get(x,y);
                letter_matrix = getString(matrix); 
            
                if (config.debug) {
                    cout << "2:" << endl;
                    cout << "letter_matrix is " << letter_matrix << endl;
                    cout << "letter_lex is " << letter_lex << endl; }
        

                if (letter_matrix == letter_lex)
                {
                    checkBog(x,y,current_word,letter_matrix,letter_lex,here);
                    recursive_boggle(x,y,current_word,letter_lex,letter_matrix,here);
                }
            }
            y--;
        }
        if (y > 0 && x > 0) 
        {
            x--;
            y--;
            if (here.get(x,y) != '1') 
            {
                matrix = g_grid.get(x,y);
                letter_matrix = getString(matrix); 

                if (config.debug) {
                    cout << "3:" << endl;
                    cout << "letter_matrix is " << letter_matrix << endl;
                    cout << "letter_lex is " << letter_lex << endl;}

                if (letter_matrix == letter_lex)
                {
                    checkBog(x,y,current_word,letter_matrix,letter_lex,here);
                    recursive_boggle(x,y,current_word,letter_lex,letter_matrix,here);
                } 
            }
            x++;
            y++;
        }
        if (x > 0 && y + 1 < here.height())
        {
            x--;
            y++;
            if (here.get(x,y) != '1') 
            {
                matrix = g_grid.get(x,y);
                letter_matrix = getString(matrix);

                if (config.debug) {
                    cout << "4:" << endl;
                    cout << "letter_matrix is " << letter_matrix << endl;
                    cout << "letter_lex is " << letter_lex << endl; }
        
                if (letter_matrix == letter_lex)
                {
                    checkBog(x,y,current_word,letter_matrix,letter_lex,here);
                    recursive_boggle(x,y,current_word,letter_lex,letter_matrix,here);
                } 
            }
            x++;
            y--;
        }
        if (x + 1 < here.width() && y > 0)
        {
            x++;
            y--;
            if (here.get(x,y) != '1') {
                matrix = g_grid.get(x,y);
                letter_matrix = getString(matrix);

                if (config.debug) {
                    cout << "5:" << endl;
                    cout << "letter_matrix is " << letter_matrix << endl;
                    cout << "letter_lex is " << letter_lex << endl; }
        
                if (letter_matrix == letter_lex)
                {
                    checkBog(x,y,current_word,letter_matrix,letter_lex,here);
                    recursive_boggle(x,y,current_word,letter_lex,letter_matrix,here);
                } 
            }
            x--;
            y++;
        }
        if (x > 0)
        {
            x--;
            if (here.get(x,y) != '1')
            {
                matrix = g_grid.get(x,y);
                letter_matrix = getString(matrix);

                if (config.debug) {
                    cout << "6:" << endl;
                    cout << "letter_matrix is " << letter_matrix << endl;
                    cout << "letter_lex is " << letter_lex << endl; }
        
                if (letter_matrix == letter_lex)
                {
                    checkBog(x,y,current_word,letter_matrix,letter_lex,here);
                    recursive_boggle(x,y,current_word,letter_lex,letter_matrix,here);
                } 
            }
            x++;
        }
        if (x + 1 < here.width())
        {
            x++;
            if (here.get(x,y) != '1')
            {
                matrix = g_grid.get(x,y);
                letter_matrix = getString(matrix);

                if (config.debug) {
                cout << "7:" << endl;
                cout << "letter_matrix is " << letter_matrix << endl;
                cout << "letter_lex is " << letter_lex << endl; }
        
                if (letter_matrix == letter_lex)
                {
                    checkBog(x,y,current_word,letter_matrix,letter_lex,here);
                    recursive_boggle(x,y,current_word,letter_lex,letter_matrix,here);
                } 
            }
            x--;
        }
        if (x + 1 < here.width() && y + 1 < here.height())
        {
            x++;
            y++;
            if (here.get(x,y) != '1') 
            {
                matrix = g_grid.get(x,y);
                letter_matrix = getString(matrix);

                if (config.debug) {
                    cout << "8:" << endl;
                    cout << "letter_matrix is " << letter_matrix << endl;
                    cout << "letter_lex is " << letter_lex << endl;
                }
        
                if (letter_matrix == letter_lex)
                {
                    checkBog(x,y,current_word,letter_matrix,letter_lex,here);
                    recursive_boggle(x,y,current_word,letter_lex,letter_matrix,here);
                } 
            }
            x--;
            y--;
        }
        
    }

    //puts 0's in copy matrix at end of recursion
    for (int a = 0; a < here.height(); a++) {
        for (int b = 0; b < here.width(); b++) {
            here.put(b,a,'0');
        }
    }

    if (current_word.size() == 0)
    {
        return true;
    }
    letter_lex = "<3";
    return false;
}

void boggle () {

    CharMatrix here;
    here.resize(g_grid.width(), g_grid.height());

    for (int a = 0; a < here.height(); a++) {
        for (int b = 0; b < here.width(); b++) {
            here.put(b,a,'0');
        }
    }
    
    for (unsigned int i = 0; i < g_lex.size(); i++)
    {
        string current_word = g_lex[i];
        string letter_lex = current_word.substr(0,1); // gets first letter of current_word.

        if (config.debug) 
        {
            cout << "in lex loop: " << endl;
            cout << "current word is " << current_word << endl;
        }

        for (int y = 0; y < g_grid.height(); y++)
        {
            for (int x = 0; x < g_grid.width(); x++)
            {
                char matrix = g_grid.get(x,y);
                string letter_matrix = getString(matrix);

                 if (config.debug) {
                    cout << "in boggle loop: " << endl;
                    cout << "letter_matrix is " << letter_matrix << endl;
                    cout << "letter_lex is " << letter_lex << endl;
                }
            
                if (letter_matrix == letter_lex) {
                    current_word.erase(0,1);
                    letter_lex = current_word.substr(0,1);
                    here.put(x,y,'1');

                    if (config.debug) {
                    cout << "in boggle loop after letter match: " << endl;
                    cout << "letter_matrix is " << letter_matrix << endl;
                    cout << "letter_lex is " << letter_lex << endl;
                    }

                    bool truth = recursive_boggle(x,y, current_word, letter_lex,letter_matrix, here);
                   
                    if (truth == 1) {
                    cout << g_lex[i] << endl;
                    }
                }

            }
        }

    }
}

//linked list

void linked_list_unit_test()
{

    LinkedList xoxo;
    LinkedList boo;
    string f = "first";
    string g = "second";
    string h = "third";
    string z = "bruh";

    xoxo.check();
    if (xoxo.size() != 0)
    {
        throw std::runtime_error("Size 1 expected to be 0");
    }
    xoxo.push_back(f);
    xoxo.check();
    if (xoxo.size() != 1)
    {
        throw std::runtime_error("Size 2 expected to be 1");
    }
    xoxo.push_back(z);
    xoxo.check();
    if (xoxo.size() != 2)
    {
        throw std::runtime_error("Size 3 expected to be 2");
    }
    xoxo.pop_front();
    xoxo.check();
    if (xoxo.size() != 1)
    {
        throw std::runtime_error("Size 4 expected to be 1");
    }
    xoxo.push_back(g);
    xoxo.check();
    xoxo.push_back(h);
    xoxo.check();
    if (xoxo.size() != 3)
    {
        throw std::runtime_error("Size 5 expected to be 3");
    }
    xoxo.split(boo, 2);
    xoxo.check();
    if (boo.size() != 2)
    {
        throw std::runtime_error("Size 6 for boo expected to be 2");
    }
    if (xoxo.size() != 1)
    {
        throw std::runtime_error("Size 6 for xoxo expected to be 1");
    }
    boo.check();

    for(int i = 0; i <= xoxo.size(); i++)
    {
        xoxo.pop_front();
    }
    xoxo.check();

    for(int j = 0; j <= boo.size(); j++)
    {
        boo.pop_front();
    }
    boo.check();

    cout << "passed";

}

// merge sort

void merge_sort_main() 
{   
    LinkedList* fml = new LinkedList();
    string current_lex;
    string current_linked;

    for(unsigned int i = 0; i < g_lex.size(); i++)
    {
        current_lex = g_lex[i];
        fml->push_back(current_lex);
    }
        
    fml->merge_sort();
    tear_down();

    int be_so_fr = fml->size();
    for(int j = 0; j < be_so_fr; j++)
    {
        g_lex.push_back(fml->getter());
        fml->pop_front();
    }

    int bruh = 0;
    fml->get_comparison(bruh);
    cout << "comparisons: " << bruh << endl;

    delete fml;

}

// data set

void load_csv()
{
    cout << "Please enter a filename of a .csv file:" << endl;
    cout << "> ";
    string filename;
    getline(std::cin, filename);
    g_dataset.load_csv(filename);
    g_dataset.index_data();
}

void query()
{
    // Ask for query parameters
    cout << "Please enter a column index:" << endl;
    cout << "> ";
    string column_str;
    getline(std::cin, column_str);
    int column = stoi(column_str);
    cout << "Please enter a starting value:" << endl;
    cout << "> ";
    string start;
    getline(std::cin, start);
    cout << "Please enter an ending value:" << endl;
    cout << "> ";
    string end;
    getline(std::cin, end);

    // Perform the query
    g_dataset.query(start, end, column); // (you will write this method)
}

// memory leak

void report()
{
    cout << "instantiated: " << g_instantiations << endl;
    cout << "deleted: " << g_deletions << endl;
}

//priority queue

void insert_into_priority_queue()
{
    cout << "Please enter a string:" << endl;
    cout << "> ";
    string value;
    getline(std::cin, value);
    g_priority_queue.insert(value);
}

void pop_first_from_priority_queue()
{
    string value = g_priority_queue.pop_front();
    cout << value << endl;
}

// main

int main(int argc, char** argv)
{
    config.parse_flags(argc, argv);
    
    while (true)
    {
    for (int i = 0; i < 5; i++)
    {
        cout << endl;
    }
    cout << "Lillian Morris's PF2 projects" << endl << endl;
    cout << "Lexicon size: " << g_lex.size() << endl << endl;
    cout << "0. Quit" << endl;
    cout << "1. Fill lexicon" << endl;
    cout << "2. Tear down lexicon" << endl;
    cout << "3. Load char matrix" << endl;
    cout << "4. Print char matrix" << endl;
    cout << "5. Flood fill" << endl;
    cout << "6. Boggle" << endl;
    cout << "7. Linked List Unit Test" << endl;
    cout << "8. Merge Sort" << endl;
    cout << "9. Load CSV file" << endl;
    cout << "10. Query" << endl;
    cout << "11. Memory report" << endl;
    cout << "14. Insert into priority queue" << endl;
    cout << "15. pop_first from priority queue" << endl;
    cout << "> ";
    string option;
    getline(cin, option);
    if (option.compare("0") == 0) {
        break;
    } else if (option.compare("1") == 0) {
        fill_lexicon();
    } else if (option.compare("2") == 0) {
        tear_down();
    } else if (option.compare("3") == 0) {
        load_char_matrix();
    } else if (option.compare("4") == 0) {
        print_char_matrix();
    } else if (option.compare("5") == 0) {
        flood_fill();
    } else if (option.compare("6") == 0) {
        boggle();
    } else if (option.compare("7") == 0){
        linked_list_unit_test();
    } else if (option.compare("8") == 0) {
        merge_sort_main();
    } else if (option.compare("9") == 0) {
        load_csv();
    } else if (option.compare("10") == 0) {
        query();
    } else if (option.compare("11") == 0) {
        report();
    } else if (option.compare("14") == 0)
    {
        insert_into_priority_queue();
    } else if (option.compare("15") == 0)
    {
        pop_first_from_priority_queue();
    }
    else {
        cout << option << " was not one of the options. Quitting." << endl;
        break;
    }
    }

}