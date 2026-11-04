#include "Screen.h"
#include <bits/stdc++.h>

using namespace std;

Screen::Screen() = default;

void Screen::clear()
{
    output_.clear();
}

void Screen::write(uint8_t value)
{
    if (value >= 32 && value <= 126){
        output_+=(char(value));
    }
    else{
        cout << uppercase << hex << setw(2) << setfill('0');
        output_ += "<";
        // Format the value as hexadecimal
        char hexDigits[] = "0123456789ABCDEF";
        output_ += hexDigits[value >> 4];
        output_ += hexDigits[value & 0x0F];
        output_ += ">";
    }
}

void Screen::display() const
{
    cout << output_ << endl;
}