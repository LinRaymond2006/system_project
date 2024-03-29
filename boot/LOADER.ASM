%INCLUDE "ADDR_CFG.INC"
%INCLUDE "FAT32HEADERS_ADDR.INC"
%DEFINE TMP_PAGE_TABLE_BASE (0X90000)

%DEFINE DATASZ32 DB 0X66

[BITS 16]
[ORG LOADERBASE]
INIT:
    MOV SP, 0X7C00      ;RESET THE STACK    

DO_ENABLE_A20_FASTGATE:
	IN AL, 0X92
	OR AL, 00000010B
	OUT 0X92, AL







CALL GET_MEM_STRUCT
;+-----------+------+---------+----------------------+-------------+
;| SVGA MODE | ROWS | COLUMNS | FRAME BUFFER ADDRESS | PIXEL WIDTH |
;+-----------+------+---------+----------------------+-------------+
;|   0X180   | 1440 |   900   | 0XE0000000 (PHYSICAL)| 32BIT(4BYTE)|
;+-----------+------+---------+----------------------+-------------+
;|   0X143   |  800 |   600   | 0XE0000000 (PHYSICAL)| 32BIT(4BYTE)|
;+-----------+------+---------+----------------------+-------------+


;                             0X180
;++========================1440=PIXELS===========================>
;||
;||
;||
;||
;||
;||9
;||0
;||0
;||
;||P
;||I
;||X
;||E
;||L
;||S
;||
;\/

CALL GRAB_VBE_SVGA_INFO

SETUP_PMODE:

    DATASZ32
    LGDT [GDT32PTR]

    DATASZ32
    LIDT [IDT32PTR]
    MOV EAX, CR0
    OR EAX, 1
    MOV CR0, EAX
    JMP DWORD SELECTOR_CODE32:PMODE_ENTRY

[BITS 32]

%define ROOTDIRBASE 0x100000
;the FAT table will be temporarily located at 0x100000, the FATchain will be
;stored at 0x500, so there is no need to worried about the memory space!
%define FATBASE 0x100000
%define FATCHAINBASE 0x500
%DEFINE SFNSHIFT 5
%define FILENAME_LENGTH 11
%define SFNSIZE 32
%DEFINE FAT_ENTRY_SIZE 4



PMODE_ENTRY:
	mov	eax, SELECTOR_DATA32
	mov	ds,	ax
	mov	es,	ax
	mov	fs,	ax
	mov	ss,	ax
	mov	esp, 0x7c00
	


;THE FOLLOWING CODE ARE MOSTLY COPIED FROM THE BOOT RECORD
;SOME FUNCTION INTERFACE MAY BE REPLACED WITH NEW ONES

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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
;EAX IS NOW FIRSTDATASECTOR, THEREFORE NOT SET AGAIN
.LOAD_INTO_MEM:

MOV EBX, ROOTDIRBASE
MOVZX ECX, BYTE [BPB_SECPERCLUS]
CALL READ_ATA32

;CX IS NOW BPB_SECPERCLUS
;CHECKOUT FILES IN THE FIRST CLUSTER (NOTE:ONLY SEARCH FOR THE FIRST ROOTDIRECTORY CLUSTER)
.COMPARE_FILENAME:
%DEFINE SFNSHIFT 5



.SETUP:
MOV ESI, ROOTDIRBASE
.COMPARE_LOOP:
	MOV ECX, FILENAME_LENGTH
	MOV EDI, KERNEL_FILENAME
	MOV EDX, ESI
	;THE ASSEMBLER I USE NOW (NASM) SEEMS NOT RECONIZING THE FOLLWING INSTRUCTION, SO I USE
	;HEX VALUE TO DO THE SAME THING
	DB 0XF3 ,0XA6	; REPE CMPSB BYTE PTR DS:[ESI], BYTE PTR ES:[EDI]
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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;GETTING THE FIRST CLUSTER OF THE GIVEN FILE HERE;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

MOV ESI, EDX
MOV DX, WORD [ESI+20]
SHL EDX, 16
MOVZX ECX, WORD [ESI+26]
ADD EDX, ECX

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
LOAD_FATTABLE:
;IF THE FILE IS FOUND, THAN THE FAT TABLE SHOULD BE LOADED, OTHERWISE, THERE IS NO NEED TO LOAD THE TABLE
MOVZX EAX, WORD [BPB_RESVDSECCNT]
MOV EBX, FATBASE
;UNFORTUNATELY, THE INTURRUPT FUNCTION ONLY PROVIDES 16-BIT SECTOR COUNT(WHICH IS STORE IN CX REGISTER)
;SO, IF THE FAT TABLE IS LARGE, THE UPPER PART WILL NOT BE LOADED INSIDE MEMORY
;PLUS, THE CONVENTIONAL MEMORY (0X7E00~0X100000) IS ONLY ABLE TO HELD 1985 SECTORS

;CHECK THE FATSZ(NOTE:IN SECTOR OF 512)
CMP EAX, 0XFFFFFFFF
JBE .SMALL_FAT
;ELSE: THE FAT IS BIGGER THAN 1985 SECTORS
;THEN: LOAD THE FIRST 1985 SECTORS FROM THE DISK
MOV EAX, 0XFFFFFFFF

.SMALL_FAT:
MOV ECX, DWORD [BPB_FATSZ32]
CALL READ_ATA32

%DEFINE FAT_ENTRY_SIZE 4


;I CALLED IT "FATCHAIN"
;THE CLUSTER NUMBER ARE STORED INSIDE FATCHAIN
LINK_FATCHAIN:
MOV ESI, FATCHAINBASE	;SI IS ENOUGH FOR THE LOWER 1MB
MOV EBP, LOAD_KERNEL

;EDX IS NOW THE FIRST CLUSTER OF THE FILE, NOT SET AGAIN NOW
MOV EBP, DWORD PREPARE_LOAD_LOADER	;IF THE CHAIN HITS EOF, THAN GO TO THE NEXT STEP "LOAD_LOADER"
MOV EAX, EDX				;BOTH CMP_STOP AND NXT_CLUSTER TAKE EAX AS ARGUMENT
.CREATE_CHAIN:
	SUB EAX, DWORD [BPB_ROOTCLUS]

	MOV DWORD [ESI], EAX
	ADD ESI, FAT_ENTRY_SIZE
	ADD EAX, DWORD [BPB_ROOTCLUS]

	CALL NXT_CLUSTER32
	CALL CMP_STOP32

	JMP .CREATE_CHAIN


;NOTE: THE SECTOR NUMBER TO BE READ=FIRSTDATASECTOR+(VALUE_IN_FATCHAIN*BPB_SECPERCLUS)
PREPARE_LOAD_LOADER:
MOV DWORD [ESI], 0XFFFFFFFF
;FINISH THE FATCHAIN, SET A 'STOP' SIGN THERE
MOV EBX, KERNELBASE
MOV ESI, FATCHAINBASE


LOAD_KERNEL:

	MOV EAX, DWORD [ESI]
	CALL CLUSTER2SECTOR32
	MOVZX CX, BYTE [BPB_SECPERCLUS]
	CALL READ_ATA32
	ADD ESI, 4
	MOV EAX, DWORD [ESI]
	MOV EBP, DWORD SETUP_KERNEL

	;TWEAKED VERSION
	;THE "TWEAKED" VERSION RECOGNIZE THE VALUE 0XFFFFFFFF AS EOF
	CMP EAX, 0XFFFFFFFF
	JNE .2
	.1:
	;JMP EBP
	PUSH EBP
	RET

	.2:
	;IF NOT EOF...

	;POINTER TO CURRENT BUFFER+=(BPB_SECPERCLUS*512)
	MOVZX EBP, BYTE [BPB_SECPERCLUS]
	SHL EBP, 9	;*512(SECTOR SIZE IN BYTES)
	ADD EBX, EBP

	JMP LOAD_KERNEL

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
;the code above are tested, and no bugs are found


;SETUP SOME NECESSARY FEATURES FOR KERNEL TO USE
;FEATURES TO BE SET UP:
;	> IS CPUID INSTRUCTION SUPPORTED, IF NOT, HALT THE SYSTEM
;	> QUERY WHETHER THE LONG MODE SUPPORT WITH CPUID
;	> SETUP TEMPORARY PAGE TABLE
;	> LONG 64-BIT GDT INTO GDTR
;	> SET PAE BITS IN CR4
;	> LOAD BASE ADDRESS OF PAGE TABLE INTO CR3 REGISTER
;	> READ MSR REGISTER AND SET IA32_EFER BIT, THAN WRITE THE NEW VALUE BACK INTO THE MSR REGISTER
;	> OPEN PE BIT (PROTECT MODE ENABLED) AND PAGING BIT IN CR0 IN CASE THEY ARE NOT CORRECTLY ENABLED
;	> FLUSH THE CODE SELECTOR AND GOTO LONG MODE
SETUP_KERNEL:

CHECK_PROCESSOR_FEATURES:
	
	CALL DETECT_CPUID_SUPPORT
	
	CALL DETECT_LONG_MODE_SUPPORT

;THE FOLLOWING CODE FOR SETTING UP PAGE TABLE ARE MOSTLY CLONED FROM BOOK <<一个64位操作系统的设计与实现>> (ISBN 9787115475251)
;**TEMPORARILY** PAGE TABLE, THE KERNEL WILL SET UP A NEW ONE 
LOAD_TMP_PAGE_TABLE:
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    MOV	DWORD [TMP_PAGE_TABLE_BASE+0X0],0X91007
    MOV	DWORD [TMP_PAGE_TABLE_BASE+0X4],0X00000
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    MOV	DWORD [TMP_PAGE_TABLE_BASE+0X800],0X91007
    MOV	DWORD [TMP_PAGE_TABLE_BASE+0X804],0X00000
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    MOV	DWORD [TMP_PAGE_TABLE_BASE+0X1000],0X92007
    MOV	DWORD [TMP_PAGE_TABLE_BASE+0X1004],0X00000
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    MOV	DWORD [TMP_PAGE_TABLE_BASE+0X2000],0X000083
    MOV	DWORD [TMP_PAGE_TABLE_BASE+0X2004],0X000000
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    MOV	DWORD [TMP_PAGE_TABLE_BASE+0X2008],0X200083
    MOV	DWORD [TMP_PAGE_TABLE_BASE+0X200C],0X000000
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    MOV	DWORD [TMP_PAGE_TABLE_BASE+0X2010],0X400083
    MOV	DWORD [TMP_PAGE_TABLE_BASE+0X2014],0X000000
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    MOV	DWORD [TMP_PAGE_TABLE_BASE+0X2018],0X600083
    MOV	DWORD [TMP_PAGE_TABLE_BASE+0X201C],0X000000
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    MOV	DWORD [TMP_PAGE_TABLE_BASE+0X2020],0X800083
    MOV	DWORD [TMP_PAGE_TABLE_BASE+0X2024],0X000000
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    MOV	DWORD [TMP_PAGE_TABLE_BASE+0X2028],0XA00083
    MOV	DWORD [TMP_PAGE_TABLE_BASE+0X202C],0X000000
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

FLUSH_DESCRIPTOR:
	;THE NASM ASSEMBLER SEEMS NOT TO ADD A 0X66 BEFORE LGDT INSTRUCTION
	;SO I ADD IT MANUALLY
    DATASZ32
	LGDT [GDT64PTR]
	MOV	AX, 0x10
	MOV	DS,	AX
	MOV	ES,	AX
	MOV	FS,	AX
	MOV	GS,	AX
	MOV	SS,	AX
ENABLE_PAE:
	MOV	EAX, CR4
	BTS	EAX, 5
	MOV	CR4, EAX
LOAD_PAGE_TABLE:
	MOV	EAX, TMP_PAGE_TABLE_BASE
	MOV	CR3, EAX

%DEFINE IA32_EFER 0x0C0000080
%DEFINE IA32_EFER_LMODE 8
GO_TO_LONG_MODE:
	MOV	ECX, IA32_EFER
	RDMSR
	BTS	EAX, IA32_EFER_LMODE
	WRMSR

%DEFINE CR0_PE_BIT 0
%DEFINE CR0_PAGING_BIT 31
CHECK_AGAIN:
	MOV EAX, CR0
	BTS	EAX, 0
	BTS	EAX, 31
	MOV CR0, EAX
GOTO_KERNEL:
	JMP SELECTOR_CODE64:KERNELBASE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[SECTION DATA]
MEM_STRUCT_COUNT: DB 0
FIRSTDATASECTOR: DD 0
WORD_PRINTED: DW 0
CURSOR_PTR: DW 0
FATCHAIN_COUNT: DD 0
[SECTION .STRINGS]
KERNEL_FILENAME: DB "KERNEL  SYS"

;END OF STRING
%DEFINE EOS (0X00)
%DEFINE CHAR_SPEC_N 10
%DEFINE CHAR_SPEC_B 8

[SECTION .GDT]
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

LABEL_GDT32:  DD 0,0
LABEL_DESCRIPTOR_CODE32:    DD 0X0000FFFF, 0X00CF9A00
LABEL_DESCRIPTOR_DATA32:    DD 0X0000FFFF, 0X00CF9200
LABLE_GDT32_END:
GDT32LEN    EQU     (LABLE_GDT32_END - LABEL_GDT32)
GDT32LIM    EQU     (GDT32LEN-1)
GDT32PTR:
    DW GDT32LIM
    DD LABEL_GDT32
SELECTOR_CODE32 EQU     (LABEL_DESCRIPTOR_CODE32-LABEL_GDT32)
SELECTOR_DATA32 EQU     (LABEL_DESCRIPTOR_DATA32-LABEL_GDT32)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
LABEL_GDT64:DQ 0X0000000000000000
LABEL_DESCRIPTOR_CODE64:    DQ 0X0020980000000000
LABEL_DESCRIPTOR_DATA64:    DQ 0X0000920000000000
LABLE_GDT64_END:
GDT64LEN    EQU     (LABLE_GDT64_END-LABEL_GDT64)
GDT64LIM    EQU     (GDT64LEN-1)
GDT64PTR:
    DW GDT64LIM
    DD LABEL_GDT64
SELECTOR_CODE64 EQU (LABEL_DESCRIPTOR_CODE64-LABEL_GDT64)
SELECTOR_DATA64 EQU (LABEL_DESCRIPTOR_DATA64-LABEL_GDT64)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[SECTION .IDT]
LABEL_IDT32:
    TIMES 0X50 DQ 0
LABEL_IDT32_END:
IDT32LEN    EQU     (LABEL_IDT32_END - LABEL_IDT32)
IDT32LIM    EQU     (IDT32LEN-1)
IDT32PTR:
    DW IDT32LIM
    DD LABEL_IDT32



[SECTION SUBROUTINE]

%INCLUDE "MAIN32.INC"