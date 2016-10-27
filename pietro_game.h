#ifndef PIETRO_GAME_H
#define PIETRO_GAME_H

#ifdef __LLVM

extern void            game_fill_row(unsigned char f_row, unsigned char f_asc);

#endif

#ifdef __SDCC

extern void            game_fill_row(unsigned char f_row, unsigned char f_asc) __z88dk_callee;

#endif

#ifdef __SCCZ80

extern void __CALLEE__ game_fill_row(unsigned char f_row, unsigned char f_asc);

#endif

extern unsigned int	 game_calc_index(unsigned char l, unsigned char c);
extern void			 game_draw_pow(void);
extern void			 game_back_fix1(void);
extern void			 game_back_fix2(void);
extern void			 game_back_fix3(void);
extern void			 game_back_fix4(void);
extern void			 game_back_fix5(void);
extern void			 game_cortina_brick(void);
extern void			 game_cortina_pipes(void);
extern void			 game_draw_back(void);
extern void			 game_print_lives(void);
extern void			 game_tick(void);
extern void			 game_start_timer(void);
extern unsigned char game_phase_calc(void);
extern void			 game_phase_init(void);
extern void			 game_phase_print(unsigned char f_row);
extern void			 player_set1(void);
extern void			 player_set2(void);
extern void			 game_loop(void);
extern void			 game_kill_all_sprites(void);
extern unsigned char game_check_maze(int f_index);
extern unsigned char game_enemy_add(void);
extern unsigned char game_enemy_rnd(void);
extern unsigned char game_enemy_quota(void);
extern unsigned char game_enemy_add1(unsigned char f_class);
extern unsigned char game_enemy_add_get_index(unsigned char f_search);
extern void			 game_freeze(unsigned char f_lin, unsigned char f_col);
extern void			 game_print_score(void);
extern void			 game_menu_e(unsigned char f_col, unsigned char e_c0,unsigned char e_c1,unsigned char e_start,unsigned char f_sign);
extern unsigned char game_menu_handle( unsigned char f_col, unsigned char f_inc, unsigned char f_start, unsigned char f_end, unsigned int timeout);
extern void			 game_paint_attrib(unsigned char e_r1);
extern void			 game_paint_attrib_lin(unsigned char f_start,unsigned char f_end,unsigned char f_lin);
extern void			 game_paint_attrib_lin_h(unsigned char f_start,unsigned char f_end,unsigned char f_lin);
extern void			 game_colour_message( unsigned char f_row, unsigned char f_col, unsigned char f_col2, unsigned int f_miliseconds	);
extern void			 game_rotate_attrib( );
extern void			 game_menu(void);
extern void			 game_menu_config(void);
extern unsigned char game_check_time(unsigned int start, unsigned int lapse);
extern void			 game_clear_water_splash();
extern void			 game_draw_water_splash( unsigned char f_col);
extern void			 game_draw_clear(void);
extern void			 game_bonus_summary(void);
extern void			 game_bonus_summary_player(unsigned char f_index);
extern void			 game_bonus_clock(void);
extern void			 game_phase_print_score_back(void);
extern void			 game_score_osd(void);
extern void			 game_hall_of_fame(void);
extern void			 game_hall_enter(void);
extern void			 game_hall_enter_phs(unsigned char f_row,unsigned char f_col);
extern unsigned char game_hall_check(unsigned char p_index);
extern void			 game_hall_edit_p(unsigned char *player, unsigned char *selected, unsigned char *cnt, unsigned char f_col, unsigned char f_row, unsigned char f_inc);
extern void			 game_hall_print_p(unsigned char selected, unsigned char f_row,unsigned char f_col, unsigned char f_tile, unsigned char f_inc);

#endif
