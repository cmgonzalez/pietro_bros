#ifndef MACROS_H
#define MACROS_H

#define BIT_SET(a, b)(a |=  (1 << b))	/* a=target variable, b=bit number to act upon 0-n */ 
#define BIT_CLR(a, b)(a &= ~(1 << b))	/* a=target variable, b=bit number to act upon 0-n */ 
#define BIT_FLP(a, b)(a ^=  (1 << b))	/* a=target variable, b=bit number to act upon 0-n */ 
#define BIT_CHK(a, b)(a &   (1 << b))	/* a=target variable, b=bit number to act upon 0-n */
//#define CALC_INDEX(l, c)(c+ ((l >> 3) << 5))
#endif
