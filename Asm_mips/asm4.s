#Author: Cesar D. Quihuis-Romero
#Purpose: This program will be an exercise in functions utilizing structs. 
#This program will read and write the feilds of a given struct, as well as, 
#perform searches through arrays.

#turtle_init: Takes as arguments *obj and *name.
#acts as the constructor for the Turtle object. It will initialise all 
#fields of the struct, starting at position (0,0) and direction 0 (north).

#turtle_debug: Takes as argument *obj. fuction returns the fields of the struct. 
#used to verify the fields of a given instance of the struct.

#turtle_turnLeft/turtle_turnRight: Takes as arguemnt *obj. This function will turn the 
#turtle 90 degress to the left or right respectively. It will update the respective direction 
#field. Limit range to (0,3) inclusive.

#turtle_move: Takes as arguments *obj and dist. This function will move the turtle by a
#given distance. the relative position is clapped to clamp(x/y, -10, 10). Function will also
#update the odometer field. 

#turtle_searchName: takes as arguements *arr, arrLen, and *needle. This function searches
#through an array of Turtle objects and returns the index of the Turtle object that has a 
#name field that matches the needle arguement. If no match is found it will return -1.

#turtle_sortByX_indirect: takes as arguements **arr and arrLen. This function iterates throught the array of 
#turtle objects and gets a referace to the turtle's x field, compares and sorts in ascending order
#Turtles are put in order with in the array of turtle objects, the turtles objects themselves are not modified

#The struct that will be utilized or defined during the is exersice is Turtle which is defined as such:
#struct Turtle {
#  char x;
#  char y;
#  char dir;
#  char *name;
#  int odometer;
#};

.data
	turtle_label: .asciiz "Turtle "
	pos_label: .asciiz "pos "
	dir_label: .asciiz "dir "
	odom_label: .asciiz "odometer "
	north_label: .asciiz "North"
	east_label: .asciiz "East"
	south_label: .asciiz "South"
	west_label: .asciiz "West"
	
.text

.globl turtle_init
turtle_init:
	#turtle_init prologue
	addiu $sp, $sp, -24		#grow stack
	sw    $fp, 0($sp)		#push old fp on stack	
	sw    $ra, 4($sp)		#push ra on stack
	addiu $fp, $sp, 20		#update fp
	
	sb    $zero, 0($a0)		#init turtle x value to 0
	sb    $zero, 1($a0)		#init turtle y value to 0
	sb    $zero, 2($a0)		#init turtle dir value to 0
	sw    $a1,   4($a0)		#init turtle name to name in arg $a1
	sw    $zero, 8($a0)		#init turtle odom to 0
	
	#turtle_init epilogue
	lw    $ra, 4($sp)		#pop ra off stack
	lw    $fp, 0($sp)		#pop old fp off stack
	addiu $sp, $sp, 24		#shrink stack
	jr    $ra			#jump reg to ra
	
.globl turtle_debug
turtle_debug:
	#turtle_debug prologue
	addiu $sp, $sp, -24		#move sp by 24 bytes
	sw    $fp, 0($sp)		#put old fp on stack
	sw    $ra, 4($sp)		#put ra on stack
	addiu $fp, $sp, 20		#update fp
	
	lb    $t0, 0($a0)		#$t0 = char x
	lb    $t1, 1($a0)		#$t1 = char y
	lb    $t2, 2($a0)		#$t2 = char dir
	lw    $t3, 4($a0)		#$t3 = name
	lw    $t4, 8($a0)		#$t4 = odometer
	
	addiu  $sp, $sp, -16		#grow stack by 16 bytes
	#perserve s regesters
	sw   $s0, 4($sp)		#put s0 on stack
	sw   $s1, 8($sp)		#put s1 on stack
	sw   $s2, 12($sp)		#put s2 on stack
	
	addi $s0, $zero, 1		#$s0 = 1
	addi $s1, $zero, 2		#$s1 = 2
	addi $s2, $zero, 3		#$s2 = 3
	    
	addi $v0, $zero, 4		#$v0 = 4 (print string)
	la   $a0, turtle_label		#$a0 = "Turtle "
	syscall   			#write to display	
	
	addi $v0, $zero, 11      	#$v0 = 11 (print char)
	addi $a0, $zero, '"'		#$a0 = '"'
	syscall				#write to display
	
	addi $v0, $zero, 4		#$v0 = 4 (print stirng)
	add  $a0, $zero, $t3		#$a0 = $t3 = Name
	syscall				#write to display
	
	addi $v0, $zero, 11		#$v0 = 11 (print char)
	addi $a0, $zero, '"'		#$a0 = '"'
	syscall 			#write to display 
	
	addi $v0, $zero, 11		#$v0 = 11 (print char)
	addi $a0, $zero, '\n'		#$a0 = '\n'
	syscall 			#write to display
	
	addi $v0, $zero, 4		#$v0 = 4 (print string)
	la   $a0, pos_label		#$a0 = "pos "
	syscall				#write to display
	
	addi $v0, $zero, 1		#$v0 = 1 (print int)
	add  $a0, $zero, $t0		#$a0 = x position
	syscall				#write to display
	
	addi $v0, $zero, 11		#$v0 = 11 (print char)
	addi $a0, $zero, ','		#$a0 = ','
	syscall				#write to display
	
	addi $v0, $zero, 1		#$v0 = 1 (print int)
	add  $a0, $zero, $t1		#$a0 = y position
	syscall				#write to display
	
	addi $v0, $zero, 11		#$v0 = 11 (print char)
	addi $a0, $zero, '\n'		#$a0 = '\n'
	syscall				#write to display
	
	addi $v0, $zero, 4		#$v0 = 4 (print string)
	la   $a0, dir_label		#$a0 = "dir "
	syscall				#write to display
	
	beq $t2, $zero, dir_north	#if (dir == 0) print "North"
	beq $t2, $s0, dir_east		#if (dir == 1) print "East"
	beq $t2, $s1, dir_south		#if (dir == 2) print "South"
	beq $t2, $s2, dir_west		#if (dir == 3) print "West"
	
dir_north:
	
	addi $v0, $zero, 4		#$v0 = 4 (print string)
	la   $a0, north_label		#$a0 = "North"
	syscall 			#write to display
	j after_dir			#jump to after direction has been evaluated
	
dir_east:
				
	addi $v0, $zero, 4		#$v0 = 4 (print string)
	la   $a0, east_label		#$a0 = "East"
	syscall 			#write to display
	j after_dir			#jump to after direction has been evaluated

dir_south:
	
	addi $v0, $zero, 4		#$v0 = 4 (print string)
	la   $a0, south_label		#$a0 = "South"
	syscall 			#write to display
	j after_dir			#jump to after direction has been evaluated
		
dir_west:

	addi $v0, $zero, 4		#$v0 = 4 (print string)
	la   $a0, west_label		#$a0 = "West"
	syscall				#write to display
	
	#no jump will fall out if executed
after_dir:
	
	addi $v0, $zero, 11		#$v0 = 11 (print char)
	addi $a0, $zero, '\n'		#$a0 = '\n' 
	syscall				#write to display
	
	addi $v0, $zero, 4		#$v0 = 4 (print string)
	la   $a0, odom_label		#$a0 = "odometer"
	syscall				#write to display
	
	addi $v0, $zero, 1		#$v0 = 1 (print int)
	add  $a0, $zero, $t4		#$a0 = odometer value
	syscall				#write to display
	
	addi $v0, $zero, 11		#$v0 = 11 (print char)
	addi $a0, $zero, '\n'		#$a0 = '\n' 
	syscall				#write to display
	
	addi $v0, $zero, 11		#$v0 = 11 (print char)
	addi $a0, $zero, '\n'		#$a0 = '\n' 
	syscall				#write to display
	
	lw   $s0, 4($sp)		#restore $s0
	lw   $s1, 8($sp)		#restore $s1
	lw   $s2, 12($sp)		#restore $s2
	
	addiu  $sp, $sp, 16		#shrink stack
	
	#turtle_debug epilogue
	lw    $ra, 4($sp)		#pop ra off stack
	lw    $fp, 0($sp)		#pop old fp off stack
	addiu $sp, $sp, 24		#shrink stack by 24 bytes
	jr    $ra			#jump to reg ra
	
.globl turtle_move
turtle_move:
	#turtle_move prologue
	addiu $sp, $sp, -24		#move sp by 24 bytes
	sw    $fp, 0($sp)		#push old fp on stack
	sw    $ra, 4($sp)		#push ra on stack
	addiu $fp, $sp, 20		#update fp 
	
	addiu $sp, $sp, -12		#grow stack by 12 bytes

	#perserve s regesters
	sw $s0, 4($sp)			#store $s0
	sw $s1, 8($sp)			#store $s1
	
	lb $t0, 2($a0)			#$t0 = dir
	lb $s0, 0($a0)			#$s0 = x
	lb $s1, 1($a0)			#$s1 = y
	
	addi $t1, $zero, 1		#$t1 = 1
	addi $t2, $zero, 2		#$t2 = 2
	addi $t3, $zero, 3		#$t3 = 3
	
	beq  $t0, $zero, dirNorth 	#if ($t0 == 0) dir = north
	beq  $t0, $t1,   dirEast	#if ($t0 == 1) dir = east
	beq  $t0, $t2,   dirSouth	#if ($t0 == 2) dir = south
	beq  $t0, $t3,   dirWest	#if ($t0 == 3) dir = west
	
dirNorth:
#if dir = north movement affects y value
	add  $t1, $s1, $a1		#$t1 = y + dist
	addi $t3, $zero, -10		#$t3 = -10
	addi $t4, $zero, 10		#$t4 = 10
	
	slt $t2, $t1, $t3		#$t2 = 1 iff ((y + dist) < -10) else = 0 
	bne $t2,$zero,clampNegY		#if ($t2 != 0) clamp(y,-10)
	
	slt $t2, $t4, $t1		#$t2 = 1 iff ((y+dist) > 10) else = 0
	bne $t2, $zero, clampPosY	#if (10 < (y+dist)) clamp(y,10)
	
	sb $t1,1($a0)			#if -10<(y+dist)<10 write to struct
	
	j update_odom			#jump to update odometer
	
dirEast:
#if dir = east movement affects x value
	add  $t1, $s0, $a1		#$t1 = x + dist
	addi $t3, $zero, -10		#$t3 = -10
	addi $t4, $zero, 10		#$t4 = 10
	
	slt  $t2, $t1,$t3		#$t2 = 1 iff ((x + dist) < -10) else = 0
	bne  $t2, $zero, clampNegX	#if ($t2 != 0) clamp(x,-10)
				
	slt  $t2, $t4, $t1		#$t2 =  1 iff ((x + dist) > 10) else = 0
	bne  $t2, $zero, clampPosX	#if (10 < (x + dist)) clamp (x,10)
			
	sb   $t1, 0($a0)			#if -10<(x+dist)<10 write to struct
	
	j update_odom			#jump to update odometer
	
dirSouth:
#if dir = south movement affects y value
	sub  $t1, $zero,$a1		#$t1 = -dist
	add  $t1, $s1, $t1		#$t1 = y + (-dist)
	addi $t3, $zero, -10		#$t3 = -10
	addi $t4, $zero, 10		#$t4 = 10
	
	slt  $t2, $t1, $t3		#$t2 = 1 iff ((y + (-dist)) < -10) else = 0
	bne  $t2,$zero,clampNegY		#if ($t2 != 0) clamp(y,-10)
	
	slt  $t2, $t4, $t1		#$t2 = 1 iff ((y + dist) > 10) else = 0
	bne  $t2, $zero, clampPosY	#if (10 < (y + dist)) clamp (y,10)
	
	sb   $t1,1($a0)			#if -10<(y+dist)<10 write to struct
	
	j update_odom			#jump to update odometer

dirWest:
#if dir = west movement affects x value
	sub  $t1,$zero,$a1		#$t1 = -dist
	add  $t1, $s0, $t1		#$t1 = x + (-dist)
	addi $t3, $zero, -10		#$t3 = -10
	addi $t4, $zero, 10		#$t4 = 10
	
	slt  $t2, $t1,$t3		#$t2 = 1 iff ((x + (-dist)) < -10) else = 0
	bne  $t2, $zero, clampNegX	#if ($t2 != 0) clamp(x,-10)
		
	slt  $t2, $t4, $t1		#$t2 = 1 iff ((x + dist) > 10) else = 0
	bne  $t2, $zero, clampPosX	#if (10 < (x + dist)) clamp (
	
	
	sb   $t1, 0($a0)		#if -10<(x+dist)<10 write to struct
	
	j update_odom			#jump to update odometer

clampNegY:

	addi $t3,$zero,-10		#$t3 = -10
	sb   $t3,1($a0)			#update y pos to -10
	j update_odom			#jump to update odometer

clampNegX:

	addi $t3, $zero, -10		#$t3 = -10
	sb   $t3,0($a0)			#update x pos to -10
	j update_odom			#jump to update odometer
	
clampPosY: 

	addi $t3, $zero, 10		#$t3 = 10
	sb   $t3, 1($a0)		#update y pos to 10
	j update_odom			#jump to update odometer
	
clampPosX:

	addi $t3, $zero, 10		#$t3 = 10
	sb   $t3, 0($a0)		#update x pos to 10
					#fall out to update odometer
					
update_odom:	
	slt $t0, $a1, $zero		#$t0 = 1 iff (dist < 0) else  = 0
	bne $t0, $zero, absVal		#if ($t0 != 0) dist is neg calc absolute value
	
	lw  $t1, 8($a0)			#$t1 = turtle current dist
	add $t1, $t1, $a1		#$t1 = (turtle dist + dist to move)
	sw  $t1, 8($a0)			#write to struct
	j move_epilogue			#jump to epilogue

absVal:
	sub $t0, $zero, $a1		#$t0 = 0 - (-dist)
	lw  $t1, 8($a0)			#$t1 = turtle curr dist
	
	add $t0, $t0,$t1		#$t0 = (turtle dist + abs(dist to move))
	
	sw  $t0, 8($a0)			#write to struct

move_epilogue:

	lw $s0, 4($sp)			#restore $s0
	lw $s1, 8($sp)			#restore $s1
	
	addiu $sp, $sp, 12		#shrink stack

	#turtle_move epilogue
	lw    $ra, 4($sp)		#pop ra off stack
	lw    $fp, 0($sp)		#pop old fp off stack
	addiu $sp, $sp, 24		#shrink stack
	jr    $ra			#jump regester to ra
	
.globl turtle_turnLeft
turtle_turnLeft:
	#turtle_turnLeft prologue
	addiu $sp, $sp, -24		#grow stack by 24 bytes
	sw    $fp, 0($sp)		#push old fp on stack
	sw    $ra, 4($sp)		#push ra on stack
	addiu $fp, $sp, 20		#update fp 
	
	lb    $t0, 2($a0)		#$t0 = dir
	addi  $t0, $t0, -1		#$t0 += -1
	
	andi  $t0, $t0, 0x3		#$t0 = $t0 & 0b11 = [00,01,10,11]
	
	sb    $t0, 2($a0)		#write back to struct
	
	#turtle_turnLeft epilogue
	lw    $ra, 4($sp)		#pop ra off stack
	lw    $fp, 0($sp)		#pop old fp off stack
	addiu $sp, $sp, 24		#shrink stack
	jr    $ra			#jump regester to ra
	
.globl turtle_turnRight
turtle_turnRight:
	#turtle_turnRight prologue
	addiu $sp, $sp, -24		#grow stack by 24 bytes
	sw    $fp, 0($sp)		#push old fp to stack
	sw    $ra, 4($sp)		#push ra to stack
	addiu $fp, $sp, 20		#update fp
	
	lb    $t0, 2($a0)		#$t0 = dir
	addi  $t0, $t0, 1		#$t0 += 1
	
	andi  $t0, $t0, 0x3		#$t0 = $t0 & 0b11 = [00,01,10,11]
	
	sb    $t0, 2($a0)		#write back to struct
	
	#turtle_turnRight epilogue
	lw    $ra, 4($sp)		#pop ra off stack
	lw    $fp, 0($sp)		#pop old fp off stack
	addiu $sp, $sp, 24		#shrink stack
	jr    $ra			#jump regester to ra

.globl turtle_searchName
turtle_searchName:
	#turtle_searchName prologue
	addiu $sp, $sp, -24		#grow stack by 24 bytes
	sw    $fp, 0($sp)		#push old fp on stack
	sw    $ra, 4($sp)		#push ra on stack
	addiu $fp, $sp, 20		#update fp
	
	addiu $sp,$sp,-16		#grow stack by 16 bytes
	#preserve s regesters
	sw $s0, 4($sp)			#push s0 on stack
	sw $s1, 8($sp)			#push s1 on stack
	sw $s2, 12($sp)			#push s2 on stack
	
	add $s0, $zero, $a0		#$s0 = $a0 = &array<turtle>
	add $s1, $zero, $a1		#$s1 = $a1 = arrayLen
	add $s2, $zero, $a2		#$s2 = $a2 = &needle
	
	addi $t0, $zero, 0              #$t0 = 0 (iterator) 

search_loop:
	slt $t1,$t0,$s1			#$t1 = 1 iff ($t0 < $s1) else = 0
	
	beq $t1,$zero,notFound		#if ($t0 >= $s1) no hit through end of array
	
	lw   $a0, 4($s0)		#$a0 = &array.name
	add  $a1, $zero, $s2		#$a1 = needle
	
	addiu $sp,$sp,-12		#grow stack

	sw  $t0, 4($sp)			#push $t0 on stack
	sw  $t1, 8($sp)			#push $t1 on stack
	
	jal strcmp			#funct call to strcmp
	
	lw    $t0, 4($sp)		#pop $t0 off stack
	lw    $t1, 8($sp)		#pop $t1 off stack
	addiu $sp, $sp, 12		#shrink stack
	
	beq $v0,$zero, foundNeedle	#if strcmp return value == 0 positive hit
	
	addi $s0, $s0, 12		#slide window to next turtle object
	addi $t0, $t0, 1		#else i++
	j search_loop			#loop
	
notFound:
	addi $v0,$zero,-1		#if not found return -1
	j searchName_epilogue		#jump to epilogue
	
foundNeedle:
	add $v0,$zero,$t0		#if found return i
					#fallout to epilogue
searchName_epilogue:
	#restore s regesters
	lw $s0, 4($sp)			#pop $s0 off stack
	lw $s1, 8($sp)			#pop $s1 off stack
	lw $s2, 12($sp)			#pop $s2 off stack
	
	addiu $sp,$sp,16		#shrink stack

	#turtle_searchName epilogue
	lw    $ra, 4($sp)		#pop ra off stack
	lw    $fp, 0($sp)		#pop old fp off stack
	addiu $sp, $sp, 24		#shrink stack
	jr    $ra			#jump regester to ra
	
.globl turtle_sortByX_indirect
turtle_sortByX_indirect:
	#turtle_sortBy prologue
	addiu $sp, $sp, -24		#grow stack by 24 bytes
	sw    $fp, 0($sp)		#push old fp on stack
	sw    $ra, 4($sp)		#push ra on stack
	addiu $fp, $sp, 20		#update fp 
	
	#preserve s regs
	addiu $sp, $sp, -24		#grow stack by 24 bytes
	
	sw    $s0, 0($sp)		#push $s0 to stack
	sw    $s1, 4($sp)		#push $s1 to stack
	sw    $s2, 8($sp)		#push $s2 to stack
	sw    $s3, 12($sp)		#push $s3 to stack
	sw    $s4, 16($sp)		#push $s4 to stack
	sw    $s5, 20($sp)		#push $s5 to stack

	add   $s2, $zero, $a1		#$s1 = lenArray
	addi  $s3, $a1, -1		#$s2 = lenArray - 1
	
	add   $t0, $zero, $zero    	#$t0 = i = 0

first_loop:
	slt   $t1, $t0, $s2		#$t1 = 1 iff ($t0 < $s2) else = 0
	beq   $t1, $zero, end_sort	#if ($t1 == 0) i >= lenArray
	add   $s5, $zero, $a0		#$s6 = &array
	add   $t2, $zero, $zero   	#$t2 = j = 0
	
second_loop:

	slt   $t3, $t2,$s3		#$t3 = ($t2 < $s3) == (j < arryLen - 1)
	beq   $t3, $zero, end_second	#if (j >= arryLen - 1) break else loop
	
	lw    $s1, 0($s5)		#$s1 = &array[0] == &turtle_1
	lw    $s4, 4($s5)		#$s4 = &array[1] == $turtle_2
	
	lb    $t4, 0($s1)		#$t4 = &turtle_1[0]
	lb    $t5, 0($s4)		#$t5 = &turtle_2[0]
	
	slt   $t6, $t4, $t5		#$t6 = ($t4 < $t5)
	
	bne   $t6, $zero, no_swap	#if ($t6 != 0) no swap else not ascending swap req 
	
	#swap
	sw   $s1, 4($s5)		#write turtle obj one into two position in array
	sw   $s4, 0($s5)		#write turtle obj two into one position in array
	
	addi  $t2, $t2, 1		#j++
	addi  $s5, $s5, 4		#array[i + 1] (move window)
	j second_loop

no_swap:
	addi  $t2, $t2, 1		#j++
	addi  $s5, $s5, 4		#array[i + 1] 9(move window)
	j second_loop

end_second:
	addi $t0, $t0, 1		#i++
	j first_loop			
	
end_sort:
 	#restore regs
	lw $s0, 0($sp)			#pop $s0 off stack
	lw $s1, 4($sp)			#pop $s1 off stack
	lw $s2, 8($sp)			#pop $s2 off stack
	lw $s3, 12($sp)			#pop $s3 off stack
	lw $s4, 16($sp)			#pop $s4 off stack
	lw $s5, 20($sp)			#pop $s5 off stack
	
 	addiu $sp,$sp,24		#shrink stack by 24 bytes
 	
	#turtle_sortBy epilogue
	lw    $ra, 4($sp)		#pop ra off stack
	lw    $fp, 0($sp)		#pop old fp off stack
	addiu $sp, $sp, 24		#shrink stack
	jr    $ra			#jump regester to ra
