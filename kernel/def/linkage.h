#ifndef __LINKAGE_H__
#define __LINKAGE_H__

#define L1_ALIGNMENT 32

/* gcc compiler directive */
#define asmlinkage __attribute__ ( (regparm(0)) )
/* alignment definition */
#define CACHE_ALIGNED __attribute__ ( ( __aligned__(L1_ALIGNMENT) ) )

/* get symbol */
#define SYM(SYMBOL) SYMBOL
/* get name of a symbol, which is a string */
#define SYM_NAME(SYMBOL) #SYMBOL
/* define a label */
#define DEF_LABEL(LABEL_NAME) LABEL_NAME##:

/* define a GLOBAL label */
#define GLOBAL_LABEL(LABEL_NAME)                \
        .globl SYM_NAME(LABEL_NAME)                 \
        DEF_LABEL(LABEL_NAME)

#endif