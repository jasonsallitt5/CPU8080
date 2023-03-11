/*
This file contain the implementation of the bus class
*/

#include "Bus.h"

Bus::Bus()
{
    //Must link the CPU to the Bus
    cpu.ConnectBus(this);

    //clear the RAM contents to make sure it doesn't have uninitialized values
    for(auto &it : ram){
        it = 0x00;
    }
}

Bus::~Bus()
{
}


//write
void Bus::write(uint16_t addr, uint8_t data){ 
    //check so you don't seg fault
    if(addr >= 0x0000 && addr <= 0xFFFF){
        ram[addr] = data;
    }
}


//read, might include second paramter
uint8_t Bus::read(uint16_t addr){
    
    //check so you don't seg fault
    if(addr >= 0x0000 && addr <= 0xFFFF){
        return ram[addr];
    }
    return 0;
}


void Bus::PrintRAM(){

    //byte counter
    uint16_t byteCounter = 0x0000;
    for(unsigned int index=0; index < 16; index++){
   
        printf("$%04X: " , byteCounter); //print out the address label


        for(unsigned int j=0; j< 16; j++){
            printf("%02X ", ram[byteCounter + j]);     //actual contents
        }

        byteCounter += 0x0010;
        printf("\n");
    }
 
    //possibly want to print out the location of the program
    
    std::cout << "......................................................." 
    << std::endl << std::endl;  
}