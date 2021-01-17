#ifndef ENEMY_H
#define ENEMY_H

#include "basic.h"

#define ENEMY_NUM 2

#define ENEMY_UP	field[(enemy->y-1)* W + enemy->x]
#define ENEMY_RIGHT	field[enemy->y*W +  (enemy->x+1)]
#define ENEMY_DOWN	field[(enemy->y+1)* W + enemy->x]
#define ENEMY_LEFT	field[enemy->y*W +  (enemy->x-1)]

extern void enemy_walk(int *field, Chara *enemy, int dire);
extern int  set_direction(int dire);
extern void enemy_act(int *field, Chara *enemy);

#endif