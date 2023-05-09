#ifndef __GPIO_H__
#define __GPIO_H__

//N constrian: constant Data which can be loaded directly into the reigster
//there's no need to add a "mfence" instruction since in/out instructions themselves are acting as memory barriers
volatile extern inline char in_b(unsigned int Port) {
	char Data;
	__asm__ __volatile__("inb %1, %0"
				:"=a"(Data)
				:"Nd"(Port)
				:"memory");
	return Data;
}

volatile extern inline short in_w(unsigned int Port) {
    short Data;
    __asm__ __volatile__("inw %1, %0" : "=a"(Data) : "Nd"(Port):"memory");
    return Data;
}


volatile extern inline int in_d(unsigned int Port) {
	int Data;
	__asm__ __volatile__("inl %%dx, %0"
				:"=a"(Data)
				:"Nd"(Port)
				:"memory");
	return Data;
}
volatile extern inline void in_l(unsigned int Port) {} WEAK_FUNC_ALIAS(in_d);



volatile extern inline void out_b(unsigned int Port,unsigned char Data) {
	__asm__ __volatile__(	"outb	%0,	%%dx"
				:
				:"a"(Data),"Nd"(Port)
				:"memory");
}



volatile extern inline void out_w(unsigned int Port, unsigned short Data) {
    __asm__ __volatile__("outw %0, %1" : : "a"(Data), "Nd"(Port):"memory");
	return;
}


volatile extern inline void out_d(unsigned int Port,unsigned int Data) {
	__asm__ __volatile__(	"outl	%0,	%%dx	\n\t"
				:
				:"a"(Data),"Nd"(Port)
				:"memory");
	return;
}
volatile extern inline void out_l(unsigned int Port,unsigned int Data) {} WEAK_FUNC_ALIAS(out_d);


#endif