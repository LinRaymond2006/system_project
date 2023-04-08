[bits 16]
[org 0x7c00]


;code here

mov ax, cs
mov ds, ax

mov ax, 0x7e0
mov es, ax
xor di, di


;from:https://wiki.osdev.org/VGA_Fonts
;in: es:di=4k buffer
;out: buffer filled with font
push ds
push es
;ask BIOS to return VGA bitmap fonts
mov ax, 1130h
mov bh, 6
int 10h
;copy charmap
push es
pop ds
pop es
mov si, bp
mov cx, 256*16/4
rep movsd
pop ds
;code ends here




jmp $
times 510-($-$$) db 0

dw 0xaa55

