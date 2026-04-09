@echo off
@echo.
@echo building image ...

nasm -f bin ../boot/boot.asm -o boot.bin
nasm -f win32 ../boot/kernel_entry.asm -o kernel_entry.obj

@REM rem adding files here

gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -c ../kernel/kernel.c -o kernel.o
gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -c ../kernel/pit.c -o pit.o
gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -c ../kernel/io.c -o io.o
gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -c ../kernel/commands.c -o commands.o
gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -c ../libs/string.c -o string.o
gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -c ../libs/graphic.c -o graphic.o

@REM rem linking... add .o files to the command list
gcc -m32 -nostdlib -Wl,-T,../linker/linker.ld -Wl,-e,_start -o kernel.exe kernel_entry.obj kernel.o pit.o io.o commands.o string.o graphic.o

objcopy -O binary kernel.exe kernel.bin
copy /b boot.bin+kernel.bin miniOS.img

del *.bin
del kernel_entry.obj
del *.o
del *.exe

@echo starting ...

./run.bat