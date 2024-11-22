
start:
#	addic 12,3,0xff
#	addic. 12,3,0xff
#	addi 12,3,0xff
#	addis 12,3,0xff

#	ori 12,3,0xff
#	oris 12,3,0xff
#	xori 12,3,0xff
#	xoris 12,3,0xff
#	andi. 12,3,0xff
#	andis. 12,3,0xff
#	
#	crxor 1, 2, 3
	subfic 1, 12, -1653
        twi 2, 1, -100
        cmpli 5, 1, 3, 16000
        cmpi 5, 1, 3, -8000
	b 0x10
	sc
	sc 5
