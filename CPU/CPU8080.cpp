
#include "CPU8080.h"
#include "Bus.h" //this has to be included to use any bus functions


#include <iostream>



CPU8080::CPU8080(/* args */)
{
    //One lone coder implements a table to be able to look up the intructions
}

CPU8080::~CPU8080()
{
}

//-----------------------------------------------------------------------------
// Disassembling and running

 void CPU8080::Disassemble(){

    //take in the file (rom) and open it

    //run instructions in a loop
    //right now in a one by one fashion until you figure out how to end a program
    //have to determine what instruction to run 

 } 

 void CPU8080::runInstruction(){
    //chooses the correct instruction to run
 } 



//-----------------------------------------------------------------------------
//DISPLAY

void CPU8080::PrintCPU(){
    std::cout << "......PrintCPU()......................................." << std::endl; 

    //uint8_t status: S Z AC P C
    //applies mask to look at specific bits
    bool s        = (status & S)   ? 1:0;          
    bool z        = (status & Z)   ? 1:0;
    bool ac       = (status & AC)  ? 1:0;
    bool p        = (status & P)   ? 1:0;
    bool cStatus  = (status & C)  ? 1:0;

    printf("status: S:%d Z:%d AC:%d P:%d C:%d\n", s, z, ac, p, cStatus);
   
    //a,b,c,d,e,h,l
    printf("a: 0x%02X \n", a);
    
    printf("b: 0x%02X ", b);
    printf("c: 0x%02X\n", c);

    printf("d: 0x%02X ", d);
    printf("e: 0x%02X\n", e);

    printf("h: 0x%02X ", h);
    printf("l: 0x%02X\n", l);

    printf("stackp: 0x%02X\n", stackp);
    printf("pc:     0x%04X\n", pc);

    std::cout << "......................" << std::endl << std::endl; 
}


//END of Display
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//CPU to Bus read/write
uint8_t CPU8080::read(uint16_t addr){
    return bus->read(addr);
}
void CPU8080::write(uint8_t addr, uint8_t data){
    bus->write(addr, data);
}



//-----------------------------------------------------------------------------
//Helpers, getters, setters

//sets or clears a bit in the status register
//uses enumerator so you can input the letter of the status
void CPU8080::SetFlag(FLAG8080 f,  bool v){

    //sets the bit, |= is a bitwise OR
    if(v){status |= f;} // status = status | f

    //sets the specified bit to 0
    //makes everything in f(but the specified register) = 1 
    //so the f looks like: 1111 1110
    //so that bit=0 and then you AND so you always clears the orginal 
    //register specified in f but it keeps anything already set to 1
    else{ status &= ~f;} // status = status & f
}


uint8_t CPU8080::GetFlag(FLAG8080 f){

    //the result of the AND will only be above 0 if the f and status 
    //register match up therefore return 1
    return ((status & f) > 0  )  ? 1 : 0;
}


bool CPU8080::Parity(uint8_t num){

    //if num is 0
    if(!(num && 0xff)){
        return false;
    }


    int count=0;

    for(int i=0; i<8;i++){         
        if(num && 0x01){count++;}
        //shift evrything right 
        num = num >> 1;
    }
       
    
    return !(count % 2);
}



//-----------------------------------------------------------------------------
//INTRUCTIONS BY OPCODE



//Opcode  instruction  size   flags     function
//0x00	  NOP	       1
uint8_t CPU8080::NOP(){

    std::cout << "NOP()" << std::endl;

    //Doesn't need to do anything expect maybe increment the program

    return 0;
}

//-----------------------------------------------------------------------------
//0x10
//-----------------------------------------------------------------------------
//0x20
//-----------------------------------------------------------------------------
//0x30
//-----------------------------------------------------------------------------
//0x40

//Opcode  instruction  size   flags     function
//0x40	  MOV B,B	   1	  *none*	B <- B
uint8_t CPU8080::MOVBB(){
    b = b;

    //increment pc here or at another point in the program

    return 0;
}

//Opcode  instruction  size   flags     function
//0x41	  MOV B,C	   1	  *none*	B <- C
uint8_t CPU8080::MOVBC(){
    b = c;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x42	  MOV B,D	   1	  *none*	B <- D
uint8_t CPU8080::MOVBD(){
    b = d;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x43	  MOV B,E	   1	  *none*	B <- E
uint8_t CPU8080::MOVBE(){
    b = e;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x44	  MOV B,H	   1	  *none*	B <- H
uint8_t CPU8080::MOVBH(){
    b = h;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x45	  MOV B,L	   1	  *none*	B <- L
uint8_t CPU8080::MOVBL(){
    b = l;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x46	  MOV B,M	   1	  *none*	B <- (HL)
uint8_t CPU8080::MOVBM(){

    //HL points to a byte and B should be loaded with what HL points too
    
    //cast each to a 16bit and then just shift h
    uint16_t h16 = (uint16_t) h;
    h16 = (h16 << 8);
    uint16_t l16 = (uint16_t) l;
    uint16_t addrHL = h16 + l16;

    b = read(addrHL);

    return 0;
}

//Opcode  instruction  size   flags     function
//0x47	  MOV B,A	   1	  *none*	B <- A
uint8_t CPU8080::MOVBA(){
    b = a;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x48	  MOV C,B	   1	  *none*	C <- B
uint8_t CPU8080::MOVCB(){
    c = b;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x49	  MOV C,C	   1	  *none*	C <- C
uint8_t CPU8080::MOVCC(){
    c = c;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x4a	  MOV C,D	   1	  *none*	C <- D
uint8_t CPU8080::MOVCD(){
    c = d;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x4b	  MOV C,E	   1	  *none*	C <- E
uint8_t CPU8080::MOVCE(){
    c = e;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x4c	  MOV C,H	   1	  *none*	C <- H
uint8_t CPU8080::MOVCH(){
    c = h;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x4d	  MOV C,L	   1	  *none*	C <- L
uint8_t CPU8080::MOVCL(){
    c = l;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x4e	  MOV C,M	   1	  *none*	C <- (HL)
uint8_t CPU8080::MOVCM(){
    
    //HL points to a byte and B should be loaded with what HL points too
    
    //cast each to a 16bit, shift h, then add them together
    uint16_t h16 = (uint16_t) h;
    h16 = (h16 << 8);
    uint16_t l16 = (uint16_t) l;
    uint16_t addrHL = h16 + l16;

    c = read(addrHL);
    return 0;
}

//Opcode  instruction  size   flags     function
//0x4f	  MOV C,A	   1	  *none*	C <- A
uint8_t CPU8080::MOVCA(){
    c = a;
    return 0;
}


//-----------------------------------------------------------------------------
//0x50

//Opcode  instruction  size   flags     function
//0x50	  MOV D,B	   1	  *none*	D <- B
uint8_t CPU8080::MOVDB(){
    d = b;

    //increment pc here or at another point in the program

    return 0;
}

//Opcode  instruction  size   flags     function
//0x51	  MOV D,C	   1	  *none*	D <- C
uint8_t CPU8080::MOVDC(){
    d = c;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x52	  MOV D,D	   1	  *none*	D <- D
uint8_t CPU8080::MOVDD(){
    d = d;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x53	  MOV D,E	   1	  *none*	D <- E
uint8_t CPU8080::MOVDE(){
    d = e;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x54	  MOV D,H	   1	  *none*	D <- H
uint8_t CPU8080::MOVDH(){
    d = h;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x55	  MOV D,L	   1	  *none*	D <- L
uint8_t CPU8080::MOVDL(){
    d = l;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x56	  MOV D,M	   1	  *none*	D <- (HL)
uint8_t CPU8080::MOVDM(){

    //HL points to a byte and B should be loaded with what HL points too
    
    //cast each to a 16bit and then just shift h
    uint16_t h16 = (uint16_t) h;
    h16 = (h16 << 8);
    uint16_t l16 = (uint16_t) l;
    uint16_t addrHL = h16 + l16;

    d = read(addrHL);

    return 0;
}

//Opcode  instruction  size   flags     function
//0x57	  MOV D,A	   1	  *none*	D <- A
uint8_t CPU8080::MOVDA(){
    d = a;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x58	  MOV E,B	   1	  *none*	E <- B
uint8_t CPU8080::MOVEB(){
    e = b;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x59	  MOV E,C	   1	  *none*	E <- C
uint8_t CPU8080::MOVEC(){
    e = c;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x5a	  MOV E,D	   1	  *none*	E <- D
uint8_t CPU8080::MOVED(){
    e = d;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x5b	  MOV E,E	   1	  *none*	E <- E
uint8_t CPU8080::MOVEE(){
    e = e;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x5c	  MOV E,H	   1	  *none*	E <- H
uint8_t CPU8080::MOVEH(){
    e = h;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x5d	  MOV E,L	   1	  *none*	E <- L
uint8_t CPU8080::MOVEL(){
    e = l;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x5e	  MOV E,M	   1	  *none*	E <- (HL)
uint8_t CPU8080::MOVEM(){
    
    //HL points to a byte and B should be loaded with what HL points too
    
    //cast each to a 16bit, shift h, then add them together
    uint16_t h16 = (uint16_t) h;
    h16 = (h16 << 8);
    uint16_t l16 = (uint16_t) l;
    uint16_t addrHL = h16 + l16;

    e = read(addrHL);
    return 0;
}

//Opcode  instruction  size   flags     function
//0x5f	  MOV E,A	   1	  *none*	E <- A
uint8_t CPU8080::MOVEA(){
    e = a;
    return 0;
}



//-----------------------------------------------------------------------------
//0x60

//Opcode  instruction  size   flags     function
//0x60	  MOV H,B	   1	  *none*	H <- B
uint8_t CPU8080::MOVHB(){
    h = b;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x61	  MOV H,C	   1	  *none*	H <- C
uint8_t CPU8080::MOVHC(){
    h = c;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x62	  MOV H,D	   1	  *none*	H <- D
uint8_t CPU8080::MOVHD(){
    h = d;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x63	  MOV H,E	   1	  *none*	H <- E
uint8_t CPU8080::MOVHE(){
    h = e;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x64	  MOV H,H	   1	  *none*	H <- H
uint8_t CPU8080::MOVHH(){
    h = h;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x65	  MOV H,L	   1	  *none*	H <- L
uint8_t CPU8080::MOVHL(){
    h = l;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x66	  MOV H,M	   1	  *none*	H <- (HL)
uint8_t CPU8080::MOVHM(){

    //HL points to a byte and B should be loaded with what HL points too
    
    //cast each to a 16bit and then just shift h
    uint16_t h16 = (uint16_t) h;
    h16 = (h16 << 8);
    uint16_t l16 = (uint16_t) l;
    uint16_t addrHL = h16 + l16;

    h = read(addrHL);

    return 0;
}

//Opcode  instruction  size   flags     function
//0x67	  MOV H,A	   1	  *none*	H <- A
uint8_t CPU8080::MOVHA(){
    h = a;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x68	  MOV L,B	   1	  *none*	L <- B
uint8_t CPU8080::MOVLB(){
    l = b;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x69	  MOV L,C	   1	  *none*	L <- C
uint8_t CPU8080::MOVLC(){
    l = c;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x6a	  MOV L,D	   1	  *none*	L <- D
uint8_t CPU8080::MOVLD(){
    l = d;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x6b	  MOV L,E	   1	  *none*	L <- E
uint8_t CPU8080::MOVLE(){
    l = e;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x6c	  MOV L,H	   1	  *none*	L <- H
uint8_t CPU8080::MOVLH(){
    l = h;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x6d	  MOV L,L	   1	  *none*	L <- L
uint8_t CPU8080::MOVLL(){
    l = l;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x6e	  MOV L,M	   1	  *none*	L <- (HL)
uint8_t CPU8080::MOVLM(){
    
    //HL points to a byte and B should be loaded with what HL points too
    
    //cast each to a 16bit, shift h, then add them together
    uint16_t h16 = (uint16_t) h;
    h16 = (h16 << 8);
    uint16_t l16 = (uint16_t) l;
    uint16_t addrHL = h16 + l16;

    l = read(addrHL);
    return 0;
}

//Opcode  instruction  size   flags     function
//0x6f	  MOV L,A	   1	  *none*	L <- A
uint8_t CPU8080::MOVLA(){
    l = a;
    return 0;
}





//-----------------------------------------------------------------------------
//0x70

//Opcode  instruction  size   flags     function
//0x70	  MOV M,B	   1	  *none*	(HL) <- B
uint8_t CPU8080::MOVMB(){
    
    //move data speicified into the address HL points to
    uint16_t h16 = (uint16_t) h;
    h16 = (h16 << 8);
    uint16_t l16 = (uint16_t) l;
    uint16_t addrHL = h16 + l16;

    write(addrHL, b);

    return 0;
}

//Opcode  instruction  size   flags     function
//0x71	  MOV M,C	   1	  *none*	(HL) <- C
uint8_t CPU8080::MOVMC(){
    
    //move data speicified into the address HL points to
    uint16_t h16 = (uint16_t) h;
    h16 = (h16 << 8);
    uint16_t l16 = (uint16_t) l;
    uint16_t addrHL = h16 + l16;

    write(addrHL, c);

    return 0;
}

//Opcode  instruction  size   flags     function
//0x72	  MOV M,D	   1	  *none*	(HL) <- D
uint8_t CPU8080::MOVMD(){
    
    //move data speicified into the address HL points to
    uint16_t h16 = (uint16_t) h;
    h16 = (h16 << 8);
    uint16_t l16 = (uint16_t) l;
    uint16_t addrHL = h16 + l16;

    write(addrHL, d);

    return 0;
}

//Opcode  instruction  size   flags     function
//0x73	  MOV M,E	   1	  *none*	(HL) <- E
uint8_t CPU8080::MOVME(){
    
    //move data speicified into the address HL points to
    uint16_t h16 = (uint16_t) h;
    h16 = (h16 << 8);
    uint16_t l16 = (uint16_t) l;
    uint16_t addrHL = h16 + l16;

    write(addrHL, e);

    return 0;
}

//Opcode  instruction  size   flags     function
//0x74	  MOV M,H	   1	  *none*	(HL) <- H
uint8_t CPU8080::MOVMH(){
    
    //move data speicified into the address HL points to
    uint16_t h16 = (uint16_t) h;
    h16 = (h16 << 8);
    uint16_t l16 = (uint16_t) l;
    uint16_t addrHL = h16 + l16;

    write(addrHL, h);

    return 0;
}

//Opcode  instruction  size   flags     function
//0x75	  MOV M,L	   1	  *none*	(HL) <- L
uint8_t CPU8080::MOVML(){
    
    //move data speicified into the address HL points to
    uint16_t h16 = (uint16_t) h;
    h16 = (h16 << 8);
    uint16_t l16 = (uint16_t) l;
    uint16_t addrHL = h16 + l16;

    write(addrHL, l);

    return 0;
}

//Opcode  instruction  size   flags     function
//0x76	  HLT	       1	  *none*	special
uint8_t CPU8080::HLT(){
    
    //not sure how to implement hlt yet, it probably is used to effect the clock cycles

    return 0;
}

//Opcode  instruction  size   flags     function
//0x75	  MOV M,A	   1	  *none*	(HL) <- A
uint8_t CPU8080::MOVMA(){
    
    //move data speicified into the address HL points to
    uint16_t h16 = (uint16_t) h;
    h16 = (h16 << 8);
    uint16_t l16 = (uint16_t) l;
    uint16_t addrHL = h16 + l16;

    write(addrHL, a);

    return 0;
}

//Opcode  instruction  size   flags     function
//0x78	  MOV A,B	   1	  *none*	A <- B
uint8_t CPU8080::MOVAB(){
    a = b;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x79	  MOV A,C	   1	  *none*	A <- C
uint8_t CPU8080::MOVAC(){
    a = c;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x7a	  MOV A,D	   1	  *none*	A <- D
uint8_t CPU8080::MOVAD(){
    a = d;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x7b	  MOV A,E	   1	  *none*	A <- E
uint8_t CPU8080::MOVAE(){
    a = e;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x7c	  MOV A,H	   1	  *none*	A <- H
uint8_t CPU8080::MOVAH(){
    a = h;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x7d	  MOV A,L	   1	  *none*	A <- L
uint8_t CPU8080::MOVAL(){
    a = l;
    return 0;
}

//Opcode  instruction  size   flags     function
//0x7e	  MOV A,M	   1	  *none*	A <- (HL)
uint8_t CPU8080::MOVAM(){
    
    //HL points to a byte and B should be loaded with what HL points too
    
    //cast each to a 16bit, shift h, then add them together
    uint16_t h16 = (uint16_t) h;
    h16 = (h16 << 8);
    uint16_t l16 = (uint16_t) l;
    uint16_t addrHL = h16 + l16;

    a = read(addrHL);
    return 0;
}

//Opcode  instruction  size   flags     function
//0x7f	  MOV A,A	   1	  *none*	A <- A
uint8_t CPU8080::MOVAA(){
    a = a;
    return 0;
}

//-----------------------------------------------------------------------------
//0x80

//Opcode  instruction  size   flags         function
//0x80	  ADD B 	   1	  Z,S,P,CY,AC	A <- A + B
uint8_t CPU8080::ADDB(){
    
    //doing the math with a higher precision so I can capture the carry bit
    uint16_t ans = (uint16_t) a + (uint16_t) b;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0x81	  ADD C 	   1	  Z,S,P,CY,AC	A <- A + C
uint8_t CPU8080::ADDC(){
    
    //doing the math with a higher precision so I can capture the carry bit
    uint16_t ans = (uint16_t) a + (uint16_t) c;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0x82	  ADD D 	   1	  Z,S,P,CY,AC	A <- A + D
uint8_t CPU8080::ADDD(){
    
    //doing the math with a higher precision so I can capture the carry bit
    uint16_t ans = (uint16_t) a + (uint16_t) d;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0x83	  ADD E 	   1	  Z,S,P,CY,AC	A <- A + E
uint8_t CPU8080::ADDE(){
    
    //doing the math with a higher precision so I can capture the carry bit
    uint16_t ans = (uint16_t) a + (uint16_t) e;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0x84	  ADD H 	   1	  Z,S,P,CY,AC	A <- A + H
uint8_t CPU8080::ADDH(){
    
    //doing the math with a higher precision so I can capture the carry bit
    uint16_t ans = (uint16_t) a + (uint16_t) h;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0x85	  ADD L 	   1	  Z,S,P,CY,AC	A <- A + L
uint8_t CPU8080::ADDL(){
    
    //doing the math with a higher precision so I can capture the carry bit
    uint16_t ans = (uint16_t) a + (uint16_t) l;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0x86	  ADD M 	   1	  Z,S,P,CY,AC	A <- A +(HL)
uint8_t CPU8080::ADDM(){
    
    //cast each to a 16bit, shift h, then add them together
    uint16_t h16 = (uint16_t) h;
    h16 = (h16 << 8);
    uint16_t l16 = (uint16_t) l;
    uint16_t addrHL = h16 + l16;

    uint8_t hldata = read(addrHL);


    //doing the math with a higher precision so I can capture the carry bit
    uint16_t ans = (uint16_t) a + (uint16_t) hldata;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}


//Opcode  instruction  size   flags         function
//0x87	  ADD A 	   1	  Z,S,P,CY,AC	A <- A + A
uint8_t CPU8080::ADDA(){

    //doing the math with a higher precision so I can capture the carry bit
    uint16_t ans = (uint16_t) a + (uint16_t) a;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}


//Opcode  instruction  size   flags         function
//0x88	  ADC B 	   1	  Z,S,P,CY,AC	A <- A + B + CY
uint8_t CPU8080::ADCB(){

    //doing the math with a higher precision so I can capture the carry bit
    uint16_t ans = (uint16_t) a + (uint16_t) b +  GetFlag(C);

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}


//Opcode  instruction  size   flags         function
//0x89	  ADC C 	   1	  Z,S,P,CY,AC	A <- A + C + CY
uint8_t CPU8080::ADCC(){

    //doing the math with a higher precision so I can capture the carry bit
    uint16_t ans = (uint16_t) a + (uint16_t) c +  GetFlag(C);

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0x8a	  ADC D 	   1	  Z,S,P,CY,AC	A <- A + D + CY
uint8_t CPU8080::ADCD(){

    //doing the math with a higher precision so I can capture the carry bit
    uint16_t ans = (uint16_t) a + (uint16_t) d +  GetFlag(C);

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0x8b	  ADC E 	   1	  Z,S,P,CY,AC	A <- A + E + CY
uint8_t CPU8080::ADCE(){

    //doing the math with a higher precision so I can capture the carry bit
    uint16_t ans = (uint16_t) a + (uint16_t) e +  GetFlag(C);

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0x8c	  ADC H  	   1	  Z,S,P,CY,AC	A <- A + H + CY
uint8_t CPU8080::ADCH(){

    //doing the math with a higher precision so I can capture the carry bit
    uint16_t ans = (uint16_t) a + (uint16_t) h +  GetFlag(C);

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0x8d	  ADC L  	   1	  Z,S,P,CY,AC	A <- A + L + CY
uint8_t CPU8080::ADCL(){

    //doing the math with a higher precision so I can capture the carry bit
    uint16_t ans = (uint16_t) a + (uint16_t) l +  GetFlag(C);

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0x8e	  ADC M 	   1	  Z,S,P,CY,AC	A <- A +(HL) + CY
uint8_t CPU8080::ADCM(){
    
    //cast each to a 16bit, shift h, then add them together
    uint16_t h16 = (uint16_t) h;
    h16 = (h16 << 8);
    uint16_t l16 = (uint16_t) l;
    uint16_t addrHL = h16 + l16;

    uint8_t hldata = read(addrHL);


    //doing the math with a higher precision so I can capture the carry bit
    uint16_t ans = (uint16_t) a + (uint16_t) hldata + GetFlag(C);

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0x8f	  ADC A  	   1	  Z,S,P,CY,AC	A <- A + A + CY
uint8_t CPU8080::ADCA(){

    //doing the math with a higher precision so I can capture the carry bit
    uint16_t ans = (uint16_t) a + (uint16_t) a +  GetFlag(C);

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}


//-----------------------------------------------------------------------------
//0x90


//Opcode  instruction  size   flags         function
//0x90	  SUB B 	   1	  Z,S,P,CY,AC	A <- A - B
uint8_t CPU8080::SUBB(){

    uint8_t ans =  a - b;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}


//-----------------------------------------------------------------------------
//0xA0
//-----------------------------------------------------------------------------
//0xB0
//-----------------------------------------------------------------------------
//0xC0
//-----------------------------------------------------------------------------
//0xD0
//-----------------------------------------------------------------------------
//0xE0
//-----------------------------------------------------------------------------
//0xF0