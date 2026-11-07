#pragma once

#include "VoleMachine.h"

class MachineUI
{
private:
    VoleMachine& machine_;

public:
    explicit MachineUI(VoleMachine& machine);

    void displayMainMenu() const;
    void runProgram();
    void runProgramStepByStep();
    void loadProgram();
    void modifyMemory();
    void modifyRegister();
    void printMachineStatus() const;
};
