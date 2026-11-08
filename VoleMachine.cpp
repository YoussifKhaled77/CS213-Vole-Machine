#include "VoleMachine.h"
#include <iostream>
#include <iomanip>

using namespace std;

VoleMachine::VoleMachine()
    : screen_(),
      memory_(screen_),
      registers_(),
      cpu_(memory_, registers_),
      loader_(memory_)
{
}

void VoleMachine::loadProgram(const string& filename, int startAddress)
{
    reset(startAddress);

    loader_.loadFromFile(filename, startAddress);
}

void VoleMachine::run()
{
    while (!cpu_.isHalted())
    {
        cpu_.executeStep();
    }

    cout << "\nProgram execution finished.\n";

    displayMachineState();
    displayScreenOutput();
}

void VoleMachine::step()
{
    if (cpu_.isHalted())
    {
        cout << "\nProgram has already halted.\n";
        return;
    }

    cpu_.executeStep();

    displayMachineState();
    displayScreenOutput();
}

void VoleMachine::reset(int startAddress)
{
    memory_.clear();
    registers_.clear();
    screen_.clear();
    cpu_.reset(startAddress);
}

void VoleMachine::writeMemory(size_t address, uint8_t value)
{
    memory_.write(address, value);
}

void VoleMachine::writeRegister(int index, uint8_t value)
{
    registers_.write(index, value);
}

void VoleMachine::displayMachineState() const
{
    cout << "\n========== MACHINE STATE ==========\n";

    cout << "\nProgram Counter (PC): "
         << hex << uppercase << cpu_.getPC() << dec << '\n';

    cout << "Instruction Register (IR): "
         << cpu_.getIR() << '\n';

    registers_.display();

    memory_.display();

    cout << dec;
    cout << "\n===================================\n";
}

void VoleMachine::displayScreenOutput() const
{
    cout << "\nScreen Output:\n";
    cout << "---------------------------------\n";

    screen_.display();

    cout << "---------------------------------\n";
}