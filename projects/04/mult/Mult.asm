// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.

@R0
D=M // load the value of R0 into data register
@opera1
M=D //opera1=R0, this variavle opera1 will be the number of times i iterates

@i
M=1 //i=1

@R1
D=M // load the value of R1 into data register
@opera2
M=D //opera2=R1


@product
M=0 //product=0

(LOOP)
@i
D=M
@opera1
D=D-M
@STOP
D; JGT //if i>n goto STOP

@product
D=M
@opera2
D=D+M
@product
M=D // product=product+opera2

@i
M=M+1 // i=i+1
@LOOP
0; JMP

(STOP)
@product
D=M
@R2
M=D // RAM[2]=product

(END)
@END
0; JMP