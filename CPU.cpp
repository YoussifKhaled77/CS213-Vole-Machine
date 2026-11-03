#include "CPU.h"
#include "Memory.h"
#include "RegisterFile.h"
#include <iomanip>
#include <sstream>
#include <cmath>

CPU::CPU(Memory& mem, RegisterFile& regs)
    : pc_(0), ir_("0000"), halted_(false),
      memory_(mem), registers_(regs), decoder_() {}

void CPU::reset(int startAddress) {
    pc_ = startAddress;
    ir_ = "0000";
    halted_ = false;
}

void CPU::updatePC(int address) {
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

    std::stringstream ss;
    ss << std::hex << std::uppercase << std::setfill('0')
       << std::setw(2) << static_cast<int>(highByte)
       << std::setw(2) << static_cast<int>(lowByte);
    ir_ = ss.str();

    pc_ += 2;
}

void CPU::executeStep() {
    if (halted_) return;

    fetch();
    if (halted_) return;

    if (!decoder_.validate(ir_)) {
        halted_ = true;
        return;
    }

    Instruction instr = decoder_.decode(ir_);
    executeInstruction(instr);
}

void CPU::executeInstruction(const Instruction& instr) {
    switch (instr.opcode) {
        case 0x1: {
            registers_.write(instr.R, memory_.read(instr.XY));
            break;
        }
        case 0x2: {
            registers_.write(instr.R, instr.XY);
            break;
        }
        case 0x3: {
            memory_.write(instr.XY, registers_.read(instr.R));
            break;
        }
        case 0x4: {
            registers_.write(instr.T, registers_.read(instr.S));
            break;
        }
        case 0x5: {
            uint8_t sum = static_cast<uint8_t>(
                registers_.read(instr.S) + registers_.read(instr.T));
            registers_.write(instr.R, sum);
            break;
        }
        case 0x6: {
            registers_.write(instr.R,
                addFloatingPoint(registers_.read(instr.S),
                                 registers_.read(instr.T)));
            break;
        }
        case 0x7: {
            registers_.write(instr.R,
                registers_.read(instr.S) | registers_.read(instr.T));
            break;
        }
        case 0x8: {
            registers_.write(instr.R,
                registers_.read(instr.S) & registers_.read(instr.T));
            break;
        }
        case 0x9: {
            registers_.write(instr.R,
                registers_.read(instr.S) ^ registers_.read(instr.T));
            break;
        }
        case 0xA: {
            uint8_t val = registers_.read(instr.R);
            int count = instr.T % 8;
            uint8_t rotated = static_cast<uint8_t>(
                (val >> count) | (val << (8 - count)));
            registers_.write(instr.R, rotated);
            break;
        }
        case 0xB: {
            if (registers_.read(instr.R) == registers_.read(0)) {
                updatePC(instr.XY);
            }
            break;
        }
        case 0xC: {
            halted_ = true;
            break;
        }
        default:
            halted_ = true;
            break;
    }
}

uint8_t CPU::addFloatingPoint(uint8_t val1, uint8_t val2) {
    auto decode = [](uint8_t b, float& val) {
        if (b == 0) { val = 0.0f; return; }
        int sign = (b & 0x80) ? -1 : 1;
        int exp = ((b & 0x70) >> 4) - 3;
        float mantissa = 0.5f + ((b & 0x0F) / 32.0f);
        val = sign * mantissa * std::pow(2.0f, exp);
    };

    float f1 = 0.0f, f2 = 0.0f;
    decode(val1, f1);
    decode(val2, f2);

    float res = f1 + f2;
    if (res == 0.0f) return 0x00;

    uint8_t signBit = (res < 0) ? 0x80 : 0x00;
    res = std::abs(res);

    int exp = 0;
    float mantissa = std::frexp(res, &exp);
    int mantissaBits = static_cast<int>(std::round((mantissa - 0.5f) * 32.0f));

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

    return signBit | static_cast<uint8_t>(biasedExp << 4)
                   | static_cast<uint8_t>(mantissaBits & 0x0F);
}