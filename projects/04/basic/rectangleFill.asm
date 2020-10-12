// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/rectangleFill.asm
// i.e. writes "black" in every pixel;
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.
// SCREEN is 256 x 512
// Put your code here.

@SCREEN
D=A  // access the screen

@addr
M=D  // put addr=16834

@n
M=D // n=RAM[0]

@i
M=512 // i=0

(LOOP)
    @i
    D=M

    @END
    D;JEQ // if i==0 goto END

    @addr
    A=M
    M = -1 // blacken // RAM[addr]=-1
    //@1
    //M=-1 //blacken

    @i
    M=M+1  // i= i+1
    @1
    D=A
    @addr
    M=M+D  // addr = addr+256
    @LOOP
    0;JMP  //goto LOOP
(END)
    @END
    0;JMP //end
