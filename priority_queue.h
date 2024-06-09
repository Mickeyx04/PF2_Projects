#ifndef priority_queue
#define priority_queue

#include <vector>
#include <string>

using std::vector;
using std::string;

class Heap
{
protected:
    vector<string> heap;

public:
    Heap();
    virtual ~Heap();

    void insert(string s);
    string pop_front();

    void print();
    void check_heap_property(string s);

protected:
    void sink(int index);
    void swim(int index);
};

#endif