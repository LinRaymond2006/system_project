[BITS 32]
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;eax:sector offset
;ebx:buffer location
;cx:number of sectors to read
%define ATA0_MASTER_base_port 0x1f0
;0x1f0:ata0-master
;reading via LBA24
READ_ATA32:
	;clear the higher part of ecx
	shl ecx, 16
	shr ecx, 16
	pushad
	mov ebp, eax
	.1:
		push ecx
		mov cx, 1
		call READ_ATA32_imp
		inc eax
		pop ecx
		loop .1
	popad
	ret

READ_ATA32_imp:
	
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
	JnBE .RETURN_LABEL

		;JMP EBP
		PUSH EBP
		;THAN, RETURN

	.RETURN_LABEL:
		RET




NXT_CLUSTER32:
	
	
	;CLUSTER NUMBER STARTS FROM 2
	;THAT IS TO SAY, THE FIRST DATA SECTOR IS CLUSTER 2, AND SO ON.
	;BUT IN THIS FUNCTION, ASSUME CLUSTER NUMBER STARTS FROM 0 (THE FIRST DATA SECTOR IS CLUSTER 0)

	;STEP 1: IF THE FIRST DATA SECTOR IS CLUSTER 0, THAN PLUS 2
	;ADD EAX, DWORD [BPB_ROOTCLUS]

	;STEP 2: LOCATE THE NXTCLUS ENTRY IN FAT TABLE 
	;NOW, THE EAX REGISTER IS THE CLUSTER NUMBER (STARTS FROM 2)

	;STEP 2.1: MUL EAX BY 4 ((OPERATOR *4) = (OPERATOR << 2))
	SHL EAX, 2

	;STEP 2.2: EAX = EAX + FATBASE
	ADD EAX, FATBASE

	;STEP 3:EXTRACT THE VALUE OF "NXTCLUSTER", WRITEBACK INTO EAX
	MOV EAX, DWORD [EAX]

	;SUB EAX, DWORD [BPB_ROOTCLUS]

	RET





CLUSTER2SECTOR32:
	PUSH ECX
	PUSH EDX
	

	;MOV EDX, DWORD [BPB_ROOTCLUS]
	;SUB EAX, EDX

	XOR EDX, EDX
	;EAX IS PASSED BY THE CALLER TO CALLEE
	MOVZX ECX, BYTE [BPB_SECPERCLUS]
	MUL CX
	ADD EAX, DWORD [FIRSTDATASECTOR]

	POP EDX
	POP ECX
	RET

DETECT_CPUID_SUPPORT:
	PUSHAD

    PUSHFD
    POP EAX
    MOV ECX, EAX
    XOR EAX, 0X200000
    PUSH EAX
    POPFD
    PUSHFD
    POP EAX
    XOR EAX, ECX
    JZ $ ; CPUID IS NOT SUPPORTED!

	POPAD
	RET




%DEFINE CPUID_LM (1 << 29)
DETECT_LONG_MODE_SUPPORT:
    ;REFERENCE:https://wiki.osdev.org/Setting_Up_Long_Mode
    MOV EAX, 0X80000000
    CPUID
    CMP EAX, 0X80000001
    JB LONG_MODE_NOT_SUPPORT
    MOV EAX, 0X80000001
    CPUID
    TEST EDX, CPUID_LM
    JZ LONG_MODE_NOT_SUPPORT

LONG_MODE_NOT_SUPPORT:
	JMP $
CPUID_NOT_SUPPORTED:
	JMP $