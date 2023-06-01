#include "reg_irq.h"
#include "printf.h"
#include "asm.h"

/*

idea about scheduling:

the handler will use a global variable, which will hold how many time slice left for the current process
once the counter reaches zero, scheduler loads the time slices number of next process to be scheduled


*/


#define SCHEDULE

/*

should context structure uses rounded doubly link list? or a rounded singly link list?
    - a round doubly link list?

0 <-> 1 <-> 2
^           ^
|           |
\----------/


context operations:
    insert
    delete
    a pointer to the last context is needed (FILO structure)
        push
        pop
    grouping the context (priority)
*/

//context (pcb) definition should be in "pcb.h" under the same directory

//the context should

//printf("registering interrupt handler for context switching\n");


void TestContextSwitchHandler() {

}

void ContextSwitch(/* a pointer to the current pcb struct */) {
    cli();
    //do some context switching stuff here
    sti();
    return;

} 