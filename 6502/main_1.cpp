#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip> // for std::hex, std::setw, std::setfill

using Byte= unsigned char;
using Word= unsigned short;    
using u32 = unsigned int;



struct Memory {
    static constexpr u32 max_mem = 1024*64;
    Byte data[max_mem];

    void initialaze(){
        for (u32 i=0;i<max_mem;i++){
            data[i]=0;
        }
    };
    //read 1 byte
    Byte operator [] (u32 Address) const  {
        return data[Address];
    }

};



struct CPU {
    
    Word PC ;       // Program Counter
    Byte SP ;       // Stack Pointer
    Byte A,X,Y ;    // Registers 
    //Byte P;         // Processor Status 
    Byte C : 1; //0
    Byte Z : 1; //1
    Byte I : 1; //2
    Byte D : 1; //3
    Byte B : 1; //5
    Byte V : 1; //6
    Byte N : 1; //7


    void Reset(Memory& mem){
        PC = 0XFFFC;
        SP = 0X00;
        C=Z=I=D=B=V=N= 0;
        A=X=Y=0;
        mem.initialaze();
    }

    Byte Fetch_byte ( u32& Cycle , Memory& mem){
        Byte data = mem.data[PC];
        PC= PC + 1;
        Cycle = Cycle - 1;
        return data;

    }


    void PUSH_byte ( u32& Cycle , Memory& mem , Byte yoo){
        mem.data[ ((0X01 << 8) | SP) ] = yoo;
        SP = SP-1;
        Cycle = Cycle - 1;

    }


    Byte Read_byte ( u32& Cycle , Memory& mem , Word yoo){


        Byte data = mem.data[yoo];
        Cycle = Cycle - 1;
        return data;

    }


    static constexpr Byte
        //LDA
        INST_LDA_IM = 0XA9,
        INST_LDA_ZP = 0XA5,
        INST_LDA_ZP_X = 0XB5,
        INST_LDA_ABS = 0XAD,
        INST_LDA_ABS_X = 0XBD,
        INST_LDA_ABS_Y = 0XB9,
        INST_LDA_INDIRECT_X = 0XA1,
        INST_LDA_INDIRECT_Y = 0XB1,
        INST_JSR = 0X20; 
        
    



    void Excute( u32 Cycle , Memory& mem){
        while (Cycle > 0){
            //search in the memory using the PC and for a instrection and  increment the pc 
            Byte inst = Fetch_byte (Cycle ,mem);
            
            
            // check what opcode is the ins
            switch (inst)
            {
            case INST_LDA_IM : {
                Byte value = Fetch_byte(Cycle , mem); 
                A = value;
                Z = (A == 0);  //if A==0 : Z=1 
                N = (A > 127); //if A<0 : N=1
            } break;

            case INST_LDA_ZP : {
                Byte Zero_Page_Address = Fetch_byte(Cycle , mem); 
                Byte value = Read_byte(Cycle , mem , Zero_Page_Address); 
                A = value;
                Z = (A == 0);
                N = (A > 127); 
            } break;

            case INST_LDA_ZP_X : {
                Byte Zero_Page_Address = Fetch_byte(Cycle , mem); 
                // i did decrement the Cycle because of that the cpu take a cycle when do the operation
                Zero_Page_Address = (Zero_Page_Address + X) & 0xFF;
                //printf("reading the address %d",Zero_Page_Address);
                Cycle = Cycle-1;
                Byte value = Read_byte(Cycle , mem , Zero_Page_Address); 
                A = value;
                Z = (A == 0);
                N = (A > 127);
            } break;

            case INST_LDA_ABS : {
                Byte LOW_Adress = Fetch_byte(Cycle , mem); 
                Byte HIGH_Adress = Fetch_byte(Cycle , mem);
                Word Adress = (HIGH_Adress << 8) | LOW_Adress;
                Byte value = Read_byte(Cycle , mem , Adress);
                A = value;
                Z = (A == 0);
                N = (A > 127);
            } break;


            case INST_LDA_ABS_X : {
                Byte LOW_Adress = Fetch_byte(Cycle , mem); 
                Byte HIGH_Adress = Fetch_byte(Cycle , mem);
                Word Adress = (HIGH_Adress << 8) | LOW_Adress;
                Adress = Adress+X;
                if ( ((Adress >> 8) & 0xFF) != HIGH_Adress ){
                    Cycle=Cycle-1;
                }
                Byte value = Read_byte(Cycle , mem , Adress);
                A = value;
                Z = (A == 0);
                N = (A > 127);
            } break;


            case INST_LDA_ABS_Y : {
                Byte LOW_Adress = Fetch_byte(Cycle , mem); 
                Byte HIGH_Adress = Fetch_byte(Cycle , mem);
                Word Adress = (HIGH_Adress << 8) | LOW_Adress;
                Adress = Adress+Y;
                if ( ((Adress >> 8) & 0xFF) != HIGH_Adress ){
                    Cycle=Cycle-1;
                }
                Byte value = Read_byte(Cycle , mem , Adress);
                A = value;
                Z = (A == 0);
                N = (A > 127);
            } break;


            case INST_LDA_INDIRECT_X : {
                Byte  yoo= Fetch_byte(Cycle , mem); //2
                Word Adress = (0X00 << 8) | yoo; //turn from X = 04 to X0004
                Word firs_address = Adress + X; //3 // add X to the adress 
                Cycle=Cycle-1;
                Byte value_1 = Read_byte(Cycle , mem , firs_address); //4
                Byte value_2 = Read_byte(Cycle , mem , firs_address + 1); //5
                Word second_address = (value_2 << 8) | value_1;
                Byte value = Read_byte(Cycle , mem , second_address); //6
                A = value;
                Z = (A == 0);
                N = (A > 127);
            } break;



            case INST_LDA_INDIRECT_Y : {
                Byte  yoo= Fetch_byte(Cycle , mem); //2
                Word Adress = (0X00 << 8) | yoo; //turn from Y = 04 to X0004
                Byte value_1 = Read_byte(Cycle , mem , Adress); //3
                Byte value_2 = Read_byte(Cycle , mem , Adress + 1); //4
                Word second_address = (value_2 << 8) | value_1;
                second_address = second_address + Y;
                if (((second_address >> 8) & 0xFF) != value_2){
                    Cycle=Cycle-1;  // 6
                }
                Byte value = Read_byte(Cycle , mem , second_address); //5
                
                A = value;
                Z = (A == 0);
                N = (A > 127);
            } break;
            case INST_JSR : {
                Byte value_1 = Fetch_byte(Cycle , mem); //2  8002
                Byte value_2 = Fetch_byte(Cycle , mem); //3  8003
                Word address_PC = PC-1;                //   8002
                Byte A1 = (address_PC >> 8) & 0xFF;   //HIGH
                Byte A2 = address_PC & 0xFF;          //LOW
                PUSH_byte(Cycle , mem , A1);            //4
                PUSH_byte(Cycle , mem , A2);            //5
                PC = (value_2 << 8) | value_1;         
                Cycle = Cycle -1;                       //6
                

            } break;


            default : {
                printf ("\ninstrection does not exist %d",inst);
                return; // used this instead of break as a chatgpt solution that actually worked change it later .. 
                
            } break;
            

            }


        }

    }

};



void printMemory(const Memory& mem, u32 start = 0, u32 end = Memory::max_mem) {
    // Limit end to SIZE
    if (end > Memory::max_mem) end = Memory::max_mem;

    for (u32 addr = start; addr < end; addr += 16) {
        // Print address
        std::cout << std::hex << std::setw(4) << std::setfill('0') << addr << ": ";

        // Print 16 bytes per line
        for (u32 i = 0; i < 16 && (addr + i) < end; i++) {
            std::cout << std::hex << std::setw(2) << std::setfill('0')
                      << (int)mem.data[addr + i] << " ";
        }

        std::cout << std::endl;
    }
}





int main() {
    CPU cpu;
    Memory memory;
    cpu.Reset(memory);
    cpu.SP = 0XFD;
    cpu.PC = 0X8000;

    printf("\nPC : %d , SP = %d , M[01FD] = %d ,M[01FE] = %d ", cpu.PC ,cpu.SP, memory.data[0X01FD] ,memory.data[0X01FC]  );
    
    // //test
    // memory.data[0X8000]=cpu.INST_JSR; // 8000
    // memory.data[0X8001]=0X21;         // 8001  
    // memory.data[0X8002]=0XF3;         // 8002
    // memory.data[0X8003]=0XEA;         // it should ignore ignore
    // //end test
    
    memory.data[0X0081]=0XF3;         // 8002

    
    cpu.Excute(6, memory);
    printMemory(memory);
    printf("\nPC : %d , SP = %d , M[01FD] = %d ,M[01FE] = %d ", cpu.PC ,cpu.SP, memory.data[0X01FD] ,memory.data[0X01FC]  );
    return 0;
}
