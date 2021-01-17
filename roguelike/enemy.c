#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "enemy.h"

void enemy_walk(int *field, Chara *enemy, int dire){

	if(dire == 0){
        if(ENEMY_UP == 2 || ENEMY_UP == 3){
        	field[enemy->y*W + enemy->x] = enemy->memo;
        	(enemy->y)--;
        	enemy->memo = field[enemy->y*W + enemy->x];
        	field[enemy->y*W + enemy->x] = 6;
        }
    }else if (dire == 3){
        if(ENEMY_RIGHT == 2 || ENEMY_RIGHT == 3){
        	field[enemy->y*W + enemy->x] = enemy->memo;
        	(enemy->x)++;
        	enemy->memo = field[enemy->y*W + enemy->x];
        	field[enemy->y*W + enemy->x] = 6;
        }
    }else if (dire == 6){
        if(ENEMY_DOWN == 2 || ENEMY_DOWN == 3){
        	field[enemy->y*W + enemy->x] = enemy->memo;
        	(enemy->y)++;
        	enemy->memo = field[enemy->y*W + enemy->x];
        	field[enemy->y*W + enemy->x] = 6;
       	}
    }else if (dire == 9){
        if(ENEMY_LEFT == 2 || ENEMY_LEFT == 3){
        	field[enemy->y*W + enemy->x] = enemy->memo;
        	(enemy->x)--;
        	enemy->memo = field[enemy->y*W + enemy->x];
        	field[enemy->y*W + enemy->x] = 6;
        }
    }else{
    	exit(EXIT_FAILURE);
    }


	return;
}

int set_direction(int dire){

	dire = rand()%4;

	if(dire == 0) 		dire = 0;
	else if(dire == 1)	dire = 3;
	else if(dire == 2)	dire = 6;
	else if(dire == 3)	dire = 9;

	return dire;
}


/*
 敵の行動
*/
void enemy_act(int *field, Chara *enemy){

	int dire = 0;
	int exe_count = 0;		// 実行回数

	if(exe_count %10 == 0)	dire = set_direction(dire);

	enemy_walk((int *)field, enemy, dire);

	
	return;
}
