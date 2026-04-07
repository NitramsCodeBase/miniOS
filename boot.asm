BITS 16
ORG 0x7C00

CODE_SEG equ 0x08
DATA_SEG equ 0x10

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Textmodus 80x25
    mov ax, 0x0003
    int 0x10

    ; Kernel nach 0x1000 laden
    mov bx, 0x1000
    mov ah, 0x02
    mov al, 24          ; Anzahl Sektoren
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, 0x80
    int 0x13
    jc disk_error

    ; A20 aktivieren
    call enable_a20

    ; GDT laden
    lgdt [gdt_descriptor]

    ; Protected Mode aktivieren
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Far jump in 32-bit Code
    jmp CODE_SEG:protected_mode

disk_error:
    mov si, disk_error_msg
.print:
    lodsb
    or al, al
    jz $
    mov ah, 0x0E
    int 0x10
    jmp .print

enable_a20:
    in al, 0x92
    or al, 00000010b
    out 0x92, al
    ret

gdt_start:
gdt_null:
    dq 0x0000000000000000

gdt_code:
    dq 0x00CF9A000000FFFF

gdt_data:
    dq 0x00CF92000000FFFF

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

disk_error_msg db "Disk read error!", 0

BITS 32
protected_mode:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esp, 0x90000

    jmp 0x1000

times 510 - ($ - $$) db 0
dw 0xAA55