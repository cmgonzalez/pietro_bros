#ifndef PIETRO_ENEMIES_H
#define PIETRO_ENEMIES_H

extern void          enemy_coin1(void);
extern void          enemy_collition(void);
extern unsigned char enemy_collition_check(void);
extern void          enemy_hit(void);
extern void          enemy_flip(unsigned int f_tile);
extern void          enemy_flip_sidestepper(unsigned int f_tile);
extern void          enemy_turn(void);
extern void          enemy_standard(void);
extern void          enemy_slipice(void);
extern void          enemy_fireball_red(void);
extern void          enemy_fireball_green(void);
extern void          enemy_fighterfly(void);
extern void          enemy_trip(void);
extern void          enemy_standard_hit(void);
extern void          enemy_evolve(void);
extern void          enemy_upgrade(unsigned char f_class, unsigned int f_tile);
extern void          enemy_walk(void);
extern void          enemy_init(unsigned char f_sprite,unsigned char f_lin,unsigned  char f_col,unsigned  char f_class,unsigned  char f_dir);
extern void          enemy_kill(unsigned char f_sprite);

#endif
