#ifndef UM_H
#define UM_H

#include <cstdint>

static const int NUM_REGISTERS = 8; //number of registers
static const int NUM_INSTRUCTIONS = 14; //number of instructions

class Machine;
typedef void(Machine::*operation)(int r1, int r2, int r3);

class Machine
{
public:
    
    //Default constructor that sets up the machine and begins execution.
    Machine();
    
private:

    //Member functions-----------------
    
    //main fetch-decode-execute cycle
    void run();
    
    //#0 - Conditional Move: destination register receives the value in source register unless condition
    //                       register contains 0. cond is the 3 least meaningful bits, src next least
    //                       meaningful, dest next least meaningful after that.
    void CMOV(int dest, int src, int cond);
    
    //TODO IMPLEMENT 1 AND 2!
    
    //#3 - Addition: Adds r1 to r2 and stores the value in dest.
    void ADD(int dest, int r1, int r2);

    //#4 - Multiplication: Multiplies r1 and r2 and stores the value in dest.
    void MUL(int dest, int r1, int r2);

    //5 - Division: divides r1 by r2 and stores the result in dest.
    void DIV(int dest, int r1, int r2);

    //#6 - Not-And: Each bit in dest receives the 1 bit if either register r1 or register r2 has a 
    //              0 bit in that position.  Otherwise the bit in dest receives the 0 bit. (from spec)
    void NAND(int dest, int r1, int r2);

    //#7 - Halt: stops the machine.
    void HALT();

    //TODO IMPLEMENT 8, 9, 11, 12!!!
    
    //#10 - Output: outputs the value in the register argument (as a char?) Only displays values b/w 0 and 255
    void OUT(int regist);
    
    //#13 - Orthography: Stores an immediate value into a register.
    void ORTH(int regist, uint32_t immed);
    
    //return the value from RIGHT-shifting and isolating bits (bitwise and) (shifts first, then isolates)
    //shift: number of bits to shift right
    //isolate: an int whose binary value will isolate the chosen bits using bitwise and i.e. 7 will isolate the last 3 bits.
    static uint32_t getBits(int value, int shift, int isolate);

    //debug function to print the values in the registers
    void printRegisters();
    
    
    
    //Member variables----------------
    
    //array of registers
    uint32_t registers[NUM_REGISTERS];
    
    //array of operations (only has 0-6 for now)
    operation operations[NUM_INSTRUCTIONS];
    
    //Instructions array?
    uint32_t zero_array[128]; //TODO CHANGE SIZE OF ARRAY!!!
    
    //program counter
    int pc = 0;
};

#endif
