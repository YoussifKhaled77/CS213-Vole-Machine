#include "InstructionDecoder.h"
#include <cctype>

bool InstructionDecoder::isValidRegister(int number) const {
    return number >= 0 && number <= 15;
}

bool InstructionDecoder::isValidFormat(const std::string& instruction) const {
    if (instruction.size() != 4) return false;
    for (char c : instruction) {
        if (!std::isxdigit(static_cast<unsigned char>(c))) return false;
    }
    return true;
}

Instruction InstructionDecoder::decode(const std::string& instruction) const {
    unsigned long raw = std::stoul(instruction, nullptr, 16);

    Instruction instr;
    instr.opcode = static_cast<uint8_t>((raw >> 12) & 0x0F);
    instr.R      = static_cast<uint8_t>((raw >> 8) & 0x0F);
    instr.S      = static_cast<uint8_t>((raw >> 4) & 0x0F);
    instr.T      = static_cast<uint8_t>(raw & 0x0F);
    instr.XY     = static_cast<uint8_t>(raw & 0xFF);
    return instr;
}

bool InstructionDecoder::validate(const std::string& instruction) const {
    if (!isValidFormat(instruction)) return false;

    Instruction instr = decode(instruction);

    if (instr.opcode < 0x1 || instr.opcode > 0xC) return false;
    if (!isValidRegister(instr.R)) return false;
    if (!isValidRegister(instr.S)) return false;
    if (!isValidRegister(instr.T)) return false;

    return true;
}