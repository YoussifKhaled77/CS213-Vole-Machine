#pragma once
#include <bits/stdc++.h>
#include "Screen.h"
using namespace std;

class Memory
{
public:
    Memory(Screen& screen);
    static constexpr size_t MemorySize = 256;

    unsigned char read(size_t address) const;
    void write(size_t address, unsigned char value);
    void clear();
    void loadInstruction(size_t address, unsigned short instruction);
    void display() const;

private:
    array<unsigned char, MemorySize> cells_ = {};
    Screen& screen_;

};
