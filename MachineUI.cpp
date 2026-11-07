#include "MachineUI.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

MachineUI::MachineUI(VoleMachine& machine): machine_(machine){}

void MachineUI::displayMainMenu() const
{
    cout << "\n========== VOLE MACHINE ==========\n"
         << "1. Run program\n"
         << "2. Run step-by-step\n"
         << "3. Load program file\n"
         << "4. Modify memory\n"
         << "5. Modify register\n"
         << "6. Print machine status\n"
         << "0. Exit\n"
         << "==================================\n"
         << "Enter your choice: ";
}

void MachineUI::runProgram()
{
    machine_.run();
    cout << "Program executed completely.\n";
}

void MachineUI::runProgramStepByStep()
{
    string choice;
    cout << "Starting step-by-step execution. Press 's' for next step, 'q' to quit.\n";
    
    while (true)
    {
        cout << "Command (s/q): ";
        cin >> choice;
        
        if (choice == "q" || choice == "Q")
        {
            break;
        }
        else if (choice == "s" || choice == "S")
        {
            machine_.step();
            printMachineStatus();
        }
        else
        {
            cout << "Please enter 's' or 'q' only.\n";
        }
    }
}

void MachineUI::loadProgram()
{
    string filename;
    cout << "Enter file name (e.g., program.txt): ";
    cin >> filename;
    
    int address;
    cout << "Enter starting address in hex (e.g., 00): ";
    cin >> hex >> address;
    
    machine_.loadProgram(filename, address);
    cout << "Program loaded successfully!\n";
}

void MachineUI::modifyMemory()
{
    int address;
    int value;
    
    cout << "Enter memory address in hex (e.g., 00): ";
    cin >> hex >> address;
    
    cout << "Enter new value in hex (e.g., FF): ";
    cin >> hex >> value;
    
    machine_.writeMemory(address, static_cast<uint8_t>(value));
    cout << "Memory updated successfully!\n";
}

void MachineUI::modifyRegister()
{
    int regIndex;
    int value;
    
    cout << "Enter register index (0-F): ";
    cin >> hex >> regIndex;
    
    cout << "Enter new value in hex: ";
    cin >> hex >> value;
    
    machine_.writeRegister(regIndex, static_cast<uint8_t>(value));
    cout << "Register updated successfully!\n";
}

void MachineUI::printMachineStatus() const
{
    cout << "\n--- Current Machine Status ---\n";
    machine_.displayMachineState();
    machine_.displayScreenOutput();
    cout << "------------------------------\n";
}
