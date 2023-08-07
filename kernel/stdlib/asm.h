#ifndef __ASM_H__
#define __ASM_H__

//N constrian: constant Data which can be loaded directly into the reigster
//there's no need to add a "mfence" instruction since in/out instructions themselves are acting as memory barriers
volatile extern inline char in_b(unsigned int Port);
volatile extern inline short in_w(unsigned int Port);
volatile extern inline int in_d(unsigned int Port);
volatile extern inline void in_l(unsigned int Port);
volatile extern inline void out_b(unsigned int Port,unsigned char Data);
volatile extern inline void out_w(unsigned int Port, unsigned short Data);
volatile extern inline void out_d(unsigned int Port,unsigned int Data);
volatile extern inline void out_l(unsigned int Port,unsigned int Data);
volatile extern inline sti();
volatile extern inline cli();


#endif