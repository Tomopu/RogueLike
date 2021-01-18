#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "enemy.h"


void enemy_walkUp(int *field, Chara *enemy){
    
    field[enemy->y*W + enemy->x] = enemy->memo;
    (enemy->y)--;
    enemy->memo = field[enemy->y*W + enemy->x];
    field[enemy->y*W + enemy->x] = 6;
    
    return;
}

void enemy_walkRight(int *field, Chara *enemy){

    field[enemy->y*W + enemy->x] = enemy->memo;
    (enemy->x)++;
    enemy->memo = field[enemy->y*W + enemy->x];
    field[enemy->y*W + enemy->x] = 6;

    return;
}

void enemy_walkDown(int *field, Chara *enemy){

    field[enemy->y*W + enemy->x] = enemy->memo;
    (enemy->y)++;
    enemy->memo = field[enemy->y*W + enemy->x];
    field[enemy->y*W + enemy->x] = 6;    

    return;
}

void enemy_walkLeft(int *field, Chara *enemy){

    field[enemy->y*W + enemy->x] = enemy->memo;
    (enemy->x)--;
    enemy->memo = field[enemy->y*W + enemy->x];
    field[enemy->y*W + enemy->x] = 6;

    return;
}

void enemy_walk(int *field, Chara *enemy, int *dire){

    while(1){

        if(*dire == 0){
            if(ENEMY_UP == 2 || ENEMY_UP == 3){
                enemy_walkUp((int *)field, enemy);
                break;
            }else{
                *dire = set_direction(*dire);
            }
        }else if (*dire == 3){
            if(ENEMY_RIGHT == 2 || ENEMY_RIGHT == 3){
                enemy_walkRight((int *)field, enemy);
                break;
            }else{
                *dire = set_direction(*dire);
            }
        }else if (*dire == 6){
            if(ENEMY_DOWN == 2 || ENEMY_DOWN == 3){
                enemy_walkDown((int *)field, enemy);
                break;
            }else{
                *dire = set_direction(*dire);
            }
        }else if (*dire == 9){
            if(ENEMY_LEFT == 2 || ENEMY_LEFT == 3){
                enemy_walkLeft((int *)field, enemy);
                break;
            }else{
                *dire = set_direction(*dire);
            }
        }else{
            exit(EXIT_FAILURE);
        }

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

	if(exe_count %20 == 0)	dire = set_direction(dire);

    if(ENEMY_UP == 5)         printf("up");
    else if(ENEMY_RIGHT == 5) printf("right");
    else if(ENEMY_DOWN == 5)  printf("doen");
    else if(ENEMY_LEFT == 5)  printf("left");
    else enemy_walk((int *)field, enemy, &dire);

	return;
}
