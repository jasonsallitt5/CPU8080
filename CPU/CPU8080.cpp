
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

 int CPU8080::LoadROM(){
    //loading the ROM starting at address 0 for now
    return 0;
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

//TODO: put this into a separate file

//Opcode  instruction  size   flags     function
//0x00	  NOP	       1
uint8_t CPU8080::NOP(){
    //Doesn't need to do anything expect maybe increment the program counter?
    return 0;
}

//Opcode  instruction  size   flags     function
//0x01	  LXI B, D16   3                B <- byte 3, C <-byte 2
uint8_t CPU8080::LXIBD16(){
    pc++;
    c = read(pc);
    pc++;
    b = read(pc);
    return 0;
}

//Opcode  instruction  size   flags     function
//0x02	  STAX B       1                (BC) <- A
uint8_t CPU8080::STAXB(){
    // a is stored in the memory location referred to by (BC)
    uint16_t b16 = (uint16_t) b;
    b16 = (b16 << 8);
    uint16_t c16 = (uint16_t) c;
    uint16_t addrBC = b16 + c16;

    write(addrBC, a);

    return 0;
}

//Opcode  instruction  size   flags     function
//0x03	  INX B        1                (BC) <- (BC) + 1 
uint8_t CPU8080::INXB(){

    uint16_t b16 = (uint16_t) b;
    b16 = (b16 << 8);
    uint16_t c16 = (uint16_t) c;
    uint16_t addrBC = b16 + c16;

    addrBC += 1;

	c = (uint8_t) (addrBC);
	b = (uint8_t) (addrBC >> 8);

    return 0;
}

//Opcode  instruction  size   flags     function
//0x04	  INR B        1      Z,S,P,AC  B <- B + 1 
uint8_t CPU8080::INRB(){
    b += 1;

    SetFlag(Z, !(b && 0xff));       //zero
    SetFlag(S,  (b && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)b)); //parity (num of bits is even)

    return 0;
}

//Opcode  instruction  size   flags     function
//0x05	  DCR B        1      Z,S,P,AC  B <- B - 1 
uint8_t CPU8080::DCRB(){
    b -= 1;

    SetFlag(Z, !(b && 0xff));       //zero
    SetFlag(S,  (b && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)b)); //parity (num of bits is even)

    return 0;
}

//Opcode  instruction  size   flags     function
//0x06	  MVI B,D8     2                B <- byte 2 
uint8_t CPU8080::MVIBD8(){

    pc++;
    b = read(pc);

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

//Opcode  instruction  size   flags         function
//0x91	  SUB C 	   1	  Z,S,P,CY,AC	A <- A - C
uint8_t CPU8080::SUBC(){

    uint8_t ans =  a - c;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0x92	  SUB D 	   1	  Z,S,P,CY,AC	A <- A - D
uint8_t CPU8080::SUBD(){

    uint8_t ans =  a - d;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0x93	  SUB E 	   1	  Z,S,P,CY,AC	A <- A - E
uint8_t CPU8080::SUBE(){

    uint8_t ans =  a - e;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0x94	  SUB H 	   1	  Z,S,P,CY,AC	A <- A - H
uint8_t CPU8080::SUBH(){

    uint8_t ans =  a - h;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0x95	  SUB L 	   1	  Z,S,P,CY,AC	A <- A - L
uint8_t CPU8080::SUBL(){

    uint8_t ans =  a - l;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0x96	  SUB M 	   1	  Z,S,P,CY,AC	A <- A - (HL)
uint8_t CPU8080::SUBM(){

    //cast each to a 16bit, shift h, then add them together
    uint16_t h16 = (uint16_t) h;
    h16 = (h16 << 8);
    uint16_t l16 = (uint16_t) l;
    uint16_t addrHL = h16 + l16;

    uint8_t hldata = read(addrHL);


    uint8_t ans =  a - hldata;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0x97	  SUB A 	   1	  Z,S,P,CY,AC	A <- A - A
uint8_t CPU8080::SUBA(){

    uint8_t ans =  a - a;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0x98	  SBB B 	   1	  Z,S,P,CY,AC	A <- A - B - CY
uint8_t CPU8080::SBBB(){

    uint8_t ans =  a - b - GetFlag(C);

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0x99	  SBB C 	   1	  Z,S,P,CY,AC	A <- A - C - CY
uint8_t CPU8080::SBBC(){

    uint8_t ans =  a - c - GetFlag(C);

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0x9a	  SBB D 	   1	  Z,S,P,CY,AC	A <- A - D - CY
uint8_t CPU8080::SBBD(){

    uint8_t ans =  a - d - GetFlag(C);

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0x9b	  SBB E 	   1	  Z,S,P,CY,AC	A <- A - E - CY
uint8_t CPU8080::SBBE(){

    uint8_t ans =  a - e - GetFlag(C);

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0x9c	  SBB H 	   1	  Z,S,P,CY,AC	A <- A - H - CY
uint8_t CPU8080::SBBH(){

    uint8_t ans =  a - h - GetFlag(C);

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0x9d	  SBB L 	   1	  Z,S,P,CY,AC	A <- A - L - CY
uint8_t CPU8080::SBBL(){

    uint8_t ans =  a - l - GetFlag(C);

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0x9e	  SBB M 	   1	  Z,S,P,CY,AC	A <- A - (HL) - CY
uint8_t CPU8080::SBBM(){

    //cast each to a 16bit, shift h, then add them together
    uint16_t h16 = (uint16_t) h;
    h16 = (h16 << 8);
    uint16_t l16 = (uint16_t) l;
    uint16_t addrHL = h16 + l16;

    uint8_t hldata = read(addrHL);


    uint8_t ans =  a - hldata - GetFlag(C);

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0x9f	  SBB A 	   1	  Z,S,P,CY,AC	A <- A - A - CY
uint8_t CPU8080::SBBA(){

    uint8_t ans =  a - a - GetFlag(C);

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//-----------------------------------------------------------------------------
//0xA0

//Opcode  instruction  size   flags         function
//0xa0	  ANA B 	   1	  Z,S,P,CY,AC	A <- A & B
uint8_t CPU8080::ANAB(){

    uint8_t ans =  a & b;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0xa1	  ANA C 	   1	  Z,S,P,CY,AC	A <- A & C
uint8_t CPU8080::ANAC(){

    uint8_t ans =  a & c;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0xa2	  ANA D 	   1	  Z,S,P,CY,AC	A <- A & D
uint8_t CPU8080::ANAD(){

    uint8_t ans =  a & d;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0xa3	  ANA E 	   1	  Z,S,P,CY,AC	A <- A & E
uint8_t CPU8080::ANAE(){

    uint8_t ans =  a & e;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0xa4	  ANA H 	   1	  Z,S,P,CY,AC	A <- A & H
uint8_t CPU8080::ANAH(){

    uint8_t ans =  a & h;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0xa5	  ANA L 	   1	  Z,S,P,CY,AC	A <- A & L
uint8_t CPU8080::ANAL(){

    uint8_t ans =  a & l;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0xa6	  ANA M 	   1	  Z,S,P,CY,AC	A <- A & (HL)
uint8_t CPU8080::ANAM(){

    //cast each to a 16bit, shift h, then add them together
    uint16_t h16 = (uint16_t) h;
    h16 = (h16 << 8);
    uint16_t l16 = (uint16_t) l;
    uint16_t addrHL = h16 + l16;

    uint8_t hldata = read(addrHL);

    uint8_t ans =  a & hldata;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}


//Opcode  instruction  size   flags         function
//0xa7	  ANA A 	   1	  Z,S,P,CY,AC	A <- A & A
uint8_t CPU8080::ANAA(){

    uint8_t ans =  a & a;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}


//Opcode  instruction  size   flags         function
//0xa8	  XRA B 	   1	  Z,S,P,CY,AC	A <- A ^ B
uint8_t CPU8080::XRAB(){

    uint8_t ans =  a ^ b;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}


//Opcode  instruction  size   flags         function
//0xa9	  XRA C 	   1	  Z,S,P,CY,AC	A <- A ^ C
uint8_t CPU8080::XRAC(){

    uint8_t ans =  a ^ c;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0xaa	  XRA D 	   1	  Z,S,P,CY,AC	A <- A ^ D
uint8_t CPU8080::XRAD(){

    uint8_t ans =  a ^ d;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0xab	  XRA E 	   1	  Z,S,P,CY,AC	A <- A ^ E
uint8_t CPU8080::XRAE(){

    uint8_t ans =  a ^ d;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0xac	  XRA H 	   1	  Z,S,P,CY,AC	A <- A ^ H
uint8_t CPU8080::XRAH(){

    uint8_t ans =  a ^ h;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0xad	  XRA L 	   1	  Z,S,P,CY,AC	A <- A ^ L
uint8_t CPU8080::XRAL(){

    uint8_t ans =  a ^ l;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0xae	  XRA M 	   1	  Z,S,P,CY,AC	A <- A ^ (HL)
uint8_t CPU8080::XRAM(){

    //cast each to a 16bit, shift h, then add them together
    uint16_t h16 = (uint16_t) h;
    h16 = (h16 << 8);
    uint16_t l16 = (uint16_t) l;
    uint16_t addrHL = h16 + l16;

    uint8_t hldata = read(addrHL);

    uint8_t ans =  a ^ hldata;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}


//Opcode  instruction  size   flags         function
//0xaf	  XRA A 	   1	  Z,S,P,CY,AC	A <- A ^ A
uint8_t CPU8080::XRAA(){

    uint8_t ans =  a ^ a;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//-----------------------------------------------------------------------------
//0xB0

//Opcode  instruction  size   flags         function
//0xb0	  ORA B 	   1	  Z,S,P,CY,AC	A <- A | B
uint8_t CPU8080::ORAB(){

    uint8_t ans =  a | b;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0xb1	  ORA C 	   1	  Z,S,P,CY,AC	A <- A | C
uint8_t CPU8080::ORAC(){

    uint8_t ans =  a | c;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0xb2	  ORA D 	   1	  Z,S,P,CY,AC	A <- A | D
uint8_t CPU8080::ORAD(){

    uint8_t ans =  a | d;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0xb3	  ORA E 	   1	  Z,S,P,CY,AC	A <- A | E
uint8_t CPU8080::ORAE(){

    uint8_t ans =  a | e;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0xb4	  ORA H 	   1	  Z,S,P,CY,AC	A <- A | H
uint8_t CPU8080::ORAH(){

    uint8_t ans =  a | h;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0xb5	  ORA L 	   1	  Z,S,P,CY,AC	A <- A | L
uint8_t CPU8080::ORAL(){

    uint8_t ans =  a | l;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0xb6	  ORA M 	   1	  Z,S,P,CY,AC	A <- A | (HL)
uint8_t CPU8080::ORAM(){

    //cast each to a 16bit, shift h, then add them together
    uint16_t h16 = (uint16_t) h;
    h16 = (h16 << 8);
    uint16_t l16 = (uint16_t) l;
    uint16_t addrHL = h16 + l16;

    uint8_t hldata = read(addrHL);


    uint8_t ans =  a | hldata;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0xb7	  ORA A 	   1	  Z,S,P,CY,AC	A <- A | A
uint8_t CPU8080::ORAA(){

    uint8_t ans =  a | a;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry
    
    a = (uint8_t) ans;
    return 0;
}

//Opcode  instruction  size   flags         function
//0xb8	  CMP B 	   1	  Z,S,P,CY,AC	A - B
uint8_t CPU8080::CMPB(){

    uint8_t ans = a - b;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry

    return 0;
}

//Opcode  instruction  size   flags         function
//0xb9	  CMP C 	   1	  Z,S,P,CY,AC	A - C
uint8_t CPU8080::CMPC(){

    uint8_t ans = a - c;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry

    return 0;
}

//Opcode  instruction  size   flags         function
//0xba	  CMP D 	   1	  Z,S,P,CY,AC	A - D
uint8_t CPU8080::CMPD(){

    uint8_t ans =  a - d;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry

    return 0;
}

//Opcode  instruction  size   flags         function
//0xbb	  CMP E 	   1	  Z,S,P,CY,AC	A - E
uint8_t CPU8080::CMPE(){

    uint8_t ans =  a - e;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry

    return 0;
}

//Opcode  instruction  size   flags         function
//0xbc	  CMP H 	   1	  Z,S,P,CY,AC	A - H
uint8_t CPU8080::CMPH(){

    uint8_t ans =  a - h;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry

    return 0;
}

//Opcode  instruction  size   flags         function
//0xbd	  CMP L 	   1	  Z,S,P,CY,AC	A - L
uint8_t CPU8080::CMPL(){

    uint8_t ans =  a - l;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry

    return 0;
}

//Opcode  instruction  size   flags         function
//0xbe	  CMP M 	   1	  Z,S,P,CY,AC	A - (HL)
uint8_t CPU8080::CMPM(){

    //cast each to a 16bit, shift h, then add them together
    uint16_t h16 = (uint16_t) h;
    h16 = (h16 << 8);
    uint16_t l16 = (uint16_t) l;
    uint16_t addrHL = h16 + l16;

    uint8_t hldata = read(addrHL);

    uint8_t ans =  a - hldata;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry

    return 0;
}

//Opcode  instruction  size   flags         function
//0xbf	  CMP A 	   1	  Z,S,P,CY,AC	A - A
uint8_t CPU8080::CMPA(){

    uint8_t ans =  a - a;

    SetFlag(Z, !(ans && 0xff));       //zero
    SetFlag(S,  (ans && 0x80));       //sign
    SetFlag(P, Parity((uint8_t)ans)); //parity (num of bits is even)
    SetFlag(C,  (ans > 0xff));        //carry

    return 0;
}

//-----------------------------------------------------------------------------
//0xC0
//-----------------------------------------------------------------------------
//0xD0
//-----------------------------------------------------------------------------
//0xE0
//-----------------------------------------------------------------------------
//0xF0