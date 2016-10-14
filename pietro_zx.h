#ifndef PIETRO_ZX
#define PIETRO_ZX

#include <arch/zx.h>
#include <intrinsic.h>
#include <z80.h>
#include "globals.h"

extern unsigned char pietro_font[];

#ifdef __SDCC

extern void            zx_print_bonus_time(unsigned char ui_row, unsigned char ui_col, unsigned int time) __z88dk_callee;
extern void            zx_print_chr(unsigned char ui_row, unsigned char ui_col, unsigned char val) __z88dk_callee;
extern void            zx_print_int(unsigned char ui_row, unsigned char ui_col, unsigned int val) __z88dk_callee;
extern void            zx_print_str(unsigned char ui_row, unsigned char ui_col, unsigned char *s) __z88dk_callee;

#endif

#ifdef __SCCZ80

extern void __CALLEE__ zx_print_bonus_time(unsigned char ui_row, unsigned char ui_col, unsigned int time);
extern void __CALLEE__ zx_print_chr(unsigned char ui_row, unsigned char ui_col, unsigned char val);
extern void __CALLEE__ zx_print_int(unsigned char ui_row, unsigned char ui_col, unsigned int val);
extern void __CALLEE__ zx_print_str(unsigned char ui_row, unsigned char ui_col, unsigned char *s);

#endif

extern void          debug(int i);
extern void          debug2(int i);
extern void          debug3(int i);

// INLINED

#define zx_clock()          intrinsic_load16(_curr_time)
#define zx_paper_fill(a)    zx_cls(a)
#define zx_print_ink(a)     (screen_ink = (a))
#define zx_print_paper(a)   (screen_paper = (a))

#endif
