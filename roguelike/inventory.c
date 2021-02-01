#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "header_file/rogue.h"

/*
 ノードを追加
*/
List *new_node(void){

	List *node;

	// メモリを確保
  	node = (List*)malloc(sizeof(List));

  	// メモリが足りなければ終了
	if(node == NULL){
		printf("メモリが取得できませんでした\n");
		exit(EXIT_FAILURE);
	}

	node->next = NULL;

	return node;
} 

/*
 リストを追加
*/
void append_list(List *list, int effect_value, char name[]){

	while(list->next != NULL) list = list->next;

	// リストに要素を追加
	list->effect_value = effect_value;
	strcpy(list->name, name);

	list->next = new_node();

	return;
}


/*
 リストの長さ
 return : リストの長さ
*/
int list_len(List *list){

	int len = 0;

	while(list->next != NULL){
		list = list->next;
		len++;
	}

	return len;
}

/*
 リストを削除
 return : 成功1, 失敗-1
*/
int remove_list(List **ndPtrPtr, int index){

	List  *next_node;

	while(index > 0 && *ndPtrPtr != NULL){
		ndPtrPtr = &((*ndPtrPtr)->next);
		index--;
	}

	if (*ndPtrPtr != NULL) {
        next_node  = (*ndPtrPtr)->next;

        free(*ndPtrPtr);
        *ndPtrPtr = next_node;

        return SUCCESS;
    } else {

        return FAILURE;
    }

}

/*
 アイテムを取る
*/
void get_item(Chara *player, List *storage, Item *item){

	if(player->memo == 7){
		for(int i = 0; i < MAX_ITEMS; i++){
			if(item[i].y == player->y && item[i].x == player->x){
				add_item(item[i].type, item[i].model_num, storage);
				player->memo = 2;
				break;
			}
		}
	}

	return;
}


/*
 インベントリにアイテムを追加
   type : 文章のタイプ, model : 型番
 name[] : 文章内に表示する名前
 	num : 表示する効果値 
*/
void add_item(int type, int model, List *storage){

	FILE *fp;

	char buf[BUFLEN];
	char tok[BUFLEN];	// トークン文字列のバッファ配列
	char *expr;			// line 内の次のトークンへのポインタ

	int effect_value;
	int flag;

	fp = fopen("../roguelike/text/item.txt", "r");
	if(fp == NULL){
		perror("fopen failed (log.c)");
        exit(EXIT_FAILURE);
	}

	while (fgets(buf, BUFLEN, fp) != NULL) {

		expr = buf;
		flag = 0;
		effect_value = 0;

		for(int i = 0; i < 5; i++){

			expr = sgettok(expr, tok, BUFLEN);

				// tok を抽出，expr を更新
			if (expr == NULL) break;

			if(i == 0 && (int)*tok != type  + 48) break;
			else if(i == 1 && (int)*tok != model + 48) break;
			else if(i == 3) effect_value = *tok;
			else if(i == 4) flag = 1;
			
		}

		if(flag == 1){
			append_list(storage, effect_value, tok);
		}
	}

	//後処理
	if(fp != NULL) fclose(fp);
	return ;
}