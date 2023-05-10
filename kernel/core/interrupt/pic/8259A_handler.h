#ifndef __8259A_HANDLER_H__
#define __8259A_HANDLER_H__

extern void Register8259AIrq();
extern void SetMasterIcw();
extern void SetSlaveIcw();
extern void SetMasterOcw();
extern void SetSlaveOcw();
extern inline void InitMasterOcw1();
extern inline void InitSlaveOcw1();


#endif