#pragma once
#include <bits/stdc++.h>

using namespace std;

class Screen
{
public:
    Screen();
    void write(uint8_t value);
    void display() const;
    void clear();

private:
    string output_;
};
