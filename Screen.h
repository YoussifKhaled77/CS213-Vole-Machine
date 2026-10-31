#pragma once
#include <bits/stdc++.h>

using namespace std;

class Screen
{
public:
    Screen();
    void write(unsigned char value);
    void display() const;
    void clear();

private:
    string output_;
};
