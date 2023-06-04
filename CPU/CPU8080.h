#ifndef CPU8080_H
#define CPU8080_H


/*
this is equivalent to the stdint.h in C
it defines fixed width integer types
link for reference: https://en.cppreference.com/w/cpp/header/cstdint
ex: uint8_t is an unsigned int type with a width of 8 bits
*/
#include <cstdint>

//this is used for the ram which may be seperate later on; not sure tho
#include <array>



// Forward declaration of generic communications bus class to
// prevent circular inclusions
class Bus;



class CPU8080
{

public:
    CPU8080();
    ~CPU8080();


    //CPU registers can be used in register pairs
    uint8_t a = 0x00;  //flags go here
    uint8_t b = 0x00;  uint8_t c = 0x00;
    uint8_t d = 0x00;  uint8_t e = 0x00;
    uint8_t h = 0x00;  uint8_t l = 0x00;

    //index registers
    uint8_t stackp = 0x00;       // stack pointer
    uint16_t pc    = 0x0000;     // program counter note: its 16bits

    //Status register
    uint8_t status = 0x00; //will possibly set the bit in position 1 to 1 as default 
                           //which is 0x02 = 0000 0010

    //followed oneLoneCoder implementation of the status register creating
    //this enum for ease of use. These represent bitwise operations.
    //you can use this to get or set the status
    enum FLAG8080{
        /*looks like this:
        7 6 5 4 3 2 1 0
        S Z 0 A 0 P 1 C

        1 postion bit always set to 1
        3 postion bit always set to 0
        5 postion bit always set to 0
        */
        C = (1 << 0),  //Carry
        
        P = (1 << 2),  //Parity, if the numner of 1 bits in the result is even, this is 0100
        
        AC = (1 << 4), //Auxillary carry, used for binary-coded decimal also called A
                       //AC is apparently only used in 1 instruction DAA

        Z = (1 << 6),  //Zero
        S = (1 << 7)   //Sign bit
    };

    //helper to determine if the number of 1 bits is even in number
    bool Parity(uint8_t num);




    //this function needs to take in the file and load it into a 
    void Disassemble(); 

    //chooses the right instruction to run
    void runInstruction();

    // //show the next instructions to be run
    // void PrintInstructions();

    // //maybe createa a seperate one for a single instruction vs storing multiple
    // int LoadInstructions();






    void PrintCPU();

    //links to the bus which is a private data member
    void ConnectBus(Bus *n){bus=n;}



private:

    //linkage to the communications bus
    //so the cpu will read and write to the bus and then
    //the bus will interact with the ram
    Bus *bus = nullptr;
    uint8_t read(uint16_t addr);    //again why is there a second variable in read?
    void write(uint8_t addr, uint8_t data); 



    //convience functions
    uint8_t GetFlag(FLAG8080 f);
    void SetFlag(FLAG8080 f, bool v); //why is there a v in the parameters?
                                      // so you can set it to 0 or 1
                                        

    //assistive variables
    //things like opcode, cycles, and clock count


    

    //Opcodes


    //-------------------------------------------------------------------------
    //0x00
    uint8_t NOP();
    //0x08 is nothing

    //-------------------------------------------------------------------------
    //0x10
    //0x10 is nothing 
    //0x18 is nothing 

    //-------------------------------------------------------------------------
    //0x20
    //0x20 is nothing 
    //0x28 is nothing 

    //-------------------------------------------------------------------------
    //0x30
    //0x30 is nothing 
    //0x38 is nothing 

    //-------------------------------------------------------------------------
    //0x40
    uint8_t MOVBB();
    uint8_t MOVBC();
    uint8_t MOVBD();
    uint8_t MOVBE();
    uint8_t MOVBH();
    uint8_t MOVBL();
    uint8_t MOVBM();
    uint8_t MOVBA();

    uint8_t MOVCB();
    uint8_t MOVCC();
    uint8_t MOVCD();
    uint8_t MOVCE();
    uint8_t MOVCH();
    uint8_t MOVCL();
    uint8_t MOVCM();
    uint8_t MOVCA();

    //-------------------------------------------------------------------------
    //0x50
    uint8_t MOVDB();
    uint8_t MOVDC();
    uint8_t MOVDD();
    uint8_t MOVDE();
    uint8_t MOVDH();
    uint8_t MOVDL();
    uint8_t MOVDM();
    uint8_t MOVDA();

    uint8_t MOVEB();
    uint8_t MOVEC();
    uint8_t MOVED();
    uint8_t MOVEE();
    uint8_t MOVEH();
    uint8_t MOVEL();
    uint8_t MOVEM();
    uint8_t MOVEA();

    //-------------------------------------------------------------------------
    //0x60
    uint8_t MOVHB();
    uint8_t MOVHC();
    uint8_t MOVHD();
    uint8_t MOVHE();
    uint8_t MOVHH();
    uint8_t MOVHL();
    uint8_t MOVHM();
    uint8_t MOVHA();

    uint8_t MOVLB();
    uint8_t MOVLC();
    uint8_t MOVLD();
    uint8_t MOVLE();
    uint8_t MOVLH();
    uint8_t MOVLL();
    uint8_t MOVLM();
    uint8_t MOVLA();

    //-------------------------------------------------------------------------
    //0x70
    uint8_t MOVMB();
    uint8_t MOVMC();
    uint8_t MOVMD();
    uint8_t MOVME();
    uint8_t MOVMH();
    uint8_t MOVML();
    uint8_t HLT();   // not implemented
    uint8_t MOVMA();

    uint8_t MOVAB();
    uint8_t MOVAC();
    uint8_t MOVAD();
    uint8_t MOVAE();
    uint8_t MOVAH();
    uint8_t MOVAL();
    uint8_t MOVAM();
    uint8_t MOVAA();

    //-------------------------------------------------------------------------
    //0x80
    uint8_t ADDB();
    uint8_t ADDC();
    uint8_t ADDD();
    uint8_t ADDE();
    uint8_t ADDH();
    uint8_t ADDL();
    uint8_t ADDM();
    uint8_t ADDA();

    uint8_t ADCB();
    uint8_t ADCC();
    uint8_t ADCD();
    uint8_t ADCE();
    uint8_t ADCH();
    uint8_t ADCL();
    uint8_t ADCM();
    uint8_t ADCA();

    //-------------------------------------------------------------------------
    //0x90
    uint8_t SUBB();
    uint8_t SUBC();
    uint8_t SUBD();
    uint8_t SUBE();
    uint8_t SUBH();
    uint8_t SUBL();
    uint8_t SUBM();
    uint8_t SUBA();

    uint8_t SBBB();
    uint8_t SBBC();
    uint8_t SBBD();
    uint8_t SBBE();
    uint8_t SBBH();
    uint8_t SBBL();
    uint8_t SBBM();
    uint8_t SBBA();


    //-------------------------------------------------------------------------
    //0xa0

    //-------------------------------------------------------------------------
    //0xb0

    //-------------------------------------------------------------------------
    //0xc0
    //0xcb is nothing

    //-------------------------------------------------------------------------
    //0xd0
    //0xd9 is nothing
    //0xdd is nothing    uint8_t SUBB();

    //-------------------------------------------------------------------------
    //0xe0
    //0xed is nothing
    
    //-------------------------------------------------------------------------
    //0xf0
    //0xfd is nothing
};

#endif