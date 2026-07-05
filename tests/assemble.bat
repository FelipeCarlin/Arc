::w:\_Tools\powerpc-eabi-tools-windows\powerpc-eabi-as simple.asm -o simple.o
::w:\_Tools\powerpc-eabi-tools-windows\powerpc-eabi-objcopy -O binary -j .text simple.o simple.bin

w:\_Tools\powerpc-eabi-tools-windows\powerpc-eabi-as -mregnames -mbig memcmp.asm -o memcmp.o
w:\_Tools\powerpc-eabi-tools-windows\powerpc-eabi-objcopy -O binary -j .text memcmp.o memcmp.bin
