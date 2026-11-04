#include "CPU.h"
#include "Memory.h"
#include "RegisterFile.h"
#include <iomanip>
#include <sstream>
#include <cmath>

CPU::CPU(Memory& memory, RegisterFile& registers) : memory_(memory), registers_(registers){}

void CPU::reset(int startAddress){
    if (startAddress < 0 || startAddress >= 256)
        throw out_of_range("Invalid start address");

    pc_ = startAddress;
    ir_ = "";
    halted_ = false;
}

void CPU::updatePC(int address){
    if (address < 0 || address >= 256)
        throw out_of_range("Invalid jump address");

    pc_ = address;
}

void CPU::fetch() {
    if (halted_) return;

    if (pc_ < 0 || pc_ > 254) {
        halted_ = true;
        return;
    }

    uint8_t highByte = memory_.read(pc_);
    uint8_t lowByte  = memory_.read(pc_ + 1);

    stringstream ss;
    ss << hex << uppercase << setfill('0')
       << setw(2) << int(highByte)
       << setw(2) << int(lowByte);
    ir_ = ss.str();

    pc_ += 2;
}

void CPU::executeStep() {
    if (halted_) return;

    fetch();
    if (halted_) return;

    if (!decoder_.isValidFormat(ir_)) {
        halted_ = true;
        return;
    }

    Instruction instr = decoder_.decode(ir_);
    executeInstruction(instr);
}

void CPU::executeInstruction(const Instruction& instruction) {

    int opcode = instruction.opcode;
    int first = instruction.first;
    int second = instruction.second;
    int third = instruction.third;

    switch (opcode) {
        case 1: {
            // Load memory[XY] into register R.
            registers_.write(first, memory_.read(second * 16 + third));
            break;
        }
        case 2: {
            // Load immediate XY into register R.
            registers_.write(first, second * 16 + third);
            break;
        }
        case 3: {
            // Store register R in memory[XY].
            memory_.write(second * 16 + third, registers_.read(first));
            break;
        }
        case 4: {
            // Move register R into register S.
            registers_.write(first, registers_.read(second));
            break;
        }
        case 5: {
            // Add registers S and T, store the result in R.
            uint8_t sum = static_cast<uint8_t>(
                registers_.read(second) + registers_.read(third));
            registers_.write(first, sum);
            break;
        }
        case 6: {
            // Floating-point addition.
            registers_.write(first,
                addFloatingPoint(registers_.read(second),
                                 registers_.read(third)));
            break;
        }
        case 7: {
            // Bitwise OR.
            registers_.write(first,
                registers_.read(second) | registers_.read(third));
            break;
        }
        case 8: {
            // Bitwise AND.
            registers_.write(first,
                registers_.read(second) & registers_.read(third));
            break;
        }
        case 9: {
            // Bitwise XOR.
            registers_.write(first,
                registers_.read(second) ^ registers_.read(third));
            break;
        }
        case 10: {
            // Rotate register R right X times.(circular shift)
            uint8_t val = registers_.read(first);
            int count = third % 8;
            uint8_t rotated = static_cast<uint8_t>(
                (val >> count) | (val << (8 - count)));
            registers_.write(first, rotated);
            break;
        }
        case 11: {
            // Jump to address XY if register R equals R0.
            if (registers_.read(first) == registers_.read(0)) {
                updatePC(second * 16 + third);
            }
            break;
        }
        case 12: {
            // Halt the CPU.
            halted_ = true;
            break;
        }
        default:
            throw invalid_argument("Unknown instruction opcode");
    }
}

uint8_t CPU::addFloatingPoint(uint8_t val1, uint8_t val2) {
    auto decode = [](uint8_t b, float& val) {
        if (b == 0) { val = 0.0f; return; }
        int sign = (b & 0x80) ? -1 : 1;
        int exp = ((b & 0x70) >> 4) - 3;
        float mantissa = 0.5f + ((b & 0x0F) / 32.0f);
        val = sign * mantissa * pow(2.0f, exp);
    };

    float f1 = 0.0f, f2 = 0.0f;
    decode(val1, f1);
    decode(val2, f2);

    float res = f1 + f2;
    if (res == 0.0f) return 0x00;

    uint8_t signBit = (res < 0) ? 0x80 : 0x00;
    res = abs(res);

    int exp = 0;
    float mantissa = frexp(res, &exp);
    int mantissaBits = int(round((mantissa - 0.5f) * 32.0f));

    if (mantissaBits > 15) {
        mantissaBits = 0;
        ++exp;
    }

    int biasedExp = exp + 3;
    if (biasedExp <= 0) return 0x00;
    if (biasedExp > 7) {
        biasedExp = 7;
        mantissaBits = 15;
    }

    return signBit | uint8_t(biasedExp << 4)
                   | uint8_t(mantissaBits & 0x0F);
}