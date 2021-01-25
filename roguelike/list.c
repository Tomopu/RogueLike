#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "header_file/rogue.h"

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

void append_list(List *list, int type, int model_num){

	while(list->next != NULL) list = list->next;

	// リストに要素を追加
	list->type 	   = type;
	list->model_num = model_num;

	list->next = new_node();

	return;
}

void print_list(List *list){

	printf("( ");

	while(list->next != NULL){

		printf("(%d %d) ", list->type, list->model_num);
		list = list->next;
	}
	
	printf(")\n");

	return ;
}

int list_len(List *list){

	int len = 0;

	while(list->next != NULL){
		list = list->next;
		len++;
	}

	return len;
}

int remove_list(List **ndPtrPtr, Property *item, int index){

	List  *next_node;

	while(index > 0 && *ndPtrPtr != NULL){
		ndPtrPtr = &((*ndPtrPtr)->next);
		index--;
	}

	if (*ndPtrPtr != NULL) {
        next_node  = (*ndPtrPtr)->next;

        item->type = (*ndPtrPtr)->type;
        item->model_num = (*ndPtrPtr)->model_num;

        free(*ndPtrPtr);
        *ndPtrPtr = next_node;

        return SUCCESS;
    } else {

        return FAILURE;
    }

}


int abcd(void){

	int storage_max = 5;
	int type;
	int model_num;
	int x;

	List  *storage;
	Property item = {0, 0};

	//初期化
	storage = new_node();

	for(int i = 0; i < 7; i++){
			append_list(storage, i, i);
		}

	while(1){
		/*
		print_list(storage);
		printf("%d\n", list_len(storage));

		printf("入力 >");
		scanf("%d %d", &type, &model_num);
		append(storage, type, model_num);
		*/

		print_list(storage);
		printf("%d\n", list_len(storage));

		printf("削除しますか? >");
		scanf("%d", &x);
		if(x >= 0){
			remove_list(&storage, &item, x);
		}

		printf("type : %d, model_num : %d", item.type, item.model_num);
	}

	return 0;
}



/*
 インベントリにアイテムを追加
   type : 文章のタイプ, model : 型番
 name[] : 文章内に表示する名前
 	num : 表示する効果値 
*/
void add_sentence(int type, int model, char name[], int num, Queue *queue){

	FILE *fp;

	char buf[BUFLEN];
	char tok[BUFLEN];	// トークン文字列のバッファ配列
	char *expr;			// line 内の次のトークンへのポインタ
	char input[BUFLEN];

	int flag;

	fp = fopen("../roguelike/text/log.txt", "r");
	if(fp == NULL){
		perror("fopen failed (log.c)");
        exit(EXIT_FAILURE);
	}

	while (fgets(buf, BUFLEN, fp) != NULL) {

		expr = buf;
		flag = 0;

		for(int i = 0; i < 3; i++){

			expr = sgettok(expr, tok, BUFLEN);

				// tok を抽出，expr を更新
			if (expr == NULL) break;

			if(i == 0 && (int)*tok != type  + 48) break;
			else if(i == 1 && (int)*tok != model + 48) break;
			else if(i == 2) flag = 1;
			
		}

		if(flag == 1){
			sprintf(input, tok, name, num);
			enqueue(queue, input);
		}
	}

	//後処理
	if(fp != NULL) fclose(fp);
	
	return ;
}