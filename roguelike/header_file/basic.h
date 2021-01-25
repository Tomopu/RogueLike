#ifndef BASIC_H
#define BASIC_H

// main.c, make_dungeon.c
#define H			34		// fieldの高さ
#define W			74		// fieldの幅

// main.c テスト用
//#define H 		30
//#define W 		30

#define	BUFLEN		256

#define MAX_ITEMS     3       // アイテムの数

#define MAX_INVENTORY 1

#define MAX_MESSAGE   8

#define SUCCESS 1

#define FAILURE 0


// main.c, make_dungeon.c
// 座標に関する構造体
typedef struct {
	int y;
	int x;
} Coord;

// main.c
// キャラクターの能力を記録
typedef struct {
	int max_hp;
	int hp;
	int attack;
	int defence;
	int exp;
	int border_exp;
	int level;
	unsigned int money;	// (0 ～ 4,294,967,295)
	int floor;
} Status;

// main.c
// キャラクターの位置を記録
typedef struct {
	int    x;
	int    y;
	int    memo;		// 前回いたマス目
	Status status;		// そのキャラクターのレベル
	char   rank;		// キャラクターランク	
} Chara;

// make_dungeon.c
// 始まりの座標と終わりの座標を記録
typedef struct {
	Coord start;
	Coord end;
} Mark;

// make_dungeon.c
// 通路に関する構造体
typedef struct {
	Coord start;
	Coord end;
	int dire;		// 0:上, 3:右, 6:下, 9:左
	int flag;
} Road;

// main.c
// ログに表示するメッセージ
typedef struct {
	int  type;
	int  model_num;
	char sentence[BUFLEN];
} Category;

// キューのデータ
typedef struct DATA{
  char data[BUFLEN];
  struct DATA *next;
} Data;

// キューの情報を構造体
typedef struct {
  Data *head;	// キューの先頭ポインタ 
  Data *tail;	// キューの最後尾ポインタ
} Queue;

/*
 Category = {type, model_num, sentence};
      type : アイテムの種類	1:武器, 2:防具, 3:回復・強化アイテム
 model_num : そのアイテムの種類内での型番号
  sentence : そのアイテムに関する説明
*/

/*
Category item[MAX_ITEMS] = {
	{3, 1, "薬草"},
	{3, 2, "回復薬"}
};
*/
#endif
