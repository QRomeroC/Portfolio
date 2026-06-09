#ASM3 - Author Cesar D. Quihuis-Romero
#Purpose: exercise in functions, 5 functions will be implamented to complete 5 
#seperate tasks:

#TASK ONE: strlen() - reads a single parameter: a pointer to a string and counts the
#number of characters in the string. The address will not be read in from a label.

#TASK TWO: int gcf(int a, int b) - evaluates two integer values to determine the greatest
#common factor between them, the solution will be recursive rather then iterative.

#TASK THREE: bottles(int): writes to display a string counting the number of bottles of
#a given type that are on the wall, which will be removed, passed around, and a new count
#writen to display (as the child rhyme goes).

#TASK FOUR: int longestSorted(int &array, int len): array is a pointer to an array of integer
#values, this function will scan the array and calculate the length of the longest series of
#sorted values (ascending only). 

#TASK FIVE: int rotate(int count, int a, int b, int c, int d, int e, int f): this function
#takes in 6 integer values (a - f) and a 7th int value count and rotates the values a - f 
#in a circular manner.
.data
	bottlesStr: .asciiz " bottles of "
	onWall:     .asciiz " on the wall"
	takeOne:    .asciiz "Take one down, pass it around, "
	noMore:     .asciiz "No more bottles of "
	whitespace: .asciiz " "
.text	
.globl strlen

strlen:
	#strlen prologue
	addiu  $sp, $sp, -24      	#add 24 bytes to the stack
	sw     $fp, 0($sp)		#push frame pointer to the stack
	sw     $ra, 4($sp)       	#push return address to the stack
	addiu  $fp, $sp, 20		#update frame pointer
	
	#start of function instructions
	add $t0, $zero, $a0		#$t0 = &str
	add $t1, $zero, $zero		#$t0 = char count = 0
	
	strlen_loop:			#loop to iterate through string chars
	
	lb  $t2, 0($t0)			#load the byte at address contained in $t0
	beq $t2, $zero, end_strlen	#if (char == \0) brake
	
	#else:
	addi $t1, $t1, 1		#inc count
	addi $t0, $t0, 1		#inc address
	j strlen_loop			#loop
	
	end_strlen:			#reached when value of char in $t1 == \0
	
	add $v0, $zero, $t1		#$v0 = $t1 = char count
	
	#strlen epilogue
	lw    $ra, 4($sp)		#pop $ra off stack
	lw    $fp, 0($sp)		#pop $fp off stack
	addiu $sp, $sp, 24		#dec stack
	jr    $ra			#jump back to caller
	
.globl gcf
gcf:
	#gcf prologue
	addiu  $sp, $sp, -24      	#add 24 bytes to the stack
	sw     $fp, 0($sp)		#push frame pointer to the stack
	sw     $ra, 4($sp)       	#push return address to the stack
	addiu  $fp, $sp, 20		#update frame pointer
	
	#start of function instructions

	add $t0, $zero, $a0		#$t0 = $a0 = int a
	add $t1, $zero, $a1		#$t1 = $a1 = int b
	
	slt $t2, $t0, $t1		#if (a < b) $t2 = 1
	beq $t2, $zero, bEquals		#if (a >= b) skip swapping go to check b
	
	#if no branch swap a and b      triangle swap
	add $t3, $zero, $t0		#$t3 = a
	add $t0, $zero, $t1		#$t0 = a = b
	add $t1, $zero, $t3		#$t1 = b = a
	
bEquals:
	addi $t3, $zero, 1		#t3 = 1
	beq  $t1,$t3, bEqualsOne	#if b == 1 jump
	
	div  $t4, $t0,$t1		#$t4 = a / b
	mfhi $t5			#$t5 = move from high = a % b
	
	beq $t5,$zero,aModBIsZero	#if a % b == 0 jump	
	#else 
	add $a0,$zero,$t1		#update a to equal b
	add $a1,$zero,$t5		#update b to equal a % b
	jal gcf				#recurse with updated arguments
	j gcfEpilogue			#v0 = reutrn value jump to epilogue
	
bEqualsOne:
	addi $v0, $zero, 1		#set to return 1 if b == 1
	j gcfEpilogue			#go to epilogue

aModBIsZero:
	add $v0, $zero, $t1		# return b and fall out to epilogue
	
gcfEpilogue:			
	#gcf epilogue
	lw   $ra, 4($sp)		#pop $ra off stack
	lw   $fp, 0($sp)		#pop $fp off stack
	addi $sp, $sp, 24		#dec stack
	jr   $ra			#jump back to caller
				
.globl bottles
bottles:
	#bottles prologue
	addi  $sp, $sp, -24      	#add 24 bytes to the stack
	sw    $fp, 0($sp)		#push frame pointer to the stack
	sw    $ra, 4($sp)       	#push return address to the stack
	addi  $fp, $sp, 20		#update frame pointer

	add $t0, $zero, $a0      	#$t0 = $a0 == count
	add $t2, $zero, $t0		#$t2 == counting var
	
bottleLoop: 
	beq $t2, $zero, brakeBottles	#if count == 0 brake
	
	addi $v0, $zero, 1 		#print int
	add  $a0, $zero, $t2		#int to print is count
	syscall				#write to display
	
	addi $v0, $zero, 4		#print string
	la   $a0, bottlesStr		#string to print is label bottles def in data
	syscall				#write to display
	
	addi $v0, $zero, 4		#print string
	add  $a0, $zero, $a1		#string to print is from address in $t1
	syscall				#write to display
	
	addi $v0, $zero, 4		#print string
	la   $a0, onWall		#string to print is label onWall def in data
	syscall				#write to display
	
	addi $v0, $zero, 11		#print char
	addi $a0, $zero, ','            #char to print is "," 
	syscall				#write to display 
	
	addi $v0, $zero, 4		#print string
	la   $a0, whitespace		#string pting is lable whitespace def in data
	syscall				#write to display
	
	addi $v0, $zero, 1		#print int
	add  $a0, $zero, $t2		#int to print is count
	syscall				#write to display
		
	addi $v0, $zero, 4		#print string
	la   $a0, bottlesStr		#string to print is label bottles def in data
	syscall				#write to dislay
	
	addi $v0, $zero, 4		#print string
	add  $a0, $zero, $a1		#string to print is from address in $t1
	syscall				#write to dislay
	
	addi $v0, $zero, 11		#print char
	addi $a0, $zero, '!'		#char to print "!"
	syscall				#write to display
	
	addi $v0, $zero, 11		#print char
	addi $a0, $zero, '\n'		#char to print "\n"
	syscall				#write to display
	
	addi $v0, $zero, 4		#print string
	la   $a0, takeOne		#string to print is label takeOne def in data
	syscall				#write to display
	
	addi $t2, $t2, -1		#count--
	
	addi $v0, $zero, 1		#print int
	add  $a0, $zero, $t2		#int to print is count
	syscall				#write to display
		
	addi $v0, $zero, 4		#print string
	la   $a0, bottlesStr		#string to print is label bottles def in data
	syscall				#write to display
	
	addi $v0, $zero, 4		#print string
	add  $a0, $zero, $a1		#string to print is from address in $t1
	syscall				#write to display
	
	addi $v0, $zero, 4		#print string
	la   $a0, onWall		#string to print is label onWall def in data
	syscall				#write to display
	
	addi $v0, $zero, 11		#print char
	addi $a0, $zero, '.'		#char to print "."
	syscall				#write to display
	
	addi $v0, $zero, 11		#print char
	addi $a0, $zero, '\n'		#char to print "\n"
	syscall				#write to display
	
	addi $v0, $zero, 11		#print char
	addi $a0, $zero, '\n'		#char to print "\n"
	syscall				#write to display
	
	j bottleLoop			#loop with updated count
	
brakeBottles:				#label to jump to when loop brakes
	
	addi $v0, $zero, 4		#print string
	la   $a0, noMore		#string to print is label noMore def in data
	syscall				#write to display
	
	addi $v0, $zero, 4		#print string
	add  $a0, $zero, $a1		#string to print from address in $t1
	syscall				#write to display
	
	addi $v0, $zero, 4		#print string
	la   $a0, onWall		#string to print is label onWall def in data
	syscall				#write to display
	
	addi $v0, $zero, 11		#print char
	addi $a0, $zero, '!'		#char to print "!"
	syscall				#write to dispaly
	
	addi $v0, $zero, 11		#print char
	addi $a0, $zero, '\n'		#char to print "\n"
	syscall				#write to display
	
	addi $v0, $zero, 11		#print char
	addi $a0, $zero, '\n'		#char to print "\n"
	syscall				#write to display
	
	#bottles epilogue
	lw   $ra, 4($sp)		#pop $ra off stack
	lw   $fp, 0($sp)		#pop $fp off stack
	addi $sp, $sp, 24		#dec stack
	jr   $ra			#jump back to caller

.globl longestSorted
longestSorted:

	#longestSorted prologue
	addiu  $sp, $sp, -24      	#add 24 bytes to the stack
	sw     $fp, 0($sp)		#push frame pointer to the stack
	sw     $ra, 4($sp)       	#push return address to the stack
	addiu  $fp, $sp, 20		#update frame pointer
	
	add   $t0, $zero, $a1  	        #$t0 = len
	addi  $t1, $zero, 1		#$t1 = 1
	
	beq   $t0, $zero,lenZero	#if (len == 0) $v0 = 0
	beq   $t0, $t1,lenOne		#if (len == 1) $v0 = 1
	
	#if array.size >= 2 
	add   $t1, $zero, $a0		#$t1 = &$a0
	addi  $t1, $t1, 4		#$t1 = base address plus 4 (array[1])

	addi  $t3, $zero, 1		#count, starting at 1 since we start at ele 1
	addi  $t4, $zero, 1		#cuurent run starting value 1
	add   $v0, $zero, $zero		#longest 
	
longestLoop:
	beq  $t3, $t0, checkReturn	#if $t3 (count) = array len brake
	lw   $t2, 0($t1)		#$t2 = i  = array[1]
	lw   $t5, -4($t1)		#$t5 = i - 1 = array[0]
	
	sub  $t6, $t2, $t5		#$t6 = $t2(array[i]) - $t5 (array[i-1])
	beq  $t6, $zero,isEqual		#if ($t6 == 0) values are equal
	
	slt  $t7, $t6, $zero		#check if array[i-1] - array[i] < 0 
	bne  $t7, $zero,longestElse 	#if array[i - 1] > array[i] go to else label
	
	addi $t4, $t4, 1		#update curr streak
	addi $t3, $t3, 1		#update iterator
	addi $t1, $t1, 4		#update address by 4 (move i to i + 1)
	j longestLoop			#loop till iterator equals length

isEqual:
	addi $v0, $v0, 1		#update longest
	addi $t4, $t4, 1		#update streak
	addi $t3, $t3, 1		#update iterator
	addi $t1, $t1, 4		#move referance address
	j longestLoop
	
longestElse: 
#longest else would only be stepped into if array[i-1] > array[i] in which case
#we check if curr streak is greater then longest, update longest if needed, reset
#streak, inc iterator, and move address by 4

	slt  $t6, $v0, $t4		#if (longest ($v0) < currStreak ($t4)) $t6 = 1
	bne  $t6, $zero, updateLongest  #if $t6 != 0 then currStreak is larger then longest
	addi $t4, $zero, 1		#else keep $v0 same and reset count
	addi $t3, $t3, 1		#inc iterator
	addi $t1, $t1, 4		#move i
	j longestLoop			#go back to loop
	
updateLongest:
#update longest would only be stepped into if currStreak ($t4) > longest ($v0)
#if so update $v0 to equal $t4 and then reset $t4, inc iterator, move i

	add  $v0, $zero, $t4   		#make longest ($v0) = currStreak ($t4)
	addi $t4, $zero, 1		#reset currStreak($t4) to 1
	addi $t1, $t1, 4		#move address
	addi $t3, $t3, 1		#inc iterator
	j longestLoop			#go back to loop

lenZero:
	add $v0, $zero, $zero		#if len = 0 return 0
	j longestEpilogue		#go to epilogue

lenOne:
	addi $v0, $zero, 1		#if len = 1 return 1
	j longestEpilogue		#go to epilogue

checkReturn:
	slt $t6, $v0, $t4		#if longest < currStreak $t6 = 1
	bne $t6, $zero,updateReturn	#if $t6 != 0 make longest = streak
	j longestEpilogue		#go to epilogue
	
updateReturn:
	add $v0, $zero, $t4		#called if longest < then streak, make longest = streak
	
longestEpilogue:

	#longestSorted epilogue
	lw    $ra, 4($sp)		#pop $ra off stack
	lw    $fp, 0($sp)		#pop $fp off stack
	addiu $sp, $sp, 24		#dec stack
	jr    $ra			#jump back to caller

.globl rotate
rotate:
	#rotate prologue 
	addiu  $sp, $sp, -52      	#add 52 bytes to the stack
	sw     $fp, 0($sp)		#push frame pointer to the stack
	sw     $ra, 4($sp)       	#push return address to the stack
	addiu  $fp, $sp, 48		#update frame pointer
	
	sw     $s0, 8($sp)		#push $s0 to stack
	sw     $s1, 12($sp)		#push $s1 to stack
	sw     $s2, 16($sp)		#push $s2 to stack
	sw     $s3, 20($sp)		#push $s3 to stack
	sw     $s4, 24($sp)		#push $s4 to stack
	sw     $s5, 28($sp)		#push $s5 to stack
	sw     $s6, 32($sp)		#push $s6 to stack
	sw     $s7, 36($sp)		#push $s7 to stack
	
	add    $s0, $zero, $a1   	#$s0 = a
	add    $s1, $zero, $a2   	#$s1 = b
	add    $s2, $zero, $a3  	#$s2 = c
	lw     $s3, 40($sp)      	#$s3 = d
	lw     $s4, 44($sp)      	#$s4 = e
	lw     $s5, 48($sp)      	#$s5 = f
	add    $s6, $zero, $zero	#$s6 = 0 setting to 0 to clear reg of old value
	add    $s7, $zero, $zero	#$s7 = 0 setting to 0 to clear reg of old value
	
	add    $s6, $zero, $a0		#get count arguement
	add    $t0, $zero, $zero	#$t1 = 0 = iterator
	
rotateLoop:
	slt $t2, $t0, $s6		#if $t0 >= $s6 (count arg) $t2 == 1
	beq $t2, $zero, brakeRotate	#if $t1 == 1 brake
	
	add $a0, $zero, $s0		#$a0 = a
	add $a1, $zero, $s1		#$a1 = b
	add $a2, $zero, $s2		#$a2 = c
	add $a3, $zero, $s3		#$a3 = d
	
	sw $s4, -8($sp)			#push e back on stack under sp for util to use
	sw $s5, -4($sp)			#push f to stack under sp for util to use
	
	sw $t0, 40($sp)			#push $t0(iterator) on to current stack frame 
	
	jal util			#call to util
	
	lw $t0, 40($sp)			#pop $t0(iterator) off stack 
	
	add $s7, $s7, $v0		#get util return value
	
	add $t1, $zero, $s0		#temp = a
	
	add $s0, $zero, $s1		#a = b
	add $s1, $zero, $s2		#b = c
	add $s2, $zero, $s3		#c = d
	add $s3, $zero, $s4		#d = e
	add $s4, $zero, $s5		#e = f
	add $s5, $zero, $t1		#f = temp
	
	addi $t0, $t0, 1		#inc iterator
	
	j rotateLoop			#loop till iter == count argument
	
brakeRotate:
	add $v0, $zero, $s7		#get value util returns and put into $v0 for return
	#reset S register values to old values 
	lw $s0, 8($sp)			#reset $s0
	lw $s1, 12($sp)			#reset $s1
	lw $s2, 16($sp)			#reset $s2
	lw $s3, 20($sp)			#reset $s3
	lw $s4, 24($sp)			#reset $s4
	lw $s5, 28($sp)			#reset $s5
	lw $s6, 32($sp)			#reset $s6
	lw $s7, 36($sp)			#reset $s7
	
	#rotate epilogue		
	lw    $ra, 4($sp)		#pop $ra off stack
	lw    $fp, 0($sp)		#pop $fp off stack
	addiu $sp, $sp, 52		#dec stack 
	jr    $ra			#jump to caller
	
	
	
