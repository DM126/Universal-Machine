#include <iostream>
#include "um.h"

using namespace std;

Machine::Machine()
{
    //create 8 registers for unsigned 32-bit integers all initialized to 0.
    for (int i = 0; i < NUM_REGISTERS; i++)
    {
        registers[i] = 0;
    }
    
    operations[0] = &Machine::CMOV;
    //TODO ops 1 and 2!
    operations[3] = &Machine::ADD;
    operations[4] = &Machine::MUL;
    operations[5] = &Machine::DIV;
    operations[6] = &Machine::NAND;
    //ORTH is implemented separately since it uses different bits and has a different signature
    
    run();
}

void Machine::run()
{
    //debug commands
    zero_array[0] = 0b11010000000000000000000000000111;
    zero_array[1] = 0b00001111111111111111111001000000;
    zero_array[2] = 0b00111111111111111111111010000001;
    zero_array[3] = 0b01001111111111111111111011000001;
    zero_array[4] = 0b01011111111111111111111100011000;
    zero_array[5] = 0b01101111111111111111111101001010;
    zero_array[6] = 0b11011100000000000000000001001011;
    zero_array[7] = 0b10101111111111111111111111111110;
    zero_array[8] = 0b11011100000000000000000000001010;
    zero_array[9] = 0b10101111111111111111111111111110;
    zero_array[10] = 0b01111111111111111111111111111111; //Halt
    
    printRegisters(); //debug
    
    while (true) //debug change later
    {
        uint32_t instr = zero_array[pc];
        uint32_t opcode = getBits(instr, 28, 15); //instr>>28; //get first 4 bits of instruction to get the operation
        
        if (opcode >= 0 && opcode <= 6) //0 to 6
        {
            uint32_t r1 = getBits(instr, 6, 7); //(instr>>6)&7 //shift right 6 bits and isolate the 3 bits to the end to get the first register
            uint32_t r2 = getBits(instr, 3, 7); //((instr>>3)&7)
            uint32_t r3 = getBits(instr, 0, 7); //((instr>>0)&7)
            
            (this->*operations[opcode])(r1, r2, r3);
        }
        else if (opcode == 7) //7
        {
            HALT(); 
            //TODO Find some way to use generic signature function pointers so all operations can be in the array?
        }
        else if (opcode == 10) //10
        {
            uint32_t regist = getBits(instr, 0, 7);
            OUT(regist);
        }
        else if (opcode < 13) //8, 9, 11, and 12
        {
            //TODO
        }
        else if (opcode == 13) //13
        {
            uint32_t regist = getBits(instr, 25, 7); //register to store an immediate value
            uint32_t immed = getBits(instr, 0, 67108863); //immediate value to load into register (25 bits)
            
            ORTH(regist, immed);
        }
        //TODO ELSE DEFAULT?
        
        //increment program counter to next instruction
        pc++;
    }
    
    printRegisters(); //debug
}

//0
void Machine::CMOV(int dest, int src, int cond)
{
    //todo do bounds checking and stuff???
    if (registers[cond] != 0) //only move values if last 3 bits are not zero
    {
        registers[dest] = registers[src];
    }
}

//1
void Machine::INDEX(int dest, int offset, int source)
{
    //TODO UNIMPLEMENTED
}

//2
void Machine::AMEND(int arrayAddress, int offset, int dest)
{
    //TODO UNIMPLEMENTED
}

//3
void Machine::ADD(int dest, int r1, int r2)
{
    registers[dest] = registers[r1] + registers[r2];
}

//4
void Machine::MUL(int dest, int r1, int r2)
{
    registers[dest] = registers[r1] * registers[r2];
}

//5
void Machine::DIV(int dest, int r1, int r2)
{
    registers[dest] = registers[r1] / registers[r2];
}

//6
void Machine::NAND(int dest, int r1, int r2)
{
    registers[dest] = ~(registers[r1]&registers[r2]);
}

//7
void Machine::HALT()
{
    printRegisters(); //TODO DEBUG REMOVE
    exit(1); //TODO CHANGE THIS?
}

//8
void ALLOC(int dest, int src)
{
    //TODO UNIMPLEMENTED
}

//9
void ABAND(int reg);
{
    //TODO UNIMPLEMENTED
}

//10
void Machine::OUT(int regist)
{
    cout << (char)registers[regist];
}

//11
void IN(int src)
{
    //TODO UNIMPLEMENTED
}

//12
void LOAD(int src, int offset)
{
    //TODO UNIMPLEMENTED
}

//13
void Machine::ORTH(int regist, uint32_t immed)
{
    registers[regist] = immed;
}

uint32_t Machine::getBits(int value, int shift, int isolate)
{
    return ((value>>shift)&isolate);
}

void Machine::printRegisters()
{
    for (int i = 0; i < NUM_REGISTERS; i++)
    {
        cout << "r" << i << ": " << registers[i] << endl;
    }
    cout << endl;
}
