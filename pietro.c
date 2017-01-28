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


	Pietro Bros - Cristian Gonzalez - cmgonzalez@gmail.com

	This program can be compiled in Windows (with z88dk installed on c:\z88dk) as follows:
	
	zcompile.bat 
	 
	options 
	zcompile		  - Normal compile max-allocs-per-node200000
	zcompile fast     - Faster compile max-allocs-per-node10000 for debug purposes.
	zcompile pentagon - Pentagon Build.
 

 */


/*###############################################################################################
  #																								#
  # NOTES																						#
  #-SCORE TABLE--------------------------------------											#
  #| Flipping an enemy | 10 |																	#
  #| Kicking an enemy off the floor | 800 |														#
  #| Hitting Slipice | 500 |																	#
  #| Hitting a Red Fireball | 1000 |															#
  #| Hitting a Green Fireball | 200 |															#
  #| Bonus Coin | 800 |																			#
  #| Collecting all Bonus Coins (1st phase) | 3000 |											#
  # Collecting all Bonus Coins (2nd phase+) | 5000 |											#
  # =================================================											#
  #| Extra Life - 20000 points |																#
  # -------------------------------------------------											#
  #																								#
  # A-Game - When enemy gets knocked on its back, it takes 20 seconds for						#
  # it to change color and get back up															#
  # B-Game - When enemy gets knocked on its back, it takes 15 seconds for						#
  # it to change color and get back up															#
  # A-Game - Fireballs start to appear 80 seconds after the level begins						#
  # B-Game - Fireballs start to appear 60 seconds after the level begins						#
  #																								#
  ###############################################################################################*/

/*###############################################################################################
  #																								#
  # MAIN																						#
  #																								#
  ###############################################################################################*/


#include <arch/zx.h>
#include <input.h>
#include <stdlib.h>
#include "nirvana+.h"
#include "pietro.h"
#include "pietro_ay.h"
#include "pietro_game.h"
#include "pietro_sprite.h"
#include "pietro_player.h"
#include "pietro_enemies.h"
#include "pietro_zx.h"
#include "macros.h"

int main(void) {
	unsigned int counter;
	
	//INTERRUPTS ARE DISABLED
	
	//RESET AY CHIP
	ay_reset(); 
	
	//ATTRIB NORMAL
	
	attrib[0]= BRIGHT | PAPER_BLACK | INK_WHITE;
	attrib[1]= BRIGHT | PAPER_BLACK | INK_YELLOW;
	attrib[2]= PAPER_BLACK | INK_YELLOW;
	attrib[3]= PAPER_BLACK | INK_WHITE;
	
	//ATTRIB HIGHLIGHT
	attrib_hl[0]= PAPER_BLACK | INK_RED;
	attrib_hl[1]= PAPER_BLACK | INK_YELLOW;
	attrib_hl[2]= PAPER_BLACK | INK_GREEN;
	attrib_hl[3]= PAPER_BLACK | INK_CYAN;
	
	//GAME OPTIONS
	
	//ENABLE SOUND BASED ON DETECTED MODEL
	
	game_sound = spec128 ? (GAME_SOUND_AY_FX_ON | GAME_SOUND_AY_MUS_ON) : (GAME_SOUND_48_FX_ON | GAME_SOUND_48_MUS_ON);
	
	//GAME TYPE A
	
	game_type = 0;
	
	player_joy[0] = 0; /* SJ1 */
	player_joy[1] = 1; /* SJ2 */
	
	//Keyboard Handling P1
	
	k1.fire	= IN_KEY_SCANCODE_m;
	k1.left	= IN_KEY_SCANCODE_o;
	k1.right = IN_KEY_SCANCODE_p;
	k1.up    = IN_KEY_SCANCODE_DISABLE;   // must be defined otherwise up is always true
	k1.down  = IN_KEY_SCANCODE_DISABLE;   // must be defined otherwise down is always true
	
	k2.fire	= IN_KEY_SCANCODE_z;
	k2.left	= IN_KEY_SCANCODE_q;
	k2.right = IN_KEY_SCANCODE_w;
	k2.up    = IN_KEY_SCANCODE_DISABLE;   // must be defined otherwise up is always true
	k2.down  = IN_KEY_SCANCODE_DISABLE;   // must be defined otherwise down is always true
	
	game_joystick_set_menu();

	zx_border(INK_BLACK);
	
	//Wait for Keypress and Randomize

	in_wait_nokey();
	for (counter = 0x1234; !in_test_key(); ++counter) ;
	srand(counter);

	//Init SCREEN
	spr_cortina_brick();
	//INIT GAME
	game_start_timer();
	//INIT NIRVANA
	NIRVANAP_tiles(btiles);
	NIRVANAP_start();
	frame_time = zx_clock();
	//GAME MENU
	game_menu();
	//GAME EXIT
	NIRVANAP_stop();
	return 0;
}

void test_func(void) {
			//tmp = jump_lin[sprite] - lin[sprite];
		
		sprite_lin_inc_mul = 2;
		if (player_jump_c[sprite] > 4 ) sprite_lin_inc_mul = 1;
		if (player_jump_c[sprite] > 8 ) sprite_lin_inc_mul = 0;
		if (player_jump_c[sprite] > 12) sprite_lin_inc_mul = 1;
		if (player_jump_c[sprite] > 14) sprite_lin_inc_mul = 2;
		
		
		
		if ( BIT_CHK(s_state, STAT_JUMP) ) {
			/* Jump Handling */
			
			if ( player_jump_c[sprite] < PLAYER_MAX_JUMP ) {
				spr_move_up();
			} else {
				spr_set_fall();		
			}
		} else {
			if ( BIT_CHK(s_state, STAT_FALL) ){
				/* Falling Handling */
				if ( spr_move_down() ) {
					BIT_CLR( state_a[sprite] , STAT_HITBRICK );
					player_jump_c[sprite] = 0;
					jump_lin[sprite] = 0;
					player_calc_slide(lin[sprite],col[sprite]);
					if ( BIT_CHK(s_state, STAT_DIRL) == BIT_CHK(s_state, STAT_DIRR) ) {
						sliding[index_player] = 0;
					}
				}
				
			}
		}
		sprite_lin_inc_mul = 0;
		++player_jump_c[sprite];
		player_move_horizontal();
}
