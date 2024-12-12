main:
	mflr 27			# r27 <- LinkRegister 
	mr 13,3			# r13 <- r3	(argc)
	mr 14,4			# r14 <- r4	(argv)
	addi 3,3,48		# r3 <- r3 + 48 (transform argc in ASCII digit)
	bl putchar		# call to the putchar C function (libc)

	li 3,10			# r3 <- 10 (LF ASCII code)
	bl putchar		# call to the putchar C function (libc)

next:				# Prepare to loop over arguments
	lwz 3,0(14)		# r3 <- (r14)
	bl puts			# call to the puts C function from (libc)
	
	addi 14,14,4		# r14 <- r14 + 4 (4 bytes offset)
	addi 13,13,-1		# decrement r13
	cmpwi 13,0
	bgt next		# while r13 > 0 go to label :next


	lis 3,zone@ha		# load the address of label zone: in r3
	addi 3,3,zone@l
	bl puts			# call to the puts C function (libc)
	mtlr 27			# LinkRegister <- r27
	mr 3,13			# r3 <- r13
	blr			# go to the address stored in LinkRegister
	andi. 12, 2, 3

	subfc 1, 2, 3
	#mulhdu 1, 2, 3
	addc 1, 2, 3
	mulhwu 1, 2, 3

	mfcr 3
	#lwax 12, 2, 2
	#ldx 1, 2, 3
	#ldzx 1, 2, 3
	slw 1, 2, 3
	
	cntlzw 1, 2
	#sld 1, 2, 3
	and 1, 2, 3
	cmpl 1, 0, 2, 3
	subf 1, 2, 3

