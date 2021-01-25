#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* キューのデータ */
typedef struct DATA_T{
  char data[50];
  struct DATA_T *next;
} DATA;

/* キューの情報を構造体 */
typedef struct {
  /* キューの先頭を指すポインタ */
  DATA *head;
  /* キューの最後尾を指すポインタ */
  DATA *tail;
} QUEUE;

void init_queue(QUEUE *queue);
void finish_queue(QUEUE *queue);
void print_queue(QUEUE *queue);
void enqueue(QUEUE *queue, char input[]);
void deqeueue(QUEUE *queue);

/* キューを初期化する関数 */
void init_queue(QUEUE *queue){

  queue->head = NULL;
  queue->tail = NULL;
}

/* キューの終了処理をする関数 */
void finish_queue(QUEUE *queue){
  DATA *p, *next;
  p = queue->head;
  while(p != NULL){
    next = p->next;
    free(p);
    p = next;
  }
}

/* エンキューする関数 */
void enqueue(QUEUE *queue, char input[]){

  DATA *add;

  /* エンキューするデータを保持するメモリを確保 */
  add = (DATA*)malloc(sizeof(DATA));

  /* メモリが足りなければ何もせず関数終了 */
  if(add == NULL){
    printf("メモリが取得ないためエンキューできません\n");
    return ;
  }

  strcpy(add->data, input);
  add->next = NULL;

  /* キューが空の場合は先頭に追加するのみ */
  if(queue->head == NULL){
    queue->head = add;
    queue->tail = add;
    return ;
  }

  /* キューの最後尾に追加したデータを追加 */
  /* キューの最後尾のデータのnextを追加データのアドレスにセット */
  queue->tail->next = add;

  /* キューの最後尾を指すtailを追加したデータのアドレスにセット */
  queue->tail = add;
}

/* デキューする関数 */
void dequeue(QUEUE *queue){
  int i = 0;
  DATA *tmpNext;

  /* キューが空なら何もせずに関数終了 */
  if(queue->head == NULL){
    printf("キューが空です\n");
    return ;
  }

  /* デキューするデータのnextポインタの指すアドレスを退避 */
  tmpNext = queue->head->next;

  /* デキューしたデータ（先頭データ）を削除 */
  free(queue->head);

  /* 先頭を指すheadポインタを先頭の次のデータのアドレス（退避したアドレス）にセット */
  queue->head = tmpNext;

  return ;
}

/* キューの中身を表示 */
void print_queue(QUEUE *queue){
  int i = 0;
  DATA *p;

  p = queue->head;
  while(p != NULL){
    printf("%s,", p->data);
    p = p->next;
  }
  printf("\n");
}

int main(void){

  int  m;
  char input[50];
  int  output;
  QUEUE queue;

  /* キューを初期化 */
  init_queue(&queue);

  while(1){
    /* ユーザからメニューを選んでもらう */
    printf("メニュー\n 1:エンキュー\n 2:デキュー\n 3:キュー表示\n それ以外:終了\n");
    scanf("%d", &m);

    /* 選ばれたメニューに応じて処理を分岐 */
    if(m == 1){

      printf("エンキューする数は？（0以上の整数のみ可）");
      scanf("%s", input);
    
      printf("%sをエンキューします\n", input);
      enqueue(&queue, input);

    }else if(m == 2){

      printf("デキューします\n");
      dequeue(&queue);
      
    } else if(m == 3){

      printf("キューの中身を表示します\n");
      print_queue(&queue);

    } else {

      /* 1, 2, 3以外の場合はwhile文を抜け出すためにbreakを実行 */
      printf("終了します\n");
      break;

    }
  }

  finish_queue(&queue);

  return 0;
}