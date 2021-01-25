#include <ncurses.h>
#include <locale.h>		// setlocale()
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>		// usleep()

#include "header_file/rogue.h"

/*
 デキューする関数
*/
void dequeue(Queue *queue){
  
  Data *tmpNext;

  // キューが空なら何もせずに関数終了
  if(queue->head == NULL){
    printf("キューが空です\n");
    return ;
  }

  // デキューするデータのnextポインタの指すアドレスを退避
  tmpNext = queue->head->next;

  // デキューしたデータ（先頭データ）を削除
  free(queue->head);

  // 先頭を指すheadポインタを先頭の次のデータのアドレス（退避したアドレス）にセット
  queue->head = tmpNext;

  return ;
}

/*
 攻撃先の敵の特定
*/
int collation_coord(int *enemy_coord, int coord){
	
	for(int i = 0; i < ENEMY_NUM; i++){
		if(enemy_coord[i] == coord) return i;
	}

	return -1;
}

/*
 プレイヤーの攻撃メッセージを表示
*/
void player_attack_message(Chara *player, Queue *message){

	if(queue_len(message) > MAX_MESSAGE) dequeue(message);
	add_sentence(1, 1, "player", player->status.attack, message);

	return;
}

/*
 プレイヤーの攻撃
*/
void player_attack(int *field, Chara *player, Chara *enemy, Queue *message, Mark *position, int dire){

	int enemy_coord[ENEMY_NUM] = {};

	for(int i = 0; i < ENEMY_NUM; i++){
		enemy_coord[i] = enemy[i].y * W + enemy[i].x; 
	}

	if(dire == 0){
		if(UP == 6){
			enemy[collation_coord(enemy_coord, (player->y-1)*W + player->x)].status.hp -= player->status.attack;
			player_attack_message(player, message);

			for(int i = 0; i < ENEMY_NUM; i++){
				enemy_act((int *) field, &enemy[i], player, message, position);
			}
		}
	}else if(dire == 3){
		if(RIGHT == 6){
			enemy[collation_coord(enemy_coord, player->y*W + (player->x+1))].status.hp -= player->status.attack;
			player_attack_message(player, message);

			for(int i = 0; i < ENEMY_NUM; i++){
				enemy_act((int *) field, &enemy[i], player, message, position);
			}
		}
	}else if(dire == 6){
		if(DOWN == 6){
			enemy[collation_coord(enemy_coord, (player->y+1)*W + player->x)].status.hp -= player->status.attack;
			player_attack_message(player, message);

			for(int i = 0; i < ENEMY_NUM; i++){
				enemy_act((int *) field, &enemy[i], player, message, position);
			}
		}
	}else if(dire == 9){
		if(LEFT == 6){
			enemy[collation_coord(enemy_coord, player->y*W + (player->x-1))].status.hp -= player->status.attack;
			player_attack_message(player, message);

			for(int i = 0; i < ENEMY_NUM; i++){
				enemy_act((int *) field, &enemy[i], player, message, position);
			}
		}
		
	}

	return;
}


/*
 敵の攻撃メッセージを表示
*/
void enemy_attack_message(Chara *enemy, Queue *message){

	if(queue_len(message) > MAX_MESSAGE) dequeue(message);
	add_sentence(2, 1, "enemy ", enemy->status.attack, message);

	return;
}


/*
 敵の攻撃
*/
void enemy_attack(Chara *player, Chara *enemy, Queue *message, Mark *position){

	// プレイヤーに攻撃
	player->status.hp -= enemy->status.attack;
	
	enemy_attack_message(enemy, message);

	// ログを表示
	draw_log(position, message);

	return;
}




/*
 バトル
*/
void battle(int *field, Chara *player, Chara *enemy, Queue *message){


	return;
}



