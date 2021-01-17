#ifndef BASIC_H
#define BASIC_H

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
	char sentence;
} Message;


// main.c, make_dungeon.c
#define H			34		// fieldの高さ
#define W			74		// fieldの幅



#endif