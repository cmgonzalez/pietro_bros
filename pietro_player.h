#ifndef PIETRO_PLAYER_H
#define PIETRO_PLAYER_H

extern void          player_init(unsigned char f_sprite, unsigned char f_lin, unsigned char f_col, unsigned char f_tile);
extern unsigned char player_collition(void);
extern unsigned char player_collition_check(void);
extern void          player_kill(void);
extern void          player_restart(unsigned char f_sprite);
extern unsigned char player_move(void);
extern void          player_turn(void);
extern int           player_move_read_input(void);
extern void          player_hit_slipice(unsigned char f_enemies);
extern unsigned char player_hit_brick(void);
extern void          player_hit_pow(void);
extern void          player_coin(unsigned char f_enemies,unsigned char f_score);
extern void          player_score_add(unsigned int f_score);
extern unsigned char player_lost_life(void);
extern void          player_hit_brick_clear(void);
extern void          player_push(void);
unsigned char        player_push_check(void);
extern void          player_move_horizontal(void);

#endif
