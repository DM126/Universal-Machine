#ifndef UM_H
#define UM_H

#include <cstdint>

static const int NUM_REGISTERS = 8; //number of registers
static const int NUM_INSTRUCTIONS = 14; //number of instructions

//Function pointer for operations
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
    
    //#1 - Array Index: dest receives the value stored at offset
    //                  in offset in the array identified by source. 
    //TODO REWRITE COMMENT
    void INDEX(int dest, int offset, int source);

    //#2 - Array Amendment: The array identified by src is amended at the offset
    //                      in register offset to store the value in register dest. 
    //TODO REWRITE COMMENT
    void AMEND(int src, int offset, int dest);
    
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

    //#8 - Allocation: A new array is created with a capacity of platters commensurate to the value 
    //                 in the register C. This new array is initialized entirely with platters
    //                 holding the value 0. A bit pattern not consisting of exclusively the 0 bit, 
    //                 and that identifies no other active allocated array, is placed in dest 
    //TODO REDO COMMENT
    void ALLOC(int dest, int src);
    
    //#9 - Abandonment: The array identified by the register C is abandoned. Future allocations may 
    //                  then reuse that identifier. //TODO REDO COMMENT
    void ABAND(int reg);
    
    //#10 - Output: outputs the value in the register argument (as a char?) Only displays values 
    //              between 0 and 255
    void OUT(int regist);
    
    //#11 - Input: The universal machine waits for input on the console. When input arrives, the 
    //             register src is loaded with the input, which must be between and including 0 and 255.
    //             If the end of input has been signaled, then the register C is endowed with a 
    //             uniform value pattern where every place is pregnant with the 1 bit. 
    //TODO REDO COMMENT
    void IN(int src);
    
    //#12 - Load Program: The array identified by the B register is duplicated and the duplicate 
    //                    shall replace the '0' array, regardless of size. The execution finger is 
    //                    placed to indicate the platter of this array that is described by the 
    //                    offset given in C, where the value 0 denotes the first platter, 1 the 
    //                    second, et cetera.
    //
    //                    The '0' array shall be the most sublime choice for loading, and shall be 
    //                    handled with the utmost velocity. 
    //TODO REDO COMMENT
    void LOAD(int src, int offset);
    
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
    
    //Collection of arrays. [0] is the zero array, the current program?
    uint32_t* platterArrays[128]; //TODO CHANGE SIZE OF ARRAY
    
    //program counter
    int pc = 0;
};

#endif
