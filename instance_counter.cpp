#include "instance_counter.h"
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

extern int g_instantiations;
extern int g_deletions;

InstanceCounter::InstanceCounter()
{
    g_instantiations++;
}

InstanceCounter::~InstanceCounter()
{
    g_deletions++;
}
