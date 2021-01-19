#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "header_file/enemy.h"


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

void search(int *field, Chara *enemy, int *around){

    for(int i = enemy->y - SEARCH_RANGE/2; i <= enemy->y + SEARCH_RANGE/2; i++){
        for(int j = enemy->x - SEARCH_RANGE/2; j <= enemy->x + SEARCH_RANGE/2; j++){
            if(i < 0 || H < i || j < 0 || W < j)  *around = 9;
            else  *around = field[i * W + j];
            around++;
        }
    }

    return;
}

/*
 fieldをテキストファイルに格納 
 file_name : テキストファイルのパス
*/
void writing_around(char file_name[], int *around){

    FILE *fp;

    fp = fopen(file_name, "w");
    if(fp == NULL){
        perror("fopen failed (writing_around();)");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < SEARCH_RANGE; i++){
        for(int j = 0; j < SEARCH_RANGE; j++){
            fprintf(fp, "%d", around[i * SEARCH_RANGE + j]);    //　書き込み
        }
        fputc('\n', fp);
    }

    //後処理
    if(fp != NULL) fclose(fp);
    return;
}


/*
 敵の行動
*/
void enemy_act(int *field, Chara *enemy){

	int dire = 0;
	int exe_count = 0;		// 実行回数
    int around[SEARCH_RANGE][SEARCH_RANGE] = {};

    // 0:空白, 1:壁, 2:床, 3:廊下, 4:階段, 5:プレイヤー, 6:敵

    // 死亡処理
    if(enemy->status.hp <= 0){
        if(field[enemy->y * W + enemy->x] == 6) field[enemy->y * W + enemy->x] = enemy->memo;
        return;
    }

	if(exe_count %20 == 0)	dire = set_direction(dire);

    if(ENEMY_UP == 5)         printf("up");
    else if(ENEMY_RIGHT == 5) printf("right");
    else if(ENEMY_DOWN == 5)  printf("doen");
    else if(ENEMY_LEFT == 5)  printf("left");
    
    else enemy_walk((int *)field, enemy, &dire);

    search((int *)field, enemy, (int *)around);

    writing_around("../field/around_value.txt", (int *)around);

	return;
}
