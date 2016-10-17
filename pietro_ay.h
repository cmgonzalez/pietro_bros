#ifndef PIETRO_AY_H
#define PIETRO_AY_H

// AY Control

#ifdef __LLVM

extern void              ay_reset(void);              // stop all ay sound and go silent
extern unsigned char     ay_is_playing(void);         // returns 0=none, 1=effect, 2=midi
extern void              ay_midi_play(void *song);    // stop all ay sound and start new midi song
extern void              ay_fx_play(void *effect);    // stop all ay sound and start new effect

#endif

#ifdef __SDCC

extern void              ay_reset(void) __preserves_regs(b,c);
extern unsigned char     ay_is_playing(void) __preserves_regs(b,c,h);
extern void              ay_midi_play(void *song) __preserves_regs(b,c) __z88dk_fastcall;
extern void              ay_fx_play(void *effect) __preserves_regs(b,c) __z88dk_fastcall;

#endif

#ifdef __SCCZ80

extern void              ay_reset(void);
extern unsigned char     ay_is_playing(void);
extern void __FASTCALL__ ay_midi_play(void *song);
extern void __FASTCALL__ ay_fx_play(void *effect);

#endif

// AY Midi Music

extern unsigned char pb_midi_title[];                 // title music
extern unsigned char pb_midi_phase_1[];               // phase 1 preamble

// AY Sound Effects

extern unsigned char ay_effect_01[];
extern unsigned char ay_effect_02[];
extern unsigned char ay_effect_03[]; //JUMP
extern unsigned char ay_effect_04[];
extern unsigned char ay_effect_05[];
extern unsigned char ay_effect_06[];
extern unsigned char ay_effect_07[];
extern unsigned char ay_effect_08[];
extern unsigned char ay_effect_09[];
extern unsigned char ay_effect_10[];
extern unsigned char ay_effect_11[];
extern unsigned char ay_effect_12[];
extern unsigned char ay_effect_13[];
extern unsigned char ay_effect_14[]; //PAUSE NOT USED
extern unsigned char ay_effect_15[];
extern unsigned char ay_effect_16[];
extern unsigned char ay_effect_17[];
extern unsigned char ay_effect_18[];
extern unsigned char ay_effect_19[]; //BONUS  TIMER??? CAN WE REPEAT IT IF STAGE = BONUS?

#endif
