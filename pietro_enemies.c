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
#include <stdlib.h>
#include "nirvana+.h"
#include "pietro.h"
#include "pietro_enemies.h"
#include "pietro_ay.h"
#include "pietro_game.h"
#include "pietro_player.h"
#include "pietro_sprite.h"
#include "pietro_sound.h"
#include "pietro_zx.h"
#include "macros.h"


void enemy_coin1(void){
	//COINS N SLIPICE
	if ( phase_left > 0 ) {
		enemy_walk();
		if ( ( lin[sprite] == GAME_LIN_FLOOR )  && ( col[sprite] == 3 || col[sprite] == 27) ) {
			spr_destroy(sprite);
		}
	}
}

void enemy_coin2(void){
	//FIXED COINS
	++colint[sprite]; //ONLY ROTATE SPRITE
	if (colint[sprite] == SPR_COLINT) {
		colint[sprite] = 0;
	}
}


void enemy_collition(void) {
	for (enemies = 0; enemies < SPR_P2 ; ++enemies ) { 
		if ( enemy_collition_check() ) { 
			
			//TURN OTHER ENEMY (enemies)
			if ( !BIT_CHK(state[enemies], STAT_HIT) &&
				 BIT_CHK(s_state, STAT_DIRL) != BIT_CHK(state[enemies], STAT_DIRL) &&
				 BIT_CHK(s_state, STAT_DIRR) != BIT_CHK(state[enemies], STAT_DIRR) &&
				 class[sprite] != COIN_1
				) {
				BIT_FLP(state[enemies], STAT_DIRR);
				BIT_FLP(state[enemies], STAT_DIRL);
				if ( class[enemies] <= SIDESTEPPER_MAGENTA ) {
					//ONLY SHELLCREEPERS
					BIT_SET(state_a[enemies], STAT_TURN);
					spr_timer[enemies] = zx_clock();
					tile[enemies] = spr_tile(enemies);
					colint[enemies] = 0;
				}
			}
			
			//TURN CURRENT ENEMY (sprite)
			BIT_FLP(s_state, STAT_DIRR);
			BIT_FLP(s_state, STAT_DIRL);
			if ( class[sprite] <= SIDESTEPPER_MAGENTA ) {
				//ONLY SHELLCREEPERS
				BIT_SET(state_a[sprite], STAT_TURN);
				state[sprite] = s_state;
				tile[sprite] = spr_tile(sprite); 
				colint[sprite] = 0;
			}
			spr_timer[sprite] = zx_clock();
			enemies = SPR_P2; //EXIT FOR
		}
	}
}

unsigned char enemy_collition_check(void) {
	//TODO CHECK IF PERF IS AFFECTED; AFTER WE RETURN AT FIRST FALSE
	if ( class[enemies] == 0 ) return 0;
	if ( class[enemies] > FIGHTERFLY ) return 0;
	if ( enemies == sprite ) return 0;
	//TEST if ( class[enemies] > FIGHTERFLY ) return 0;
	if ( BIT_CHK(state[sprite], STAT_HIT)  ) return 0;
	if ( BIT_CHK(state[sprite], STAT_KILL)  ) return 0;
	if ( BIT_CHK(state_a[sprite], STAT_TURN)  ) return 0;
	if ( BIT_CHK(state[enemies], STAT_KILL)  ) return 0;
	if ( BIT_CHK(state[enemies], STAT_FALL) ) return 0;
	
	tmp_ui = abs( lin[enemies] - lin[sprite] );
	if ( tmp_ui >= 16 ) return 0;
	
	tmp_sc = col[sprite] - col[enemies];
	if (tmp_sc == -2 && BIT_CHK( state[sprite], STAT_DIRR ) ) return 1;
	if (tmp_sc ==  2 && BIT_CHK( state[sprite], STAT_DIRL ) ) return 1;
	return 0;
}

void enemy_hit(void){

	NIRVANAP_halt();
	NIRVANAP_fillT(PAPER, lin[enemies], col[enemies]);
	lin[enemies] = lin[enemies]-4;
	NIRVANAP_spriteT(enemies, tile[enemies], lin[enemies], col[enemies]);
	BIT_CLR(state_a[enemies], STAT_TURN);
	
	spr_timer[enemies] = zx_clock();
	switch( class[enemies] ) {
				case SHELLCREEPER_GREEN:
					enemy_flip(TILE_SHELLCREEPER_GREEN);
					break;
				case SHELLCREEPER_RED :
					enemy_flip(TILE_SHELLCREEPER_RED);
					break;
				case SHELLCREEPER_BLUE:
					enemy_flip(TILE_SHELLCREEPER_BLUE);
					break;
				case SIDESTEPPER_RED :
					enemy_flip_sidestepper(TILE_SIDESTEPPER_RED);
					break;
				case SIDESTEPPER_GREEN :
					enemy_flip_sidestepper(TILE_SIDESTEPPER_GREEN);
					break;
				case SIDESTEPPER_MAGENTA :
					enemy_flip_sidestepper(TILE_SIDESTEPPER_MAGENTA);
					break;
				case FIGHTERFLY:
					enemy_flip(TILE_FIGHTERFLY);
					break;
				case SLIPICE:
					player_coin(enemies,50);
					break;
				case COIN_1:
					ay_fx_play(ay_effect_10);
					player_coin(enemies,80);
					break;
				case FIREBALL_RED:
					player_coin(enemies,100);
					break;
				case FIREBALL_GREEN:
					player_coin(enemies,20);
				break;
	}
}
void enemy_flip_change_dir( unsigned char f_keep ) {
	
	if (BIT_CHK(state[enemies], STAT_DIRR)) {
		BIT_SET(state_a[enemies], STAT_LDIRR);
		BIT_CLR(state_a[enemies], STAT_LDIRL);
	} else {
		BIT_SET(state_a[enemies], STAT_LDIRR);
		BIT_CLR(state_a[enemies], STAT_LDIRL);
	}
	
	if (col[sprite] > col[enemies]) {
		BIT_SET(state[enemies], STAT_DIRL);
		BIT_CLR(state[enemies], STAT_DIRR);
	}
	if (col[sprite] < col[enemies]) {
		BIT_SET(state[enemies], STAT_DIRR);
		BIT_CLR(state[enemies], STAT_DIRL);
	}
	if (!f_keep && col[sprite] == col[enemies]) {
		BIT_CLR(state[enemies], STAT_DIRL);
		BIT_CLR(state[enemies], STAT_DIRR);
	}
}
void enemy_flip(unsigned int f_tile){

	spr_timer[enemies] = zx_clock();
	BIT_SET(state[enemies], STAT_JUMP);
	BIT_CLR(state[enemies], STAT_FALL);
	BIT_FLP(state[enemies], STAT_HIT);
	BIT_CLR(state[enemies], STAT_UPGR);
	jump_lin[enemies] =  lin[enemies];
	
	if (BIT_CHK(state[enemies], STAT_HIT)){
		//Normal
		ay_fx_play(ay_effect_02);
		enemy_flip_change_dir(0);

		tile[enemies] = f_tile + 6; 
		player_score_add(1);
	} else {
		//Flipped
		if (BIT_CHK(state_a[enemies], STAT_LDIRR)) {
			BIT_SET(state[enemies], STAT_DIRR);
			BIT_CLR(state[enemies], STAT_DIRL);
		} else {
			BIT_SET(state[enemies], STAT_DIRR);
			BIT_CLR(state[enemies], STAT_DIRL);
		}
		tile[enemies] = spr_tile(enemies); 
	}
	
	
}

void enemy_flip_sidestepper(unsigned int f_tile){
	if ( BIT_CHK(state[enemies], STAT_ANGRY) ) {
		enemy_flip(f_tile);
	} else {
		enemy_flip_change_dir(1);
		BIT_FLP(state[enemies], STAT_JUMP);
		BIT_CLR(state[enemies], STAT_FALL);
		BIT_SET(state[enemies], STAT_ANGRY);
		jump_lin[enemies] =  lin[enemies];
		tile[enemies] = spr_tile(enemies);
	}
}

void enemy_turn(void){
	for ( sprite = 0; sprite < 6 ; ++sprite ) {
		if ( class[sprite] > 0 && spr_chktime(&sprite) ) {
			s_lin0  = lin[sprite];
			s_col0  = col[sprite];
			s_tile0 = tile[sprite] + colint[sprite];
			s_state = state[sprite];
			last_time[sprite] = zx_clock();
			//KILLED ENEMY
			if ( BIT_CHK(s_state, STAT_KILL) && !BIT_CHK(s_state, STAT_JUMP) ) {
				spr_killed(sprite);
				continue;
			}
			
			//FLIPPED ENEMY
			if ( BIT_CHK(s_state,STAT_HIT) && !BIT_CHK(s_state,STAT_FALL) && !BIT_CHK(s_state,STAT_JUMP) ) {
				sprite_speed_alt[sprite] = ENEMY_KILLED_SPEED; /* Flipped enemy */
				enemy_standard_hit();
				++colint[sprite];
				if (colint[sprite] > 2) colint[sprite] = 0;
				spr_redraw();
				continue;
			}
			
			//NORMAL ENEMY
			switch ( class[sprite] ) {
				case FIREBALL_RED:
					enemy_fireball_red();
				break;
				case FIREBALL_GREEN:
					enemy_fireball_green();
				break;
				case FIGHTERFLY:
					enemy_fighterfly();
				break;
				case SLIPICE:
					enemy_slipice();
				break;
				case COIN_1:
					enemy_coin1();
				break;
				case COIN_2:
					enemy_coin2();
				break;
				default:
					enemy_standard();
				break;
			}
			spr_redraw();
			state[sprite] = s_state;
		}
	}
}

void enemy_standard(void){
	//SHELLCREEPERS - SIDESTEPPERS
	//if ( spr_chktime(&sprite) ) {
		if ( !BIT_CHK(s_state, STAT_JUMP) ) {
			//WALKING OR FALLING
			enemy_walk();
	 		//JUMP BEFORE ENTER PIPES
			if ( lin[sprite] == GAME_LIN_FLOOR && ( col[sprite] < 5 || col[sprite] > 25) ) {
				BIT_SET(s_state, STAT_JUMP);
			}
		} else {
			//JUMPING
			sprite_speed_alt[sprite] = ENEMY_JUMP_SPEED;
			spr_move_horizontal();
			spr_move_up();
			if ( BIT_CHK(s_state, STAT_JUMP) ){
				//MAX HIT JUMP
				if ( jump_lin[sprite] - lin[sprite] >= ENEMIES_MAXJUMP ) {
					spr_set_fall();
				}
			}	
		}
		//TRAVEL TROUGH PIPES
		enemy_trip();
	//}
}

void enemy_slipice(void){
	//COINS N SLIPICE
	if ( phase_left > 0 ) {
		if (BIT_CHK(state[sprite], STAT_ANGRY) ) {
			//ANGRY OR FREEZING
			NIRVANAP_halt(); // synchronize with interrupts
			NIRVANAP_drawT(TILE_SLIPICE + 18 + colint[sprite],lin[sprite]+8, col[sprite]);
			++colint[sprite];
			if (colint[sprite] > 2) colint[sprite] = 0;
			if ( game_check_time(spr_timer[sprite],40) ) {
				game_freeze(lin[sprite]+8+16, col[sprite] );
				NIRVANAP_fillT(PAPER,lin[sprite]+8, col[sprite]);
				spr_destroy(sprite);
			}
		} else {
			//NORMAL
			if ( !BIT_CHK(state[sprite], STAT_JUMP) && !BIT_CHK(state[sprite], STAT_FALL) && 
		     ( col[sprite] == 7 || col[sprite] == 15 || col[sprite] == 24 ) &&
			 ( col[sprite] < 136 )
			 ) {

				tmp_ui = rand();
				index1 = game_calc_index ( lin[sprite]+16, col[sprite]  );
				if (lvl_1[index1] == GAME_MAP_PLATFORM && tmp_ui < 13106) {
					ay_fx_play(ay_effect_16);
					BIT_SET(s_state, STAT_ANGRY);
					spr_timer[sprite] = zx_clock();
					lin[sprite] -= 8;
					colint[sprite] = 0;
					tile[sprite] = TILE_SLIPICE;
				} else {
					enemy_walk();
				}
			} else {
				enemy_walk();
			}
			if ( (lin[sprite] == GAME_LIN_FLOOR)  && ( col[sprite] == 3 || col[sprite] == 27) ) {
				spr_destroy(sprite);
			}
		}
	}
}

void enemy_fireball_red(void){
	
	//BOUNCE EVERYWHERE
	if ( phase_left > 0) {
		if ( game_check_time( spr_timer[sprite], TIME_FIREBALL_RED) ) {
			spr_timer[sprite] = zx_clock();
			sprite_speed_alt[sprite] = ENEMY_KILLED_SPEED; /* Kill by timeout */
			BIT_SET(s_state, STAT_KILL);
		} else {
			
			if ( BIT_CHK(s_state, STAT_JUMP)) {
				if ( spr_move_up() ) spr_set_fall();
				tmp = 0;
			} else {
				if (spr_move_down()){
					BIT_CLR(s_state, STAT_FALL);
					BIT_SET(s_state, STAT_JUMP);
				}
				tmp = 16;
			}
			spr_move_horizontal();
			
			
			if (BIT_CHK(s_state, STAT_DIRR)) {
				//tmp0 = 2;
				s_col1 = col[sprite]+2;
			} else {
				//tmp0 = -1;
				s_col1 = col[sprite]-1;
			}
			index1 = game_calc_index(lin[sprite], s_col1);
			
			//if ( lin[sprite] < GAME_LIN_FLOOR && lvl_1[index1] != 0 || col[sprite] < 2 || col[sprite] > 29) {
			
			if ( lvl_1[index1] >= TILE_POW1 || col[sprite] < 2 || col[sprite] > 29) {
				BIT_FLP(s_state, STAT_DIRR);
				BIT_FLP(s_state, STAT_DIRL);
			}
		}
	}
}

void enemy_fireball_green(void){
	// FIREBALL_GREEN
	if ( phase_left > 0 ) {
		index1 = abs(jump_lin[sprite] - lin[sprite]);
		if (index1 > 8) {
			BIT_FLP(s_state, STAT_JUMP);
			BIT_FLP(s_state, STAT_FALL);
			jump_lin[sprite] = lin[sprite];
		}
		
		if ( BIT_CHK(s_state, STAT_JUMP)) {
			spr_move_horizontal(); 
			spr_move_up();
		} else {
			spr_move_horizontal(); 
			spr_move_down();
		}
		if ( ( col[sprite] ==  0 && BIT_CHK(s_state, STAT_DIRL) ) ||
			 ( col[sprite] == 30 && BIT_CHK(s_state, STAT_DIRR)) ) {
			spr_timer[sprite] = zx_clock();
			sprite_speed_alt[sprite] = ENEMY_KILLED_SPEED; /* Kill by timeout */
			BIT_SET(s_state, STAT_KILL);
		}
	}
}

void enemy_fighterfly(void){
	// FIGHTERFLY
	if ( !BIT_CHK(s_state, STAT_JUMP) && !BIT_CHK(s_state, STAT_FALL) ) {
		if (ay_is_playing() < AY_PLAYING_FOREGROUND) ay_fx_play(ay_effect_13);
		//TODO TIMER ...
		if ( game_check_time( spr_timer[sprite], 26 ) ) {
			BIT_SET(s_state, STAT_JUMP);
			jump_lin[sprite] = lin[sprite];
			spr_timer[sprite]=0;
		}
	}
	if ( BIT_CHK(s_state, STAT_JUMP) ) {
		//FIREFLY MAX JUMP
		if ( jump_lin[sprite] - lin[sprite] >= 8) {
			spr_set_fall();
		}
	
		spr_move_horizontal();
		spr_move_up();
	} else {
		if (spr_timer[sprite]==0) {
			spr_timer[sprite] = zx_clock();
		}
		if (BIT_CHK(s_state, STAT_FALL)) {
			enemy_walk();
			spr_move_horizontal();
		}
	}
	enemy_trip();
}

void enemy_trip(void){
	//TRAVEL THROUGH PIPES
	if (lin[sprite]  > 128) {
		tmp = 0;
		if( col[sprite] == 27 ) {
			tmp = 1;
			NIRVANAP_fillT(PAPER, lin[sprite], col[sprite]-1);
			lin[sprite]  = ENEMY_SLIN_R;
			col[sprite] =  ENEMY_SCOL_R;
			BIT_SET(s_state, STAT_DIRL);
			BIT_CLR(s_state, STAT_DIRR);
			BIT_CLR(s_state, STAT_JUMP);
			sound_enter_enemy();
			if (ay_is_playing() < AY_PLAYING_FOREGROUND) ay_fx_play(ay_effect_04);
		}
		if ( col[sprite] == 3 ) {
			tmp = 1;
			NIRVANAP_fillT(PAPER, lin[sprite], col[sprite]+1);
			lin[sprite]  = ENEMY_SLIN_L;
			col[sprite] =  ENEMY_SCOL_L;
			BIT_SET(s_state, STAT_DIRR);
			BIT_CLR(s_state, STAT_DIRL);
			BIT_CLR(s_state, STAT_JUMP);
			sound_enter_enemy();
			if (ay_is_playing() < AY_PLAYING_FOREGROUND) ay_fx_play(ay_effect_04);
		}
		if (tmp && class[sprite] <= SHELLCREEPER_BLUE ) {
				colint[sprite] =  0;
				state[sprite] = s_state;
				tile[sprite] = spr_tile(sprite);
				spr_set_fall();
		}
	}
}

void enemy_standard_hit(void) {
	if ( !BIT_CHK(state[sprite],STAT_UPGR) && ( game_check_time(spr_timer[sprite], game_time_flipped - 50) )) {
		enemy_evolve();
	}
	
	if ( game_check_time(spr_timer[sprite], game_time_flipped) ) {
		enemies = sprite;
		enemy_flip(spr_tile(sprite));
		s_state = state[sprite];
	}
}

void enemy_evolve(void){
	if ( class[sprite] == SHELLCREEPER_GREEN ) {
		enemy_upgrade(SHELLCREEPER_RED, TILE_SHELLCREEPER_RED);
	} else {
		if ( class[sprite] == SHELLCREEPER_RED ) {
			enemy_upgrade(SHELLCREEPER_BLUE, TILE_SHELLCREEPER_BLUE);
		}
	}
	if ( class[sprite] == SIDESTEPPER_RED ) {
		enemy_upgrade(SIDESTEPPER_GREEN,TILE_SIDESTEPPER_GREEN);
	} else {
		if ( class[sprite] == SIDESTEPPER_GREEN ) {
			enemy_upgrade(SIDESTEPPER_MAGENTA,TILE_SIDESTEPPER_MAGENTA);
		}
	}
}

void enemy_upgrade(unsigned char f_class, unsigned int f_tile){
	class[sprite] = f_class;
	tile[sprite] = f_tile + 6;
	BIT_SET(state[sprite], STAT_UPGR);
}

void enemy_walk(void){
	if (BIT_CHK(s_state, STAT_JUMP) == 0 && BIT_CHK(s_state, STAT_FALL) == 0) {
		if ( (col[sprite] & 1) && !BIT_CHK(s_state, STAT_HIT) ) {
			index_d = 0;
			tmp = 0;
		} else { 
			tmp = game_check_maze(game_calc_index(lin[sprite] + 16,col[sprite]));
		}
	} else {
		tmp = game_check_maze( game_calc_index( lin[sprite] + 16 , col[sprite] ) );
	}
	
	if ( tmp ) {
		//FALLING
		if ( !BIT_CHK(s_state, STAT_JUMP) ) {
			BIT_SET(s_state, STAT_FALL);
		}
		if (jump_lin[sprite] == 0) {
			jump_lin[sprite] = lin[sprite];
		}
		if ( BIT_CHK(s_state, STAT_FALL) ) {
			lin[sprite] = lin[sprite] + SPRITE_LIN_INC;
		} else {
			lin[sprite] = lin[sprite] - SPRITE_LIN_INC;
		}
		if (class[sprite] != FIGHTERFLY) {
			sprite_speed_alt[sprite] = ENEMY_FALL_SPEED;
			if ( lin[sprite] - jump_lin[sprite] <= 8 ) spr_move_horizontal(); 
		}
		
	} else {
		//OVER PLATFORM
		
		if ( BIT_CHK(s_state, STAT_FALL) ) {
			BIT_CLR(s_state, STAT_FALL);
			jump_lin[sprite] = 0;
			sprite_speed_alt[sprite] = 0;
		}
		
		if ( !BIT_CHK(s_state, STAT_HIT) && !BIT_CHK(state_a[sprite], STAT_TURN) ) {
			//COLLITION CHECK BETWEEN ENEMIES TODO PERFORMANCE...
			if ( game_check_time( spr_timer_c[sprite], 5 ) ) { 
				spr_timer_c[sprite] = zx_clock();
				enemy_collition();
			}
			spr_move_horizontal(); 
		}
		
		if (BIT_CHK(state_a[sprite], STAT_TURN)) {
			spr_timer[sprite] = zx_clock();
			if (colint[sprite] < 2) {
				++colint[sprite]; 
			} else {
				BIT_CLR(state_a[sprite], STAT_TURN);
				tile[sprite] = spr_tile(sprite);
			}
		}
	}
}

void enemy_init(unsigned char f_sprite,unsigned  char f_lin,unsigned  char f_col,unsigned  char f_class,unsigned  char f_dir) {
	++spr_count;
	class[f_sprite] = f_class;
	lin[f_sprite]  = f_lin;
	col[f_sprite]  = f_col;
	tmp = 0;
	state[f_sprite] = 0;
	state_a[f_sprite] = 0;
	jump_lin[f_sprite] = 0;
	if (f_dir == 1){
		BIT_SET(state[f_sprite], STAT_DIRR);
	} else {
		BIT_SET(state[f_sprite], STAT_DIRL);
	}
	BIT_SET(state[f_sprite], STAT_FALL);
	colint[f_sprite] = 0;
	tile[f_sprite] = spr_tile(f_sprite);
	last_time[f_sprite] = 0;
	spr_timer[f_sprite] = zx_clock();
	sprite_speed_alt[f_sprite] = 0;
}

void enemy_kill(unsigned char f_sprite){
	NIRVANAP_halt();
	sound_kill();
	if ( BIT_CHK(state[sprite], STAT_DIRL) ) {
		BIT_SET(state[f_sprite], STAT_DIRL);
		BIT_CLR(state[f_sprite], STAT_DIRR);
	} else {
		BIT_SET(state[f_sprite], STAT_DIRR);
		BIT_CLR(state[f_sprite], STAT_DIRL);
	}
	BIT_CLR(state[f_sprite], STAT_FALL);
	BIT_SET(state[f_sprite], STAT_JUMP);
	BIT_SET(state[f_sprite], STAT_KILL);
	if (class[f_sprite] == FIGHTERFLY) class[f_sprite] = SHELLCREEPER_GREEN; /* Hack */
	sprite_speed_alt[f_sprite] = ENEMY_KILLED_SPEED; //TODO CHECK DIFF SPEEDS
	
	player_score_add(80 << hit_count); //BONUS!
	++hit_count;
	spr_timer[f_sprite] = zx_clock();
	jump_lin[f_sprite] = lin[f_sprite];
	if ( class[f_sprite] <= FIGHTERFLY  ) {
		if (game_type != GAME_RANDOM_TYPE) --phase_left;
		++phase_coins;
	} 
	if (phase_left > 0 ) {
		ay_fx_play(ay_effect_11);
	} else {
		ay_reset(); //FORCE PLAY
		if (!game_bonus) ay_fx_play(ay_effect_17);
	}
}
