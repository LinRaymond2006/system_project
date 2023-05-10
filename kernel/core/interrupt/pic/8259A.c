#include "8259A_cw.h"
#include "reg_irq.h"
#include "linkage.h"
extern void SetMasterIcw(char Icw1, char Icw2, char Icw3, char Icw4) {
    //ICW1
    out_b(MASTER_ICW1_PORT, Icw1);
    //ICW2
    out_b(MASTER_ICW2_PORT, Icw2);          //inturrupt number
    //ICW3
    out_b(MASTER_ICW3_PORT, Icw3);       //if the bit is 1: there is a slave 8259A connected
    //ICW4
    if (Icw1 & ICW1_ICW4_PRESENTED) {
        out_b(MASTER_ICW4_PORT, Icw4);
    }
    return;
}
extern void SetSlaveIcw(char Icw1, char Icw2, char Icw3, char Icw4) {
    //ICW1
    out_b(SLAVE_ICW1_PORT, Icw1);
    //ICW2
    out_b(SLAVE_ICW2_PORT, Icw2);        //inturrupt number
    //ICW3
    out_b(SLAVE_ICW3_PORT, Icw3);       //which pin is connected to master 8259A
    //ICW4
    if (Icw1 & ICW1_ICW4_PRESENTED) {
        out_b(SLAVE_ICW4_PORT, Icw4);
    }
    return;
}

extern void SetMasterOcw(char Ocw1, char Ocw2, char Ocw3) {
    //OCW1
    out_b(MASTER_OCW1_PORT, Ocw1);
    //OCW2
    out_b(MASTER_OCW2_PORT, Ocw2);
    //OCW3
    out_b(MASTER_OCW3_PORT, Ocw3);
    return;

}
extern void SetSlaveOcw(char Ocw1, char Ocw2, char Ocw3) {
    //OCW1
    out_b(SLAVE_OCW1_PORT, Ocw1);
    //OCW2
    out_b(SLAVE_OCW2_PORT, Ocw2);
    //OCW3
    out_b(SLAVE_OCW3_PORT, Ocw3);
    return;
}
extern inline void InitMasterOcw1(char Ocw1) {
    out_b(MASTER_OCW1_PORT, Ocw1);
    return;
}
extern inline void InitSlaveOcw1(char Ocw1) {
    out_b(SLAVE_OCW1_PORT, Ocw1);
    return;
}