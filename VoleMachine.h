#pragma once

#include <string>
#include "Screen.h"
#include "Memory.h"
#include "RegisterFile.h"
#include "CPU.h"
#include "ProgramLoader.h"

class VoleMachine
{
private:
    Screen screen_;
    Memory memory_;
    RegisterFile registers_;
    CPU cpu_;
    ProgramLoader loader_;

public:
    VoleMachine();

    void loadProgram(const std::string& filename, int startAddress);

    void run();
    void step();

    void reset(int startAddress = 0);

    void writeMemory(size_t address, uint8_t value);
    void writeRegister(int index, uint8_t value);

    void displayMachineState() const;
    void displayScreenOutput() const;
};