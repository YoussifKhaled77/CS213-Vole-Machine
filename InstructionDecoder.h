#pragma once
#include <bits/stdc++.h>
using namespace std;

struct Instruction {
    uint8_t opcode;
    uint8_t first;
    uint8_t second;
    uint8_t third;
};

class InstructionDecoder{
public:
    Instruction decode(const string& instruction) const;
    bool isValidFormat(const string& instruction) const;
    bool isValidRegister(int number) const;
};