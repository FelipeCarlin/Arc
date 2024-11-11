square:
        stwu 1,-32(1)
        stw 31,28(1)
        mr 31,1
        stw 3,8(31)
        lwz 9,8(31)
        mullw 9,9,9
        mr 3,9
        addi 11,31,32
        lwz 31,-4(11)
        mr 1,11
        blr
main:
	twui 12, -124

	mulli 2,3,157

        stwu 1,-48(1)
        mflr 0
        stw 0,52(1)
        stw 31,44(1)
        mr 31,1
        stw 3,24(31)
        stw 4,28(31)
        li 3,12
        bl square
        stw 3,8(31)
        lwz 9,8(31)
        mr 3,9
        addi 11,31,48
        lwz 0,4(11)
        mtlr 0
        lwz 31,-4(11)
        mr 1,11
        blr
