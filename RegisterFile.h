#pragma once
#include <bits/stdc++.h>
using namespace std;

class RegisterFile
{
private:
    array<unsigned char, 16> registers_{};

public:
    RegisterFile();
    
    unsigned char read(int index) const;
    void clear();
    void write(int index, unsigned char value);
    void display() const;
};