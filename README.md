Instructions to build miniOS Shell 1.0
======================================

1. kernel entry
nasm -f win32 kernel_entry.asm -o kernel_entry.obj

2. kernel.c
gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -c kernel.c -o kernel.o

3. linken
gcc -m32 -nostdlib -Wl,-T,linker.ld -Wl,-e,_start -o kernel.exe kernel_entry.obj kernel.o

4. bin erzeugen
objcopy -O binary kernel.exe kernel.bin

5. image bauen
copy /b boot.bin+kernel.bin os-image.bin

6. starten
qemu-system-i386 -drive format=raw,file=os-image.bin
