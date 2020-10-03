// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.

@product
M=0 //product=0

(LOOP)
    @R0
    D=M // load the value of R0 into data register
    @STOP
    D; JEQ // if R[0] == 0, goto end

    @product
    D=M   // load the product
    
    @R1
    D=D+M // load the value of R[1] into data register, to get product 

    @product
    M=D // save product

    @R0
    M=M-1 // R[0] -= 1
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