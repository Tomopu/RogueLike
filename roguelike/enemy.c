#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

#include "header_file/rogue.h"


/*
 上に歩く
*/
void enemy_walkUp(int *field, Chara *enemy){
    
    field[enemy->y*W + enemy->x] = enemy->memo;
    (enemy->y)--;
    enemy->memo = field[enemy->y*W + enemy->x];
    field[enemy->y*W + enemy->x] = 6;
    
    return;
}


/*
 右に歩く
*/
void enemy_walkRight(int *field, Chara *enemy){

    field[enemy->y*W + enemy->x] = enemy->memo;
    (enemy->x)++;
    enemy->memo = field[enemy->y*W + enemy->x];
    field[enemy->y*W + enemy->x] = 6;

    return;
}


/*
 下に歩く
*/
void enemy_walkDown(int *field, Chara *enemy){

    field[enemy->y*W + enemy->x] = enemy->memo;
    (enemy->y)++;
    enemy->memo = field[enemy->y*W + enemy->x];
    field[enemy->y*W + enemy->x] = 6;    

    return;
}


/*
 左に歩く
*/
void enemy_walkLeft(int *field, Chara *enemy){

    field[enemy->y*W + enemy->x] = enemy->memo;
    (enemy->x)--;
    enemy->memo = field[enemy->y*W + enemy->x];
    field[enemy->y*W + enemy->x] = 6;

    return;
}


/*
 敵の歩く処理
 */
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


/*
 進む方角をランダムに決定
*/
int set_direction(int dire){

	dire = rand()%4;

	if(dire == 0) 		dire = 0;
	else if(dire == 1)	dire = 3;
	else if(dire == 2)	dire = 6;
	else if(dire == 3)	dire = 9;

	return dire;
}


/*
 周囲を見渡す
*/
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
 周囲にプレイヤーがいるか
 return : プレイヤーがいれば1, いなければ0を返す
*/
int search_player(int *around){

    int flag = 0;

    for(int i = 0; i < SEARCH_RANGE; i++){
        for(int j = 0; j < SEARCH_RANGE; j++){
            if(around[i * SEARCH_RANGE + j] == 5){
                flag = 1;
                break;
            }
        }
    }

    return flag;
}


/*
 敵との距離
*/
int distance_to_player(int *around, int dire){

    Coord player;
    Coord enemy = {SEARCH_RANGE/2, SEARCH_RANGE/2};

    int y_dist = 0;     // y軸のplayerとenemyの距離
    int x_dist = 0;     // x軸のplayerとenemyの距離

    for(int i = 0; i < SEARCH_RANGE; i++){
        for(int j = 0; j < SEARCH_RANGE; j++){
            if(around[i * SEARCH_RANGE + j] == 5){
                player.x = j;
                player.y = i;
                break;
            }
        }
    }

    y_dist = player.y - enemy.y;
    x_dist = player.x - enemy.x;

    if(square(y_dist) > square(x_dist)){
        if     (y_dist > 0)   dire = 6;
        else if(y_dist < 0)   dire = 0;
    }else if(square(y_dist) < square(x_dist)){
        if     (x_dist > 0)   dire = 3;
        else if(x_dist < 0)   dire = 9;
    }

    return dire;
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
 敵の攻撃メッセージを表示
*/
void enemy_exp_message(Chara *enemy, Queue *message){

    if(queue_len(message) > MAX_MESSAGE) dequeue(message);
    add_sentence(2, 2, "経験値", enemy->status.exp, message);

    return;
}


/*
 敵の行動
*/
void enemy_act(int *field, Chara *enemy, Chara *player, Queue *message, Mark *position){

	int dire = 0;
	int exe_count = 0;		// 実行回数
    int around[SEARCH_RANGE][SEARCH_RANGE] = {};

    // 0:空白, 1:壁, 2:床, 3:廊下, 4:階段, 5:プレイヤー, 6:敵

    // 死亡処理
    if(enemy->status.hp <= 0){
        if(field[enemy->y * W + enemy->x] == 6){
            field[enemy->y * W + enemy->x] = enemy->memo;
            player->status.exp += enemy->status.exp;
            enemy_exp_message(enemy, message);
        }
        return;
    }

    writing_around("../field/around_value.txt", (int *)around);

    // 周囲を見渡す
    search((int *)field, enemy, (int *)around);

    if(ENEMY_UP == 5)         enemy_attack(player, enemy, message, position);
    else if(ENEMY_RIGHT == 5) enemy_attack(player, enemy, message, position);
    else if(ENEMY_DOWN == 5)  enemy_attack(player, enemy, message, position);
    else if(ENEMY_LEFT == 5)  enemy_attack(player, enemy, message, position);
    
    else{
        if(search_player((int *)around) == 1){
            dire = distance_to_player((int *)around, dire);
        }else if(exe_count %20 == 0){
            dire = set_direction(dire);
        }

        enemy_walk((int *)field, enemy, &dire);
    }

	return;
}
