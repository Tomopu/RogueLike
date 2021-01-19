#ifndef DUNGEON_H
#define DUNGEON_H

#include "basic.h"

// マクロ定義
#define AREA_MAX 	8		// 分割回数の最大値

#define A 			5		// 分割パラメータ A
#define B 			3		// 分割パラメータ B

#define MIN_WIDTH	4		// 部屋の最小幅
#define MAX_WIDTH	15		// 部屋の最大幅

// プロトタイプ宣言

// エリアの分割
extern void init_split(Mark *area);								// 分割の初期処理
extern int  sizeY(Mark *elem);									// 縦幅を取得
extern int  sizeX(Mark *elem);									// 横幅を取得
extern int  max_area(Mark *area, int split_count);				// 最大の面積を持つエリアの番号
extern int  split_adj(int split, Mark *area, int line, int m);	// 分割位置の調整
extern void split_area(Mark *area, int line, int m, int n);		// エリアを分割

//部屋を作成
extern void make_room(Mark *area, Mark *room, int room_num);	// 部屋を作成
extern void store_room(int *field, Mark *room, int room_num);	// ルームをfieldに格納
extern void frame(int *field, Mark *room, int room_num);		// ルームを枠で囲む

// 通路を作成
extern int  extend_aisle(Mark *area, Road *aisle, Mark *room, int room_num);	// 通路を伸ばす
extern int  square(int x);														// 自乗計算
extern int  nearest(Road *aisle, int candidate[], int asl_count, int dire, int index);  //一番近い通路
extern int  search_aisle(Road *aisle, int asl_num, int dire, int coord, int index, int conect_num);  // 繋げる通路を探す
extern int  connect_aisle(Road *aisle, int asl_num);			// 通路同士を繋げる
extern int  greater(int a, int b);								// aとbを比べて大きい方を返す
extern int  less(int a, int b);									// aとbを比べて小さい方を返す
extern void store_aisle(int *field, Road *aisle, int asl_num);	// 通路をfieldに格納

extern void dungeon(int *field);								// ダンジョンの生成
extern void writing_field(char file_name[], int *field);		// fieldをテキストファイルに格納
extern void make_dungeon(void);									// ダンジョンの自動生成	

#endif