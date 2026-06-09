This directory contains asm1.s through asm5.s, these are Assembly language source code.
They were developed in a emulated MIPS environment, utilizing MARS. 

asm1.s  program works with six integer variables:
			red, 
			orange, 
			yellow, 
			green, 
			blue, 
			purple.
		after loading the six values into saved registers;
		it performs up four tasks depending on the flag variables:
			equals -> checks if any pair of values in registers [red,orange,yellow,green] are equal
			order -> checks if all values are: all equal, ascending, descending, unordered
			reverse -> reverses registers order s.t.:
					   red <- old purple
					   orange <- old blue
					   yellow <- old green
					   green <- old yellow
					   blue <- old orange
					   purple <- old red
					   
					   after reversing, the program outputs REVERSE
			print -> prints current values of the six integer variables
			
asm2.s  performs five tasks depending on flag variables:
		fib -> prints fibonacci numbers from 0 up to [fib]
		square -> draws a square of size [square_size] with boarders being from the set of chars [+,-,|], 
		          and the inside using the char stored in var [square_fill].
		runCheck -> checks if variable [intAarry] is sorted in ascending, descending, or neither.
		countWords -> scans variable [str] (a string) and counts groups of non white space or non newline characters
		revString -> reverse the characters of variable [str] in memory.
	    
asm3.s  performs five functions:
		strlen - returns the length of a given string
		gcf(a,b) - recursive Euclidean algorithm to find greatest common factor.
		bottles(count,itemName) - prints out "bottles on the wall" with $a0 being the starting count
		                    and $a1 is a pointer to the type of bottle.
		longestSorted(array,len) - finds the longest ascending sorted run in array
		rotate(count, a,b,c,d,e,f) - rotates the values [a-f] "left" by count.
									a <- b
									b <- c
									c <- d
									d <- e
									e <- f
									f <- old a					

asm4.s  program initalizes instances of a struct [Turtle] and manipulates it.
		Turtle -  a struct with members: {char x, char y, char dir, char *name, int odometer}
		turtle_init(obj, name) - initializes a Turtle object with x,y,dir,odometer = 0 and name = name.
		turtle_debug(obj) - prints the given turtles' current data.
		turtle_move(obj,dist) - moves the turtles forward bases on current direction
									0 = North
									1 = East
									2 = South
									3 = West
								moves are clamped between -10 and 10
								increments odometer.
		turtle_turnLeft(obj) - turns 90 left
		turtle_turnRight(obj) - turns 90 right
		turtle_searchName(arr,arrLen,needle) - searches an array of Turtle structs to find a turtle with the given name.
		turtle_sortByX_indirect(arr,arrLen) - sorts an array of turtle pointers by each turtle's x value, in ascending order.
		
asm5.s	program utilizes the stack to perform the following 3 functions,
		functions do not use saved registers rather it grows and shrinks stack as needed:
		countLetters(str) - counts the frequency of each alphabetic characters in a string
		subsCipher(str,map) - encodes a string using a substitution cipher
		strlen(str) - returns the length of a string
		