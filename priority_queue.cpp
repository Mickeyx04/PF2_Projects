#include "priority_queue.h"
#include <iostream>

using std::cout;
using std::endl;

Heap::Heap()
{
    //so it's a one index array
    this->heap.push_back("x");
}

Heap::~Heap()
{

}

void Heap::print()
{
    cout << "----- start of heap -----" << endl;
    for (unsigned int i = 1; i < this->heap.size(); i++)
    {
        cout << this->heap[i] << endl;
    }
    cout << "----- end of heap -----" << endl;
}

void Heap::check_heap_property(string s)
{
    for (unsigned int i = 1; i < this->heap.size(); i++)
    {
        unsigned int child1 = i * 2;
        unsigned int child2 = child1 + 1;
        if (child1 < this->heap.size())
        {
            if (this->heap[child1] < this-> heap[i])
                throw std::runtime_error(s + " Got a problem1");
        }
        if (child2 < this->heap.size())
        {
            if (this->heap[child2] < this-> heap[i])
                throw std::runtime_error(s + " Got a problem2");
        }
    }
}

void Heap::insert(string s)
{
    this->check_heap_property("Insert begin");

    this->heap.push_back(s);
    this->sink(this->heap.size() - 1);

    this->check_heap_property("Insert end");
}

string Heap::pop_front()
{
    this->check_heap_property("Pop_front begin");

    string return_value = this->heap[1];
    this->swim(1);

    this->check_heap_property("Pop_front end");  
    return return_value;
}

void Heap::sink(int index)
{
    int parent_index = index / 2;

    if (index > 1 && (this->heap[index] < this->heap[parent_index]))
    {
        std::swap(this->heap[index], this->heap[parent_index]);
        this->sink(parent_index);
    }
    this->check_heap_property("Sink end");
}

void Heap::swim(int index)
{
    while (true)
    {
        unsigned int child1 = index * 2;
        unsigned int child2 = child1 + 1;

        if (child1 >= this-> heap.size())
            break;
        if (child2 >= this->heap.size())
        {
            this->heap[index] = this->heap[child1];
            index = child1;
        }
        else if (this->heap[child1] < this->heap[child2])
        {
            this->heap[index] = this->heap[child1];
            index = child1;
        }
        else
        {
            this->heap[index] = this->heap[child2];
            index = child2;
        }
    }

    this->heap[index] = this->heap[this->heap.size() - 1];
    this->sink(index);
    this->heap.pop_back();
    this->check_heap_property("Swim end");
}