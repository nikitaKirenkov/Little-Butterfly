#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <stdio.h>
#include <sstream>

using namespace std;

const double PI = atan(1) * 4;

int randomNumber(int from, int to)
{
    return from + rand() % (to - from + 1);
}

string intToString(int number)
{
    ostringstream buf;
    buf << number;
    return buf.str();
}

string floatToString(float number)
{
    ostringstream buf;
    buf << number;
    return buf.str();
}

#endif // FUNCTIONS_HPP
