#include <ncurses.h>
#include <locale.h>		// setlocale()
#include <stdlib.h>
#include <stdio.h>

//構造体
typedef struct {
	int y;
	int x;
} Coord;

typedef struct {
	int x;
	int y;
	int memo;			// 前回いたマス目
} Chara;

typedef struct {
	int hp;
	int attack;
	int defence;
	int exp;
	unsigned int money;	// (0 ～ 4,294,967,295)
	
} Ability;

// split と共通(ヘッダーファイル等)
//#define H 		30
//#define W 		30

#define H 		34
#define W 		74


#define UP		field[(player->y-1)* W + player->x]
#define RIGHT	field[player->y*W +  (player->x+1)]
#define DOWN	field[(player->y+1)* W + player->x]
#define LEFT	field[player->y*W +  (player->x-1)]


// プロトタイプ宣言
void export(char file_name[], int *field);		// field 情報を書き出し
void init_game(Chara *player, int *field);		// プレイヤーの初期位置を確定
void draw_dungeon(int *field, int h, int w);	// mapの描画
void output(int *field, int h, int w);			// 画面へ出力
void ctrl(int *field, int key, Chara *player);	// 入力
void walk(int *field, Chara *player, int dire);	// 歩けるか確認



int  Title(Coord *disp);				// タイトル画面を表示
void Game(Coord *disp, int mode);		// ゲーム本体
void Instr(void);						// 説明書を表示




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
    init_pair(0, COLOR_BLACK, COLOR_BLACK); // 色0 : 黒地に黒文字
    init_pair(1, COLOR_GREEN, COLOR_BLACK);	// 色1 : 黒地に緑文字
    init_pair(2, COLOR_WHITE, COLOR_GREEN);	// 色2 : 緑地に白文字
    init_pair(3, COLOR_WHITE, COLOR_BLACK); // 色3 : 黒地に白文字
    init_pair(4, COLOR_WHITE, COLOR_RED  ); // 色4 : 赤地に白文字
    init_pair(5, COLOR_WHITE, COLOR_BLUE ); // 色5 : 青地に白文字
    init_pair(6, COLOR_BLUE , COLOR_BLACK); // 色6 : 黒地に青文字
    init_pair(7, COLOR_CYAN , COLOR_BLACK); // 色7 : 黒地に水色文字
    

    
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
void export(char file_name[], int *field){

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
void init_game(Chara *player, int *field,){

	int player_h, player_w;		// 仮初期位置

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
	field[player->y * W + player->x] = 4;	// 3:プレイヤー

	player->memo = 2;

	return;
}

/*
 playerのbilityを準備
*/

void init_player_ability(Ability *plyr_ability){

	plyr_ability.hp      = 100;
	plyr_ability.attack  = 15;
	plyr_ability.defence = 5;
	plyr_ability.exp 	 = 0;
	plyr_ability.money   = 0;

}



// 色0 : 黒地に黒文字
// 色1 : 黒地に緑文字
// 色2 : 緑地に白文字
// 色3 : 黒地に白文字
// 色4 : 赤地に白文字
// 色5 : 青地に白文字
// 色6 : 黒地に青文字
// 色7 : 黒字に水色文字
/*
 ダンジョンを表示
*/
void draw_dungeon(int *field, int h, int w){
    for(int y = 0; y < H; y++){
        for(int x = 0; x < W; x++){

        	#ifdef DEB
        		attrset(COLOR_PAIR(5));  // 青地に白文字
        		mvaddstr(0+h  , 0+w  , " ");
        		mvaddstr(0+h  , W-1+w, " ");
        		mvaddstr(H-1+h, 0+w  , " ");
        		mvaddstr(H-1+h, W-1+w, " ");
        	#endif

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
                attrset(COLOR_PAIR(7));  // 黒地に緑文字
                mvprintw(y+h, x+w, "#");
            }else if(field[y*W+x] == 4){
                attrset(COLOR_PAIR(4));  // 赤地に白文字
              mvprintw(y+h, x+w, "@");
            }else{
                exit(EXIT_FAILURE);
            }
        }
    }

    return ;
}

void draw_info(void){

}


void output(int *field, int h, int w){


	return;
}


/*
 入力の管理
*/
void ctrl(int *field, int key, Chara *player){
	switch (key){
		case 'w': walk((int *)field, player, 0); break;
		case 'd': walk((int *)field, player, 3); break;
		case 's': walk((int *)field, player, 6); break;
		case 'a': walk((int *)field, player, 9); break;
    }

	return;
}


/*
 歩けるか確認
 dire : 歩く先の方向
*/
void walk(int *field, Chara *player, int dire){

    if(dire == 0){
        if(UP == 2 || UP == 3){
        	field[player->y*W + player->x] = player->memo;
        	(player->y)--;
        	player->memo = field[player->y*W + player->x];
        	field[player->y*W + player->x] = 4;
        }
    }else if (dire == 3){
        if(RIGHT == 2 || RIGHT == 3){
        	field[player->y*W + player->x] = player->memo;
        	(player->x)++;
        	player->memo = field[player->y*W + player->x];
        	field[player->y*W + player->x] = 4;
        }
    }else if (dire == 6){
        if(DOWN == 2 || DOWN == 3){
        	field[player->y*W + player->x] = player->memo;
        	(player->y)++;
        	player->memo = field[player->y*W + player->x];
        	field[player->y*W + player->x] = 4;
       	}
    }else if (dire == 9){
        if(LEFT == 2 || LEFT == 3){
        	field[player->y*W + player->x] = player->memo;
        	(player->x)--;
        	player->memo = field[player->y*W + player->x];
        	field[player->y*W + player->x] = 4;
        }
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
	mvaddstr( sth+13, frmpos, "╔══════════════════════════════════╗");
	for(int i = 0; i < 6; i++){
		mvaddstr( sth+14+i, frmpos	 , "║");
		mvaddstr( sth+14+i, frmpos+35, "║");
	}
	mvaddstr( sth+20, frmpos, "╚══════════════════════════════════╝");


	refresh();				// 画面の再表示
	return (getch());
}


/*
  ゲーム本体
  mode = 0 : Newgame
  mode = 1 : rode
*/
void Game(Coord *disp, int mode){

	int field[H][W] = {};		// 0:空白, 1:壁, 2:床, 3:廊下, 4:プレイヤー
	int key;
	int 

	Chara 　player;				// プレイヤーの座標

	Ability plyr_ability;		// プレイヤーの能力

	// 構造体の中身を全て0に設定
	memset(player, 0, sizeof(player));
	memset(plyr_ability , 0, sizeof(plyr_ability));

    // フィールドをロード
	if(mode == 0){			// ニューゲーム

		// コマンドを実行
		system("./make");
		export("../field/map.txt", (int *)field);

		// 初期設定
		init_game(&player, (int *)field);


	} else if(mode == 1){	// ロード

		// セーブデータをロード
		export("../save/save_data.txt", (int *)field);

		// 初期設定
		init_game(&player, (int *)field);

	} 


	// メインループ
	while (1){
		
		erase();		// 画面の消去

		draw_dungeon((int *)field, disp->y - H/3, disp->x - W*1.1);   // field を表示

		mvprintw(100, 1, "player.y : %d, player.x : %d\n", player.y, player.x);
		mvprintw(101, 1, "up:%d, right:%d, down:%d, left:%d\n", field[(player.y-1)*W+player.x], field[player.y*W+player.x+1], field[(player.y+1)*W+player.x], field[player.y*W+player.x-1]);

		refresh();      // 画面の再表示

		// 入力
		key = getch();

		ctrl((int *) field, key, &player);

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

		attrset(COLOR_PAIR(0));				// 色を指定 (黒地に黒文字)

		mvaddstr( chrpos.y    , chrpos.x   , " 【 移動 】");	
		mvaddstr( chrpos.y +2 , chrpos.x +1, "  [W] - 前");
		mvaddstr( chrpos.y +3 , chrpos.x +1, "  [A] - 左        [W]");
		mvaddstr( chrpos.y +4 , chrpos.x +1, "  [S] - 下     [A][S][D]");
		mvaddstr( chrpos.y +5 , chrpos.x +1, "  [D] - 右");

		attrset(COLOR_PAIR(2));				// 色を指定 (緑地に白文字)

		mvaddstr( chrpos.y +20, chrpos.x, "  [Q] - 終了  ");	

		refresh();                  // 画面の再表示

		//終了
		key = getch();
		if(key == 'q') break;

	}

	return;
}