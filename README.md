chessreduce
===========

Chess game potential solver in C

A 'potential' is a description of possible states.  
There are several kinds of potential and often we combine different types at the same time.  
Potential can also be used as a measure of knowledge.  
In Chess, we would like to model a situation and ask questions about it:

  We know that X is possible.
  We know that X is not possible.
  We know everything that can be known about X.
  We know nothing at all about X.
  Nothing about X makes sense.
  
##Potential Information Theory

Given N bits, there are 2^N possible states.  
A specific description of possibilities is called a 'potential'.  
To describe the potential of N bits, we need 2^N bits.  

Each bit in the potential describes a fact as 'known' or 'unknown'.  
The potential bit changes meaning by the context it is used,  
therefore potential acts more like numbers than units.  

##Chess Potential

There can only be one brick at one position at one time.  

The potential for one location is the possible brick types.  
For the whole board, this can be described as 64 bytes.  

    STATE		BIT
    empty		0
    pawn		1
    rook		2
    knight		3
    bishop		4
    queen		5
    king		6
    color		7

The potential for one brick is the possible locations.  
 
 64 bits, each for one location on board.
 Here we have the potential for a knight at location marked 'x'.
 
    0 0 1 0 1 0 0 0
    0 1 0 0 0 1 0 0
    0 0 0 x 0 0 0 0
    0 1 0 0 0 1 0 0
    0 0 1 0 1 0 0 0
    0 0 0 0 0 0 0 0
    0 0 0 0 0 0 0 0
    0 0 0 0 0 0 0 0
 