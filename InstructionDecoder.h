#pragma once

#include <cstdint>
#include <string>

struct Instruction {
    uint8_t opcode;
    uint8_t R;
    uint8_t S;
    uint8_t T;
    uint8_t XY;
};

class InstructionDecoder {
public:
    bool validate(const std::string& instruction) const;
    Instruction decode(const std::string& instruction) const;
    bool isValidRegister(int number) const;
    bool isValidFormat(const std::string& instruction) const;
};