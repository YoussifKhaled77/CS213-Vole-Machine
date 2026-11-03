#pragma once

#include <string>
#include <cstdint>
#include "InstructionDecoder.h"

class Memory;
class RegisterFile;

class CPU {
private:
    int pc_;
    std::string ir_;
    bool halted_;

    Memory& memory_;
    RegisterFile& registers_;
    InstructionDecoder decoder_;

    uint8_t addFloatingPoint(uint8_t val1, uint8_t val2);

public:
    CPU(Memory& mem, RegisterFile& regs);

    void fetch();
    void executeStep();
    void executeInstruction(const Instruction& instr);
    void updatePC(int address);

    void reset(int startAddress = 0);
    bool isHalted() const { return halted_; }
    int getPC() const { return pc_; }
    std::string getIR() const { return ir_; }
};