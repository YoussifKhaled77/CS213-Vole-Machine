#include "Memory.h"

Memory::Memory(Screen& screen) : screen_(screen) {}

unsigned char Memory::read(size_t address) const{
    if(address >= MemorySize) {
        throw out_of_range("Memory read: Address out of range");
    }
    return cells_[address];
}

void Memory::write(size_t address, unsigned char value){
    if (address >= MemorySize) {
        throw out_of_range("Memory write: Address out of range");
    }
    cells_[address] = value;

    if (address == 0xFF) { 
        screen_.write(value);
    }
}

void Memory::loadInstruction(size_t address, unsigned short instruction) {
    if (address >= MemorySize - 1) {
        throw out_of_range("Memory loadInstruction: Address out of range");
    }
    cells_[address] = (instruction >>8) & 0xFF;
    cells_[address+1] = instruction & 0xFF;
}


void Memory::display() const
{
    cout << "\nMemory:\n";
    cout << "---------------------------------\n";
    for(size_t row = 0; row < 16; row++){

        cout << "[" << uppercase << hex << setw(2) << setfill('0') << row << "] ";
        for(size_t col = 0; col < 16; col++){
            size_t address = row * 16 + col;

            cout << uppercase << hex << setw(2) << setfill('0') << int(cells_[address]);

            if (col < 15) cout << ' ';
        }
        cout << endl;
    }
}


void Memory::clear() {
    cells_.fill(0);
}


