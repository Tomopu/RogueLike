#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header_file/rogue.h"


/*
 キューを初期化する関数
*/
void init_queue(Queue *queue){

  queue->head = NULL;
  queue->tail = NULL;
}

/*
 キューの終了処理をする関数
*/
void finish_queue(Queue *queue){
  
  Data *p, *next;
  p = queue->head;
  while(p != NULL){
    next = p->next;
    free(p);
    p = next;
  }
}

/*
 エンキューする関数
*/
void enqueue(Queue *queue, char input[]){

  Data *add;

  // エンキューするデータを保持するメモリを確保
  add = (Data*)malloc(sizeof(Data));

  // メモリが足りなければ何もせず関数終了
  if(add == NULL){
    printf("メモリが取得できないためエンキューできません\n");
    return ;
  }

  strcpy(add->data, input);
  add->next = NULL;

  // キューが空の場合は先頭に追加するのみ
  if(queue->head == NULL){
    queue->head = add;
    queue->tail = add;
    return ;
  }

  // キューの最後尾に追加したデータを追加
  // キューの最後尾のデータのnextを追加データのアドレスにセット
  queue->tail->next = add;

  // キューの最後尾を指すtailを追加したデータのアドレスにセット
  queue->tail = add;
}


/*
 キューの中身を表示
*/
void print_queue(Queue *queue){
  
  Data *p;

  p = queue->head;
  while(p != NULL){
    printf("%s ", p->data);
    p = p->next;
  }
  printf("\n");
}

int queue_len(Queue *queue){
  
  int i = 0;
  Data *p;

  p = queue->head;
  while(p != NULL){
    i++;
    p = p->next;
  }

  return i;
}


char *sgettok(char *str, char *tok, int n){

	char *s;		// 行文字列内の文字へのポインタ
	char *t;		// トークン内の文字へのポインタ

	s = str;
	t = tok;

	n--;							// 終端記号分

	for (int i = 0; i < n; i++) {

		if(*s == '#' ) return NULL;
		if(*s == '\n') s++;
		if(*s == '\0') break;

		if(*s == ',' ){
			s++; 
			break;
		}

		*t = *s; 
		t++;	
		s++;
	}

	if (t == tok) return NULL;	// トークンなしの場合，エラー
	*t = '\0';	// トークンを終端		

	return (s);
}


/*
 文章を追加
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
