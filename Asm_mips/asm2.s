.data
	NEW_LINE: 	 .asciiz "\n"			 	   #new line label
	FIB_LABEL:	 .asciiz "Fibonacci Numbers:\n"  	   #fib num label
	COLON_LABEL: 	 .asciiz ": " 			       	   #colon label
	FIB_ZERO_LABEL:  .asciiz "0: 1\n"		 	   #first fib sequence
	FIB_ONE_LABEL:   .asciiz "1: 1\n"		    	   #second fib sequence
	RUN_CHECK_LABEL: .asciiz "Run Check: "		 	   #label for run check print
	ASCENDING_LABEL: .asciiz "ASCENDING\n"		 	   #label for ascending
	DECENDING_LABEL: .asciiz "DESCENDING\n"		 	   #label for descending
	NEITHER_LABEL: 	 .asciiz "NEITHER\n"		 	   #label for neither
	COUNT_LABEL: 	 .asciiz "Word Count: "		 	   #label for word count: 
	REV_LABEL: 	 .asciiz "String successfully swapped!\n"  #label for revString print			
.text

.globl studentMain
studentMain:
	addiu $sp, $sp, -24 		# allocate stack space -- default of 24 here
	sw $fp, 0($sp) 			# save caller’s frame pointer
	sw $ra, 4($sp) 			# save return address
	addiu $fp, $sp, 20 		# setup main’s frame pointer

FIB:
	#FIB prints out the FIbonacci Numbers, if fib == 0 skip else: 
	#while (n <= fib) calculate the next fib number and print
	
	la $t0, fib			#$t0 = &fib
	lw $t0,0($t0)			#$t0 = fib

	beq $t0,$zero,END_FIB		#if ($t0 == 0) jump to task two
					#else:
	addi $v0,$zero,4		#$v0 = 4 (print string)
	la   $a0,FIB_LABEL		#$ao = &fib_label
	syscall
	
	addi $v0, $zero,4 		#$v0 = 4 (print string)
	la   $a0, FIB_ZERO_LABEL	#a0 = "0: "
	syscall
	
	addi $v0, $zero,4		#$v0 = 4 (print string) 
	la   $a0, FIB_ONE_LABEL		#a0 = "1: "
	syscall
	
	addi $t1,$zero,1		#$t1 = prev = 1
	addi $t2,$zero,1		#$t2 = beforeThat = 1
	addi $t3,$zero,2		#$t3 = n = 2
	
	
FIB_LOOP:
	#FIB_LOOP loops while fib < n branch when fib == n
	slt $t6,$t0,$t3			#$t6 = (fib < n)
	bne $t6,$zero,END_FIB		#if (fib < n) jump
					#else:
	add $t4,$t1,$t2         	#$t4 = curr = prev + beforeThat
	
	addi $v0, $zero,1		#$v0 = 1 (print int)
	add  $a0,$zero,$t3		#$a0 = n
	syscall
	
	addi $v0,$zero,4		#v0 = 4 (print string)
	la   $a0,COLON_LABEL		#$a0 = ": "
	syscall
	
	addi $v0,$zero,1		#$v0 = 1 (print int)
	add  $a0,$zero,$t4		#$a0 = curr
	syscall
	
	addi $v0,$zero,4		#$v0 = 4 (print string)
	la   $a0,NEW_LINE		#a0 = "\n"
	syscall
	
	addi $t3,$t3,1          	#n++
	add  $t2,$zero,$t1       	#beforeThat = prev
	add  $t1,$zero,$t4		#prev = curr;

	j FIB_LOOP			#j to top of loop

END_FIB:
	#end of fib, next task is square, square creates a string of chars '+','-','|'
	#for (int row=0; row < square_size; row++){
	#char lr, mid;
	#if (row == 0 || row == square_size-1){
	#	lr = ’+’;
	#	mid = ’-’;
	#}else{
	#	lr = ’|’;
	#	mid = square_fill;
	#}
	#printf("%c", lr);
	#for (int i=1; i<square_size-1; i++)
	#printf("%c", mid);
	#printf("%c\n", lr);
	#}
	#printf("\n");
	# $s0 = square
	# $s1 = square_size
	# $s2 = square_fill
	# $t0 = row
	# $t1 = (row < square_fill)
	# $t2 =	lr = "+" || "|"
	# $t3 = mid = "-" || "square_fill"
	# $t4 = square_size - 1
	
	la $s0,square			#$t0 = &square
	lw $s0,0($s0)			#$t0 = square
	beq $s0,$zero,END_SQUARE	#if (square == 0) skip to end
					#else complete square
					
	la $s1,square_size		#$t1 = &square_size
	lw $s1,0($s1)			#$t1 = square_size
	
	la $s2,square_fill		#$t2 = &square_fill
	lb $s2,0($s2)			#$t2 = square_fill
	
	add $t0,$zero,$zero		#$t3 = row = 0
	addi $t4,$s1, -1		#$t4 = square_size - 1
SQUARE_LOOP:
	#Loop for(row = 0; row < square_size; row++)
	
	slt $t1,$t0,$s1			#$t1 = 1 iff row < square_size
	beq $t1,$zero,END_SQUARE	#branch if $t1 != 1, (if row >= square_size)
		
	beq  $t0,$zero,IF_SQUARE	#if (row == 0)
	
	
	beq  $t0,$t4,IF_SQUARE 		#if (row == square_size - 1)
	
	bne  $t0,$zero,ELSE_SQUARE	#if row != 0
	bne  $t0,$t4,ELSE_SQUARE	#if (row != square_size - 1)

IF_SQUARE:
	addi $t2,$zero,'+'		#$t2 = lr = '+'
	addi $t3,$zero,'-'		#$t3 = mid = '-'
	j SQUARE_PRINT			#jump to print 
	
ELSE_SQUARE:
	addi $t2,$zero,'|'		#$t2 = lr = '|'
	add  $t3,$zero,$s2		#$t3 = mid = square_fill
	j SQUARE_PRINT			#jump to print
	
SQUARE_PRINT:				#print block to make string
	addi $v0,$zero,11		#$v0 = 11 (print char)
	add  $a0,$zero,$t2		#a0 = $t2 = '+' || '|'
	syscall				
	
	addi $t5,$zero,1		#$t5 = 1 (row = 1)
	
SQUARE_PRINT_LOOP:
	slt $t6,$t5,$t4			#$t6 = $t5(row) < $t4(square_size - 1)
	beq $t6,$zero,SQUARE_PRINT_AFTER
	
	addi $v0,$zero,11		#$v0 = 11 (print char)
	add $a0,$zero,$t3		#$a0 = $t3 = mid = '-' || string_fill
	syscall
	
	addi $t5,$t5,1			#$t5++ (row++)
	j SQUARE_PRINT_LOOP		#loop 
	
SQUARE_PRINT_AFTER:
	addi $v0,$zero,11		#$v0 = 11 (print char)
	add $a0,$zero,$t2		#a0 = $t2 = lr
	syscall
	
	addi $v0,$zero,11		#$v0 = 11 (print char)
	addi $a0,$zero,'\n'		#$a0 = '\n'
	syscall
	
	addi $t0,$t0,1			#row++ 
	j SQUARE_LOOP			#jump back to original square loop
	
END_SQUARE:				#next task runCheck
#run check iterate through array of integers and see if the array of ints 
#are ascending,descending,or neither
#arrays of length zero or one report both ascending and descending
#in the case of both ascending is reported first
	
	la $s0, runCheck		#$s0 = $runCheck
	lw $s0,0($s0)			#$s0 = runCheck
	
	beq $s0,$zero,END_RUN_CHECK	#if (runCheck == 0) skip
	
	la $s1, intArray_len		#$s1 = &intArray_len
	lw $s1,0($s1)			#$s1 = intArray_len
	addi $t1,$zero,1		#$t1 = 1 (to check for arrays of len 1)
	beq  $s1,$t1,PRINT_ASCENDING	#if array_len == 1 go to print ascending label
	beq $s1,$zero,PRINT_ASCENDING	#if array_len == 0 go to print ascending
	addi $s1,$s1,-1			#len - 1 to avoid null at end of array
	
	la $s2,intArray			#$s2 = &intArray
	
	add  $t0,$zero,$zero		#$t0 = 0 (loop counter)
	add  $s3,$zero,$zero		#$s3 = 0 (flag to indicate ascending print took place)
ASCENDING_CHECK_LOOP:
	beq $t0,$s1,PRINT_ASCENDING	#if (Loop Counter == intArray_len) break
	
	lw $t2,0($s2)			#$t2 = intArray[curr]
	addi $s2,$s2,4			#add 4 to base address
	lw $t3,0($s2)			#$t3 = intArray[next]
	
	slt $t4,$t3,$t2			#$t4 = 1 iff $t3 < $t2
	bne $t4,$zero,DECENDING		#if ($t4 != 0) then jump (since ele[i] >= ele[i + 1]
	
	addi $t0,$t0,1			#add 1 to loop count
	j ASCENDING_CHECK_LOOP		#loop
	
PRINT_ASCENDING:
	addi $v0,$zero,4		#$v0 = 4 (print string)
	la   $a0,RUN_CHECK_LABEL	#$a0 = &RUN_CHECK_LABEL
	syscall				#Print (Run Check: )
	
	addi $v0,$zero,4		#$v0 = 4 (print string)
	la   $a0,ASCENDING_LABEL	#$a0 = $ASCENDING_LABEL
	syscall				#print (ASCENDING\n)
	addi $s3,$zero,1		#$s3 = 1 (indicating to later checks if a print ascending took place)
DECENDING:
	la  $s2,intArray		#get base address (unmodified)
	add $t0,$zero,$zero		#reset count
	
DECENDING_CHECK_LOOP:
	beq $t0,$s1,PRINT_DECENDING	#if (count == len(array)) jump
	
	lw $t2,0($s2)			#$t2 = array[i]
	addi $s2,$s2,4			#advance window
	lw $t3,0($s2)			#$t3 = array[i + 1]
	
	slt $t4, $t2,$t3		#if (array[i] < array[i + 1])

	bne $t4,$zero,PRINT_NEITHER	#if (array[i] >= array[i + 1])
	
	addi $t0,$t0,1			#advanced counter
	j DECENDING_CHECK_LOOP		#loop

PRINT_DECENDING:

	addi $v0,$zero,4		#$v0 = 4 (print str)
	la $a0,RUN_CHECK_LABEL		#$a0 = "Run Check: "
	syscall				#Write to display
	
	addi $v0,$zero,4		#$v0 = 4 (print str)
	la $a0,DECENDING_LABEL		#$a0 = "Descending\n"
	syscall				#Write to display
			
	j END_RUN_CHECK			#jump to end
	
PRINT_NEITHER:
	bne $s3,$zero,END_RUN_CHECK	#if ($s3 != 0) then false neither, skip to end
	addi $v0,$zero,4		#$v0 = 4 (print str)
	la $a0,RUN_CHECK_LABEL		#$a0 = "Run Check: "
	syscall				#Write to display
	
	addi $v0,$zero,4		#$v0 = 4 (print str)
	la $a0,NEITHER_LABEL		#$a0 = "Neither\n"
	syscall				#Write to display
		
END_RUN_CHECK:
#TASK 4: countWords, the following reads into a reg a global countWords, if (countWords == 1)
#then it will read a string (str) and count the number of words (excluding whitespace and newlines)
#ws = 0x20, null = 0x00, carriage return = 0x0D and line feed = 0x0A

	la $s0,countWords     		#$s0 = $&countWords
	lw $s0,0($s0)			#$so = countWords
	
	beq $s0,$zero,REV_STRING	#if (countWords == 0) skip
	
	la $s0,str			#$s0 = &str
	
	addi $t0,$zero,0x20		#$t0 = " " (whitespace)
	addi $t1,$zero,0x00		#$t1 = "\0" (null terminator)
	addi $t2,$zero,0x0a		#$t2 = "\n" (new line)
	add  $t3,$zero,$zero		#$t3 = wordCount =0	
			
WORD_COUNT_LOOP:
	lb $t4,0($s0)			#load byte from reg $s0 (str)
	
	beq $t4,$t1,END_WORD_COUNT	#if curr char == null terminator jump
	bne $t4,$t0,NON_WHITESPACE      #if curr char != " " (whitespace)
	
	addi $s0,$s0,1			#move address right by one

	j WORD_COUNT_LOOP		#loop till null
	
NON_WHITESPACE:
	beq  $t4,$t2,NEWLINE_LOGIC	#see if non whitespace char was \n
	addi $t3,$t3,1			#if not \n wordCount++
	
NON_WHITESPACE_LOOP:
	addi $s0,$s0,1			#move address right by one
	lb   $t5,0($s0)			#get char at new window
	
	beq  $t5,$t1,END_WORD_COUNT	#if null terminator end
	beq  $t5,$t2,WORD_COUNT_LOOP	#if \n go back to original loop
	beq  $t5,$t0,WORD_COUNT_LOOP	#if " " (ws) go back to original loop
	
	j NON_WHITESPACE_LOOP		#loop till next ws or new line or null
	
NEWLINE_LOGIC:
	addi $s0,$s0,1			#if char was \n don't incrament wordCount but move window
	j WORD_COUNT_LOOP		#go back to original with new window

END_WORD_COUNT:
	addi $v0,$zero,4		#$v0 = 4 (pring string)
	la $a0,COUNT_LABEL		#$a0 = "word count: "
	syscall				#write to display
	
	addi $v0,$zero,1		#$v0 = 1 (print int)
	add $a0,$zero,$t3		#$a0 = value contained in $t3 (wordCount)
	syscall				#write to display
	
	addi $v0,$zero,4		#$v0 = 4 (print str)
	la $a0,NEW_LINE			#$a0 = "\n"
	syscall				#write to display

REV_STRING:
#TASK 5: revString - reverse a string in memory
#the following C outlines the process:
#if (revString != 0){
#	int head = 0;
#	int tail = 0;
#	while (str[tail] != '\0'){
#		tail++;
#	}
#	tail--;
#	while (head<tail){
#		head++;
#		tail--;
#	}
#	printf("String swapped!\n");
#	printf("\n");
#}		
	la  $s0,revString		#$s0 = &revString
	lw  $s0,0($s0)			#$s0 = revString
	beq $s0,$zero,END_OF_ALL_TASKS	#if (revString ==  0) skip
	
	#$s1 will be my head
	la  $s1,str			#$s1 = &str
	lb  $s3,0($s1)			#$s3 = str
	beq $s3,$zero,END_REV		#if ($s3 = "\0") end
	
	#$s2 will be my tail
	la  $s2,str			#$s2 = &str

	addi $t0,$zero,0x00		#$t0 = "\0"
	
FIND_TAIL_LOOP:
	lb  $s4,0($s2)			#$s4 = str 
	beq  $s4,$t0,TAIL_FOUND		#if ($s4 = "\0") break
	addi $s2,$s2,1			#tail++
	j FIND_TAIL_LOOP		#loop to inc tail
	
TAIL_FOUND:
	addi $s2,$s2,-1			#when "\0" found -1 to get address of last non null char
	
REV_STR_LOOP:
	slt $t1,$s1,$s2			#$t1 = (head<tail) = 1 until head meets tail
	beq $t1,$zero,END_REV		#if (head >= tail) break
	
	lb $s4,0($s2)			#$s4 = tail value
	lb $s3,0($s1)			#$s3 = head value
	
	sb $s4,0($s1)			#write tail value to head address
	sb $s3,0($s2)			#write head value to tail address
	
	addi $s1,$s1,1			#advance head
	addi $s2,$s2,-1			#decrament tail
	j REV_STR_LOOP			#loop till head meets tail

END_REV:
	addi $v0,$zero,4		#$v0 = 4 (print string)
	la $a0,REV_LABEL		#$a0 = "String successfully swapped!\n" 
	syscall				#write to display
	
END_OF_ALL_TASKS:
	
	lw $ra, 4($sp) 			# get return address from stack
	lw $fp, 0($sp) 			# restore the caller’s frame pointer
	addiu $sp, $sp, 24 		# restore the caller’s stack pointer
	jr $ra 				# return to caller’s code
