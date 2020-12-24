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

(LOOP)
    // no key
    @0
    D=A
    @0
    M=D // reg0 <- color0
    // key status
    @24576
    D=M
    @SCREEN
    D; JEQ

    // yes key
    @1
    D=A
    @0
    M=D // reg0 <- color1
    // key status
    @24576
    D=M
    @SCREEN
    D; JNE

    @LOOP
    0; JMP

(SCREEN)
    @99999999
    D=A
    @16384
    D=D+A
    @1
    M=D // max pos (reg1)
    
    @16384 // start screen address
    D=A
    @2
    M=D // set now screen pos

(SCREENLOOP)
    @0
    D=M // get color
    @2
    A=M // get and set pos
    M=D // update color

    D=A+1 
    @2
    M=D // update screen pos

    @1
    D=M
    @2
    D=D-M // check pos
    @SCREENLOOP
    D; JNE

    @LOOP
    0; JMP