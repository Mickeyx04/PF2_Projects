#ifndef charmatrix_h
#define charmatrix_h
#include <vector>
#include <iostream>

#include "instance_counter.h"

class CharMatrix : public InstanceCounter
{
    protected:
        std::vector<std::string> chars;

    public:
        CharMatrix();
        virtual ~CharMatrix();

        void resize(int width, int height);
        int width();
        int height();
        char get(int x, int y);
        void put(int x, int y, char c);

};

#endif