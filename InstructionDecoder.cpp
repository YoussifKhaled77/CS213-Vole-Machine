#include "InstructionDecoder.h"
#include <stdexcept>
#include <cctype>

using namespace std;

bool InstructionDecoder::isValidRegister(int number) const
{
    return number >= 0 && number <= 15;
}

bool InstructionDecoder::isValidFormat(const string& instruction) const
{
    // An instruction must contain exactly 4 hexadecimal digits.
    if (instruction.length() != 4)
        return false;

    for (char c : instruction)
    {
        if (!isxdigit(static_cast<unsigned char>(c)))
            return false;
    }

    int opcode = stoi(instruction.substr(0, 1), nullptr, 16);
    int first = stoi(instruction.substr(1, 1), nullptr, 16);

    // MOVE instruction: 4 0 R S
    if (opcode == 4 && first != 0)
        return false;

    // ROTATE instruction: A R 0 X
    if (opcode == 10 && instruction[2] != '0')
        return false;

    // HALT instruction: C 0 0 0
    if (opcode == 12)
        return instruction == "C000" || instruction == "c000";

    // Supported opcodes: 1 through B.
    return opcode >= 1 && opcode <= 11;
}

Instruction InstructionDecoder::decode(const string& instruction) const
{
    if (!isValidFormat(instruction))
        throw invalid_argument("Invalid instruction format");

    Instruction result;

    result.opcode = stoi(instruction.substr(0, 1), nullptr, 16);
    result.first = stoi(instruction.substr(1, 1), nullptr, 16);
    result.second = stoi(instruction.substr(2, 1), nullptr, 16);
    result.third = stoi(instruction.substr(3, 1), nullptr, 16);

    return result;
}