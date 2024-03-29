/*
	This file is part of Pietro Bros.

	Pietro Bros is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Pietro Bros is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Pietro Bros.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef PIETRO_AY_H
#define PIETRO_AY_H

// Returned from ay_is_playing()

#define AY_PLAYING_NONE          0
#define AY_PLAYING_BACKGROUND    1
#define AY_PLAYING_FOREGROUND    2
#define AY_PLAYING_FX            2
#define AY_PLAYING_MUSIC         3

// AY Control

extern void              ay_reset(void) __preserves_regs(b,c);
extern unsigned char     ay_is_playing(void) __preserves_regs(b,c,d,e);
extern void              ay_midi_play(void *song) __preserves_regs(b,c) __z88dk_fastcall;
extern void              ay_fx_play(void *effect) __preserves_regs(b,c) __z88dk_fastcall;

// AY Midi Music

extern unsigned char pb_midi_title[];                 // title music
extern unsigned char pb_midi_phase_1[];               // phase 1 preamble

// AY Sound Effects

extern unsigned char ay_effect_01[]; //SLIDE
extern unsigned char ay_effect_02[]; //FLIP ENEMY
extern unsigned char ay_effect_03[]; //JUMP
extern unsigned char ay_effect_04[]; //SHELLCREPPER ENTER
extern unsigned char ay_effect_05[]; //SIDESTEPPER ENTER
extern unsigned char ay_effect_06[]; //HIT POW
extern unsigned char ay_effect_07[]; //COIN ENTER
extern unsigned char ay_effect_08[]; //WATER SPLASH
extern unsigned char ay_effect_09[]; //COIN ON BONUS
extern unsigned char ay_effect_10[]; //HIT COIN
extern unsigned char ay_effect_11[]; //ENEMY KILL
extern unsigned char ay_effect_12[]; //HIT SLIPICE
extern unsigned char ay_effect_13[]; //FIGHTER FLY JUMP
extern unsigned char ay_effect_14[]; //PAUSE NOT USED YET, SHOULD WE IMPLEMENT PAUSE?
extern unsigned char ay_effect_15[]; //PLAYER RESTART
extern unsigned char ay_effect_16[]; //PLATFORM FREZE
extern unsigned char ay_effect_17[]; //LAST ENEMY KILL
extern unsigned char ay_effect_18[]; //PLAYER HIT
extern unsigned char ay_effect_19[]; //BONUS TIMER (LOOPING)
extern unsigned char ay_effect_20[]; //WALKING (LOOPING)

#endif
