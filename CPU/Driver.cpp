/*
Driver.cpp will be the main file that runs the Bus
The bus object contains the CPU and other objects
The bus is essentaily the "computer" and will hold everything because it 
connects all the pieces together
*/
#include "CPU8080.h"
#include "Bus.h"

#include <string.h>
#include <iostream>



int main(){

    std::cout << "\nStart of the INTEL 8080 CPU test\n" << std::endl;

    /*
    //give cpu a file to dissassmble
    string fileName;
    std::cout << "enter file name:";
    std::cin >> fileName;
    */

    //note: you dont have to make a cpu because the bus already has one
    Bus bus;

    //load the file into memory
    //bus.cpu.loadROM(fileName);


    std::string exitFlag;

    //loop that pauses every iteration
    while(exitFlag != "exit"){
        

        //figure out what the instruction is based on the program counter
        //this could either mean read from a file, or just integrate
        //the disassembler itself into the cpu

        //execute the intruction, need to make the functions public for testing in the
        // header
        //bus.cpu.NOP();


        //what to do if instruction not successful?
        //for now just shut everything down

        //Show Current Memory and CPU
        std::cout << "Resulting Cpu and RAM after instruction run:\n\n";
        bus.cpu.PrintCPU();
        bus.PrintRAM();

        // show next instruction to be executed
        // meaning you print out what the current pc decodes to
        // this mean the program counter starts elsewhere
        // bus.cpu.PrintNextInstruction

        //bus.cpu.runNextInstruction();

        std::cout << "any button to increment or type exit:";
        std::cin >> exitFlag;
    }

    return 0;
}
