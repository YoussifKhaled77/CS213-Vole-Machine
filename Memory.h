#pragma once
#include <bits/stdc++.h>
#include "Screen.h"
using namespace std;

class Memory
{
public:
    Memory(Screen& screen);
    static constexpr size_t MemorySize = 256;

    uint8_t read(size_t address) const;
    void write(size_t address, uint8_t value);
    void clear();
    void loadInstruction(size_t address, uint16_t  instruction);
    void display() const;

private:
    array<uint8_t, MemorySize> cells_ = {};
    Screen& screen_;

};
