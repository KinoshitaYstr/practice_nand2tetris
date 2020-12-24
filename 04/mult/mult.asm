// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.
// R0*R1 -> R2

    // init
    @0
    D=A // data register is 0
    @2
    M=D // reg2 <- data register(0)

(LOOP)
    // check
    @1
    M=M-1  // reg1 <- reg1-1
    D=M
    @END
    D; JLT // if data register < 0

    // add
    @0
    D=M    // data register <- reg0
    @2
    M=M+D  // reg2 <- reg2+reg0
    
    @LOOP
    0; JMP
(END)