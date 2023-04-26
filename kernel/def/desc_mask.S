//SEGMENT DESCRIPTOR


//ACCESS RIGHTS FIELD ("ACCESS" FIELD IN DESCRIPTOR)

//present bit, must be set in a valid descriptor
.set _SEGDESC_ACCESS_P,           1<<47
.set VALID_SEGDESC,               _SEGDESC_ACCESS_P

//privillage of descriptor
.set _SEGDESC_ACCESS_DPL_0,       0<<45
.set _SEGDESC_ACCESS_DPL_1,       1<<45
.set _SEGDESC_ACCESS_DPL_2,       2<<45
.set _SEGDESC_ACCESS_DPL_3,       3<<45
.set SEGDESC_DPL_SYS,              _SEGDESC_ACCESS_DPL_0
.set SEGDESC_DPL_USR,              _SEGDESC_ACCESS_DPL_3

//descriptor type
.set _SEGDESC_ACCESS_S_SEG,       1<<44
.set _SEGDESC_ACCESS_S_SYSDESC,   0<<44
.set DESC_GDT,                    _SEGDESC_ACCESS_S_SEG
.set DESC_TSS,                    _SEGDESC_ACCESS_S_SYSDESC

//executable
.set _SEGDESC_ACCESS_E,           1<<43
.set EXEC,                        _SEGDESC_ACCESS_E

//direction/conform
/*
data selectors:
    If set, the segment grows down ( offset > limit )
    Otherwise the flag is cleared, segment grows up like a normal segment
    using flat model, they should be the same, unconfirmed (?

code selectors, use the bit for determining whether the segment is executable for sepecific DPL:
    Set, indicating a conforming code segment:
        A conforming code segment, can only be executed as lower or equal privillages
        for example: a conforming segment of dpl 1 can be executed with DPL1, 2, and 3. but not accessable for DPL0, which have higher privillage
    Cleared, indicating a non-conforming segment:
        the segment can only be executed with the same ring set in the DPL field of the access subfield in the desriptor.
*/
.set _SEGDESC_ACCESS_DC,                         1<<42
.set SEG_DATA_DOWN,                              _SEGDESC_ACCESS_DC
.set SEG_DATA_UP,                                0<<42
.set SEG_CODE_CONFORMED,                         _SEGDESC_ACCESS_DC
.set SEG_CODE_NONCONFIRMED,                      0<<42

//readable/writable
/*
code segments:
    if cleared, read access is not allowd.
    if set, read is allowed.
    note that writint to a code segment is never allowed
data segments:
    if cleared, the segment is read-only.
    if set, the segment is writable.
*/
.set _SEGDESC_ACCESS_RW,          1<<41
.set SEG_CODE_READBALE,                      _SEGDESC_ACCESS_RW
.set SEG_CODE_UNREADBALE,                    0<<41
.set SEG_DATA_WRITABLE,                      _SEGDESC_ACCESS_RW
.set SEG_DATA_RO,                            0<<41

//access bit
//processor will automatically set it once it's been accessed.
.set SEGDESC_ACCESS_A,                       1<<40





//DESCRIPTOR FLAGS ("FLAG" FIELD IN DESCRIPTOR)

//granularity
//the limit will be scaled by 4kib (a standard page size) if set, or the limit will just be scaled by 1 byte
.set _SEGDESC_FLAG_G,                         1<<55
.set LIMIT_SCALE_1B,                          0<<55
.set LIMIT_SCALE_4K,                          _SEGDESC_FLAG_G

//0 for 16-bit real mode, 1 for protected mode.
.set _SEGDESC_FLAG_DB,                        1<<54
.set _SEGDESC_FLAG_DB_16,                     0<<54
.set _SEGDESC_FLAG_DB_32,                     1<<54

//0 for 32-bit protected mode segment, 1 for 64-bit long mode segment. note that the L bit should also match DB bit

.set _SEGDESC_FLAG_L,                         1<<53
.set _SEGDESC_FLAG_L_32,                      0<<53
.set _SEGDESC_FLAG_L_64,                      1<<53

//use these pre-defined stuff, misconfiguration could lead to system failure
.set SEG_16BIT,                               _SEGDESC_FLAG_DB_16
.set SEG_32BIT,                               _SEGDESC_FLAG_DB_32 | _SEGDESC_FLAG_L_32
.set SEG_64BIT,                               _SEGDESC_FLAG_L_64

//set some more pre-defined stuff in case later program misconfigured something
//do it someday





//SYSTEM DESCRIPTOR (NOT USING LDT NOW, BUT AT LEAST ONE TSS HAVE TO BE DEFINED USED FOR SWITCHING RING SET PRIVILLAGES)

//since i am working on a 64 bit system, 32 bit value will be commented out
//do not mix segment descriptor and system descriptor up, there are several conflict definitions for the same subfield

.set _SYSDESC_ACCESS_P,           1<<47
.set VALID_TSS,               _SYSDESC_ACCESS_P

.set _SYSDESC_ACCESS_DPL_0,       0<<45
.set _SYSDESC_ACCESS_DPL_1,       1<<45
.set _SYSDESC_ACCESS_DPL_2,       2<<45
.set _SYSDESC_ACCESS_DPL_3,       3<<45
.set TSS_DPL_SYS,              _SYSDESC_ACCESS_DPL_0
.set TSS_DPL_USR,              _SYSDESC_ACCESS_DPL_3

//"DESC_TSS" is defined before

.SET _SYSDESC_ACCESS_TYPE_BUSY_TSS_64 0xb
.SET _SYSDESC_ACCESS_TYPE_AVAILABLE_TSS_64 0x9
.SET _SYSDESC_ACCESS_TYPE_LDT 0x2


.set BUSY_TSS64         _SYSDESC_ACCESS_TYPE_BUSY_TSS_64<<40
.set AVAILABLE_TSS64    _SYSDESC_ACCESS_TYPE_AVAILABLE_TSS_64<<40
.set SYSDESC_LDT        _SYSDESC_ACCESS_TYPE_LDT<<40

//set some pre-defined stuff here, working on it later