#include "charmatrix.h"

CharMatrix::CharMatrix()
{
}

CharMatrix::~CharMatrix()
{
}

void CharMatrix::resize(int width, int height)
{
    this->chars.clear();
    for (int i = 0; i < height; i++)
    {
        this->chars.push_back(std::string(width, ' '));
    }
}

int CharMatrix::width()
{
    if (height() == 0)
        return 0;
    return this->chars[0].size();
}

int CharMatrix::height()
{
    return this->chars.size();
}

char CharMatrix::get(int x, int y)
{
    return this->chars[y][x];
}

void CharMatrix::put(int x, int y, char c)
{
    this->chars[y][x] = c;
}