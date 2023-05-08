#include "8259A_cw.h"

void SetMasterIrq(char Irq, char Icw1, char PinMap, char Icw4) {
    //ICW1
    out_b(MASTER_ICW1_PORT, Icw1);
    //ICW2
    out_b(MASTER_ICW2_PORT, Irq);          //inturrupt number
    //ICW3
    out_b(MASTER_ICW3_PORT, PinMap);       //if the bit is 1: there is a slave 8259A connected
    //ICW4
    if (Icw1 & ICW1_ICW4_PRESENTED) {
        out_b(MASTER_ICW4_PORT, Icw4);
    }
    return;
}
void SetSlaveIrq(char Irq, char Icw1, char CascadedPin, char Icw4) {
    //ICW1
    out_b(SLAVE_ICW1_PORT, Icw1);
    //ICW2
    out_b(SLAVE_ICW2_PORT, Irq);                //inturrupt number
    //ICW3
    out_b(SLAVE_ICW3_PORT, CascadedPin);       //which pin is connected to master 8259A
    //ICW4
    if (Icw1 & ICW1_ICW4_PRESENTED) {
        out_b(SLAVE_ICW4_PORT, Icw4);
    }
    return;
}

void ConfigMasterOcw(char Ocw1, char Ocw2, char Ocw3) {
    //OCW1
    out_b(MASTER_OCW1_PORT, Ocw1);
    //OCW2
    out_b(MASTER_OCW2_PORT, Ocw2);
    //OCW3
    out_b(MASTER_OCW3_PORT, Ocw3);

}
void ConfigSlaveOcw(char Ocw1, char Ocw2, char Ocw3) {
    //OCW1
    out_b(SLAVE_OCW1_PORT, Ocw1);
    //OCW2
    out_b(SLAVE_OCW2_PORT, Ocw2);
    //OCW3
    out_b(SLAVE_OCW3_PORT, Ocw3);
}