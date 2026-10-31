#include "RegisterFile.h"

RegisterFile::RegisterFile() = default;

void RegisterFile::clear(){
    registers_.fill(0);
}

unsigned char RegisterFile::read(int index) const{
        if (index < 0 || index >= 16) throw out_of_range("Register index out of range");

    return registers_[index];
}

void RegisterFile::write(int index, unsigned char value){
    if (index < 0 || index >= 16) throw out_of_range("Register index out of range");

    registers_[index] = value;
}

void RegisterFile::display() const{
    cout << "\nRegister File:\n";
    cout << "---------------------------------\n";

    for (int i = 0 ; i < 16 ; i++){
        cout << "R" << hex << uppercase << i
             << ": " << setw(2) << setfill('0')
             << (int)registers_[i] << "\t";

        if ((i + 1) % 4 == 0)
            cout << endl;
    }

    cout << dec;
    cout << "---------------------------------\n";
}
