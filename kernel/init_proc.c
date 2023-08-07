 extern void init_proc() {
    __asm__ __volatile__ ("jmp .":::"memory");  
    return;
}