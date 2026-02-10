#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip> // for std::hex, std::setw, std::setfill


using Byte = unsigned char;
using Word = unsigned short;
using u32 = unsigned int;



struct Memory{
    static constexpr u32 SIZE = 1024*64;
    Byte data[SIZE];

    void initialize_memory () {
        for (u32 i=0;i<SIZE;i++){
            data[i]=0;
        }
    }


    
};

struct CPU{
    Word PC;
    Word SP;
    Byte A,X,Y;

    Byte C : 1; //0
    Byte Z : 1; //1
    Byte I : 1; //2
    Byte D : 1; //3
    Byte B : 1; //5
    Byte V : 1; //6
    Byte N : 1; //7

    void reset(Memory& mem){
        PC = 0XFFFC;
        SP = 0X0100;
        C=Z=I=D=B=V=N= 0;
        A=X=Y=0;
        mem.initialize_memory();
    }
    Byte fetch(Byte& Cycle, Memory& mem){
        Byte data=mem.data[PC];
        PC++;
        Cycle--;
        return data;
    }
    void excute(Byte Cycle, Memory& mem){
        while (Cycle > 0 ){
            Byte inst= fetch(Cycle, mem);
            
            //printf("%d : %d \n",PC-1,inst);
            
        }
    }


};


// void printMemory(const Memory& mem, u32 start = 0, u32 end = Memory::SIZE) {
//     // Limit end to SIZE
//     if (end > Memory::SIZE) end = Memory::SIZE;

//     for (u32 addr = start; addr < end; addr += 16) {
//         // Print address
//         std::cout << std::hex << std::setw(4) << std::setfill('0') << addr << ": ";

//         // Print 16 bytes per line
//         for (u32 i = 0; i < 16 && (addr + i) < end; i++) {
//             std::cout << std::hex << std::setw(2) << std::setfill('0')
//                       << (int)mem.data[addr + i] << " ";
//         }

//         std::cout << std::endl;
//     }
// }


int main(){
    CPU cpu;
    Memory memory;
    cpu.reset(memory);
    // printMemory(memory);
    cpu.excute(2,memory);
    
    
    
    

    return 0;
}