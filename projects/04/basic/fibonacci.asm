// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/fib.asm

// Fibbonaci, n = R0,  R1, R2 store the first 2 and R3 stores the result
// (R0, R1, R2, R3 refer to RAM[0], RAM[1], RAM[2], and RAM[3] respectively.)

// Put your code here.

@R0	// load the value given by user into D
D=M

@n	// load the value into variable n
M=D

@R1	// initialize value at R1 as 1
M=0

@R2	//initialize value at R2 as 1
M=1

@i	//initialize i as 2
M=1 

(LOOP)
@i	// load the i in D
D=M
	
@n 	// load and subtract the i from n
D=D-M
	
@END	// goto end
D; JGT 
	
 // Logic begins
	
 @R2	// load the value of this
 D=M
 @temp	//temp variable stores value for R2
 M=D	

 @R1	// Load value of first reg into D
 D=M	

 @R2	// add two values (R1 and R2)
 D=D+M 
 M=D   //update the value of R2

 @R3	// store the value in D
 M=D 


 @temp	//load temp into D
 D=M	

 @R1	//store the value of temp into R1
 M=D

	
 @i	//increment i
 M=M+1

 @LOOP	//goto beginning of the loop
 0; JMP


(END)
@END
0; JMP
