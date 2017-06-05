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


unsigned char enemy_collision(void) {
	if ( BIT_CHK(state[sprite]  , STAT_HIT)  ) return 0;
	if ( BIT_CHK(state[sprite]  , STAT_KILL)  ) return 0;
	if ( BIT_CHK(state[sprite]  , STAT_FALL)  ) return 0;
	if ( BIT_CHK(state[sprite]  , STAT_JUMP)  ) return 0;
	if ( BIT_CHK(state_a[sprite], STAT_TURN)  ) return 0;

	for (enemies = 0; enemies < SPR_P2 ; ++enemies ) {
		if ( class[enemies] == 0 || class[enemies] > FIGHTERFLY ) continue;
		if ( enemies == sprite ) continue;

		if ( BIT_CHK(state[enemies], STAT_KILL)  ) continue;
		if ( BIT_CHK(state[enemies], STAT_FALL) ) continue;
		if ( spr_collision_check( sprite, enemies, 0 ) ) {

			//TURN OTHER ENEMY (enemies)
			if ( !BIT_CHK(state[enemies], STAT_HIT) &&
				 BIT_CHK(s_state, STAT_DIRL) != BIT_CHK(state[enemies], STAT_DIRL) &&
				 BIT_CHK(s_state, STAT_DIRR) != BIT_CHK(state[enemies], STAT_DIRR) &&
				 class[sprite] != COIN_1
				) {
				enemy_collision_turn_dir(enemies);
			}

			//TURN CURRENT ENEMY (sprite)
			state[sprite] = s_state;
			enemy_collision_turn_dir(sprite);
			s_state = state[sprite];
			spr_timer[sprite] = zx_clock();
			return 1;
		}
	}
	BIT_CLR(state_a[sprite], STAT_PUSH);
	return 0;
}

void enemy_collision_turn_dir( unsigned char f_sprite) __z88dk_fastcall {
	if ( !BIT_CHK(state_a[f_sprite], STAT_PUSH) ) {
		BIT_FLP(state[f_sprite], STAT_DIRR);
		BIT_FLP(state[f_sprite], STAT_DIRL);
		BIT_SET(state_a[f_sprite], STAT_PUSH);
		if ( class[f_sprite] <= SIDESTEPPER_MAGENTA ) {
			//ONLY SHELLCREEPERS TURNING ANIMATION
			BIT_SET(state_a[f_sprite], STAT_TURN);
			spr_timer[f_sprite] = zx_clock();
			tile[f_sprite] = spr_tile(f_sprite);
			colint[f_sprite] = 0;
		}
	}
}

void enemy_hit(void){

	NIRVANAP_halt();
	NIRVANAP_fillT(PAPER, lin[enemies], col[enemies]);
	lin[enemies] = lin[enemies]-4; /* Move up the hitted brick */
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
void enemy_flip_change_dir( unsigned char f_keep ) __z88dk_fastcall {

  unsigned char x_player;
	unsigned char x_enemy;

	if (BIT_CHK(state[enemies], STAT_DIRR)) {
		BIT_SET(state_a[enemies], STAT_LDIRR);
		BIT_CLR(state_a[enemies], STAT_LDIRL);
	} else {
		BIT_SET(state_a[enemies], STAT_LDIRR);
		BIT_CLR(state_a[enemies], STAT_LDIRL);
	}

  x_player = spr_calc_hor(sprite);
	x_enemy = spr_calc_hor(enemies);

	if (x_player  > x_enemy ) {
		BIT_SET(state[enemies], STAT_DIRL);
		BIT_CLR(state[enemies], STAT_DIRR);
	} else {
		if (col[sprite] < col[enemies]) {
			BIT_SET(state[enemies], STAT_DIRR);
			BIT_CLR(state[enemies], STAT_DIRL);
		} else {
			//col[sprite] == col[enemies]
			if (!f_keep) {
				BIT_CLR(state[enemies], STAT_DIRL);
				BIT_CLR(state[enemies], STAT_DIRR);
			}
		}
	}
	colint[enemies] = 0;
}

void enemy_flip(unsigned int f_tile) __z88dk_fastcall {

	spr_timer[enemies] = zx_clock();
	BIT_SET(state[enemies], STAT_JUMP);
	BIT_CLR(state[enemies], STAT_FALL);
	BIT_FLP(state[enemies], STAT_HIT);
	jump_lin[enemies] =  lin[enemies];
  colint[enemies] = 0;

	if (BIT_CHK(state[enemies], STAT_HIT)) {
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

void enemy_flip_sidestepper(unsigned int f_tile) __z88dk_fastcall {
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
	//SHELLCREEPERS || SIDESTEPPERS
		if ( BIT_CHK(s_state, STAT_JUMP) ) {
			//JUMPING
			sprite_speed_alt[sprite] = ENEMY_JUMP_SPEED;
			spr_move_horizontal();
			spr_move_up();
			//MAX HIT JUMP
			if ( jump_lin[sprite] - lin[sprite] >= ENEMIES_MAXJUMP ) {
				//enemy_ugly_fix();
				spr_set_fall();
			}
		} else {
			//WALKING OR FALLING
			enemy_walk();
	 		//JUMP BEFORE ENTER PIPES
			if ( lin[sprite] == GAME_LIN_FLOOR && ( col[sprite] < 5 || col[sprite] > 25) ) {
				BIT_SET(s_state, STAT_JUMP);
			}
		}
		//TRAVEL TROUGH PIPES
		enemy_trip();
}

void enemy_slipice(void){
	//COINS N SLIPICE
	if ( phase_left > 0 ) {
		if (BIT_CHK(state[sprite], STAT_ANGRY) ) {
			//ANGRY OR FREEZING
			NIRVANAP_halt(); // synchronize with interrupts
			NIRVANAP_drawT(TILE_RAY + colint[sprite],lin[sprite]+8, col[sprite]);
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
				if (lvl_1[index1] == TILE_BRICK && tmp_ui < 13106) {
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


		if (s_col0 != col[sprite]) {
			if (BIT_CHK(s_state, STAT_DIRR)) {
				index1 = index1 + 2;
			} else {
				index1 = index1 - 1;
			}
			if ( lvl_1[index1] <= TILE_POW1 || col[sprite] < 2 || col[sprite] > 29) {
				BIT_FLP(s_state, STAT_DIRR);
				BIT_FLP(s_state, STAT_DIRL);
			}
		}

    spr_move_horizontal();



		if ( BIT_CHK(s_state, STAT_JUMP)) {
			if ( spr_move_up() ) spr_set_fall();
		} else {
			if (spr_move_down()){
				BIT_CLR(s_state, STAT_FALL);
				BIT_SET(s_state, STAT_JUMP);
			}
		}


		if ( game_check_time( spr_timer[sprite], TIME_FIREBALL_RED) ) {
			spr_timer[sprite] = zx_clock();
			sprite_speed_alt[sprite] = ENEMY_KILLED_SPEED; /* Kill by timeout */
			BIT_SET(s_state, STAT_KILL);
			BIT_CLR(s_state, STAT_JUMP);
			BIT_CLR(s_state, STAT_FALL);
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
			BIT_CLR(s_state, STAT_JUMP);
		}
	}
}

void enemy_fighterfly(void){

	if ( !BIT_CHK(s_state, STAT_JUMP) && !BIT_CHK(s_state, STAT_FALL) ) {
		if (ay_is_playing() < AY_PLAYING_FOREGROUND) ay_fx_play(ay_effect_13);
		if ( game_check_time( spr_timer[sprite], ENEMY_FIGHTERFLY_STAND_TIME ) ) {
			BIT_SET(s_state, STAT_JUMP);
			jump_lin[sprite] = lin[sprite];
			spr_timer[sprite] = 0;
		}
	}

	if ( BIT_CHK(s_state, STAT_JUMP) ) {
		//FIREFLY MAX JUMP
		if ( jump_lin[sprite] - lin[sprite] >= 8) {
			//enemy_ugly_fix();
			spr_set_fall();
		}
		spr_move_horizontal();
		spr_move_up();
	} else {
		if (spr_timer[sprite] == 0) {
			spr_timer[sprite] = zx_clock();
		}
		if (BIT_CHK(s_state, STAT_FALL)) {
			if ( lin[sprite] < jump_lin[sprite] ) {
				spr_move_horizontal();
			} else {
				sprite_speed_alt[sprite] = 4;
				if (colint[sprite] == 0 ) {
					colint[sprite] = 2;
				} else {
					colint[sprite] = 0;
				}

			}
			if (spr_move_down()){
				sprite_speed_alt[sprite] = 0;
				BIT_CLR(s_state, STAT_FALL);
			}
		}
	}
	enemy_trip();
}

void enemy_trip(void){
	//TRAVEL THROUGH PIPES
	if (lin[sprite]  > 128) {
		tmp = 0;
		if( col[sprite] == 27 ) {
			enemy_trip_move(ENEMY_SLIN_R, ENEMY_SCOL_R, STAT_DIRL, STAT_DIRR);
		}
		if ( col[sprite] == 3 ) {
			 enemy_trip_move(ENEMY_SLIN_L, ENEMY_SCOL_L, STAT_DIRR, STAT_DIRL);
		}

		if (tmp && class[sprite] <= SHELLCREEPER_BLUE ) {
				colint[sprite] =  0;
				state[sprite] = s_state;
				tile[sprite] = spr_tile(sprite);
				spr_set_fall();
		}
	}
}
void enemy_trip_move(unsigned char f_slin, unsigned char f_scol, unsigned char f_dir, unsigned char f_dir_alt) {
	tmp = 1;
	NIRVANAP_halt();
	NIRVANAP_fillT(PAPER, s_lin0, s_col0);
	lin[sprite]  = f_slin;
	jump_lin[sprite]  = f_slin+16;
	col[sprite] =  f_scol;
	BIT_SET(s_state, f_dir);
	BIT_CLR(s_state, f_dir_alt);
	BIT_CLR(s_state, STAT_JUMP);
	sound_enter_enemy();
	if (ay_is_playing() < AY_PLAYING_FOREGROUND) ay_fx_play(ay_effect_04);
}
void enemy_standard_hit(void) {
	if ( game_check_time(spr_timer[sprite], game_time_flipped - 50) ) {
		if (!BIT_CHK(state[sprite],STAT_UPGR)) {
		  enemy_evolve(sprite);
			BIT_SET(state[sprite],STAT_UPGR);
		}
	}

	if ( game_check_time(spr_timer[sprite], game_time_flipped) ) {
		enemies = sprite;
		enemy_flip(spr_tile(sprite));
		s_state = state[sprite];
		BIT_CLR(state[sprite],STAT_UPGR);
	}
}

void enemy_evolve(unsigned char f_enemy) __z88dk_fastcall{
	switch (class[f_enemy]) {
		case SHELLCREEPER_GREEN:
      enemy_upgrade(f_enemy, SHELLCREEPER_RED, TILE_SHELLCREEPER_RED);
			break;
		case SHELLCREEPER_RED:
  		enemy_upgrade(f_enemy, SHELLCREEPER_BLUE, TILE_SHELLCREEPER_BLUE);
	  	break;
		case SIDESTEPPER_RED:
      enemy_upgrade(f_enemy, SIDESTEPPER_GREEN, TILE_SIDESTEPPER_GREEN);
			break;
		case SIDESTEPPER_GREEN:
  		enemy_upgrade(f_enemy, SIDESTEPPER_MAGENTA, TILE_SIDESTEPPER_MAGENTA);
	  	break;
	}
}

void enemy_upgrade(unsigned char f_enemy, unsigned char f_class, unsigned int f_tile){
	tile[f_enemy] = f_tile + 6;
  class[f_enemy] = f_class;
}

void enemy_walk(void){
	if (BIT_CHK(s_state, STAT_JUMP) == 0 && BIT_CHK(s_state, STAT_FALL) == 0) {
		if ( (col[sprite] & 1) && !BIT_CHK(s_state, STAT_HIT) ) {
			//index_d = 0;
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
			spr_move_down();
		} else {
			spr_move_up();
		}
		sprite_speed_alt[sprite] = ENEMY_FALL_SPEED;
		tmp = 1;
		tmp0 = game_check_maze( game_calc_index( lin[sprite]+16, col[sprite] + 1 ) );
		tmp1 = game_check_maze( game_calc_index( lin[sprite]+16, col[sprite] - 1 ) );
		tmp = !tmp0 && !tmp1;
		if ( tmp && ( lin[sprite] - jump_lin[sprite] <= 8 ) ) spr_move_horizontal();
	} else {
		//OVER PLATFORM
		if ( BIT_CHK(s_state, STAT_FALL) ) {
			BIT_CLR(s_state, STAT_FALL);
			jump_lin[sprite] = 0;
			sprite_speed_alt[sprite] = 0;
		}
		if ( !BIT_CHK(state_a[sprite], STAT_TURN) ) {
			if ( (loop_count & 3) == 0 ) enemy_collision();
			spr_move_horizontal();
		} else {
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

	if (f_dir == DIR_RIGHT){
		BIT_SET(state[f_sprite], STAT_DIRR);
	}
	if (f_dir == DIR_LEFT){
		BIT_SET(state[f_sprite], STAT_DIRL);
	}
	colint[f_sprite] = 0;
	tile[f_sprite] = spr_tile(f_sprite);
	last_time[f_sprite] = 0;
	spr_timer[f_sprite] = zx_clock();
	sprite_speed_alt[f_sprite] = 0;
}

void enemy_kill(unsigned char f_sprite) __z88dk_fastcall {

	NIRVANAP_halt();
	sound_kill();
	if ( BIT_CHK(state[sprite], STAT_DIRL) ) {
		BIT_SET(state[f_sprite], STAT_DIRL);
		BIT_CLR(state[f_sprite], STAT_DIRR);
	} else {
		BIT_SET(state[f_sprite], STAT_DIRR);
		BIT_CLR(state[f_sprite], STAT_DIRL);
	}

	if (class[f_sprite] == FIGHTERFLY) class[f_sprite] = SHELLCREEPER_GREEN; /* HACK */
	BIT_SET(state[f_sprite], STAT_KILL);
	if (class[f_sprite] <= SIDESTEPPER_MAGENTA) {
		BIT_SET(state[f_sprite], STAT_JUMP);
	} else {
		BIT_CLR(state[f_sprite], STAT_JUMP);
	}

	sprite_speed_alt[f_sprite] = ENEMY_KILLED_SPEED;

	++hit_count;
	player_score_add(80 << hit_count); //BONUS!

	spr_timer[f_sprite] = zx_clock();
	jump_lin[f_sprite] = lin[f_sprite];
	if ( class[f_sprite] <= SIDESTEPPER_MAGENTA && game_type != GAME_RANDOM_TYPE) {
		--phase_left;
    if (phase_left == 1) {
			/*Evolve last Enemy*/
			phase_angry = 1;
			for (tmp0 = 0; tmp0 < SPR_P2 ; ++tmp0 ) {
				if ( class[tmp0] > 0 && class[tmp0] <= FIGHTERFLY && !BIT_CHK(state[tmp0],STAT_KILL) )
				{
					/*evolve twice*/
					enemy_evolve(tmp0);
					enemy_evolve(tmp0);
					tile[tmp0] = spr_tile(tmp0);
				}
			}
		}
		++phase_coins;
	}

	if (phase_left > 0 ) {
		ay_fx_play(ay_effect_11);
	} else {
		ay_reset(); //FORCE PLAY
		if (!game_bonus) ay_fx_play(ay_effect_17);
	}
}
