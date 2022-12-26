[BITS 32]
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;eax:sector offset
;ebx:buffer location
;cx:number of sectors to read
%define ATA0_MASTER_base_port 0x1f0
;0x1f0:ata0-master
;reading via LBA24


READ_ATA32:
	
	push esi
	push edi
	push edx
	push eax
	
	
	mov esi, eax
	mov di, cx
	mov dx, ATA0_MASTER_base_port + 2
	mov al, cl
	out dx, al 		;how many sectors to read
	mov eax, esi
	mov dx, ATA0_MASTER_base_port + 3
	out dx, al  	;LBAddr 0~7
	mov cl, 8
	shr eax, cl
	mov dx, ATA0_MASTER_base_port + 4
	out dx, al  	;LBAddr 8~15
	shr eax, cl
	mov dx, ATA0_MASTER_base_port + 5
	out dx, al  	;LBAddr 16~23
	shr eax, cl
	and al, 0xe0    ;bit7~bit4=1110->using LBA
	or al, 0xe0
	mov dx, ATA0_MASTER_base_port + 6
	out dx, al
	mov dx, ATA0_MASTER_base_port + 7
	mov al, 0x20
	out dx, al  	;0x20->read

.diskstat_busy: ;write->command word port. read->disk status
	nop
	in al, dx
	and al, 0x88  ;bit3=1->ready, bit7=1->disk in busy
	cmp al, 0x08
	jnz .diskstat_busy
	mov ax, di
	mov dx, 256
	mul dx
	mov cx, ax
	;di=loader_sect_size, each sector is composed by 512 bytes
	;therefore need di*512/2 times to finish the operation(2bytes comprise a word)
	mov dx, ATA0_MASTER_base_port
.contrd:
	in ax, dx
	;;;;;;;;;;use ebx instead of bx to address in unreal mode with A20 enabled here!;;;;;;;;;;;;;;;;;;;;
	mov [ebx], ax
	add ebx, 2
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	loop .contrd
	
	pop eax
	pop edx
	pop edi
	pop esi
	
	ret



CMP_STOP32:
	CMP EAX, 0X00_00_00_02
	JGE .IN_VALID_CLUSTERNUM_RANGE
	.IN_VALID_CLUSTERNUM_RANGE:CMP EAX, 0X0F_FF_FF_EF
	JBE .RETURN_LABEL	;FINISH READING, RETURN
	.NOT_VALID_COMPARE_EOF: CMP EAX, 0X0F_FF_FF_F8
	JGE .IN_EOF_VALUE_RANGE
	.IN_EOF_VALUE_RANGE: CMP EAX, 0X0F_FF_FF_FF
	JBE .1
	.1:
	;JMP EBP
	PUSH DWORD EBP
	;THAN, RETURN
	.RETURN_LABEL:
	RET





NXT_CLUSTER32:
	ADD EAX, DWORD [BPB_ROOTCLUS]
	SHL EAX, 2
	ADD EAX, FATBASE
	MOV EAX, DWORD [EAX]
	SUB EAX, DWORD [BPB_ROOTCLUS]
	RET





CLUSTER2SECTOR32:
	PUSH ECX
	PUSH EDX
	MOV EDX, DWORD [BPB_ROOTCLUS]
	SUB EAX, EDX

	XOR EDX, EDX
	MOVZX ECX, BYTE [BPB_SECPERCLUS]
	MUL CX
	ADD EAX, DWORD [FIRSTDATASECTOR]

	POP EDX
	POP ECX
	RET