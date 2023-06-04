/*
Header file for an implementation of the Bus Class

Remember the Bus (internal) is a just a series of wires that transfer data between
different components for example the CPU and Memory
There are different types of buses such as external buses for devices such as 
printers 

video tutorial for reference: https://www.youtube.com/watch?v=8XmxKPJDGU0&t=1539s

*/

#ifndef BUS_H
#define BUS_H


#include <array>

/*
this is equivalent to the stdin.h in C
it defines fixed width integer types
link for reference: https://en.cppreference.com/w/cpp/header/cstdint
*/
#include <cstdint>

#include <iostream>

//this included because we need to make a cpu object as part of the bus
#include "CPU8080.h"


class Bus
{
public:
    //constructor and deconstructor
    Bus();
    ~Bus();


    //The read and write functionality of the bus
    //Inputs include the addresses you need to access and in the case of write
    //the data you are writing to (in the ram)

    //why is there a second parameter in read?
    uint8_t read(uint16_t addr );
    void write(uint16_t addr, uint8_t data);




    //Devices on the bus including the cpu and a fake RAM
    //in the future is it likely that all other devices will be included here

    CPU8080 cpu;

    /*The following is the fake RAM implemented with array of type uint8_t
    uint8_t is a signed integer type with width of exactly 8 or (, 16, 32 
    and 64 bits respectively) with no padding bits and using 2's complement 
    for negative values (provided if and only if the implementation directly 
    supports the type)
    */
    
    //including this for now but should probably make a seperate ram class
    //that would just make it more similar to a computer
    //that is 65536 bytes, 1024 is the size of a kilobyte and it has 64 of them
    std::array< uint8_t, 64 * 1024> ram;
    
    //helper for output
    void PrintRAM();

private:
    /* data */
};

#endif