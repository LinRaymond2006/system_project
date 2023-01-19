;FOR FAT32 FILE SYSTEM ON HARD DISK
;CAUSION: THE SECTOR SIZE MUST BE 512
[BITS 32]

;ROOT DIRECTORY: 0X7E00~0X100000 (ABLE TO STORE 1985 SECTORS)
%DEFINE ROOTDIRBASE	0x1500
%DEFINE FATBASE		0X1500
%DEFINE FATCHAINBASE 0X500
;FAT32-RELATIVE
;LOAD THE ROOTDIR, GET THE FIRST CLUSTER OF THE GIVEN
;FILE, THAN OVERWRITE THE ROOTDIR REGION WITH FAT TABLE,
;AND WRITE THE LINKED LIST OF SECTOR NUMBER TO "TMPBASE" IN LOW MEMORY
;0X500, ONCETHOSE STEPS ARE DONE, LOAD THE GIVEN FILE INFO "BUFFERBASE"
;, WHICH IS 0X7E00, RIGHT AFTER THE BOOT SECTOR REGION


%DEFINE SFNSIZE	32
%DEFINE FILENAME_LENGTH 11

;=======================

[SECTION .CODE]

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CALCULATE_FIRSTDATASECTOR:

;FAT32:
;FIRSTDATASECTOR = BPB_RESVDSECCNT + (BPB_NUMFATS * FATSZ32)

;(BPB_NUMFATS * FATSZ32)
MOV ECX, DWORD [BPB_FATSZ32]
MOVZX EAX, BYTE [BPB_NUMFATS]
MUL ECX

;+BPB_RESVDSECCNT
MOVZX EDX, WORD [BPB_RESVDSECCNT]
ADD EAX, EDX

;THIS WILL RAISE A "MISMATCH IN OPERAND SIZES" EXCEPTION ON COMPILE TIME
;ADD EAX, WORD [BPB_RESVDSECCNT]


MOV DWORD [FIRSTDATASECTOR], EAX


LOAD_ROOTDIR:
;FUNCTION "NXT_CLUSTER"
;EDX:CLUSTER NUMBER (AS A CALLER, THE CLUSTER NUMBER STARTS FROM 0)
;EBP:WHERE TO GO IF RECIEVE "EOF" VALUE

;RETURNS:
;EAX: THE SECTOR NUMBER OF THE CLUSTER
;EDX:NEXT CLUSTER NUMBER (THE CLUSTER NUMBER STARTS FROM 0)

;NECESSARY CONDITIONS:
;MACRO "FATBASE" HAVE TO BE DEFINED

;EAX IS NOW FIRSTDATASECTOR, THEREFORE NOT SET AGAIN

.LOAD_INTO_MEM:

MOV EBX, ROOTDIRBASE
MOVZX ECX, BYTE [BPB_SECPERCLUS]
CALL READ_ATA

;CX IS NOW BPB_SECPERCLUS
;CHECKOUT FILES IN THE FIRST CLUSTER (NOTE:ONLY SEARCH FOR THE FIRST ROOTDIRECTORY CLUSTER)
.COMPARE_FILENAME:
%DEFINE SFNSHIFT 5



.SETUP:
MOV ESI, ROOTDIRBASE
.COMPARE_LOOP:
	MOV ECX, FILENAME_LENGTH
	MOV EDI, LOADER_FILENAME
	MOV EDX, ESI
	DB 0XF3 , 0XA6	; repe cmpsb byte ptr [esi], byte ptr es:[edi]
	JECXZ FILE_FOUND
	;ELSE: (IF FILE NOT FOUND...)
	MOV ESI, EDX
	ADD ESI, SFNSIZE

	MOVZX EDX, BYTE [BPB_SECPERCLUS]
	ADD EDX, ROOTDIRBASE
	CMP EDX, ESI
	;JE FILE_LOST
	JE $
	JMP .COMPARE_LOOP


FILE_FOUND:		;IF FILE FOUND:LOAD THE FAT TABLE
.GET_FIRSTCLUSTER:
	;ESI IS NOW THE FILE ENTRY
	;THE START OF THE SFN ENTRY (OFFSET 0)=ESI-11
	;DIR_FSTCLUSHI STARTS FROM OFFSET 20, SIZE 2 (16BITS)
	;DIR_FSTCLUSLO STARTS FROM OFFSET 26, SIZE 2 (16BITS)
	;ALL IN ALL:
	;DIR_FSTCLUSHI=ESI-11+20=ESI+9
	;DIR_FSTCLUSLO=ESI-11+26=ESI+15
	MOV ESI, EDX
	MOV DX, WORD [ESI+20]
	SHL EDX, 16
	MOVZX ECX, WORD [ESI+26]
	ADD EDX, ECX






;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
LOAD_FATTABLE:					;IF THE FILE IS FOUND, THAN THE FAT TABLE SHOULD BE LOADED, OTHERWISE, THERE IS NO NEED TO LOAD THE TABLE
;FUNCTION: "READ_ATA"
;EAX:SECTOR OFFSET
;EBX:BUFFER LOCATION
;CX:NUMBER OF SECTORS TO READ
MOVZX EAX, WORD [BPB_RESVDSECCNT]
MOV EBX, FATBASE
;UNFORTUNATELY, THE INTURRUPT FUNCTION ONLY PROVIDES 16-BIT SECTOR COUNT(WHICH IS STORE IN CX REGISTER)
;SO, IF THE FAT TABLE IS LARGE, THE UPPER PART WILL NOT BE LOADED INSIDE MEMORY
;PLUS, THE CONVENTIONAL MEMORY (0X7E00~0X100000) IS ONLY ABLE TO HELD 1985 SECTORS

;CHECK THE FATSZ(NOTE:IN SECTOR OF 512)
CMP EAX, 1985
JBE .SMALL_FAT
;ELSE: THE FAT IS BIGGER THAN 1985 SECTORS
;THEN: LOAD THE FIRST 1985 SECTORS FROM THE DISK
MOV EAX, 1985

.SMALL_FAT:
MOV ECX, DWORD [BPB_FATSZ32]

CALL READ_ATA



LINK_FATCHAIN:
MOV SI, FATCHAINBASE	;SI IS ENOUGH FOR THE LOWER 1MB
MOV EBP, LOAD_LOADER
;FUNCTION "NXT_CLUSTER"
;EDX:CLUSTER NUMBER (AS A CALLER, THE CLUSTER NUMBER STARTS FROM 0)
;EBP:WHERE TO GO IF RECIEVE "EOF" VALUE

;RETURNS:
;EDX:NEXT CLUSTER NUMBER (THE CLUSTER NUMBER STARTS FROM 0)

;NECESSARY CONDITIONS:
;MACRO "FATBASE" HAVE TO BE DEFINED

;EDX IS NOW THE FIRST CLUSTER OF THE FILE, NOT SET AGAIN NOW
%DEFINE FAT_ENTRY_SIZE 4
PUSH WORD LOAD_LOADER
MOV BP, WORD PREPARE_LOAD_LOADER	;IF THE CHAIN HITS EOF, THAN GO TO THE NEXT STEP "LOAD_LOADER"
MOV EAX, EDX				;BOTH CMP_STOP AND NXT_CLUSTER TAKE EAX AS ARGUMENT
.CREATE_CHAIN:
	MOV DWORD [SI], EAX
	ADD SI, FAT_ENTRY_SIZE

	CALL NXT_CLUSTER
	CALL CMP_STOP			;FIXED
	JMP .CREATE_CHAIN



;;;;;;;;;;;;;;;;;;;;;;

PREPARE_LOAD_LOADER:
MOV EBX, BUFFERBASE		;LOADERBASE ACTUALLY
MOV SI, FATCHAINBASE
LOAD_LOADER:
	;FIRST TWO CLUSTERS HAVE BEEN IGNORED???
	;;;;;;;;;;;;LAB;;;;;;;;;;;;;;;
	MOV EAX, DWORD [SI]
	CALL CLUSTER2SECTOR
	MOVZX CX, BYTE [BPB_SECPERCLUS]
	CALL READ_ATA
	ADD SI, 4
	MOV EAX, DWORD [SI]
	MOV BP, WORD BUFFERBASE
	CALL CMP_STOP
	;IF NOT EOF...

	MOVZX EBP, BYTE [BPB_SECPERCLUS]
	SHL EBP, 9	;*512(SECTOR SIZE IN BYTES)
	ADD EBX, EBP

	JMP LOAD_LOADER

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



;=========================
[SECTION .DATA]
FIRSTDATASECTOR: DD 0
LOADER_FILENAME: DB "KERNEL  SYS"
[SECTION SUBROUTINE]
%INCLUDE "MAIN32.INC"