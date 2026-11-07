#include <iostream>
#include "VoleMachine.h"
#include "MachineUI.h"

using namespace std;

int main()
{
    VoleMachine machine;
    MachineUI ui(machine);
    int choice;

    do
    {
        ui.displayMainMenu();
        std::cout << "Choose an option: ";

        if (!(std::cin >> choice))
            return 1;

        switch (choice)
        {
        case 1: ui.runProgram(); break;
        case 2: ui.runProgramStepByStep(); break;
        case 3: ui.loadProgram(); break;
        case 4: ui.modifyMemory(); break;
        case 5: ui.modifyRegister(); break;
        case 6: ui.printMachineStatus(); break;
        case 0: break;
        default: std::cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}