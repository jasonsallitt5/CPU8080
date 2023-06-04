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

    //load the file into memory??
    //or maybe figure out a way to integrate this into the cpu so 
    */

    //note: you dont have to make a cpu because the bus already has one
    Bus bus;

    std::string exitFlag;


    //loop that pauses every iteration
    while(exitFlag != "exit"){
        

        //figure out what the instruction is based on the program counter
        //this could either mean read from a file, or just integrate
        //the disassembler itself into the cpu

        //execute the intruction
        //the instruction will return a success or failure
        //cpu.MOVBC();

        //what to do if instruction not successful?
        //for now just shut everything down

        //Show Current Memory and CPU
        std::cout << "Resulting Cpu and RAM after instruction run:\n\n";
        bus.cpu.PrintCPU();
        bus.PrintRAM();




        //increment the program counter??
        // the disassembler already does this so I could just read it from the file
        //or find a way to integrate it for use in the cpu which is probably better
        //maybe do both


        std::cout << "any button to increment or type exit:";
        std::cin >> exitFlag;
    }

    return 0;
}
