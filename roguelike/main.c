#include <ncurses.h>
#include <locale.h>		// setlocale()
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>		// usleep()

#include "header_file/rogue.h"


int main(void){
	int 	key;		// 入力情報
	int 	w, h;		// 画面のheight, width
	Coord 	disp;		// ディスプレイの中心座標

	setlocale(LC_ALL, "");		// 日本語の利用

    // 端末制御開始
    initscr();

    noecho();                   // 入力文字を非表示
    cbreak();                   // Enter 不要
    keypad(stdscr, TRUE);       // カーソルキーを有効化
    curs_set(0);                // カーソルを非表示

    // 色の準備
    start_color();
    init_pair(0, COLOR_BLACK , COLOR_BLACK); 	// 色0 : 黒地に黒文字
    init_pair(1, COLOR_GREEN , COLOR_BLACK);	// 色1 : 黒地に緑文字
    init_pair(2, COLOR_WHITE , COLOR_GREEN);	// 色2 : 緑地に白文字
    init_pair(3, COLOR_WHITE , COLOR_BLACK); 	// 色3 : 黒地に白文字
    init_pair(4, COLOR_WHITE , COLOR_RED  ); 	// 色4 : 赤地に白文字
    init_pair(5, COLOR_WHITE , COLOR_BLUE ); 	// 色5 : 青地に白文字
    init_pair(6, COLOR_BLUE  , COLOR_BLACK); 	// 色6 : 黒地に青文字
    init_pair(7, COLOR_CYAN  , COLOR_BLACK); 	// 色7 : 黒地に水色文字
    init_pair(8, COLOR_WHITE , COLOR_MAGENTA);	// 色8 : 紫地に白文字
    init_pair(9, COLOR_YELLOW, COLOR_BLACK);	// 色9 : 黒地に黄色文字
    

    
    bkgd(COLOR_PAIR(0));                   // 背景は色0 (黒地に黒文字)

    // 画面の大きさを取得
    getmaxyx(stdscr, h, w);
    disp.y = h/2;
    disp.x = w/2;
    
    // メインループ
    while (1) {

        key = Title(&disp);
        if(key == 'q') break;

        switch (key){
        	case 'n': Game(&disp, 0);	break;
        	case 'r': Game(&disp, 1);	break;
        	case 'i': Instr();			break;
        }
    }

    // 終了
    endwin();       // 端末制御の終了	
	return 0;
}

/*
 テキストファイルから, fieldに格納
*/
void export_field(char file_name[], int *field){

	FILE *fp;
	int x;				// ファイルからの入力
	int index = 0;		// fieldのインデックス

	fp = fopen(file_name, "r");
	if(fp == NULL){
		perror("fopen failed (main.c -> export();)");
        timeout(-1);
        getch();        // キー入力
        endwin();		// 端末制御の終了
        exit(EXIT_FAILURE);
	}

	while (1){

		x = fgetc(fp);				// ファイルから1文字取得 	

    	// 改行文字飛ばし
		if(x == 10) continue;		// ASCII 10: LF(改行)

		// 終了条件
    	if(x == EOF){				// 読み尽くした場合
			if(H*W == index)  break; 
			else exit(EXIT_FAILURE);
		}

		field[index] = x - 48;		// ファイルからfieldに格納
		index++;					// index をずらす

	}

	//後処理
	if(fp != NULL) fclose(fp);
	return;
}

/*
 ゲームの準備
 プレイヤーの初期位置を確定
*/
void init_game(Chara *player, Chara *enemy, int *field){

	int player_h, player_w;		// プレイヤーの仮初期位置
	int enemy_h , enemy_w;		// 敵の仮初期位置
	int stairs_h, stairs_w;		// 階段の仮初期位置

	// プレイヤーの初期位置
	while(1){

		player_h = rand()%H;
		player_w = rand()%W;

		// 終了条件
		//if(field[player->y * W + player->x] == 2)
		if(field[player_h * W + player_w] == 2){

			player->y = player_h;
			player->x = player_w;

			break;
		}
	}
	
	// 初期位置をfieldに反映
	field[player->y * W + player->x] = 5;	// 5:プレイヤー

	player->memo = 2;


	// 敵の初期位置
	for(int i = 0; i < ENEMY_NUM; i++){
		while(1){

			enemy_h = rand()%H;
			enemy_w = rand()%W;

			// 終了条件
			if(field[enemy_h * W + enemy_w] == 2){

				enemy[i].y = enemy_h;
				enemy[i].x = enemy_w;

				enemy[i].rank = 'A';

				break;
			}

		}

		// 初期位置をfieldに反映
		field[enemy[i].y * W + enemy[i].x] = 6;	// 6:敵

		enemy[i].memo = 2;
	}


	// 階段の初期位置
	while(1){

		stairs_h = rand()%H;
		stairs_w = rand()%W;

		// 終了条件
		if(field[stairs_h * W + stairs_w] == 2){
			break;
		}

	}

	field[stairs_h * W + stairs_w] = 4;

	return;
}

/*
 playerのステータスを準備
*/
void init_player_status(Chara *player){		

	PLAYER_MAX_HP 	  = 100;				// 初期体力上限
	PLAYER_HP 		  = PLAYER_MAX_HP;		// 初期体力
	PLAYER_ATTACK 	  = 15;					// 初期攻撃力
	PLAYER_DEFENCE 	  = 5;					// 初期防御力
	PLAYER_EXP 		  = 0;					// 初期経験値
	PLAYER_BORDER_EXP = 10;					// 初期境界経験値
	PLAYER_LEVEL 	  = 1;					// 初期レベル
	PLAYER_MONEY 	  = 0;					// 初期所持金

}


void draw_enemy(Chara *enemy, int h, int w, int y, int x){

	attrset(COLOR_PAIR(8));  // 紫地に白文字
	for(int i = 0; i < ENEMY_NUM; i++){
		if(enemy[i].y == y || enemy[i].x == x){
			mvprintw(y+h, x+w, "%c", enemy[i].rank);
			break;
		}
	}

	attrset(COLOR_PAIR(1));  // 黒地に緑文字

	return;
}

// 色0 : 黒地に黒文字
// 色1 : 黒地に緑文字
// 色2 : 緑地に白文字
// 色3 : 黒地に白文字
// 色4 : 赤地に白文字
// 色5 : 青地に白文字
// 色6 : 黒地に青文字
// 色7 : 黒字に水色文字
// 色8 : 紫地に白文字
// 色9 : 黒地に黄色文字
/*
 ダンジョンを表示
*/
void draw_dungeon(int *field, Chara *enemy, int h, int w){
    for(int y = 0; y < H; y++){
        for(int x = 0; x < W; x++){

        	attrset(COLOR_PAIR(5));  // 青地に白文字
        	mvaddstr(0+h  , 0+w  , " ");
        	mvaddstr(0+h  , W-1+w, " ");
        	mvaddstr(H-1+h, 0+w  , " ");
        	mvaddstr(H-1+h, W-1+w, " ");

            if(field[y*W+x] == 0){
                attrset(COLOR_PAIR(0));	 // 黒地に黒文字
				mvaddstr(y+h, x+w, " ");
			//	mvprintw(y+h, x+w, "%d", field[y*W+x]);
            }else if(field[y*W+x] == 1){
                attrset(COLOR_PAIR(2));  // 緑地に白文字
				mvaddstr(y+h, x+w, " ");
            }else if(field[y*W+x] == 2){
                attrset(COLOR_PAIR(7));  // 黒地に水色文字
                mvaddstr(y+h, x+w, ".");
            }else if(field[y*W+x] == 3){
                attrset(COLOR_PAIR(7));  // 黒地に水色文字
                mvprintw(y+h, x+w, "#");
            }else if(field[y*W+x] == 4){
                attrset(COLOR_PAIR(9));  // 黒地に黄色文字
                mvprintw(y+h, x+w, "%%");
            }else if(field[y*W+x] == 5){
                attrset(COLOR_PAIR(4));  // 赤地に白文字
              	mvprintw(y+h, x+w, "@");
            }else if(field[y*W+x] == 6){
                draw_enemy(enemy, h, w, y ,x);
            }else{
                exit(EXIT_FAILURE);
            }
        }
    }

    return ;
}






/*
 歩けるか確認
 dire : 歩く先の方向
*/
void walk(int *field, Chara *player, Chara *enemy, int dire){

    if(dire == 0){
        if(UP == 2 || UP == 3){
        	field[player->y*W + player->x] = player->memo;
        	(player->y)--;
        	player->memo = field[player->y*W + player->x];
        	field[player->y*W + player->x] = 5;
        	for(int i = 0; i < ENEMY_NUM; i++){
				enemy_act((int *) field, &enemy[i]);
			}
        }else if(UP == 4){
        	make_dungeon();
        	export_field("../field/map.txt", (int *)field);
        	init_game(player, enemy, (int *)field);
        	player->status.floor++;
        }
    }else if (dire == 3){
        if(RIGHT == 2 || RIGHT == 3){
        	field[player->y*W + player->x] = player->memo;
        	(player->x)++;
        	player->memo = field[player->y*W + player->x];
        	field[player->y*W + player->x] = 5;
        	for(int i = 0; i < ENEMY_NUM; i++){
				enemy_act((int *) field, &enemy[i]);
			}
        }else if(RIGHT == 4){
        	make_dungeon();
        	export_field("../field/map.txt", (int *)field);
        	init_game(player, enemy, (int *)field);
        	player->status.floor++;
        }
    }else if (dire == 6){
        if(DOWN == 2 || DOWN == 3){
        	field[player->y*W + player->x] = player->memo;
        	(player->y)++;
        	player->memo = field[player->y*W + player->x];
        	field[player->y*W + player->x] = 5;
        	for(int i = 0; i < ENEMY_NUM; i++){
				enemy_act((int *) field, &enemy[i]);
			}
       	}else if(DOWN == 4){
        	make_dungeon();
        	export_field("../field/map.txt", (int *)field);
        	init_game(player, enemy, (int *)field);
        	player->status.floor++;
        }
    }else if (dire == 9){
        if(LEFT == 2 || LEFT == 3){
        	field[player->y*W + player->x] = player->memo;
        	(player->x)--;
        	player->memo = field[player->y*W + player->x];
        	field[player->y*W + player->x] = 5;
        	for(int i = 0; i < ENEMY_NUM; i++){
				enemy_act((int *) field, &enemy[i]);
			}
        }else if(LEFT == 4){
        	make_dungeon();
        	export_field("../field/map.txt", (int *)field);
        	init_game(player, enemy, (int *)field);
        	player->status.floor++;
        }
    }else{
    	exit(EXIT_FAILURE);
    }
    
    return;
}


/*
 タイトル画面を表示
*/
int Title(Coord *disp){

	int ttlen  = 74;					// タイトルの長さ
	int frmlen = 36;
	int sth    = disp->y - 15;			// 表示最上位置 
	int ttlpos = disp->x - ttlen/2;		// タイトルの表示位置(x座標)
	int chrpos = disp->x - 10;			// 文字の表示位置(x座標)
	int frmpos = disp->x - frmlen/2;	// 枠の表示位置　(x座標)
	
	erase();							// 画面の消去
	attrset(COLOR_PAIR(1));				// 色を指定 (黒)


	// タイトル (ASCII ART)
	mvaddstr( sth+1, ttlpos, "██████╗  ██████╗  ██████╗ ██╗   ██╗███████╗    ██╗     ██╗██╗  ██╗███████╗");
	mvaddstr( sth+2, ttlpos, "██╔══██╗██╔═══██╗██╔════╝ ██║   ██║██╔════╝    ██║     ██║██║ ██╔╝██╔════╝");
	mvaddstr( sth+3, ttlpos, "██████╔╝██║   ██║██║  ███╗██║   ██║█████╗      ██║     ██║█████╔╝ █████╗  ");
	mvaddstr( sth+4, ttlpos, "██╔══██╗██║   ██║██║   ██║██║   ██║██╔══╝      ██║     ██║██╔═██╗ ██╔══╝  ");
	mvaddstr( sth+5, ttlpos, "██║  ██║╚██████╔╝╚██████╔╝╚██████╔╝███████╗    ███████╗██║██║  ██╗███████╗");
	mvaddstr( sth+6, ttlpos, "╚═╝  ╚═╝ ╚═════╝  ╚═════╝  ╚═════╝ ╚══════╝    ╚══════╝╚═╝╚═╝  ╚═╝╚══════╝");

	// 案内の表示
	mvaddstr( sth+15, chrpos, "  [N] - ニューゲーム");
	mvaddstr( sth+16, chrpos, "  [R] - ロード");
	mvaddstr( sth+17, chrpos, "  [I] - 説明書");
	mvaddstr( sth+18, chrpos, "  [Q] - 終了");
	
	// 枠の表示
	mvprintw( sth+13, frmpos, "╔══════════════════════════════════╗");
	for(int i = 0; i < 6; i++){
		mvaddstr( sth+14+i, frmpos	 , "║");
		mvaddstr( sth+14+i, frmpos+35, "║");
	}
	mvprintw( sth+20, frmpos, "╚══════════════════════════════════╝");


	refresh();				// 画面の再表示
	return (getch());
}

void game_over(Mark *position){
	mvprintw(1, 1, "Game Over");
	timeout(-1);
    getch();        // キー入力
}

/*
 入力の管理
*/
void ctrl(int *field, int key, Chara *player, Chara *enemy, Mark *position, int *info_num){

	switch (key){
		case 'w': walk((int *)field, player, enemy, 0); break;
		case 'd': walk((int *)field, player, enemy, 3); break;
		case 's': walk((int *)field, player, enemy, 6); break;
		case 'a': walk((int *)field, player, enemy, 9); break;
		case 'i': info(position, info_num);	 break;
        case 'r': inventry(position);        break;
        case 'e': equipment(position);		 break;
        case 'p': player->status.hp = player->status.hp - 10;
        case KEY_UP:	player_attack((int *)field, player, enemy, 0);
        case KEY_RIGHT:	player_attack((int *)field, player, enemy, 3);
        case KEY_DOWN:	player_attack((int *)field, player, enemy, 6);
        case KEY_LEFT:	player_attack((int *)field, player, enemy, 9);
    }

	return;
}

/*
  ゲーム本体
  mode = 0 : Newgame
  mode = 1 : rode
*/
void Game(Coord *disp, int mode){

	int field[H][W] = {};		// 0:空白, 1:壁, 2:床, 3:廊下, 4:階段, 5:プレイヤー, 6:敵
	int key;
	int info_num = 1;			// infoの表示番号

	Mark flame = {{0, 0}, {52, 180}};		// 配置の大外枠
	Mark position;

	Chara  player;				// プレイヤー
	Chara  enemy[ENEMY_NUM];    // 敵

    
    /*
    Category = {type, model_num, sentence};
    type : アイテムの種類   - 1:武器, 2:防具, 3:回復・強化アイテム
    model_num : そのアイテムの種類内での型番号
    sentence : そのアイテムに関する説明
    */
//	Category item[MAX_ITEMS] = {};

	// 構造体の中身を全て0に設定
	memset(&player, 0, sizeof(player));

	// 大外枠の初期化
	position.start.y = disp->y - (flame.end.y - flame.start.y)/2;
	position.start.x = disp->x + (flame.end.x - flame.start.x)/2;
	position.end.y   = disp->y + (flame.end.y - flame.start.y)/2;
	position.end.x   = disp->x - (flame.end.x - flame.start.x)/2;

	player.status.floor = 1;
	

    // フィールドをロード
	if(mode == 0){			// ニューゲーム

		// コマンドを実行
		make_dungeon();
		export_field("../field/map.txt", (int *)field);

		// 初期設定
		init_game(&player, enemy, (int *)field);
		init_player_status(&player);

	} else if(mode == 1){	// ロード

		// セーブデータをロード
		export_field("../save/save_data.txt", (int *)field);

		// 初期設定
		init_game(&player, enemy, (int *)field);

	} 


	// メインループ
	while (1){

		// ゲームオーバー
		if(player.status.hp <= 0){
			erase_status(&position);			// ステータス画面を消去
			draw_status(&position, &player);	// ステータス画面を更新
			game_over(&position);				// ゲームオーバー画面
			
			break;
		}

		erase();		// 画面の消去

//		draw_flame(&position);

		// ステータスを表示
		draw_status(&position, &player);
		
		// ログを表示
		draw_log(&position);
		
		// インベントリーを表示
		draw_inventry(&position, 0);
		
		// 装備を表示
		draw_equipment(&position, 0);
		
		// 情報を表示
		draw_info(&position, &info_num, 0);
		
		// ダンジョンを表示
		draw_dungeon((int *)field, enemy, TOP+7, LEFTMOST+6);   // field を表示

		refresh();      // 画面の再表示

		// 入力
		key = getch();

		ctrl((int *) field, key, &player, enemy, &position, &info_num);

		// 終了
		if(key == 'q') break;

	}
	
	return;
}

/*
 Instruction 導入
 ゲームのルール説明, 操作方法など
*/
void Instr(void){

	Coord chrpos = {5, 5};			// 文字の表示位置(x座標)
	int key;

	// ループ
	while (1){

		erase();							// 画面の消去

		attrset(COLOR_PAIR(1) | A_BOLD);  // 黒地に緑文字
		mvprintw(chrpos.y   , chrpos.x   , "ダンジョン自動生成 ROGUE LIKE");

		attrset(COLOR_PAIR(1));  // 黒地に緑文字
		mvprintw(chrpos.y+1 , chrpos.x+1 , "2021.01 プログラミング言語 ⅡB 自主制作課題");
		mvprintw(chrpos.y+1 , chrpos.x+60, "3年情報工学分野 4番 泉知成");

		attrset(COLOR_PAIR(1) | A_BOLD);  // 黒地に緑文字
		mvprintw(chrpos.y+3 , chrpos.x   , "ストーリー");

		attrset(COLOR_PAIR(1));  // 黒地に緑文字
		mvprintw(chrpos.y+4 , chrpos.x+1 , "あなたは、とある城下町で有名な狩人です.　ある日、王室に招かれたあなたは、王様から");
		mvprintw(chrpos.y+5 , chrpos.x+1 , "「町外れにある洞窟を探索し、病で苦しむ姫を助けるために、地下50階にある\"秘薬\"を回収してほしい.」");
		mvprintw(chrpos.y+6 , chrpos.x+1 , "と、依頼されました. 成功すれば騎士団の一員とされる上、持ち帰った財宝を全て貰うことができるそうです。");
		mvprintw(chrpos.y+7 , chrpos.x+1 , "試練を達成するため、あなたは家族や友人に別れを告げ、その洞窟へと入っていったのでした.");

		attrset(COLOR_PAIR(1) | A_BOLD);  // 黒地に緑文字
		mvprintw(chrpos.y+9 , chrpos.x   , "ゲーム説明");

		attrset(COLOR_PAIR(1));  // 黒地に緑文字
		mvprintw(chrpos.y+10, chrpos.x+1 , "ゲームを開始すると、ランダムに生成されたダンジョンにスポーンします.　どこかに設置された階段を探して地下50回を目指してください.");
		mvprintw(chrpos.y+11, chrpos.x+1 , "ダンジョン内に現れる敵キャラに倒されると、それまで集めた所持品は全て失い、所持金は６割となってリスポーンします.");
		mvprintw(chrpos.y+12, chrpos.x+1 , "詳しい操作方法は、ゲーム開始後に[I]キーを押してINFO Windowをご覧ください.");




		attrset(COLOR_PAIR(1) | A_BOLD);  // 黒地に緑文字
//		mvprintw(chrpos.y+3 , chrpos.x+1 , "ローグ(Rogue)とは.");

		attrset(COLOR_PAIR(1));  // 黒地に緑文字
//		mvprintw(chrpos.y+4 , chrpos.x+1 , "『ローグ』 (Rogue) は、ダンジョン探索型のコンピュータRPGである。");
//		mvprintw(chrpos.y+5 , chrpos.x+1 , "その初版が公表されたのは1980年とコンピュータRPGの黎明期であり、最初期のコンピュータRPGの内の1つである。");



		/*
		mvaddstr( chrpos.y    , chrpos.x   , " 【 移動 】");	
		mvaddstr( chrpos.y +2 , chrpos.x +1, "  [W] - 前");
		mvaddstr( chrpos.y +3 , chrpos.x +1, "  [A] - 左        [W]");
		mvaddstr( chrpos.y +4 , chrpos.x +1, "  [S] - 下     [A][S][D]");
		mvaddstr( chrpos.y +5 , chrpos.x +1, "  [D] - 右");
		*/


		attrset(COLOR_PAIR(2));				// 色を指定 (緑地に白文字)

		mvaddstr( chrpos.y +20, chrpos.x, "  [Q] - 終了  ");	

		refresh();                  // 画面の再表示

		//終了
		key = getch();
		if(key == 'q') break;

	}

	return;
}
