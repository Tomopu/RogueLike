#ifndef ENEMY_H
#define ENEMY_H

#include "basic.h"

#define ENEMY_NUM 3
#define SEARCH_RANGE 13		// サーチ範囲の範囲(幅)

#define ENEMY_UP	field[(enemy->y-1)* W + enemy->x]
#define ENEMY_RIGHT	field[enemy->y*W +  (enemy->x+1)]
#define ENEMY_DOWN	field[(enemy->y+1)* W + enemy->x]
#define ENEMY_LEFT	field[enemy->y*W +  (enemy->x-1)]


extern void enemy_walkUp(int *field, Chara *enemy);
extern void enemy_walkRight(int *field, Chara *enemy);
extern void enemy_walkDown(int *field, Chara *enemy);
extern void enemy_walkLeft(int *field, Chara *enemy);


extern void enemy_walk(int *field, Chara *enemy, int *ddire);

extern int  set_direction(int dire);
extern void enemy_act(int *field, Chara *enemy);

#endif