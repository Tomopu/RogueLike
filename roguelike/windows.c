#include <ncurses.h>
#include <locale.h>		// setlocale()
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>		// usleep()

#include "header_file/rogue.h"



void draw_flame(Mark *position){

	mvaddstr(PTOP   , PLEFTMOST  , "1");
	mvaddstr(PTOP   , PRIGHTMOST , "2");
	mvaddstr(PBOTTOM, PLEFTMOST  , "3");
	mvaddstr(PBOTTOM, PRIGHTMOST , "4");

	return;
}


/*
 STATUS画面の表示
*/
void draw_status(Mark *position, Chara *player){

	int hp_bar;

	attrset(COLOR_PAIR(1));  // 黒地に緑文字

	mvprintw(PTOP  , PLEFTMOST, "╔══════/ STATUS /═════════════════════════════════════════════════════════════════════╗");
	for(int i = 1; i < 5; i++){
		mvaddstr(PTOP+i, PLEFTMOST	  ,"║");
		mvaddstr(PTOP+i, PLEFTMOST+86, "║");
	}
	mvprintw(PTOP+5, PLEFTMOST, "╚═════════════════════════════════════════════════════════════════════/ STATUS /══════╝");

	mvaddstr(PTOP+2, PLEFTMOST+3 , " HP: ");
	mvprintw(PTOP+2, PLEFTMOST+70, "地下%2d階", player->status.floor);
	mvprintw(PTOP+3, PLEFTMOST+3 , "ATK: %-3d  DEF: %-3d  EXP: %-4d(/%-4d)  LV: %-4d GOLD: %2u", PLAYER_ATTACK, PLAYER_DEFENCE, PLAYER_EXP, PLAYER_BORDER_EXP, PLAYER_LEVEL, PLAYER_MONEY);

// 	PLAYER_HP = 67;

	hp_bar = (int)(((double)PLAYER_HP/PLAYER_MAX_HP)*10);

	attrset(COLOR_PAIR(4));  // 赤地に白文字
	for(int i = 0; i < hp_bar*2; i++){
		mvprintw(PTOP+2, PLEFTMOST+8+i, " ");
	}

	attrset(COLOR_PAIR(1));  // 黒地に緑文字
	mvprintw(PTOP+2, PLEFTMOST+8+10*2, "(%4d/%4d)", PLAYER_HP, PLAYER_MAX_HP);

	return;
}


/*
 STATUS画面の初期化
*/
void erase_status(Mark *position){

	attrset(COLOR_PAIR(1));  // 黒地に緑文字

	for(int i = 1; i < 5; i++){
		for(int j = 1; j < 86; j++){
			mvaddstr(PTOP+i, PLEFTMOST+j, " ");
		}
	}

	return;
}


/*
 INFO画面の初期化
*/
void erase_log(Mark *position){

	attrset(COLOR_PAIR(1));  // 黒地に緑文字

	for(int i = 1; i < 10; i++){
		for(int j = 0; j < 85; j++){
			mvaddstr(PBOTTOM-10+i, PLEFTMOST+1+j, " ");
		}
	}

	return;
}



/*
 LOG画面の表示
*/
void draw_log(Mark *position, Queue *message){

	int i = 0;
	Data *p;

	attrset(COLOR_PAIR(1));  // 黒地に緑文字

	mvprintw(PBOTTOM-10, PLEFTMOST, "╔══════/ LOG /════════════════════════════════════════════════════════════════════════╗");
	for(int i = 1; i < 10; i++){
		mvaddstr(PBOTTOM-10+i, PLEFTMOST   , "║");
		mvaddstr(PBOTTOM-10+i, PLEFTMOST+86, "║");
	}
	mvprintw(PBOTTOM   , PLEFTMOST, "╚════════════════════════════════════════════════════════════════════════/ LOG /══════╝");

	erase_log(position);

	// ログを表示
  	p = message->head;
	while(p != NULL){
		mvprintw(PBOTTOM-9+i, PLEFTMOST+3, "%s ", p->data);
		p = p->next;
		i++;
	}

	return;
}


/*
 INVENTRY画面の初期化
*/
void erase_inventry(Mark *position){

	attrset(COLOR_PAIR(1));  // 黒地に緑文字

	for(int i = 1; i < 12; i++){
		for(int j = 0; j < 85; j++){
			mvaddstr(PTOP+i, PRIGHTMOST-85+j, " ");
		}
	}

	return;
}


/*
 INVENTRY画面の表示
*/
void draw_inventry(Mark *position, int color_flag, List *list, int choice){

	int number = 0;
	int i = 2;

	attrset(COLOR_PAIR(1));  // 黒地に緑文字

    if(color_flag == 1) attrset(COLOR_PAIR(1) | A_BOLD);    // 黒地に緑文字 強調

	mvprintw(PTOP   , PRIGHTMOST-86, "╔══════/ INVENTRY /══════════════════════════════════════════════════════[%2d/%2d]══════╗", list_len(list), 10);
	for(int i = 1; i < 12; i++){
		mvaddstr(PTOP+i, PRIGHTMOST-86, "║");
		mvaddstr(PTOP+i, PRIGHTMOST   , "║");
	}
	mvprintw(PTOP+12, PRIGHTMOST-86, "╚══════[R-key]══════════════════════════════════════════════════════/ INVENTRY /══════╝");

	mvprintw(PTOP+10, PRIGHTMOST-44, "%p", list->next);		

	while(list->next != NULL){

		attrset(COLOR_PAIR(3) | A_BOLD);    // 黒地に白文字 強調
		mvprintw(PTOP+i, PRIGHTMOST-83, "%-2d", number);

		if(choice == number){
			attrset(COLOR_PAIR(2));    			// 黒地に緑文字
			mvprintw(PTOP+i, PRIGHTMOST-80, "%s", list->name);
			attrset(COLOR_PAIR(1));    			// 黒地に緑文字
		}else{
			attrset(COLOR_PAIR(1));    			// 黒地に緑文字
			mvprintw(PTOP+i, PRIGHTMOST-80, "%s", list->name);		
		}

		i++;
		number++;
		list = list->next;
	}

	return;
}


/*
 inventryを表示
*/
void inventry(Mark *position, List *list){
    
    int key;    		// 入力キー
    int choice = -1;	// 選択したアイテム
        
    while(1){
        
        draw_inventry(position, 1, list, choice-48);
        

        attrset(COLOR_PAIR(2));  // 緑地に白文字
        mvprintw(PTOP+10 , PRIGHTMOST-84, "[R] INVENTRYを終了");

        // 入力
        key = getch();

        if(47 < key && key < list_len(list)+48){
        	if(key == choice)	choice = -1;
        	else choice = key;
        }

        // アイテムを使う・捨てる
        if(choice != -1){
        	if(key == 't'){
        		remove_list(&list, choice-48);
        		choice = -1;
        	//	draw_inventry(position, 1, list, choice-48);
        	}
        }

        erase_inventry(position);

        if(choice != -1){
        	attrset(COLOR_PAIR(2));  // 緑地に白文字
        	mvprintw(PTOP+10 , PRIGHTMOST-65, "[U] つかう");
        	mvprintw(PTOP+10 , PRIGHTMOST-54, "[T] 捨てる");
        }

        // 終了
        if(key == 'r' || key == 'q' || key == 'w' || key == 'a' || key == 's' || key == 'd') break;

    }

    return;
}


/*
 EQUIPMENT画面の表示
*/
void draw_equipment(Mark *position, int color_flag){

	attrset(COLOR_PAIR(1));  // 黒地に緑文字

	if(color_flag == 1) attrset(COLOR_PAIR(1) | A_BOLD);    // 黒地に緑文字 強調

	mvprintw(PTOP+14, PRIGHTMOST-86, "╔══════/ EQUIPMWNT /═════════════════════════════════════════════════════[%2d/%2d]══════╗", 3, 10);
	for(int i = 1; i < 15; i++){				 
		mvaddstr(PTOP+14+i, PRIGHTMOST-86, "║");
		mvaddstr(PTOP+14+i, PRIGHTMOST   , "║");
	}
	mvprintw(PTOP+29, PRIGHTMOST-86, "╚══════[E-key]═════════════════════════════════════════════════════/ EQUIPMWNT /══════╝");

	mvprintw(PTOP+16, PRIGHTMOST-84, "────────────────────────────────────────────────────────");
	mvprintw(PTOP+22, PRIGHTMOST-84, "────────────────────────────────────────────────────────");

	attrset(COLOR_PAIR(3));	// 黒地に白文字
	mvprintw(PTOP+16, PRIGHTMOST-78, "WEAPON");
	mvprintw(PTOP+22, PRIGHTMOST-78, "ARMOR");

	attrset(COLOR_PAIR(1));  // 黒地に緑文字

	return;
}


/*
 equipmentを表示
*/
void equipment(Mark *position){

	int key;    // 入力キー

	while(1){
        
        draw_equipment(position, 1);

        attrset(COLOR_PAIR(2));  // 緑地に白文字
        mvprintw(PTOP+27, PRIGHTMOST-84, "[E] EQUIPMENTを終了");

        // 入力
        key = getch();

        // 終了
        if(key == 'e' || key == 'q' || key == 'w' || key == 'a' || key == 's' || key == 'd') break;

    }

	return;
}


/*
 INFO画面の表示
*/
void draw_info(Mark *position, int *info_num, int color_flag){

	attrset(COLOR_PAIR(1));	 // 黒地に緑文字

	if(color_flag == 1) attrset(COLOR_PAIR(1) | A_BOLD);	// 黒地に緑文字 強調

	mvprintw(PBOTTOM-21, PRIGHTMOST-86, "╔══════/ INFO /═══════════════════════════════════════════════════════════════════════╗");
	for(int i = 1; i < 21; i++){				  
		mvaddstr(PBOTTOM-21+i, PRIGHTMOST-86, "║");
		mvaddstr(PBOTTOM-21+i, PRIGHTMOST   , "║");
	}
	mvprintw(PBOTTOM   , PRIGHTMOST-86, "╚══════[I-key]══════════════════════════════════════════════════════════/ INFO /══════╝");

//	mvprintw(PBOTTOM-21, PRIGHTMOST-70, "[ ]");

	if(*info_num == 1){

		erase_info(position);

		attrset(COLOR_PAIR(2));  // 緑地に白文字
		mvprintw(PBOTTOM-19, PRIGHTMOST-84, "[1] 操作方法");

		attrset(COLOR_PAIR(1));  // 黒地に緑文字
		mvprintw(PBOTTOM-19, PRIGHTMOST-70, "[2] マス種類");
		mvprintw(PBOTTOM-19, PRIGHTMOST-56, "[3] 武器情報");
		mvprintw(PBOTTOM-19, PRIGHTMOST-42, "[4] 防具情報");

		mvprintw(PBOTTOM-18, PRIGHTMOST-83, "");
		mvprintw(PBOTTOM-17, PRIGHTMOST-83, " - [W]  前移動");
		mvprintw(PBOTTOM-16, PRIGHTMOST-83, " - [A]  左移動");
		mvprintw(PBOTTOM-15, PRIGHTMOST-83, " - [S]  後移動");
		mvprintw(PBOTTOM-14, PRIGHTMOST-83, " - [D]  右移動");

		mvprintw(PBOTTOM-12, PRIGHTMOST-83, " - [↑]  上攻撃");
		mvprintw(PBOTTOM-11, PRIGHTMOST-83, " - [→]  右攻撃");
		mvprintw(PBOTTOM-10, PRIGHTMOST-83, " - [↓]  下攻撃");
		mvprintw(PBOTTOM- 9, PRIGHTMOST-83, " - [←]  左攻撃");

		mvprintw(PBOTTOM- 7, PRIGHTMOST-83, " - [Space]  アイテムの回収");
		mvprintw(PBOTTOM- 5, PRIGHTMOST-83, " アイテムや装備を回収するときは、");
		mvprintw(PBOTTOM- 4, PRIGHTMOST-83, " アイテムの上に乗ってから[Space]を押してください。");

		mvprintw(PBOTTOM-17, PRIGHTMOST-50, " - [R]  INVENTRY  の操作");
		mvprintw(PBOTTOM-16, PRIGHTMOST-50, " - [E]  EQUIPMWNT の操作");
		mvprintw(PBOTTOM-15, PRIGHTMOST-47, " - [0]  アイテムの番号を入力");
		mvprintw(PBOTTOM-14, PRIGHTMOST-47, " - [U]  アイテムを使用する");
		mvprintw(PBOTTOM-13, PRIGHTMOST-47, " - [T]  アイテムを捨てる");

		mvprintw(PBOTTOM-11, PRIGHTMOST-50, " - [I]  INFO画面   の操作");
		mvprintw(PBOTTOM-10, PRIGHTMOST-47, " - [1]  操作方法の説明");
		mvprintw(PBOTTOM- 9, PRIGHTMOST-47, " - [2]  マスの種類の説明");
		mvprintw(PBOTTOM- 8, PRIGHTMOST-47, " - [3]  武器の説明");
		mvprintw(PBOTTOM- 7, PRIGHTMOST-47, " - [4]  防具の説明");

		

		
	}else if(*info_num == 2){

		erase_info(position);

		mvprintw(PBOTTOM-19, PRIGHTMOST-84, "[1] 操作方法");

		attrset(COLOR_PAIR(2));  // 緑地に白文字
		mvprintw(PBOTTOM-19, PRIGHTMOST-70, "[2] マス種類");

		attrset(COLOR_PAIR(1));  // 黒地に緑文字
		mvprintw(PBOTTOM-19, PRIGHTMOST-56, "[3] 武器情報");
		mvprintw(PBOTTOM-19, PRIGHTMOST-42, "[4] 防具情報");

		attrset(COLOR_PAIR(4));  // 赤地に白文字
		mvprintw(PBOTTOM-17, PRIGHTMOST-81, "@");

		attrset(COLOR_PAIR(1));  // 黒地に緑文字
		mvprintw(PBOTTOM-17, PRIGHTMOST-79, ": プレイヤー");
		mvprintw(PBOTTOM-17, PRIGHTMOST-66, "操作するキャラクター ([1] 操作方法 を参照)");

		attrset(COLOR_PAIR(2));  // 緑地に白文字
		mvprintw(PBOTTOM-16, PRIGHTMOST-81, " ");

		attrset(COLOR_PAIR(1));  // 黒地に緑文字
		mvprintw(PBOTTOM-16, PRIGHTMOST-79, ": 壁");
		mvprintw(PBOTTOM-16, PRIGHTMOST-66, "ダンジョンの部屋を囲む壁 (キャラクターは進めない)");

		attrset(COLOR_PAIR(7));  // 黒地に水色文字
		mvprintw(PBOTTOM-15, PRIGHTMOST-81, ".");

		attrset(COLOR_PAIR(1));  // 黒地に緑文字
		mvprintw(PBOTTOM-15, PRIGHTMOST-79, ": 床");
		mvprintw(PBOTTOM-15, PRIGHTMOST-66, "キャラクター, アイテムが出現する");

		attrset(COLOR_PAIR(7));  // 黒地に水色文字
		mvprintw(PBOTTOM-14, PRIGHTMOST-81, "#");

		attrset(COLOR_PAIR(1));  // 黒地に緑文字
		mvprintw(PBOTTOM-14, PRIGHTMOST-79, ": 通路");
		mvprintw(PBOTTOM-14, PRIGHTMOST-66, "部屋と部屋を繋ぐ床 (アイテムは出現しない)");

		attrset(COLOR_PAIR(9));  // 黒地に黄色文字
		mvprintw(PBOTTOM-13, PRIGHTMOST-81, "%%");

		attrset(COLOR_PAIR(1));  // 黒地に緑文字
		mvprintw(PBOTTOM-13, PRIGHTMOST-79, ": 階段");
		mvprintw(PBOTTOM-13, PRIGHTMOST-66, "次のフロアへと続く階段");


		attrset(COLOR_PAIR(8));  // 紫地に白文字
		mvprintw(PBOTTOM-12, PRIGHTMOST-81, "A");

		attrset(COLOR_PAIR(1));  // 黒地に緑文字
		mvprintw(PBOTTOM-12, PRIGHTMOST-79, ": 敵");
		mvprintw(PBOTTOM-12, PRIGHTMOST-66, "(弱 A < B < ... < Y < Z 強)");

		attrset(COLOR_PAIR(9));  // 黒地に黄色文字
        mvprintw(PBOTTOM-11, PRIGHTMOST-81, "*");

        attrset(COLOR_PAIR(1));  // 黒地に緑文字
        mvprintw(PBOTTOM-11, PRIGHTMOST-79, ": アイテム");
		mvprintw(PBOTTOM-11, PRIGHTMOST-66, "次のフロアへと続く階段");



	}else if(*info_num == 3){

		erase_info(position);

		mvprintw(PBOTTOM-19, PRIGHTMOST-84, "[1] 操作方法");
		mvprintw(PBOTTOM-19, PRIGHTMOST-70, "[2] マス種類");
		
		attrset(COLOR_PAIR(2));  // 緑地に白文字
		mvprintw(PBOTTOM-19, PRIGHTMOST-56, "[3] 武器情報");

		attrset(COLOR_PAIR(1));  // 黒地に緑文字
		mvprintw(PBOTTOM-19, PRIGHTMOST-42, "[4] 防具情報");

	}else if(*info_num == 4){

		erase_info(position);

		mvprintw(PBOTTOM-19, PRIGHTMOST-84, "[1] 操作方法");
		mvprintw(PBOTTOM-19, PRIGHTMOST-70, "[2] マス種類");
		mvprintw(PBOTTOM-19, PRIGHTMOST-56, "[3] 武器情報");

		attrset(COLOR_PAIR(2));  // 緑地に白文字
		mvprintw(PBOTTOM-19, PRIGHTMOST-42, "[4] 防具情報");

		attrset(COLOR_PAIR(1));  // 黒地に緑文字
	}

	return;
}


/*
 INFO画面の初期化
*/
void erase_info(Mark *position){

	attrset(COLOR_PAIR(1));  // 黒地に緑文字

	for(int i = 0; i < 20; i++){
		for(int j = 0; j < 85; j++){
			mvaddstr(PBOTTOM-20+i, PRIGHTMOST-85+j, " ");
		}
	}

	return;
}


/*
 infoを表示
 info_num : infoの表示番号
*/
void info(Mark *position, int *info_num){

	int key;	// 入力キー

	while(1){

		draw_info(position, info_num, 1);

		attrset(COLOR_PAIR(2));  // 緑地に白文字
		mvprintw(PBOTTOM-2 , PRIGHTMOST-84, "[I] INFOを終了");

		// 入力
		key = getch();

		// 終了
		if(key == 'i' || key == 'q' || key == 'w' || key == 'a' || key == 's' || key == 'd') break;

		if(key == '1' || key == '2' || key == '3' || key == '4') *info_num = key-48;
	
	}

	return;

}