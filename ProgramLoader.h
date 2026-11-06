#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include "Memory.h"
#include "InstructionDecoder.h"

class ProgramLoader
{
private:
    Memory& memory_;
    InstructionDecoder decoder_;

    std::vector<uint16_t> parseLine(const std::string& line) const;
    bool isValidInstruction(const std::string& instruction) const;

public:
    ProgramLoader(Memory& memory);

    void loadFromFile(const std::string& filename, int startAddress);
    bool validateStartAddress(int address) const;
};