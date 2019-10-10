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
    operations[1] = &Machine::INDEX;
    operations[2] = &Machine::AMEND;
    operations[3] = &Machine::ADD;
    operations[4] = &Machine::MUL;
    operations[5] = &Machine::DIV;
    operations[6] = &Machine::NAND;
    //ORTH is implemented separately since it uses different bits and has a different signature
    
    run();
}

void Machine::run()
{
    //TODO Should program start with ALLOC or jsut do this?
    platterArrays[0] = new uint32_t[128]; //TODO TEMPORARY, ALSO CHANGE ARRAY SIZE
    nextAddress = 1; //0 is reserved, so 1 is next?
    
    //debug commands
    platterArrays[0][0]  = 0b11010000000000000000000000000111;  //ORTH r0, 7
    platterArrays[0][1]  = 0b00001111111111111111111001000000;  //CMOV r1, r0, r0
    platterArrays[0][2]  = 0b00111111111111111111111010000001;  //ADD r2, r0, r1
    platterArrays[0][3]  = 0b01001111111111111111111011000001;  //MUL r3, r0, r1
    platterArrays[0][4]  = 0b01011111111111111111111100011000;  //DIV r4, r3, r0
    platterArrays[0][5]  = 0b01101111111111111111111101001010;  //NAND r5, r1, r2
    platterArrays[0][6]  = 0b11011100000000000000000001001011;  //ORTH r6, 75 ;k
    platterArrays[0][7]  = 0b10101111111111111111111111111110;  //OUT r6
    platterArrays[0][8]  = 0b11011100000000000000000000001010;  //ORTH r6, 10 ;newline
    platterArrays[0][9]  = 0b10101111111111111111111111111110;  //OUT r6
    platterArrays[0][10] = 0b01111111111111111111111111111111;  //HALT
    
    printRegisters(); //debug
    
    while (true) //debug change later
    {
        uint32_t instr = platterArrays[0][pc];
        
        //increment program counter to next instruction
        pc++;
        
        uint32_t opcode = getBits(instr, 28, 15); //instr>>28; //get first 4 bits of instruction to get the operation
        
        if (opcode >= 0 && opcode <= 6) //0 to 6
        {
            uint32_t r1 = getBits(instr, 6, 7); //(instr>>6)&7 //shift right 6 bits and isolate the 3 bits to the end to get the first register
            uint32_t r2 = getBits(instr, 3, 7); //((instr>>3)&7)
            uint32_t r3 = getBits(instr, 0, 7); //((instr>>0)&7)
            
            (this->*operations[opcode])(r1, r2, r3);
        }
        else
        {
            //TODO REFACTOR
            switch (opcode)
            {
                //TODO Find some way to use generic signature function pointers so all operations can be in the array?
                case 7:
                {
                    HALT();
                    break;
                }
                case 8:
                {
                    //TODO TEST
                    uint32_t dest = getBits(instr, 3, 7);
                    uint32_t src = getBits(instr, 0, 7);
                    ALLOC(dest, src);
                    break;
                }
                case 9:
                {
                    uint32_t regist = getBits(instr, 0, 7);
                    ABAND(regist);
                    break;
                }
                case 10:
                {
                    uint32_t regist = getBits(instr, 0, 7);
                    OUT(regist);
                    break;
                }
                case 11:
                {
                    //TODO INPUT
                    break;
                }
                case 12:
                {
                    //TODO LOAD
                    break;
                }
                case 13:
                {
                    uint32_t regist = getBits(instr, 25, 7); //register to store an immediate value
                    uint32_t immed = getBits(instr, 0, 67108863); //immediate value to load into register (25 bits)
                    ORTH(regist, immed);
                    break;
                }
                default:
                {
                    throw invalid_argument("Invalid opcode");
                }
            }
        }
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
void Machine::INDEX(int dest, int source, int offset)
{
    //TODO TEST
    registers[dest] = platterArrays[source][offset];
}

//2
void Machine::AMEND(int arrayAddress, int offset, int src)
{
    //TODO TEST
    platterArrays[arrayAddress][offset] = registers[src];
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
void Machine::ALLOC(int dest, int src)
{
    //TODO TEST
    int arraySize = registers[src];
    
    //get the address to use for this array.
    int arrayAddress;
    if (freedMemory.empty())
    {
        arrayAddress = nextAddress;
        nextAddress++;
    }
    else
    {
        arrayAddress = freedMemory.top();
        freedMemory.pop();
    }
    platterArrays[arrayAddress] = new uint32_t[arraySize];
}

//9
void Machine::ABAND(int reg)
{
    //TODO TEST
    int arrayAddress = registers[reg];
    freedMemory.push(arrayAddress);
    delete platterArrays[arrayAddress];
}

//10
void Machine::OUT(int regist)
{
    uint32_t output = registers[regist];
    if (output > MAX_OUTPUT)
    {
        throw invalid_argument("Char value too large in OUT.");
    }
    
    cout << (char)output;
}

//11
void Machine::IN(int src)
{
    //TODO UNIMPLEMENTED
}

//12
void Machine::LOAD(int src, int offset)
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
