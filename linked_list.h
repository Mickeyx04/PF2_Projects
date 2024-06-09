#ifndef linked_list
#define linked_list

#include <string>
#include <iostream>
#include <vector>
#include <unistd.h>

#include "instance_counter.h"

// Represents a single node in a linked list
class LinkedListNode : public InstanceCounter
{
protected:

public:
    std::string value;
    LinkedListNode* next;

    LinkedListNode(std::string& val);
    virtual ~LinkedListNode();
};



// A data structure that holds a list of strings
class LinkedList : public InstanceCounter
{
protected:
    LinkedListNode* first;
    LinkedListNode* last;
    int length;

public:

    LinkedList();
    virtual ~LinkedList();

    void push_back(std::string& s);
    void push_front(std::string& s);
    void pop_front();
    int size();
    void split(LinkedList& split, int pls);
    void merge(LinkedList& a);
    void merge_sort();
    int smart_compare(std::string& a, std::string& b);
    std::string getter();
    void get_comparison(int& b);
    void transfer(LinkedList& b);
    void check();

   // int g_comparison;
};

#endif