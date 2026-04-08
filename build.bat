@echo off
@echo.
@echo building image ...

nasm -f bin boot.asm -o boot.bin
nasm -f win32 kernel_entry.asm -o kernel_entry.obj >nul

rem adding files here
gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -c kernel.c -o kernel.o >nul
gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -c libs/pit.c -o pit.o >nul
gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -c libs/io.c -o io.o >nul
gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -c libs/commands.c -o commands.o >nul
gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -c libs/string.c -o string.o >nul


rem linking... add .o files to the command list
gcc -m32 -nostdlib -Wl,-T,linker.ld -Wl,-e,_start -o kernel.exe kernel_entry.obj kernel.o pit.o io.o commands.o string.o>nul

objcopy -O binary kernel.exe kernel.bin >nul
copy /b boot.bin+kernel.bin miniOS.img >nul

del *.bin
del kernel_entry.obj
del *.o
del *.exe

start.bat