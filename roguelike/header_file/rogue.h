#ifndef ROGUE_H
#define ROGUE_H

#include "dungeon.h"
#include "enemy.h"

#define UP		field[(player->y-1)* W + player->x]
#define RIGHT	field[player->y*W +  (player->x+1)]
#define DOWN	field[(player->y+1)* W + player->x]
#define LEFT	field[player->y*W +  (player->x-1)]

#define TOP			position.start.y
#define RIGHTMOST	position.start.x
#define BOTTOM		position.end.y
#define LEFTMOST	position.end.x

#define PTOP		position->start.y
#define PRIGHTMOST	position->start.x
#define PBOTTOM		position->end.y
#define PLEFTMOST	position->end.x

#define PLAYER_MAX_HP		player->status.max_hp
#define PLAYER_HP 			player->status.hp
#define PLAYER_ATTACK 		player->status.attack
#define PLAYER_DEFENCE 		player->status.defence
#define PLAYER_EXP 			player->status.exp
#define PLAYER_BORDER_EXP 	player->status.border_exp
#define PLAYER_LEVEL 		player->status.level
#define PLAYER_MONEY 		player->status.money

// プロトタイプ宣言
extern void export_field(char file_name[], int *field);					// field 情報を書き出し
extern void init_game(Chara *player, Chara *enemy, int *field);			// プレイヤーの初期位置を確定
extern void init_player_status(Chara *player);							// プレイヤーステータスの初期化
extern void draw_enemy(Chara *enemy, int h, int w, int y, int x);
extern void draw_dungeon(int *field, Chara *enemy, int h, int w);		// mapの描画
extern void output(int *field, int h, int w);							// 画面へ出力
extern void walk(int *field, Chara *player, Chara *enemy, int dire);	// 歩けるか確認
extern int  Title(Coord *disp);				// タイトル画面を表示
extern void ctrl(int *field, int key, Chara *player, Chara *enemy, Mark *position, int *info_num);		// 入力
extern void Game(Coord *disp, int mode);		// ゲーム本体
extern void Instr(void);						// 説明書を表示


extern void draw_flame(Mark *position);

// status
extern void draw_status(Mark *position, Chara *player);
extern void erase_status(Mark *position);

// log
extern void draw_log(Mark *position);

// inventry
extern void draw_inventry(Mark *position, int color_flag);
extern void inventry(Mark *position);

// equipment
extern void draw_equipment(Mark *position, int color_flag);
extern void equipment(Mark *position);

// info
extern void draw_info(Mark *position, int *info_num, int color_flag);
extern void erase_info(Mark *position);
extern void info(Mark *position, int *info_num);

extern void player_attack(int *field, Chara *player, Chara *enemy, int dire);

#endif