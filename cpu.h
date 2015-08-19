#ifndef CPU_H_
#define CPU_H_

#include <GLUT/glut.h>
#include <vector>
#include <string>
#include "defines.h"

class C8CPU {
    public:
        C8CPU();
        void decodeOpcode();
        byte_t pixels[height][width][3] {0}; 
        bool keyPressed[16] {false};
        const char* filename;

    private:
        void Reset();
        void decreaseTimers();
        word_t fetchOpcode();

        byte_t memory[romSize] {0}; // main memory  
        byte_t registers[16] {0}; // cpu registers
        word_t address {0}; // address register
        word_t PC {0x200}; // program counter
        std::vector<word_t> stack;
        byte_t delayTimer {0};
        byte_t soundTimer {0};

        // opcode functions
        void decodeOP0(word_t opcode);       
        void decodeOP8(word_t opcode);       
        void decodeOPE(word_t opcode);       
        void decodeOPF(word_t opcode);
        void op_0NNN(word_t opcode);
        void op_00E0(word_t opcode);
        void op_00EE(word_t opcode);
        void op_1NNN(word_t opcode);
        void op_2NNN(word_t opcode);
        void op_3XNN(word_t opcode);
        void op_4XNN(word_t opcode);
        void op_5XY0(word_t opcode);
        void op_6XNN(word_t opcode);
        void op_7XNN(word_t opcode);
        void op_8XY0(word_t opcode);
        void op_8XY1(word_t opcode);
        void op_8XY2(word_t opcode);
        void op_8XY3(word_t opcode);
        void op_8XY4(word_t opcode);
        void op_8XY5(word_t opcode);
        void op_8XY6(word_t opcode);
        void op_8XY7(word_t opcode);
        void op_8XYE(word_t opcode);
        void op_9XY0(word_t opcode);
        void op_ANNN(word_t opcode);
        void op_BNNN(word_t opcode);
        void op_CXNN(word_t opcode);
        void op_DXYN(word_t opcode);
        void op_EX9E(word_t opcode);
        void op_EXA1(word_t opcode);
        void op_FX07(word_t opcode);
        void op_FX0A(word_t opcode);
        void op_FX15(word_t opcode);
        void op_FX18(word_t opcode);
        void op_FX1E(word_t opcode);
        void op_FX29(word_t opcode);
        void op_FX33(word_t opcode);
        void op_FX55(word_t opcode);
        void op_FX65(word_t opcode);
};

#endif // CPU_H_
