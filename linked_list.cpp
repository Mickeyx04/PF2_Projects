#include <iostream>
#include <vector>
#include <unistd.h>
#include "config.h"
#include "charmatrix.h"
#include "linked_list.h"
#include <stdio.h>
#include <string.h>
#include <set>

int g_comparison = 0;
extern int g_instantiations;
extern int g_deletions;

LinkedList::LinkedList()
{
    this->first = nullptr;
    this->last = nullptr;
    this->length = 0;
}

LinkedList::~LinkedList()
{
    
}

LinkedListNode::LinkedListNode(std::string& val)
{
    this->next = nullptr;
    this->value = val;
}

LinkedListNode::~LinkedListNode()
{
    delete(this->next);
}

void LinkedList::push_back(std::string& s)
{
    LinkedListNode* new_node = new LinkedListNode(s);
    if (this->first)
    {
        if (!this->last) {
            throw std::runtime_error("How is there a first, but no last?"); }
        this->last->next = new_node;
    }
    else
    {
        if (this->last) {
            throw std::runtime_error("How is there a last, but no first?"); }
        this->first = new_node;
    }
    this->last = new_node;
    this->length++;
}

void LinkedList::push_front(std::string& s)
{
    // note: there is an error in this function babes. 
    // something about there being different firsts and lasts but no next
    LinkedListNode* new_node = new LinkedListNode(s);
    LinkedListNode* tempNode = this->first;
    if (this->first)
    {
        this->first = new_node;
        this->first = tempNode;
    }
    else
    {
        this->first = new_node;
        this->last = new_node;
    }
    this->length++;

    delete new_node;
}

void LinkedList::pop_front()
{   
    LinkedListNode* temp = nullptr;

    if (this->length <= 0)
    {
        throw std::runtime_error("No nodes to remove babes");
    }

    if (this->length == 1)
    {
        delete(this->first);
        this->first = nullptr;
        this->last = nullptr;
    }
    else if (this->length == 2)
    {
        this->first->next = nullptr;
        delete(this->first);
        this->first = this->last;

    } 
    else 
    {
        temp = this->first->next;
        this->first->next = nullptr;
        delete(this->first);
        this->first = temp;
    }

    this->length--;
    
}

int LinkedList::size()
{
    return this->length;
}

void LinkedList::split(LinkedList& boo, int pls)
{
    std::string s;

    if (pls < 0) {
        throw std::runtime_error("can't be neg babes");
    }

    for(int i = 0; i < pls; i++)
    {
        s = this->first->value; 
        boo.push_back(s);
        this->pop_front();
    }
}

int LinkedList::smart_compare(std::string& a, std::string& b)
{
    g_comparison++;

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

void LinkedList::merge(LinkedList& a)
{
    LinkedList *temp = new LinkedList();
    temp->first = this->first;
    temp->last = this->last;
    temp->length = this->length;

    this->first = nullptr;
    this->last = nullptr;
    this->length = 0;

    int total_size = a.size() + temp->size();

    for(int i = 0; i < total_size; i++)
    {
        if ((temp->size() != 0) && (a.size() != 0))
        {
            int compare = smart_compare(temp->first->value, a.first->value);
            
            if (compare > 0)
            {
                this->push_back(a.first->value);
                a.pop_front();
            }
            else
            {
                this->push_back(temp->first->value);
                temp->pop_front();
            }
        }
        else if(temp->size() == 0)
        {
            this->push_back(a.first->value);
            a.pop_front();
        }
        else 
        {
            this->push_back(temp->first->value);
            temp->pop_front();
        }
    }

    delete temp;
}

void LinkedList::merge_sort()
{
    LinkedList a;

    if (this->size() < 2)
    {
        return;
    }

    this->split(a, (this->size() / 2));

    this->merge_sort();
    a.merge_sort();

    this->merge(a);
}

std::string LinkedList::getter()
{
    std::string s;
    s = this->first->value;

    return s;
}

void LinkedList::get_comparison(int& b)
{
    b = g_comparison;
}

void LinkedList::check()
{

    std::cout << "instantiations: " << g_instantiations << std::endl;
    std::cout << "deletions: " << g_deletions << std::endl;

    if (this->first && !this->last)
        throw std::runtime_error("first but no last");
    if (!this->first && this->last)
        throw std::runtime_error("last but no first");
    if (this->first == this->last && this->first && this->first->next)
        throw std::runtime_error("first and last are the same, but first has a next");
    if (this->first != this->last && !this->first->next)
        throw std::runtime_error("first and last are different, but first has no next");
    int count = 0;
    LinkedListNode* pLast = nullptr;
    std::set<LinkedListNode*> seen_before;
    for (LinkedListNode* pNode = this->first; pNode; pNode = pNode->next)
    {
        if (seen_before.find(pNode) != seen_before.end())
            throw std::runtime_error("got a loop");
        seen_before.insert(pNode);
        count++;
        pLast = pNode;
    }
    if (count != this->length)
        throw std::runtime_error("length does not match the number of nodes in the list");
    if (pLast != this->last)
        throw std::runtime_error("last is not in the list");
    if (pLast && pLast->next)
        throw std::runtime_error("last has a next");
}