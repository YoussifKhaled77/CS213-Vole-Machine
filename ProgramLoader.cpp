#include "ProgramLoader.h"
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>

using namespace std;

ProgramLoader::ProgramLoader(Memory& memory)
    : memory_(memory), decoder_()
{
}

bool ProgramLoader::validateStartAddress(int address) const
{
    return address >= 0 && address < 255 && address % 2 == 0;
}

bool ProgramLoader::isValidInstruction(const string& instruction) const
{
    return decoder_.isValidFormat(instruction);
}

vector<uint16_t> ProgramLoader::parseLine(const string& line) const
{
    vector<uint16_t> instructions;

    size_t start = line.find_first_not_of(" \t\r\n");

    if (start == string::npos || line[start] == ';')
        return instructions;

    if (line.size() - start < 4)
        throw runtime_error("Invalid instruction: " + line);

    string instruction = line.substr(start, 4);

    transform(instruction.begin(), instruction.end(),
              instruction.begin(),
              [](unsigned char c) {
                  return static_cast<char>(toupper(c));
              });

    if (!isValidInstruction(instruction))
        throw runtime_error("Invalid instruction: " + instruction);

    uint16_t value = static_cast<uint16_t>(
        stoul(instruction, nullptr, 16)
    );

    instructions.push_back(value);

    return instructions;
}

void ProgramLoader::loadFromFile(const string& filename, int startAddress)
{
    if (!validateStartAddress(startAddress))
        throw invalid_argument("Invalid start address.");

    ifstream file(filename);

    if (!file.is_open())
        throw runtime_error("Could not open program file: " + filename);

    vector<uint16_t> program;
    string line;

    while (getline(file, line))
    {
        vector<uint16_t> lineInstructions = parseLine(line);

        for (uint16_t instruction : lineInstructions)
            program.push_back(instruction);
    }

    file.close();

    if (program.empty())
        throw runtime_error("Program file contains no instructions.");

    if (startAddress + static_cast<int>(program.size() * 2) > 256)
        throw runtime_error("Program is too large for memory.");

    int address = startAddress;

    for (uint16_t instruction : program)
    {
        memory_.loadInstruction(address, instruction);
        address += 2;
    }
}