/* flag mask for 64-bit PAE paging */
/* macro definition will be expand during compile time */
/* 
 P : Present flag
    0 : invalid table
    1 : valid table
 RW : Read / Write flag
    0 : the page is marked as non-writable
    1 : the page is marked as writable
 US : User / Supervisor flag
    0 : supervisor page
    1 : user page
 PWT : Page Write-Through flag
    0 : the page is marked as write-through, memory modification will be writen through immediately
    1 : the page is marked as write-back, modification of memory will be cached, and write back
 PCD : Page Cache Disable flag
    0 : the page is marked as cacheable, the content will be stored in the processor's cache, make sure this bit is clear if the page will be accessed frequently
    1 : the page is marked as non-cacheable, the content will NOT be stored in processor's cache, set this bit if the page won't be accessed frequently
 A : Access flag
    0 : the page was accessed (regardless of read or write)
    1 : the page hasn't been accessed (regardless of read or write)
    note : this feature can be useful to swapping system (to determine wether the page was accessed)
 D : Dirty flag
 R : not sure what it is
 XD : eXecute Disable (NX)
    0 : the content in the page can be executed
    1 : the content in the page can not be executed
 G : global flag
    0 : processor will invalidate the TLB entry corresponding to the page upon a MOV to CR3 instruction
    1 : processor won't invalidate the TLB entry corresponding to the page upon a MOV to CR3 instruction
 PAT : Page Attribute Table, indicate the memory cache policy (if PAT feature is support)
 D : Drity flag
    0 : the page can be remove from the meory
    1 : the content in the page need to be write to disk before the page is evicted from the memory
    note: the feature is used by the memory swapping system to determine which page need to be saved to secondary storage device when the usable memory is low
 PDPTE_1G : 
    0 : the PDPTE is just a normal page table
    1 : the PDPTE is specifically designed for 1G page
 PDE_2M : 
    0 : the PDE is just a normal page table
    1 : the PDE is specifically designed for 2M page
*/

//***THE FILE HAVE TO BE REWRITE WITH MORE HUMAN-REABLE VARIABLE DEFINED DUE TO POOR READABILITY***


/* ======================================================== */
/* PML4E FLAGS */
.set PML4E_P, 0b1
.set PML4E_RW, 0b10
.set PML4E_US, 0b100
.set PML4E_PWT, 0b1000
.set PML4E_PCD, 0b10000
.set PML4E_A, 0b100000
.set PML4E_R, 0b100000000000
.set PML4E_XD, 0b1000000000000000000000000000000000000000000000000000000000000000

/* ======================================================== */
/* PDPTE FLAGS */
.set PDPTE_P, 0b1
.set PDPTE_RW, 0b10
.set PDPTE_US, 0b100
.set PDPTE_PWT, 0b1000
.set PDPTE_PCD, 0b10000
.set PDPTE_A, 0b100000
.set PDPTE_R, 0b100000000000
.set PDPTE_XD, 0b1000000000000000000000000000000000000000000000000000000000000000

.set PDPTE_1G, 0b10000000
.set PDPTE_1G_G, 0b100000000
.set PDPTE_1G_PAT, 0b1000000000000

/* ======================================================== */
/* PDE FLAGS */
.set PDE_P, 0b1
.set PDE_RW, 0b10
.set PDE_US, 0b100
.set PDE_PWT, 0b1000
.set PDE_PCD, 0b10000
.set PDE_A, 0b100000
.set PDE_XD, 0b1000000000000000000000000000000000000000000000000000000000000000
.set PDE_R, 0b100000000000

.set PDE_2M_D, 0b1000000
.set PDE_2M, 0b10000000
.set PDE_2M_G, 0b100000000
.set PDE_2M_PAT, 0b1000000000000



/* ======================================================== */
/* PTE FLAGS */
/* note:PTE are exclusive to 4K pages, pages with size of 2M or 1G won't use the PTE table */

.set PTE_P, 0b1
.set PTE_RW, 0b10
.set PTE_US, 0b100
.set PTE_PWT, 0b1000
.set PTE_PCD, 0b10000
.set PTE_A, 0b100000
.set PTE_D, 0b1000000
.set PTE_PAT, 0b10000000
.set PTE_G, 0b100000000
.set PTE_R, 0b100000000000
.set PTE_XD, 0b1000000000000000000000000000000000000000000000000000000000000000
