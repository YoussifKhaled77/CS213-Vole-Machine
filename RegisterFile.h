#pragma once
#include <bits/stdc++.h>
using namespace std;

class RegisterFile
{
private:
    array<uint8_t, 16> registers_{};

public:
    RegisterFile();
    
    uint8_t read(int index) const;
    void clear();
    void write(int index, uint8_t value);
    void display() const;
};