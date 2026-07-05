.text
.globl _start

_start:
    mflr    r0
    stwu    r1,-48(r1)
    stw     r0,52(r1)
    stmw    r27,20(r1)

    mr      r31,r3
    mr      r30,r4
    li      r29,0
    li      r28,-1

    cmpwi   r4,0
    ble     .Ldone

.Lloop:
    lwz     r5,0(r31)
    lwz     r6,0(r30)
    cmplw   r5,r6
    bne     .Lmismatch

    lbz     r7,4(r31)
    lhz     r8,6(r31)
    lha     r9,8(r31)

    rlwinm  r10,r5,8,0,23
    rlwimi  r10,r6,16,24,31
    rlwnm   r11,r5,r8,0,15

    andi.   r12,r5,0xff
    andis.  r13,r6,0x8000
    ori     r14,r5,0x1234
    oris    r14,r14,0x5678
    xori    r15,r6,0xf0f0
    xoris   r15,r15,0x0f0f

    addic   r16,r5,-16
    addic.  r16,r16,1
    subfic  r17,r5,100
    mulli   r18,r5,7

    add     r19,r5,r6
    add.    r19,r19,r7
    addo    r20,r5,r6
    addo.   r20,r20,r7
    addc    r21,r5,r6
    subfc   r22,r5,r6
    subf    r23,r6,r5
    mulhw   r24,r5,r6
    mulhwu  r24,r5,r6

    slw     r25,r5,r8
    and     r26,r5,r6
    andc    r26,r26,r7
    cntlzw  r0,r5

    twi     4,r5,0
    tw      4,r5,r6

    addi    r29,r29,1
    addi    r31,r31,4
    addi    r30,r30,4
    b       .Lloop

.Lmismatch:
    li      r29,-1
    b       .Ldone

.Ldone:
    stw     r29,12(r1)

    lis     r3,checksum_table@ha
    addi    r3,r3,checksum_table@l
    lwzx    r4,r3,r29
    lwzux   r5,r3,r29
    lwarx   r8,0,r3
    dcbst   0,r3

    mfcr    r9
    mfspr   r10,8
    mflr    r10
    mfctr   r11
    mtspr   8,r10
    mtlr    r10
    mtctr   r10

    cmpw    cr1,r29,r28
    cmplwi  cr2,r29,0xffff
    mcrf    cr3,cr1

    crand   4*3+0,4*1+0,4*2+0
    crxor   4*3+1,4*1+1,4*2+1
    crnand  4*3+2,4*1+2,4*2+2
    crnor   4*3+3,4*1+3,4*2+3
    creqv   4*3+0,4*1+0,4*2+0
    crandc  4*3+1,4*1+1,4*2+1
    crorc   4*3+2,4*1+2,4*2+2
    cror    4*3+3,4*1+3,4*2+3

    beq     cr1,.Lspecial
    bge     cr2,.Lspecial
    b       .Lskip

.Lspecial:
    bl      .Lhelper
    ba      0x3000
    bla     0x3004

.Lskip:
    lbzu    r12,1(r3)
    stbu    r12,1(r3)
    sthu    r12,2(r3)
    stwu    r12,4(r3)
    stb     r12,0(r3)
    sth     r12,0(r3)
    stw     r12,0(r3)

    lwz     r0,52(r1)
    mtlr    r0
    lmw     r27,20(r1)
    addi    r1,r1,48
    blr

.Lhelper:
    mflr    r0
    stw     r0,4(r1)
    stwu    r1,-16(r1)
    li      r3,0
    lwz     r0,20(r1)
    mtlr    r0
    addi    r1,r1,16
    blr

    isync
    sc

    .align 4
checksum_table:
    .long 0x00000000, 0x77073096, 0xee0e612c, 0x990951ba
    .long 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3
