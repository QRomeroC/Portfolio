#.text for code
#.data for variables
.text

.globl studentMain
studentMain:
addiu $sp, $sp, -24 # allocate stack space -- default of 24 here
sw $fp, 0($sp) # save caller’s frame pointer
sw $ra, 4($sp) # save return address
addiu $fp, $sp, 20 # setup main’s frame pointer

.data
	EQ_MSG:        	.asciiz 	"EQUALS\n"
	NOT_EQ_MSG:    	.asciiz 	"NOTHING EQUALS\n"
	ASCEND_MSG:    	.asciiz 	"ASCENDING\n"
	DECEND_MSG:    	.asciiz 	"DESCENDING\n"
	ALL_EQ_MSG:    	.asciiz 	"ALL EQUAL\n"
	UNORDERED_MSG: 	.asciiz 	"UNORDERED\n"
	REVERSED_MSG: 	.asciiz		"REVERSE\n"
	RED_MSG: 	.asciiz 	"red: "
	ORANGE_MSG: 	.asciiz 	"orange: "
	YELLOW_MSG: 	.asciiz 	"yellow: "
	GREEN_MSG: 	.asciiz 	"green: "
	BLUE_MSG: 	.asciiz 	"blue: "
	PURPLE_MSG: 	.asciiz 	"purple: "
	NEW_LINE: 	.asciiz 	"\n"

.text
	#$s0 = red
	#$s1 = orange
	#$s2 = yellow
	#$s3 = green
	#$s4 = blue
	#$s5 = purple

	la $s0, red			#load label red to reg $s0
	lw $s0, 0($s0)			#load data of label red to $s0

	la $s1, orange			#load label orange to reg $s0
	lw $s1, 0($s1) 			#load data of label orange to $s1

	la $s2, yellow 			#load label yellow to reg $s2
	lw $s2, 0($s2) 			#load data of label yellow to $s2

	la $s3, green 			#load label green to reg $s3
	lw $s3, 0($s3) 			#load data of label green to $s3

	la $s4, blue			#load label blue to reg $s4
	lw $s4, 0($s4)			#load data of label blue to $s4

	la $s5, purple			#load label purple to reg $s5
	lw $s5, 0($s5)			#load data of label purple to $s5

	la $t0, equals
	lw $t0, 0($t0)
	beq $t0, $zero, TASK_ONE_CMP

CHECK_EQS:
	beq $s0, $s1, PRINT_IF_EQ	#compare $s0 to $s1
	beq $s0, $s2, PRINT_IF_EQ	#compare $s0 to $s2
	beq $s0, $s3, PRINT_IF_EQ	#compare $s0 to $s3

	beq $s1, $s2, PRINT_IF_EQ	#compare $s1 to $s2
	beq $s1, $s3, PRINT_IF_EQ	#compare $s1 to $s3
			
	beq $s2, $s3, PRINT_IF_EQ	#compare $s2 to #s3

	j PRINT_NOT_EQ			#unconditional jump if non are equal

PRINT_IF_EQ: 
	addi $v0, $zero, 4	#$v0 = 4
	la $a0, EQ_MSG		#$a0 = "EQUALS"
	syscall
	j TASK_ONE_CMP 		#jump over print_not_eq statment
	
PRINT_NOT_EQ: 
	addi $v0, $zero, 4	#$v0 = 4
	la $a0, NOT_EQ_MSG	#$a0 = "NOTHING EQUALS"
	syscall

TASK_ONE_CMP:	#Task one complete
#TASK 2: check if items are ordered
	#$s0 = red
	#$s1 = orange
	#$s2 = yellow
	#$s3 = green
	#$s4 = blue
	#$s5 = purple
	
	la $t0, order
	lw $t0, 0($t0)
	beq $t0, $zero, TASK_TWO_CMP
	
	bne $s0, $s1, CHECK_ASCENDING	#if (red == orange) continue else go to check_ascending
	bne $s1, $s2, CHECK_ASCENDING	#if (orange == yellow) continue else go to check_ascending
	bne $s2, $s3, CHECK_ASCENDING	#if (yellow == green) continue else go to check_ascending
	bne $s3, $s4, CHECK_ASCENDING	#if (green == blue) continue else go to check_ascending
	bne $s4, $s5, CHECK_ASCENDING	#if (blue == purple) continue else go to check_ascending
	j PRINT_ALL_EQ			#if no branch occured then all must be equals

CHECK_ASCENDING:
	addi $t0, $zero, 1		#$t0 = 1
	
	slt $t1, $s5, $s4		#if (purple < blue) $t1 = 1 else $t1 = 0  
	beq $t0, $t1, CHECK_DECENDING	#if $t1 = 1 (purple>=blue) check decending
	
	slt $t1, $s4, $s3		#if (blue < green) $t1 = 1 else $t1 = 0
	beq $t0, $t1, CHECK_DECENDING	#if $t1 = 1 (blue>=green) check decending
	
	slt $t1, $s3, $s2		#if (green < yellow) $t1 = 1 else $t1 = 0
	beq $t0, $t1, CHECK_DECENDING	#if $t1 = 1 (green>=yellow) check decending
	
	slt $t1, $s2, $s1		#if (yellow < orange) $t1 = 1 else $t1 = 0
	beq $t0, $t1, CHECK_DECENDING   #if $t1 = 1 (yellow >= orange) check decending
	
	slt $t1, $s1, $s0		#if (orange < red) $t1 = 1 else $t1 = 0
	beq $t0, $t1, CHECK_DECENDING   #if $t1 = 1 (orange >= red) check decending
	
	j PRINT_ASCENDING		#if no branch then is ascending
	
CHECK_DECENDING:
	slt $t0, $s4, $s5		#if (blue < orange) $t0 = 1 else $t0 = 0
	bne $t0, $zero, PRINT_UNORDERED	#if (blue >= orange) continue else jump to unordered
		
	slt $t0, $s3, $s4		#if (green < blue) $t0 = 1 else $t0 = 0
	bne $t0, $zero, PRINT_UNORDERED	#if (green >= blue) continue else jump to unordered
	
	slt $t0, $s2, $s1		#if (yellow < green) $t0 = 1 else $t0 = 0
	bne $t0, $zero, PRINT_UNORDERED	#if (yellow >= green) continue else jump to unordered
	
	slt $t0, $s0, $s1		#if (orange < yellow) $t0 = 1 else $t0 = 0
	bne $t0, $zero, PRINT_UNORDERED	#if (orange >= yellow) continue else jump to unordered
	
	slt $t0, $s0, $s1		#if (red < yellow) $t0 = 1 else $t0 = 0
	bne $t0, $zero, PRINT_UNORDERED	#if (red >= yellow) continue else jump to unordered
	 
	j PRINT_DECENDING		#if no branch then is decending
	
PRINT_ALL_EQ:
	addi $v0, $zero, 4		#$v0 = 4
	la $a0, ALL_EQ_MSG		#$a0 = "ALL EQUAL"
	syscall
	j TASK_TWO_CMP
	
PRINT_ASCENDING:
	addi $v0, $zero, 4		#$v0 = 4
	la $a0, ASCEND_MSG		#$a0 = "ASCENDING"
	syscall
	j TASK_TWO_CMP
	
PRINT_UNORDERED:
	addi $v0, $zero, 4		#$v0 = 4
	la $a0, UNORDERED_MSG		#a0 = "UNORDERED"
	syscall
	j TASK_TWO_CMP
	
PRINT_DECENDING:
	addi $v0, $zero, 4		#$v0 = 4
	la $a0, DECEND_MSG		#a0 = "DECENDING"
	syscall
	
TASK_TWO_CMP: #Task two compleete
	#Task 3 reverse order
	#$s0 = red    reverse = $s0 = purple
	#$s1 = orange reverse = $s1 = blue
	#$s2 = yellow reverse = $s2 = green
	#$s3 = green  reverse = $s3 = yellow
	#$s4 = blue   reverse = $s4 = orange
	#$s5 = purple reverse = $s5 = red
	la $t0, reverse		#reverse flag
	lw $t0, 0($t0)
	beq $t0, $zero, TASK_THREE_CMP
	
	la $t5, purple		#$t5 = &purple
	sw $s0, 0($t5)		#&purple = red
	
	la $t4, blue		#$t4 = &blue
	sw $s1, 0($t4)		#&blue = orange
	
	la $t3, green		#$t3 = &green
	sw $s2, 0($t3)		#&green = yellow
	
	la $t2, yellow		#$t2 = &yellow
	sw $s3, 0($t2)		#&yellow = green
	
	la $t1, orange		#$t1 = &orange
	sw $s4, 0($t1)		#&orange = blue
	
	la $t0, red		#$t0 = &red
	sw $s5, 0($t0)		#&red = purple
	
	addi $v0, $zero, 4
	la $a0,REVERSED_MSG
	syscall

TASK_THREE_CMP: #task 3 complete
#task 4 print task
	la $t0, print
	lw $t0, 0($t0)
	beq $t0, $zero, TASK_FOUR_CMP
	
	la $t0, red		#$t0 = &red
	la $t1, orange		#$t1 = &orange
	la $t2, yellow		#$t2 = &yellow
	la $t3, green		#$t3 = &green
	la $t4, blue		#$t4 = &blue
	la $t5, purple		#$t5 = &purple
	
	
	addi $v0, $zero, 4	#$v0 = 4 print string
	la $a0, RED_MSG		#$a0 = "red: "
	syscall			#print call
	addi $v0, $zero, 1	#$v0 = 1 print int
	lw $a0, 0($t0)		#$a0 = red
	syscall			#print call
	addi $v0, $zero, 4	#$v0 = 4 print string
	la $a0, NEW_LINE	#$a0 = \n
	syscall			#print call
	
	addi $v0, $zero, 4	#$v0 = 4 print string
	la $a0, ORANGE_MSG	#$a0 = "orange: "
	syscall			#print call
	addi $v0, $zero, 1	#$v0 = 1 print int
	lw $a0, 0($t1)		#$a0 = orange
	syscall			#print call
	addi $v0, $zero, 4	#$v0 = 4 print string
	la $a0, NEW_LINE	#$a0 = \n
	syscall			#print call
	
	addi $v0, $zero, 4	#$v0 = 4 print string
	la $a0, YELLOW_MSG	#$a0 = "yellow: "
	syscall			#print call
	addi $v0, $zero, 1	#$v0 = 1 print int
	lw $a0, 0($t2)		#$a0 = yellow
	syscall			#print call
	addi $v0, $zero, 4	#$v0 = 4 print string
	la $a0, NEW_LINE	#$a0 = \n
	syscall			#print call
	
	addi $v0, $zero, 4	#$v0 = 4 print string
	la $a0, GREEN_MSG	#$a0 = "green: "
	syscall			#print call
	addi $v0, $zero, 1	#$v0 = 1 print int
	lw $a0, 0($t3)		#$a0 = green
	syscall			#print call
	addi $v0, $zero, 4	#$v0 = 4 print string
	la $a0, NEW_LINE	#$a0 = \n
	syscall			#print call
	
	addi $v0, $zero, 4	#$v0 = 4 print string
	la $a0, BLUE_MSG	#$a0 = "blue: "
	syscall			#print call
	addi $v0, $zero, 1	#$v0 = 1 print int
	lw $a0, 0($t4)		#$a0 = blue
	syscall			#print call
	addi $v0, $zero, 4	#$v0 = 4 print string
	la $a0, NEW_LINE	#$a0 = \n
	syscall			#print call
	
	addi $v0, $zero, 4	#$v0 = 4 print string
	la $a0, PURPLE_MSG	#$a0 = "purple: "
	syscall			#print call
	addi $v0, $zero, 1	#$v0 = 1 print int
	lw $a0, 0($t5)		#$a0 = purple
	syscall			#print call
	addi $v0, $zero, 4	#$v0 = 4 print string
	la $a0, NEW_LINE	#$a0 = \n
	syscall			#print call
	
TASK_FOUR_CMP:

lw $ra, 4($sp) # get return address from stack
lw $fp, 0($sp) # restore the caller’s frame pointer
addiu $sp, $sp, 24 # restore the caller’s stack pointer
jr $ra # return to caller’s code

