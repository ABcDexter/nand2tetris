// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/signum.asm

// Checks R0 and stores the result in R1. as per 
//  if R0 > 0 R1=0
//  else      R1=1
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.

@R0
D=M

@POSITIVE
D; JGT
@R1
M=0

@END
0; JMP

(POSITIVE)
@R1
M=1

(END)
@END
0; JMP
