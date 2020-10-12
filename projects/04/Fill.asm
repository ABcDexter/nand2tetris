// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.
// INFINITE loop
(INF)
    @SCREEN
    D=A
    @0
    M=D	// RAM[0] = SCREEN

    // look for keyboard press
    (KEYPRESS)

        @KBD
        D=M
        @BLACK
        D;JGT	// if any Keys is pressed, Jump to make the screen Black
        @WHITE
        D;JEQ	// else JUMP to make the screen white

    @KEYPRESS
    0;JMP

    //Blacken
    (BLACK)
        @1
        M=-1	// this referes to black colour(-1=11111111111111)
        @ALTER
        0;JMP

    // Whiten
    (WHITE)
     @1
     M=0	// this refers to white colour
     @ALTER
     0;JMP

    // Alternate the colour
    (ALTER)
        @1	// Check what is the reqd colour
        D=M	// RAM[1] contains either BLACK OR WHITE

        @0
        A=M	// get address for SCREEN pixel
        M=D	// fill it

        @0
        D=M+1	// INCrement to next pixel
        @KBD
        D=A-D

        @0
        M=M+1	// INCrement to next pixel
        A=M

        @ALTER
        D;JGT

@INF
0;JMP