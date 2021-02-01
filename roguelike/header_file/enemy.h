#ifndef ENEMY_H
#define ENEMY_H

#include "rogue.h"

#define ENEMY_NUM 5
#define SEARCH_RANGE 13		// サーチ範囲の範囲(幅)

#define ENEMY_UP	field[(enemy->y-1)* W + enemy->x]
#define ENEMY_RIGHT	field[enemy->y*W +  (enemy->x+1)]
#define ENEMY_DOWN	field[(enemy->y+1)* W + enemy->x]
#define ENEMY_LEFT	field[enemy->y*W +  (enemy->x-1)]


extern void enemy_walkUp(int *field, Chara *enemy);
extern void enemy_walkRight(int *field, Chara *enemy);
extern void enemy_walkDown(int *field, Chara *enemy);
extern void enemy_walkLeft(int *field, Chara *enemy);


extern void enemy_walk(int *field, Chara *enemy, int *dire);

extern int  search_player(int *around);

extern int  set_direction(int dire);
extern void enemy_exp_message(Chara *enemy, Queue *message);
extern void enemy_act(int *field, Chara *enemy, Chara *player, Queue *message, Mark *position);

#endif