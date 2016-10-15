#ifndef PIETRO_MIDI_H
#define PIETRO_MIDI_H

// midi player and data

extern void *midi_position;                            // currently playing song, zero if none
extern void  midi_play_isr(void);                      // midi player interrupt service routine

#ifdef __SDCC
extern void midi_play(void *song) __z88dk_fastcall;    // reset AY and play song
#endif

#ifdef __SCCZ80
extern void __FASTCALL__ midi_play(void *song);        // reset AY and play song
#endif

// midi songs

extern unsigned char mb_midi_title[];                  // title music

#endif
