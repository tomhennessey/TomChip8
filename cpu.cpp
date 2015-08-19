#include "cpu.h"

#include <GLUT/glut.h>

#include <iostream>
#include <random>

C8CPU::C8CPU()
{
    Reset();
}

void C8CPU::Reset()
{
    address = 0;
    PC = 0x200;
    memset(registers, 0, sizeof(registers));

    // TODO: Use input stream
    FILE *in;
    in = fopen("c8tests/clock.ch8", "rb");
    fread(&memory[0x200], romSize, 1, in);
    fclose(in);
    memset(pixels, 0, sizeof(pixels));

    // load fontset into memory
    for (int i = 0; i < 80; ++ i) {
        memory[i] = fontset[i];
    }
}

word_t C8CPU::fetchOpcode()
{
    word_t op = (memory[PC] << 8) | (memory[PC + 1]);
    PC += 2;
    return op;
}

void C8CPU::decodeOpcode()
{
    word_t opcode = fetchOpcode();
//    std::cout << std::hex << "PC: " << PC - 2 << "; opcode: " << opcode << std::endl;
    switch(0xF000 & opcode) {
        case 0x0000:
            decodeOP0(opcode); break;
        case 0x8000:
            decodeOP8(opcode); break;
        case 0xE000:
            decodeOPE(opcode); break;
        case 0xF000:
            decodeOPF(opcode); break;
        case 0x1000:
            op_1NNN(opcode); break;
        case 0x2000:
            op_2NNN(opcode); break;
        case 0x3000:
            op_3XNN(opcode); break;
        case 0x4000:
            op_4XNN(opcode); break;
        case 0x5000:
            op_5XY0(opcode); break;
        case 0x6000:
            op_6XNN(opcode); break;
        case 0x7000:
            op_7XNN(opcode); break;
        case 0x9000:
            op_9XY0(opcode); break;
        case 0xA000:
            op_ANNN(opcode); break;
        case 0xB000:
            op_BNNN(opcode); break;
        case 0xC000:
            op_CXNN(opcode); break;
        case 0xD000:
            op_DXYN(opcode); break;
        default:
            std::cout << std::hex << "Error decoding opcode " << opcode << std::endl;
    }
}

void C8CPU::decodeOP0(word_t opcode)
{
    int leastBit = opcode & 0x000F;
    if (leastBit == 0) { 
        op_00E0(opcode);
    } else if (leastBit == 0xE) {
        op_00EE(opcode);
    } else {
        op_0NNN(opcode);
    }
}

void C8CPU::decodeOP8(word_t opcode)
{
    int leastBit = opcode & 0x000F;
    switch(leastBit) {
        case 0:
            op_8XY0(opcode); break;
        case 1:
            op_8XY1(opcode); break;
        case 2:
            op_8XY2(opcode); break;
        case 3:
            op_8XY3(opcode); break;
        case 4:
            op_8XY4(opcode); break;
        case 5:
            op_8XY5(opcode); break;
        case 6:
            op_8XY6(opcode); break;
        case 7: 
            op_8XY7(opcode); break;
        case 0xE:
            op_8XYE(opcode); break;
    }
}

void C8CPU::decodeOPE(word_t opcode)
{
    int rightByte = 0x00FF & opcode;
    if (rightByte == 0x9E) {
        op_EX9E(opcode);
    } else if (rightByte == 0xA1) {
        op_EXA1(opcode);
    }
}

void C8CPU::decodeOPF(word_t opcode)
{
    decreaseTimers();
    int rightByte = 0x00FF & opcode;
    switch(rightByte) {
        case 0x07:
            op_FX07(opcode); break;
        case 0x0A:
            op_FX0A(opcode); break;
        case 0x15:
            op_FX15(opcode); break;
        case 0x18:
            op_FX18(opcode); break;
        case 0x1E:
            op_FX1E(opcode); break;
        case 0x29:
            op_FX29(opcode); break;
        case 0x33:
            op_FX33(opcode); break;
        case 0x55:
            op_FX55(opcode); break;
        case 0x65:
            op_FX65(opcode); break;
    }
}

void C8CPU::decreaseTimers() 
{
    -- delayTimer;
    -- soundTimer;
}

// 0NNN not used
void C8CPU::op_0NNN(word_t opcode)
{
    return;
}

// 00E0 CLS - clear the display
void C8CPU::op_00E0(word_t opcode)
{
    memset(pixels, 0, sizeof(pixels));
}

// 00EE RET - return from subroutine
void C8CPU::op_00EE(word_t opcode)
{
    PC = stack.back();
    stack.pop_back();
}

// JP addr - jump to location NNN
void C8CPU::op_1NNN(word_t opcode)
{
   PC = 0x0FFF & opcode;
}

// CALL addr - call subroutine at NNN
void C8CPU::op_2NNN(word_t opcode)
{
    stack.push_back(PC);
    PC = 0x0FFF & opcode;
}

// SE Vx, byte - skip next instruction if Vx == NN
void C8CPU::op_3XNN(word_t opcode)
{
    byte_t regX = (0x0F00 & opcode) >> 8;
    byte_t NN = 0x00FF & opcode;
    if (registers[regX] == NN) {
        PC += 2;
    }
}

// SNE Vx, byte - skip next instruction if Vx != NN
void C8CPU::op_4XNN(word_t opcode)
{
    byte_t regX = (0x0F00 & opcode) >> 8;
    byte_t NN = 0x00FF & opcode;
    if (registers[regX] != NN) {
        PC += 2;
    }
}

// SE Vx, Vy - skip next instruction if Vx == Vy
void C8CPU::op_5XY0(word_t opcode)
{    
    byte_t regX = (0x0F00 & opcode) >> 8;
    byte_t regY = (0x00F0 & opcode) >> 4;
    if (registers[regX] == registers[regY]) {
        PC += 2;
    }
}

// LD Vx, byte - set Vx = NN
void C8CPU::op_6XNN(word_t opcode)
{
    byte_t regX = (0x0F00 & opcode) >> 8;
    byte_t NN = (0x00FF & opcode);
    registers[regX] = NN;
}

// ADD Vx, byte - set Vx = Vx + NN
void C8CPU::op_7XNN(word_t opcode)
{
    byte_t regX = (0x0F00 & opcode) >> 8;
    byte_t NN = (0x00FF & opcode);
    registers[regX] += NN;
}

// LD Vx, Vy - set Vx = Vy
void C8CPU::op_8XY0(word_t opcode)
{
    byte_t regX = (0x0F00 & opcode) >> 8;
    byte_t regY = (0x00F0 & opcode) >> 4;
    registers[regX] = registers[regY];
}

// OR Vx, Vy - set Vx = Vx OR Vy
void C8CPU::op_8XY1(word_t opcode)
{
    byte_t regX = (0x0F00 & opcode) >> 8;
    byte_t regY = (0x00F0 & opcode) >> 4;
    registers[regX] = registers[regX] | registers[regY];
}

// AND Vx, Vy - set Vx = Vx AND Vy
void C8CPU::op_8XY2(word_t opcode)
{
    byte_t regX = (0x0F00 & opcode) >> 8;
    byte_t regY = (0x00F0 & opcode) >> 4;
    registers[regX] = registers[regX] & registers[regY];
}

// XOR Vx, Vy - set Vx = Vx XOR Vy
void C8CPU::op_8XY3(word_t opcode)
{
    byte_t regX = (0x0F00 & opcode) >> 8;
    byte_t regY = (0x00F0 & opcode) >> 4;
    registers[regX] = registers[regX] ^ registers[regY];
}

// ADD Vx, Vy - set Vx = Vx + Vy, set VF = carry
void C8CPU::op_8XY4(word_t opcode)
{
    byte_t regX = (0x0F00 & opcode) >> 8;
    byte_t regY = (0x00F0 & opcode) >> 4;
    int total = static_cast<int>(registers[regX] + registers[regY]);
    if (total > 255) {
        registers[0xF] = 1;
    } else {
        registers[0xF] = 0;
    }
    // TODO: implementation defined on overflow - need to make inc func
    registers[regX] += registers[regY];
}

// SUB Vx, Vy - set Vx = Vx - Vy, set VF = NOT borrow
void C8CPU::op_8XY5(word_t opcode)
{
    byte_t regX = (0x0F00 & opcode) >> 8;
    byte_t regY = (0x00F0 & opcode) >> 4;
    if (registers[regX] > registers[regY]) {
        registers[0xF] = 1;
    } else {
        registers[0xF] = 0;
    }
    // TODO: see todo above
    registers[regX] -= registers[regY];
}

// SHR Vx {, Vy} - set Vx = Vx SHR 1
void C8CPU::op_8XY6(word_t opcode)
{
    byte_t regX = (0x0F00 & opcode) >> 8;
    registers[0xF] = 0x01 & registers[regX]; // get LSB
    registers[regX] = registers[regX] >> 1;
}

// SUBN Vx, Vy - set Vx = Vy - Vx, set VF = NOT borrow
void C8CPU::op_8XY7(word_t opcode)
{
    byte_t regX = (0x0F00 & opcode) >> 8;
    byte_t regY = (0x00F0 & opcode) >> 4;
    if (registers[regX] < registers[regY]) {
        registers[0xF] = 1;
    } else {
        registers[0xF] = 0;
    }
    registers[regX] = registers[regY] - registers[regX];
}

// SHL Vx {, Vy} - set Vx = Vx SHL 1
void C8CPU::op_8XYE(word_t opcode)
{
    byte_t regX = (0x0F00 & opcode) >> 8;
    registers[0xF] = (0x8000 & registers[regX]); // get MSB
    registers[regX] = registers[regX] << 1;
}

// SNE Vx, Vy - skip next instruction if Vx != Vy
void C8CPU::op_9XY0(word_t opcode)
{
    byte_t regX = (0x0F00 & opcode) >> 8;
    byte_t regY = (0x00F0 & opcode) >> 4;
    if (registers[regX] != registers[regY]) {
        PC += 2;
    }
}

// LD I, addr - set I = NNN
void C8CPU::op_ANNN(word_t opcode)
{
    address = 0x0FFF & opcode;
}

// JP V0, addr - jump to location NNN + V0
void C8CPU::op_BNNN(word_t opcode)
{
    PC = (0x0FFF & opcode) + registers[0x0];
}

// RND Vx, byte - set Vx = random byte AND NN
void C8CPU::op_CXNN(word_t opcode)
{
    byte_t regX = (0x0F00 & opcode) >> 8;
    byte_t NN = (0x00FF & opcode);
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<> dis(0, 255);
    int ran = dis(mt);
    registers[regX] = ran & NN;
}

// DRW Vx, Vy, nibble - display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
void C8CPU::op_DXYN(word_t opcode)
{
    byte_t VX = registers[(0x0F00 & opcode) >> 8];
    byte_t VY = registers[(0x00F0 & opcode) >> 4];
    byte_t N = (0x000F & opcode);
    registers[0xF] = 0;
    for (int j = 0; j != N; ++ j) {
        byte_t data = memory[address + j];
        int xPixel = 0;
        int xPixelInv = 7;
        for (; xPixel < 8; ++ xPixel, -- xPixelInv) {
            int mask = 1 << xPixelInv; 
            if (data & mask) {
                int color = 255;
                if (pixels[VY + j][VX + xPixel][0] == 255) {
                    color = 0;
                    registers[0xF] = 1;
                }
                pixels[VY + j][VX + xPixel][0] = color;
                pixels[VY + j][VX + xPixel][1] = color;
                pixels[VY + j][VX + xPixel][2] = color;
            }
        }
    }
}

// SKP Vx - skip next instruction if key with the value of Vx is pressed
void C8CPU::op_EX9E(word_t opcode)
{
    byte_t regX = (0x0F00 & opcode) >> 8;
    if (keyPressed[registers[regX]]) {
        PC += 2;
    }
}

// SKNP Vx - skip next instruction if key with the value of Vx is not pressed
void C8CPU::op_EXA1(word_t opcode)
{
    byte_t regX = (0x0F00 & opcode) >> 8;
    if (!keyPressed[registers[regX]]) {
        PC += 2;
    }
}

// LD Vx, DT - set Vx = delay timer value
void C8CPU::op_FX07(word_t opcode)
{
    byte_t regX = (0x0F00 & opcode) >> 8;
    registers[regX] = delayTimer;
}

// LD Vx, K - wait for a key press, store the value of the key in Vx
void C8CPU::op_FX0A(word_t opcode)
{
    // TODO
}

// LD DT, Vx - set delay timer = Vx
void C8CPU::op_FX15(word_t opcode)
{
    byte_t regX = (0x0F00 & opcode) >> 8;
    delayTimer = registers[regX];
}

// LD ST, Vx - set sound timer = Vx
void C8CPU::op_FX18(word_t opcode)
{
    byte_t regX = (0x0F00 & opcode) >> 8;
    soundTimer = registers[regX];
}

// ADD I, Vx - set I = I + Vx
void C8CPU::op_FX1E(word_t opcode)
{
    byte_t regX = (0x0F00 & opcode) >> 8;
    address += registers[regX];
}

// LD F, Vx - set I = location of sprite for digit Vx
void C8CPU::op_FX29(word_t opcode)
{
    byte_t regX = (opcode & 0x0F00) >> 8;
    address = registers[regX] * 5;
}

// LD B, Vx - store BCD representation of Vx in memory locations I, I + 1, and I + 2
void C8CPU::op_FX33(word_t opcode)
{
    byte_t regX = (0x0F00 & opcode) >> 8;
    memory[address] = (registers[regX] & 0xF00) >> 8;
    memory[address + 1] = (registers[regX] & 0x0F0) >> 4;
    memory[address + 2] = (registers[regX] & 0x00F);
}

// LD I, Vx - store registers V0 through Vx in memory starting at location I
void C8CPU::op_FX55(word_t opcode)
{
    byte_t regX = (0x0F00 & opcode) >> 8;
    int lastReg = static_cast<int>(registers[regX]);
    for (int i = 0 ; i != lastReg; ++ i) {
        memory[address + i] = registers[i];
    }
}

// LD Vx, I - read registers V0 through Vx from memory starting at location I
void C8CPU::op_FX65(word_t opcode)
{
    byte_t regX = (0x0F00 & opcode) >> 8;
    int lastReg = static_cast<int>(registers[regX]);
    for (int i = 0 ; i != lastReg; ++ i) {
        registers[i] = memory[address + i];
    }
}
