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
#include <arch/zx.h>
#include <input.h>
#include <stdlib.h>
#include "nirvana+.h"
#include "pietro.h"
#include "pietro_player.h"
#include "pietro_ay.h"
#include "pietro_enemies.h"
#include "pietro_game.h"
#include "pietro_sprite.h"
#include "pietro_sound.h"
#include "pietro_zx.h"
#include "macros.h"


void player_init(unsigned char f_sprite, unsigned  char f_lin, unsigned  char f_col, unsigned  char f_tile) {
	//COMMON SPRITE VARIABLES
	class[f_sprite] = PLAYER;
	tile[f_sprite] = f_tile;
	lin[f_sprite]  = f_lin;//*SPRITELIN(f_sprite);
	col[f_sprite]  = f_col;//*SPRITECOL(f_sprite);
	colint[f_sprite] = 0;
	state[f_sprite] = 0;
	state_a[f_sprite] = 0;
	jump_lin[f_sprite] = f_lin;
	last_time[f_sprite] = zx_clock();
	sprite_speed_alt[f_sprite] = 0;
	//PLAYER ONLY VARIABLES
	if (sprite == SPR_P1) {
		index_player = 0;
	} else {
		index_player = 1;
	}
	hit_col[index_player] = 0;
	hit_lin[index_player] = 0;
	player_lock[index_player] = 1;
	//sliding[index_player] = 0;
	sliding[index_player] = PLAYER_SLIDE_NORMAL;
	NIRVANAP_spriteT(f_sprite, f_tile, f_lin, f_col);
}

unsigned char player_collition(void) {
	if ( class[sprite] == 0 ) return 0;
	if (BIT_CHK(state[sprite], STAT_HIT)) return 0;
	if (BIT_CHK(state[sprite], STAT_KILL)) return 0;
	hit_count = 0;
	for (enemies = 0; enemies < SPR_P2 ; ++enemies ) { //TODO USE SPRITE VARIABLE AS START....
		if ( player_collition_check() ) {
			
			if ( BIT_CHK(state[enemies], STAT_HIT) ) {
				/* Kill the enemy */
				enemy_kill(enemies);
			} else {
				if (class[enemies] == COIN_1 || class[enemies] == COIN_2) {
					/* Get Coin */
					player_coin(enemies,80);
				} else {
					/* Player Killed */
					ay_fx_play(ay_effect_18);
					player_kill();
				}
			}
		}
	}
	game_score_osd();
	return 0;
}

unsigned char player_collition_check(void) {

	if ( class[enemies] == 0 ) return 0;
	
	if ( BIT_CHK(state[enemies], STAT_KILL) ) return 0;
	
	tmp_ui = abs( lin[enemies] - lin[sprite] );
	if ( BIT_CHK(state[sprite], STAT_JUMP) || BIT_CHK(state[sprite], STAT_FALL) ) {
		if ( tmp_ui > PLAYER_VCOL_MARGIN + 2 ) return 0; //Jumping || Falling
	} else {
		if ( tmp_ui > PLAYER_VCOL_MARGIN ) return 0; //Walking
	}
	
	/* COL DIFF TO SPEED UP */
	tmp_ui = abs( col[enemies] - col[sprite] );
	tmp0 = 3*col[enemies];
	tmp1 = 3*col[sprite];
	if ( tmp_ui > PLAYER_HCOL_MARGIN ) return 0;
	
	if ( BIT_CHK(state[enemies], STAT_DIRR) ) {
		tmp0 = 3*col[enemies] + colint[enemies];
	}
	if ( BIT_CHK(state[enemies], STAT_DIRL) ) {
		tmp0 = 3*col[enemies] - colint[enemies];
	}
	
	if ( BIT_CHK(state[sprite], STAT_DIRR) ) {
		tmp1 = 3*col[sprite] + colint[sprite];
	}
	if ( BIT_CHK(state[sprite], STAT_DIRL) ) {
		tmp1 = 3*col[sprite] - colint[sprite];
	}		
	tmp_ui = abs(tmp0 - tmp1);

	if ( tmp_ui > PLAYER_HCOL_MARGIN_INT ) return 0;
	return 1;
}

void player_kill(void) {
	if ( !BIT_CHK(state[sprite], STAT_KILL) ) {
		sound_hit_brick();
		BIT_SET(state[sprite], STAT_KILL);
		sprite_speed_alt[sprite] = SPRITE_FALL_SPEED;
		if ( BIT_CHK(state[sprite], STAT_DIRR) ) {
			tile[sprite] = TILE_P1_HITR + tile_offset;
		} else {
			tile[sprite] = TILE_P1_HITR + 12 + tile_offset;
		}
		player_hit_brick_clear();
		NIRVANAP_spriteT(sprite, tile[sprite], lin[sprite], col[sprite]);
		spr_timer[sprite] = zx_clock();
	}
}

void player_restart(unsigned char f_sprite){
	ay_fx_play(ay_effect_15);
	if (f_sprite == SPR_P1) {
		player_init( SPR_P1,0,14,TILE_P1_STANR);
		NIRVANAP_drawT(  TILE_BRICK_RESTART , 16, 14 );
	} else {
		player_init( SPR_P2,0,16,TILE_P1_STANR + 24 +12);
		NIRVANAP_drawT(  TILE_BRICK_RESTART , 16, 16 );
	}
	BIT_SET(state[f_sprite],STAT_HIT);
	player_lock[index_player] = 1;
	spr_timer[f_sprite] = zx_clock();
	sprite_speed_alt[f_sprite] = SPRITE_RESTART_SPEED;
}

unsigned char player_lost_life(void){
	sound_hit_enemy();
	--game_lives[index_player];

	if (game_lives[index_player] == 0) {
		/* Player dies */
		if ( game_lives[0] == 0 && game_lives[1] == 0 ) {
			game_over = 1;
		}
		/* Do not restart player */
		return 0;
	}
	spr_check_over();
	game_print_lives();
	return 1;
}



void player_turn(void) {
	if ( class[sprite] == PLAYER && phase_left > 0 && game_lives[index_player] > 0) {
		if ( spr_chktime(&sprite)  ) {
			dirs = 0;
			if (sprite == SPR_P1) {
				dirs = (joyfunc1) (&k1);
			} else {
				dirs = (joyfunc2) (&k1);
			}
			player_move();
			player_collition();
		}
	}
}

unsigned char player_move(void){
	//STORE FOR SCREEN CLEAN
	s_state = state[sprite];
	s_lin0 = lin[sprite];
	s_col0 = col[sprite];
	s_tile0 = tile[sprite] + colint[sprite];
	
	
	if ( BIT_CHK(s_state, STAT_KILL) ) {
		/* Killed Player */
		tile[sprite] = TILE_P1_KILL + tile_offset;
		spr_killed(sprite);
		return 0;
	}
	
	if ( BIT_CHK( state_a[sprite], STAT_PUSH) ) {
		/* Pushed Player */
		if ( game_check_time(spr_timer[sprite], 20 ) ) { //TODO DEFINE
			//CLEAR PUSH
			BIT_CLR( state_a[sprite], STAT_PUSH);
			colint[sprite] = 0;
			tile[sprite] = spr_tile_dir(TILE_P1_STANR + tile_offset, sprite, 12);
		}
	}
	
	if ( BIT_CHK(state_a[sprite],STAT_INERT) && spr_timer[sprite] > 0 ) {
		/* Player Inertia */
		if ( game_check_time( spr_timer[sprite], PLAYER_INERT_TIME ) ) {
			BIT_CLR(state_a[sprite],STAT_INERT);
		}
	}
	
	
	if (BIT_CHK(s_state, STAT_HIT)) {
		/* Player re-start over safe platform */
		if ( lin[sprite] < 16 ) {
			spr_move_down();
			player_lock[index_player] = 0;
		} else {
			if ( dirs & IN_STICK_FIRE || dirs & IN_STICK_LEFT || dirs & IN_STICK_RIGHT) {
				BIT_CLR(state[sprite], STAT_HIT);
				NIRVANAP_halt();
				NIRVANAP_drawT(  TILE_EMPTY, 16, s_col0 );
				NIRVANAP_drawT(  TILE_EMPTY, 32, s_col0 );
				sprite_speed_alt[sprite] = 0;
				return 0;
			}
		}
		
		spr_redraw();
		tmp_ui = zx_clock() - spr_timer[sprite];
		tmp = 0;
		if (tmp_ui > 50) tmp = 1;
		if (tmp_ui > 100) tmp = 2;
		NIRVANAP_halt();
		NIRVANAP_drawT(  TILE_BRICK_RESTART + tmp, lin[sprite] + 16, s_col0 );

		if (tmp_ui > 150) {
			NIRVANAP_drawT(  TILE_EMPTY, lin[sprite], s_col0 );
			NIRVANAP_drawT(  TILE_EMPTY, lin[sprite] + 16, s_col0 );
			BIT_CLR(s_state, STAT_HIT);
			BIT_SET(s_state, STAT_JUMP);
			state[sprite] = s_state;
			player_lock[index_player] = 0;
			sprite_speed_alt[sprite] = 0;
		}
		
		return 0;
	}
	
	/* Read player input */
	player_move_input(); 
	
	if (player_lock[index_player]) return 0;
	
	if ( !BIT_CHK(s_state, STAT_JUMP) && !BIT_CHK(s_state, STAT_FALL)) {
		/* Check if the player have floor, and set fall if not */
		if (col[sprite] & 1 ) {
			index_d = 0;
		} else {
			index_d = game_calc_index( lin[sprite] + 16 , col[sprite] );
			if (s_lin0 == GAME_LIN_FLOOR) index_d = 0;
		}
		if ( index_d > 0 && lvl_1[index_d] < VAL_COL  ) {
			sprite_speed_alt[sprite] = PLAYER_FALL_SPEED;
			BIT_SET(s_state, STAT_FALL);
		}
	} else {
		if ( BIT_CHK(s_state, STAT_JUMP) ) {
			/* Jump Handling */
			if ( jump_lin[sprite] - lin[sprite] < PLAYER_MAX_JUMP ) {
				spr_move_up();
				/* TODO CHECK THIS */
				if ( jump_lin[sprite] - lin[sprite] >= PLAYER_MAX_JUMP ) spr_timer[sprite] = zx_clock() - 24;
			} else {
				if (game_check_time(spr_timer[sprite] , PLAYER_HIT_BRICK_TIME) ) spr_set_fall();			
			}
			
			
			tmp = jump_lin[sprite] - lin[sprite];	
			if (tmp & 7) { //mod 8
				player_move_horizontal();
			}
			
		} else {
			if ( BIT_CHK(s_state, STAT_FALL) ){
				/* Falling Handling */
				spr_move_down();
				player_move_horizontal();
				/* Determine Sliding */
				index1 = game_calc_index( lin[sprite] + 16 , col[sprite] );
				if (lvl_1[index1] == GAME_MAP_PLATFORM_FREEZE) {
					sliding[index_player] = PLAYER_SLIDE_ICE;
				} else {
					sliding[index_player] = PLAYER_SLIDE_NORMAL;
				}
				if ( BIT_CHK(s_state, STAT_DIRL) == BIT_CHK(s_state, STAT_DIRR) ) {
					sliding[index_player] = 0;
				}
			}
		}
	}
	
	if ( game_check_time( spr_timer[sprite], PLAYER_HIT_BRICK_TIME ) ) {
		/* Restored hitted platforms */
		player_hit_brick_clear();
	} 
	spr_redraw();
	state[sprite] = s_state;
	return 0;
}

int player_move_input(void) {

	if ( dirs & IN_STICK_FIRE || dirs & IN_STICK_LEFT || dirs & IN_STICK_RIGHT) {
		/* User have pressed valid input */
		player_lock[index_player] = 0;
		
		if (!(dirs & IN_STICK_FIRE)) {
			BIT_CLR(state_a[sprite], STAT_LOCK);
		}
		
		if ( !BIT_CHK(s_state, STAT_JUMP) && !BIT_CHK(s_state, STAT_FALL) ) {
			index1 = game_calc_index(s_lin0+16 , s_col0);
			if (lvl_1[index1] == GAME_MAP_PLATFORM_FREEZE) {
				sliding[index_player] = PLAYER_SLIDE_ICE;
			} else {
				sliding[index_player] = PLAYER_SLIDE_NORMAL;
			}
			
			if ( BIT_CHK(state_a[sprite],STAT_INERT) && spr_timer[sprite] == 0) {
				spr_timer[sprite] = zx_clock();
			}
			/* Key right */
			if ( dirs & IN_STICK_RIGHT ) {
				BIT_SET(s_state, STAT_DIRR);
				BIT_CLR(s_state, STAT_DIRL);
			}
			/* Key left */
			if ( dirs & IN_STICK_LEFT ) {
				BIT_SET(s_state, STAT_DIRL);
				BIT_CLR(s_state, STAT_DIRR);
			}
			/* Set Tile according to current direction */
			state[sprite] = s_state;
			tile[sprite] = spr_tile_dir(TILE_P1_RIGHT + tile_offset,sprite,12);
			
			if ( !BIT_CHK(state_a[sprite], STAT_LOCK) && dirs & IN_STICK_FIRE ) {
				/* New jump */
				if ( ay_is_playing() != AY_PLAYING_MUSIC ) ay_fx_play(ay_effect_03);
				sound_jump();
				colint[sprite]=0;
				BIT_SET(state_a[sprite], STAT_LOCK);
				BIT_SET(s_state, STAT_JUMP);
				BIT_CLR(s_state, STAT_FALL);
				BIT_CLR(state_a[sprite],STAT_INERT);
				jump_lin[sprite] = lin[sprite];
				state[sprite] = s_state;
				tile[sprite] = spr_tile_dir(TILE_P1_JUMPR + tile_offset, sprite, 12);
				sprite_speed_alt[sprite] = PLAYER_JUMP_SPEED;
				return 0;
			}
			
			if (!ay_is_playing() && !game_bonus) ay_fx_play(ay_effect_20);
			if (!BIT_CHK(state_a[sprite],STAT_INERT)) {
				player_move_horizontal();	
			} else {
				colint[sprite]++;
				if (colint[sprite] == 2) colint[sprite] = 0;
			}
			
		}
	} else {		
		/* No Input from the player */
		BIT_CLR(state_a[sprite], STAT_LOCK);
		if ( !BIT_CHK(s_state, STAT_HIT) && !BIT_CHK(s_state, STAT_JUMP) && !BIT_CHK(s_state, STAT_FALL) && sliding[index_player] > 0 ) {
			/* Sliding */
			if ( ay_is_playing() != AY_PLAYING_MUSIC ) ay_fx_play(ay_effect_01);
			sound_slide();
			player_move_horizontal();
			sliding[index_player]--;
			sprite_speed_alt[sprite] = 0;
			if ( sliding[index_player] == 0 ) { 
				BIT_SET(state_a[sprite],STAT_INERT);
				spr_timer[sprite] = 0;
				//NIRVANAP_fillT(PAPER, s_lin0,s_col0);
				colint[sprite] = 0;
				tile[sprite] = spr_tile_dir(TILE_P1_STANR + tile_offset,sprite,12);
				BIT_CLR(s_state,STAT_DIRR);
				BIT_CLR(s_state,STAT_DIRL);
			} else {
				tile[sprite] = spr_tile_dir(TILE_P1_SLIDR + tile_offset,sprite,12);
			}
		}
	}
	return 0;
}

void player_move_horizontal(void) {
	//PLAYER COLLITIONS
	
	if ( player_push_check() ) {
		player_push();
	} else {
		spr_move_horizontal();
	}
	
}

unsigned char player_push_check(void) {
	//TODO CHECK THIS WITH ENEMIES SEEMS BETTER
	if ( lin[sprite_other_player] == 0 ) return 0;
	if ( lin[sprite] != lin[sprite_other_player] ) return 0;
	tmp_sc = col[sprite] - col[sprite_other_player];
	if ( tmp_sc == -2 && BIT_CHK( state[sprite], STAT_DIRR ) ) return 1;
	if ( tmp_sc ==  2 && BIT_CHK( state[sprite], STAT_DIRL ) ) return 1;
	return 0;
}

void player_push(void){
	tmp = sprite; //HACK
	tmp0 = s_col0;
	s_lin0 = lin[sprite_other_player];
	s_col0 = col[sprite_other_player];
	
	spr_timer[sprite_other_player] = zx_clock();
	BIT_SET( state_a[sprite_other_player], STAT_PUSH );
	sprite = sprite_other_player; //for spr_move_right/left
	if ( BIT_CHK(state[tmp], STAT_DIRR) ) {
		spr_move_right();
	} else {
		spr_move_left();
	}
	spr_redraw();
	sprite = tmp;
	s_lin0 = lin[sprite];
	s_col0 = tmp0;
}

void player_hit_slipice(unsigned char f_enemies) {
	player_score_add(50);
	sound_coin();
	BIT_SET(state[f_enemies], STAT_KILL);
	spr_timer[f_enemies] = zx_clock();
}


unsigned char player_hit_brick(void){
	if ( ( hit_lin[index_player] == 0 ) && 
		 ( lin[sprite] > 16 ) && 
		 ( lvl_1[ index1 ] >= GAME_MAP_PLATFORM || lvl_1[ index2 ] >= GAME_MAP_PLATFORM ) 
		) {
		
		for (enemies = 0; enemies < 6 ; ++enemies){
			//HIT ENEMIES
			if (
					class[enemies] != 0							&& 
					( lin[sprite] - lin[enemies] == 24 )		&& 
					( !BIT_CHK(state[enemies], STAT_KILL) )     &&
					( abs(col[sprite] - col[enemies]) <= 2 ) 
				) {
					enemy_hit();
					sound_hit_enemy();
				} 
		}
		
		if ( lin[sprite_other_player] > 0 &&
			 lin[sprite] - lin[sprite_other_player] == 24 && 
		     abs(col[sprite] - col[sprite_other_player]) <= 2 ) 
		{
			/* Make other player jump */
			NIRVANAP_fillT(PAPER, lin[sprite_other_player], col[sprite_other_player]);
			lin[sprite_other_player] = lin[sprite_other_player]-4;
			NIRVANAP_spriteT(sprite_other_player, tile[sprite_other_player], lin[sprite_other_player], col[sprite_other_player]);
			NIRVANAP_halt();
			
			BIT_CLR(state[sprite_other_player], STAT_FALL);
			BIT_SET(state[sprite_other_player], STAT_JUMP);
			jump_lin[sprite_other_player] = lin[sprite_other_player] + 20;
			NIRVANAP_halt();
		}
		spr_timer[sprite] = zx_clock();
		hit_lin[index_player] = lin[sprite];
		hit_col[index_player] = col[sprite];
		spr_brick_anim(1);
		sound_hit_brick();
		return 1;
	}
	return 0;
}

void player_hit_brick_clear(void){
	//CLEAR HITTED BRICKS N MAKES THE PLAYER FALL
	if ( hit_lin[index_player] > 0 ) {
		index1 = game_calc_index( hit_lin[index_player] - 8 , hit_col[index_player] );
		index2 = index1 + 1;
		spr_brick_anim(0);
		hit_lin[index_player] = 0;
		hit_col[index_player] = 0;
		spr_set_fall();
	}
}

void player_hit_pow(void){
	if ( lvl_1[ index1 ] == IDX_POW || lvl_1[ index2 ] == IDX_POW && game_pow != 0) {
		game_pow--;
		zx_border(INK_RED);
		for (enemies = 0; enemies < 6 ; ++enemies){
			if ( !game_check_maze( game_calc_index( lin[enemies] + 16, col[enemies] ) ) ){
				enemy_hit();
			}
		}
		ay_fx_play(ay_effect_06);
		sound_hit_pow();
		spr_draw_pow();
		if (game_pow == 0) {
			NIRVANAP_fillT(PAPER, 120,15); 
			lvl_1[495] = 0;
			lvl_1[495 + 1] = 0;
			lvl_1[495 + 32] = 0;
			lvl_1[495 + 33] = 0;
		}
		zx_border(INK_BLACK);
		spr_set_fall();
	}
}


void player_coin(unsigned char f_enemies, unsigned char f_score) {
	if (class[f_enemies] == SLIPICE) {
		ay_fx_play(ay_effect_12); //SLIPICE
	} else {
		ay_fx_play(ay_effect_10); //COIN SOUND
	}
	sound_coin();
	BIT_SET(state[f_enemies], STAT_KILL);
	spr_timer[f_enemies] = zx_clock();
	if (game_bonus){
		++phase_bonus_total[index_player];
		--phase_left;
		if(phase_left <= 0) phase_end = 1;
	} else {
		player_score_add(f_score);
	}
}


void player_score_add(unsigned int f_score){
	player_score[index_player] = player_score[index_player] + f_score;
	//CHECK FOR TOP SCORE
	if ( player_score[index_player] > game_score_top ) {
		game_score_top = player_score[index_player];
	}
	//CHECK FOR EXTRA LIFE
	if ( player_score[index_player] > player_next_extra[index_player] ) {
		player_next_extra[index_player] += GAME_EXTRA_LIFE;
		++game_lives[index_player];
		if(!game_bonus) game_print_lives();
		sound_coin();
		sound_coin();
		sound_coin();
		sound_coin();
		hit_count = 8; //IMPOSIBLE WE HAVE 6 ENEMIES
	}
	game_print_score();
}

void player_set1(void){
	sprite = SPR_P1;
	sprite_other_player = SPR_P2;
	index_player = 0;
	tile_offset = 0;
	tile_offset_other_player = 24;
}

void player_set2(void){
	sprite = SPR_P2;
	sprite_other_player = SPR_P1;
	index_player = 1;
	tile_offset = 24;
	tile_offset_other_player = 0;
}

