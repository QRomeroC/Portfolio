#Author: Cesar D Quihuis-Romero
#Purpose: ASM 5 will focus on functions which utilize the stack 
#	  to store variables. Globals will only be used for string
# 	  constants.

#Task 1: countLetters() - utilizing a stack where the index represents 
#	 the respective letter (a-z//A-Z) and the value stored at the 
#	 given index is the count of that letter.

#Task 2: subsCipher() - (Substitution Cipher) this function will take 
#        as an argument a null terminated string and a table of characters 
#       (which will act as a map). The function will then encode string.

.data
	other_label: .asciiz "<other>: "
	dash_label: .asciiz "----------------\n"
	
.text

.globl countLetters
countLetters:
	#countLetters prologue
	addiu $sp, $sp, -24			#update stack pointer by 24 bytes
	sw    $fp, 0($sp)			#put old fp on stack
	sw    $ra, 4($sp)			#put ra on stack
	addiu $fp, $sp, 20			#update new fp
	
	#register preservation
	addiu $sp, $sp, -16			#update stack pointer by 12 bytes
	sw    $s0, 0($sp)			#put $s0 on stack
	sw    $s1, 4($sp)			#put $s1 on stack
	sw    $s2, 8($sp)			#put $s2 on stack
	sw    $s3, 12($sp)			#put $s3 on stack
	
	add   $s2, $zero, $a0			#move $a0 into $s2 for use in loops
	add   $s3, $zero, $a0			#move $a0 into $s3 for use in prints
	
	#letters stack
	addiu $sp, $sp, -104			#update stack pointer by 104 bytes
	
	add   $t0, $zero, $zero			#$t0 = 0 == int i = 0
	addi  $t1, $zero, 104			#$t1 = 104 == upper bounds for loop
	
initLoop:
	#init letters stack with 0
	beq   $t0, $t1, endInit			#if $t0 == $t1 (if i == 104) end loop
	add   $t3, $sp, $t0			#$t3 = (i + $sp)
	sw    $zero, 0($t3)			#store 0 into stack
	
	addi  $t0, $t0,4			#$t0 += 4
	
	j initLoop				#loop

endInit:
	
	add   $s1, $zero, $zero			#$s1 == other = 0
	
countLoop:

	lb    $s0, 0($s2)			#$s0 = String[i]
	beq   $s0, $zero, endLoop		#if $s0 = '\0' end loop
	
	addi  $t0, $zero, 'a'			#$t0 = 'a'
	addi  $t1, $zero, 'z'			#$t1 = 'z'
	
	slt   $t4, $s0, $t0			#$t4 = (String[i] < 'a')
	bne   $t4, $zero, checkUpper		#if $t4 != 0 goto check uppercase
	
	slt   $t4, $t1, $s0			#$t4 = ('z' < String[i]) 
	bne   $t4, $zero, checkUpper		#if $t4 != 0 check uppercase
	
	j lowercase				#if value in range normalize relative to 'a'

checkUpper:
#value read from string is not in range of lower case value check if in range of uppercase 

	addi  $t0, $zero, 'A'			#$t0 = 'A'
	addi  $t1, $zero, 'Z'			#$t1 = 'Z'
	
	slt   $t4, $s0, $t0			#$t4 = (String[i] < 'A')
	bne   $t4, $zero, other			#if $t4 != 0 value read from string is a "other" char
	
	slt   $t4, $t1, $s0			#$t4 = ('Z' < String[i])
	bne   $t4, $zero, other			#if $t4 != 0 value read from string is "other char
	
	j uppercase				#if value in range normalize relative to 'Z'
	
lowercase:
#value read from string is in range of 'a' - 'z'

	addi  $t0, $zero, 'a'			#$t0 = 'a'
	sub   $t1, $s0, $t0			#$t1 = (String[i] - 'a') 
	sll   $t1, $t1, 2			#$t1 = (i) * size == i * 4 (convert to stack address)
	add   $t1, $t1, $sp			#$t1 = base + (i*size)
		
	lw    $t0, 0($t1)			#load value at new address
	addi  $t0, $t0, 1			#incr value held at address
	sw    $t0, 0($t1)			#put new value back on stack
	
	addi  $s2, $s2, 1			#get next char in string (String[i+1])
	j countLoop				#loop
	
uppercase:
#value read from string is in range of 'A' - 'Z'

	addi  $t0, $zero, 'A'			#$t0 = 'A'
	sub   $t1, $s0, $t0			#$t1 = String[i] - 'A'
	sll   $t1, $t1, 2			#$t1 = i * size == i * 4
	add   $t1, $t1, $sp			#$t1 = base + (i * size)
	
	lw    $t0, 0($t1)			#get value at address
	addi  $t0, $t0, 1			#incr value
	sw    $t0, 0($t1)			#put value back
	
	addi  $s2, $s2, 1			#get next char in string (String[i+1])
	j countLoop				#loop
		
other:
#value read from string is not in range 'a' - 'z' or 'A' - 'Z'
	
	addi  $s0, $s0, 1			#s0 += 1 (get next char)
	addi  $s1, $s1, 1			#$s1 += update other
	addi  $s2, $s2, 1			#$s2 += update address window
	
	j countLoop				#loop

endLoop:
#printf("----------------\n%s\n----------------\n", str)
	addi  $v0, $zero, 4			#$v0 = 4 (print string)
	la    $a0, dash_label			#$a0 = "------...-"
	syscall					#write

	addi  $v0, $zero, 4			#$v0 = 4 (print string)
	add   $a0, $zero, $s3			#$a0 = str
	syscall					#write
	
	addi  $v0, $zero, 11			#$v0 = 11 (print char)
	addi  $a0, $zero, '\n'			#$a0 = \n
	syscall					#write
	
	addi  $v0, $zero, 4			#$v0 = 4 (print string)
	la    $a0, dash_label			#$a0 = "---...-"
	syscall					#write

	add   $t0, $zero, $zero			#$t0 = 0 (int i = 0)
	addi  $t1, $zero, 'a'			#$t1 = 'a'
	addi  $t2, $zero, 26			#$t2 = 26 (i < 26)
	add   $t4, $zero, $sp			#$t4 = current stack pointer value for addressing
	
printLoop:
#	for (int i = 0; i<26; i++) {
#		printf("%c: %d\n", 'a'+i, letters[i]);
#	}

	slt   $t3, $t0, $t2			#$t3 = (i < 26)
	beq   $t3, $zero, endPrint		#if $t3 >= 26 end loop
	
	addi  $v0, $zero, 11			#$v0 = 11 (print char)
	add   $a0, $t0, $t1			#$a0 = ('a' + i)
	syscall					#write to display
	
	addi  $v0, $zero, 11			#$v0 = 11 (print char)
	addi  $a0, $zero, ':'			#$a0 = ':'
	syscall					#write
	
	addi  $v0, $zero, 1			#$v0 = 1 (print int)
	lw    $a0, 0($t4)			#$a0 = Letters[i]
	syscall					#write
	
	addi  $v0, $zero, 11			#$v0 = 11 (print char)
	addi  $a0, $zero, '\n'			#$a0 = \n
	syscall					#write
	
	addi  $t4, $t4, 4			#$t4 += 4 (update to next address)
	addi  $t0, $t0, 1			#$t0 += 1 (i++)
	
	j printLoop				#loop
	
endPrint:
#	printf("<other>: %d\n", other);
	
	addi  $v0, $zero, 4			#$v0 = 4 (print string) 
	la    $a0, other_label			#$a0 = "<other>: "
	syscall					#write
	
	addi  $v0, $zero, 1			#$v0 = 1 (print int)
	add   $a0, $zero, $s1			#$a0 = other count
	syscall					#write
	
	addi  $v0, $zero, 11			#$v0 = 11 (print char)
	addi  $a0, $zero, '\n'			#$a0 = \n
	syscall					#write
	
	#end of print start of stack closure

	addiu $sp, $sp, 104			#close stack space for letters[]		
	
	#register preservation
	lw    $s0, 0($sp)			#restore $s0
	lw    $s1, 4($sp)			#restore $s1
	lw    $s2, 8($sp)			#restore $s2
	lw    $s3, 12($sp)			#restore $s3
	addiu $sp, $sp, 16			#update stack pointer
	
	#countLetters epilouge
	lw    $ra, 4($sp)			#pop $ra off stack
	lw    $fp, 0($sp)			#pop $fp off stack
	addiu $sp, $sp, 24			#dec stack
	jr    $ra				#jump back to caller

.globl subsCipher
subsCipher:
	#subsCipher prologue
	addiu $sp, $sp, -24			#grow stack
	sw    $fp, 0($sp)			#push $fp to stack
	sw    $ra, 4($sp)			#push $ra on stack
	addiu $fp, $sp, 20			#update new fp
	
	#perserve S regs
	addiu $sp, $sp, -20			#dec stack by 20 bytes
	
	sw    $s0, 0($sp)			#push $s0 on stack
	sw    $s1, 4($sp)			#push $s1 on stack
	sw    $s2, 8($sp)			#push $s2 on stack
	sw    $s3, 12($sp)			#push $s3 on stack
	sw    $s4, 16($sp)			#push $s4 on stack
	
	add   $s0, $zero, $a0			#move $a0 to $s0
	add   $s1, $zero, $a1			#move $a1 to $s1
	
	add   $a0, $zero, $s0			#move $s0 ($a0) to $a0 for strlen call
	jal   strlen				#call to strlen
	
	add   $s2, $v0, $zero			#move $v0 (strlen return) to $s2
	addi  $s3, $s2, 1			#$s3 = strlen + 1 ($s3 == len)
	
	addi  $t0, $s3, 3			#$t0 = len + 3
	
	addi  $t1, $zero, 0x3			#$t1 = 0x3
	nor   $t1, $t1, $zero			#t1 = ~0x3
	
	and   $s4, $t0, $t1			#$s4 = len_roundup == (len + 3) & ~0x3
	
	sub   $sp, $sp, $s4			#dup[len_roundup] 
	
	add   $t0, $zero, $zero			#$t0 = i = 0
	addi  $t1, $s3, -1			#$t1 = len - 1
	add   $t3, $zero, $zero
subsCipherLoop:
	slt   $t2, $t0, $t1		    	#$t2 = i < len - 1
	beq   $t2, $zero, endSubsCipherLoop	#if ($t2 == 0) end loop

	add   $t3, $t0, $s0			#$t3 = $t3 + $s0 ($a0) (base + i * size)
	
	lb    $t4, 0($t3)			#$t4 = str[i]
	
	add   $t4, $t4, $s1			#$t4 = $t4 + map base ($s1 = *map)
	
	lb    $t5, 0($t4)			#$t5 = map[str[i]]
	
	add   $t6, $t0, $sp			#$t6 = base + i * size
	
	sb    $t5, 0($t6)			#dup[i] = map[str[i]]
	
	addi  $t0, $t0, 1			#i++
	j subsCipherLoop			#loop
	
endSubsCipherLoop: 
	
	add  $t0, $zero, $zero			#$t0 = 0 (reseting $t0 to zero)
	add  $t0, $t0, $s3			#$t0 = 0 + len
	addi $t0, $t0, -1			#$t0 = len - 1
	
	add  $t0, $t0, $sp			#dup[len - 1] == get index of last element in dup
	
	sb   $zero, 0($t0)			#dup[len - 1] = 0
	#printSubstitutedStrin(dup)
	add  $a0, $zero, $sp			#$a0 = $sp ($sp by this point is dup[0])
	jal  printSubstitutedString		#call to printSubstitutedString
	
	#start of restoring stack
	add $sp, $sp, $s4			#shrink stack by len(dup)
	
	#perserve S regs
	lw    $s0, 0($sp)			#pull $s0 off stack
	lw    $s1, 4($sp)			#pull $s1 off stack
	lw    $s2, 8($sp)			#pull $s2 off stack
	lw    $s3, 12($sp)			#pull $s3 off stack
	lw    $s4, 16($sp)			#pull $s4 off stack
	
	addiu $sp, $sp, 20			#shirnk stack by 20 bytes (close S Reg preservation)
	
	#subsCipher epilouge
	lw    $ra, 4($sp)			#pop $ra off stack
	lw    $fp, 0($sp)			#pop $fp off stack
	addiu $sp, $sp, 24			#shrink stack
	jr    $ra				#jump back to caller
	
.globl strlen
strlen:
	#strlen prologue
	addiu $sp, $sp, -24			#grow stack
	sw    $fp, 0($sp)			#push $fp to stack
	sw    $ra, 4($sp)			#push $ra on stack
	addiu $fp, $sp, 20			#update new fp
	
	#S reg preservation
	addiu $sp, $sp, -12			#grow stack by 12 bytes
			
	sw    $s0, 0($sp)			#push $s0 to stack
	sw    $s1, 4($sp)			#push $s1 to stack
	sw    $s2, 8($sp)			#push $s2 to stack
	
	add   $s0, $zero, $a0			#$s0 = $a0 (*str)
	add   $s1, $zero, $zero			#$s1 = 0   (will become str[i])
	add   $s2, $zero, $zero			#$s2 = 0   (count)
		
strlenLoop:
	lb    $s1, 0($s0)			#$s1 = str[i]
	beq   $s1, $zero, endStrlen		#if $s1 = '\0' break
	
	addi  $s2, $s2, 1			#count++
	addi  $s0, $s0, 1			#str[i + 1]
	j  strlenLoop				#loop
	
endStrlen:
	add   $v0, $zero, $s2			#return count
	
	#S reg preservation
	lw    $s0, 0($sp)			#pop $s0 	
	lw    $s1, 4($sp)			#pop $s1
	lw    $s2, 8($sp)			#pop $s2
	
	addiu $sp, $sp, 12			#shrink stack by 12 bytes
	
	#strlen epilouge
	lw    $ra, 4($sp)			#pop $ra off stack
	lw    $fp, 0($sp)			#pop $fp off stack
	addiu $sp, $sp, 24			#shrink stack
	jr    $ra				#jump back to caller