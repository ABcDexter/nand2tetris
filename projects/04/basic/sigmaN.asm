// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/signum.asm

// Checks R0 and stores 1+2+...n into R1
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.

@R0
D=M // load the value of R0 into data register
@n
M=D //n=R0

@i
M=1 //i=1

@sum
M=0 //sum=0

(LOOP)
@i
D=M
@n
D=D-M
@STOP
D; JGT //if i>n goto STOP

@sum
D=M
@i
D=D+M
@sum
M=D // sum=sum+i

@i
M=M+1 // i=i+1
@LOOP
0; JMP

(STOP)
@sum
D=M
@R1
M=D // RAM[1]=sum
(END)
@END
0; JMP
