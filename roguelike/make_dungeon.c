#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "header_file/dungeon.h"


/*
 分割の初期処理
*/
void init_split(Mark *area){

	// エリア0の終了位置を設定
	area[0].end.y = H-1;
	area[0].end.x = W-1;

	return;
}


/*
 縦幅を取得
   elem : Mark型のポインタ要素
 return : 縦幅
*/
int sizeY(Mark *elem){
	return (elem->end.y)+1 - elem->start.y;
}


/*
 横幅を取得
   elem : Mark型のポインタ要素
 return : 横幅
*/
int sizeX(Mark *elem){
	return (elem->end.x)+1 - elem->start.x;
}


/*
 最大の面積をもつエリアの番号
 split_count : エリアが分割された回数
 return 　　　: エリアの番号
*/
int max_area(Mark *area, int split_count){

	int current;	// 現在のエリアの面積
	int max = 0;	// 面積の最大値
	int num = 0;	// 最大値をもつ index

	for(int n = 0; n <= split_count; n++){
		current = sizeX(&area[n])*sizeY(&area[n]);
		// 情報を更新
		if(current > max){
			max = current;
			num = n;
		}
	}

	return num;
}

/*
 分割位置の調整 (分割位置逆流防止)
  split : 分割の仮位置
   line : 分割方向 0:縦, 1:横
      m : 分割するエリア番号
 return : 分割位置の座標
*/
int split_adj(int split, Mark *area, int line, int m){

	int X, Y;				// 分割する座標
	int para = rand()%A-B;	// 分割位置のパラメータ

	if(line == 0){
		// パラメータを適応
		X = split - area[m].start.x + para;

		// はみ出し確認
		if(X < area[m].start.x || X > area[m].end.x)  
			  return X - para;
		else  return X;

	} else if(line == 1){
		// パラメータを適応
		Y = split - area[m].start.y + para;

		// はみ出し確認
		if(Y < area[m].start.y || Y > area[m].end.y)
			  return Y - para;
		else  return Y;
	}

	exit(EXIT_FAILURE);
}


/*
 エリアを分割
 line : 分割方向
    m : 分割するエリア番号
    n : 分割してできるエリア番号
*/
void split_area(Mark *area, int line, int m, int n){

	int X, Y;		// 分割する座標
	int split;		// 分割の仮座標

	if(line == 0){			// 縦方向に分割
		// 分割位置を確定
		split = area[m].start.x + (sizeX(&area[m])/2);
		X = split_adj(split, area, line, m);

		// 分割後のエリアの座標を設定
		area[n].start.y = area[m].start.y;
		area[n].start.x = area[m].start.x + X+1;
		area[n].end.y   = area[m].end.y;
		area[n].end.x   = area[m].end.x;
		area[m].end.x   = area[m].start.x + X;

	} else if(line == 1){	// 横方向に分割
		// 分割位置を確定
		split = area[m].start.y + (sizeY(&area[m])/2);
		Y = split_adj(split, area, line, m);

		// 分割後のエリアの座標を設定
		area[n].start.y = area[m].start.y + Y+1;
		area[n].start.x = area[m].start.x;
		area[n].end.y   = area[m].end.y;
		area[n].end.x   = area[m].end.x;
		area[m].end.y   = area[m].start.y + Y;
	}

	return;
}


/*
 部屋を作成
 room_num : 部屋の数
*/
void make_room(Mark *area, Mark *room, int room_num){
	
	int width;								// 幅
	int room_h, room_w; 					// roomの縦幅, 横幅
	int room_start, room_end;				// roomの座標 
	int paraSY, paraSX, paraEY, paraEX;		// room幅調整パラメータ

	for(int n = 0; n < room_num; n++){

		room[n].start.y = area[n].start.y + 2;
		room[n].start.x = area[n].start.x + 2;
		room[n].end.y   = area[n].end.y   - 2;
		room[n].end.x   = area[n].end.x   - 2;

		room_h = sizeY(&room[n]);
		room_w = sizeX(&room[n]);

		// room の縦幅の調整
		if(room_h > MIN_WIDTH){

			while(1){
				// パラメータの調節
				paraSY = rand()%((sizeY(&room[n]) - MIN_WIDTH) + 1);
				paraEY = rand()%((sizeY(&room[n]) - MIN_WIDTH) + 1);

				room_start = room[n].start.y + paraSY;
				room_end   = room[n].end.y   - paraEY;

				width = room_end+1 - room_start;

				// 終了条件
				if(MIN_WIDTH <= width && width <= MAX_WIDTH){
					room[n].start.y = room_start;
					room[n].end.y   = room_end;
					break;
				}
			}
		}

		// room の横幅の調整
		if(room_w > MIN_WIDTH){

			while(1){
				// パラメータの調節
				paraSX = rand()%((sizeX(&room[n]) - MIN_WIDTH) + 1);
				paraEX = rand()%((sizeX(&room[n]) - MIN_WIDTH) + 1);

				room_start = room[n].start.x + paraSX;
				room_end   = room[n].end.x   - paraEX;

				width = room_end+1 - room_start;

				// 終了条件
				if(MIN_WIDTH <= width && width <= MAX_WIDTH){
					room[n].start.x = room_start;
					room[n].end.x   = room_end;
					break;
				}
			}
		}

	}

	return;
}


/*
 ルームをfieldに格納
 room_num : 部屋の数
*/
void store_room(int *field, Mark *room, int room_num){

	for(int n = 0; n < room_num; n++){
		for(int i = room[n].start.y; i <= room[n].end.y; i++){
			for(int j = room[n].start.x; j <= room[n].end.x; j++){
				field[i * W + j] = 2;
			}
		}
	}

	return ;
}


/*
 ルームを枠で囲む
 room_num : 部屋の数
*/
void frame(int *field, Mark *room, int room_num){
	for(int n = 0; n < room_num; n++){
		for(int i = room[n].start.y-1; i <= room[n].end.y+1; i++){
			for(int j = room[n].start.x-1; j <= room[n].end.x+1; j++){
				if((i * W + j) <= (room[n].start.y-1) * W + room[n].end.x+1)
					field[i * W + j] = 1;
				else if((i * W + j) >= (room[n].end.y+1) * W + room[n].start.x-1)
					field[i * W + j] = 1;
				else if((i * W + j) == i * W + room[n].start.x-1 || (i * W + j) == i * W + room[n].end.x+1)
					field[i * W + j] = 1;
			}
		}
	}
	return;
}


/*
 通路を伸ばす
 room_num : 部屋の数 (= 分割した回数)
 return : aisle_num (通路の数)
*/
int extend_aisle(Mark *area, Road *aisle, Mark *room, int room_num){

	// 四方にareaがあるかのフラグ
	int dire[4] = {};		// 0:上, 1:左, 2:下, 3:右
	int aisle_num = 0;		// 通路数
	int door;				// ドアの位置

	for(int n = 0; n < room_num; n++){
		
		if(area[n].start.y != 0  ) dire[0] = 1;		// 上にareaがある
		if(area[n].start.x != 0  ) dire[1] = 1;		// 左にareaがある
		if(area[n].end.y   != H-1) dire[2] = 1;		// 下にareaがある
		if(area[n].end.x   != W-1) dire[3] = 1;		// 右にareaがある

		// フラグの結果から
		if(dire[0] == 1){	// 上方向に通路をのばす
			door = rand()%sizeX(&room[n]) + room[n].start.x;

			aisle[aisle_num].start.y = room[n].start.y -1;
			aisle[aisle_num].start.x = door;
			aisle[aisle_num].end.y   = area[n].start.y;
			aisle[aisle_num].end.x   = door;

			aisle[aisle_num].dire = 0;	// 上
			aisle_num++;
		}
		if(dire[1] == 1){	// 左方向に通路をのばす
			door = rand()%sizeY(&room[n]) + room[n].start.y;

			aisle[aisle_num].start.y = door;
			aisle[aisle_num].start.x = room[n].start.x -1;
			aisle[aisle_num].end.y   = door;
			aisle[aisle_num].end.x   = area[n].start.x;

			aisle[aisle_num].dire = 9;	// 左
			aisle_num++;
		}
		if(dire[2] == 1){	// 下方向に通路をのばす
			door = rand()%sizeX(&room[n]) + room[n].start.x;

			aisle[aisle_num].start.y = room[n].end.y +1;
			aisle[aisle_num].start.x = door;
			aisle[aisle_num].end.y   = area[n].end.y;
			aisle[aisle_num].end.x   = door;

			aisle[aisle_num].dire = 6;	// 下
			aisle_num++;
		}
		if(dire[3] == 1){	// 右方向に通路をのばす
			door = rand()%sizeY(&room[n]) + room[n].start.y;

			aisle[aisle_num].start.y = door;
			aisle[aisle_num].start.x = room[n].end.x +1;
			aisle[aisle_num].end.y   = door;
			aisle[aisle_num].end.x   = area[n].end.x;

			aisle[aisle_num].dire = 3;	// 右
			aisle_num++;
		}

		// 方向フラグをリセット
		for(int i = 0; i < 4; i++){
			dire[i] = 0;
		}

	}

	return aisle_num;
}


/*
 自乗計算
 return : x^2を返す
*/
int square(int x){
	return x * x;
}

/*
 一番近い通路
 candidate : 繋げる候補の通路番号が入った配列
 asl_count : 通路候補の数
      dire : 通路の向き
     index : 繋げる元となる通路の番号
 	return : 一番近い通路の番号
*/
int nearest(Road *aisle, int candidate[], int asl_count, int dire, int index){

	int min_distance = square(H+W);		// とりあえず大きな数字
	int distance;

	for(int n = 0; n < asl_count; n++){

		if(dire == 0 || dire == 6){
			distance = square(aisle[n].end.x - aisle[index].end.x);
		}else if(dire == 3 || dire == 9){
			distance = square(aisle[n].end.y - aisle[index].end.y);
		}

		if(candidate[n] < min_distance){
			min_distance = candidate[n];
		}
	}

	return min_distance;
}


// 繋げる通路を探す
// asl_num : 通路数
//    dire : 探索する通路が向いている方向
//   coord : 探索する座標
//   index : 繋げる元となる通路の番号
//  return : 
int search_aisle(Road *aisle, int asl_num, int dire, int coord, int index, int conect_num){

	int candidate[AREA_MAX] = {};		// 繋げる通路の候補
	int asl_count = 0;					// 候補通路の数
	int min_distance;					// 最小距離の通路番号

//	printf("dire : %d\n", dire);
//	printf("asl_num : %d\n", asl_num);

	// 通路の候補を探す
	if(dire == 0 || dire == 6){
		for(int n = 0; n < asl_num; n++){
			if(aisle[n].dire == dire && aisle[n].end.y == coord){
				candidate[asl_count] = n;
				asl_count++;
			}
		}
	}else if(dire == 3 || dire == 9){
		for(int n = 0; n < asl_num; n++){
			if(aisle[n].dire == dire && aisle[n].end.x == coord){
				candidate[asl_count] = n;
				asl_count++;
			}
		}
	}
	
	// 候補から繋げる通路を探す
	min_distance = nearest(aisle, candidate, asl_count, dire, index);

	// 繋げた通路のフラグを立てる
	aisle[index].flag = 1;
	aisle[min_distance].flag = 1;

	if(dire == 0 || dire == 6){

		aisle[asl_num + conect_num].start.y = coord;
		aisle[asl_num + conect_num].start.x = aisle[min_distance].end.x;
		aisle[asl_num + conect_num].end.y   = coord;
		aisle[asl_num + conect_num].end.x   = aisle[index].end.x;
	
		aisle[asl_num + conect_num].dire = 1;
		aisle[asl_num + conect_num].flag = 1;

	}else if(dire == 3 || dire == 9){

		aisle[asl_num + conect_num].start.y = aisle[min_distance].end.y;
		aisle[asl_num + conect_num].start.x = coord;
		aisle[asl_num + conect_num].end.y   = aisle[index].end.y;
		aisle[asl_num + conect_num].end.x   = coord;
	
		aisle[asl_num + conect_num].dire = 1;
		aisle[asl_num + conect_num].flag = 1;

	}

	//　通路を繋げる
	conect_num ++;		// 通路数を1増やし, そこに繋げた通路を保存する

	return conect_num;
}


/*
 通路同士を繋げる
 asl_num : 通路を繋げる
  return : 総通路数
*/
int connect_aisle(Road *aisle, int asl_num){

	int conect_num = 0;


	for(int n = 0; n < asl_num; n++){

		// どの通路とも繋がっていなければ
		if(aisle[n].flag == 0){

			// 一番近い通路を探す
			if(aisle[n].dire == 0){
				conect_num = search_aisle(aisle, asl_num, 6, aisle[n].end.y-1, n, conect_num);
			}else if(aisle[n].dire == 3){
				conect_num = search_aisle(aisle, asl_num, 9, aisle[n].end.x+1, n, conect_num);
			}else if(aisle[n].dire == 6){
				conect_num = search_aisle(aisle, asl_num, 0, aisle[n].end.y+1, n, conect_num);
			}else if(aisle[n].dire == 9){
				conect_num = search_aisle(aisle, asl_num, 3, aisle[n].end.x-1, n, conect_num);
			}

		}


	}
	
	return asl_num + conect_num;
}


/*
 aとbを比べて大きい方を返す
 return : 大きい方の値 (等しければ aを返す)
*/
int greater(int a, int b){

	if(a > b )	return a;
	if(a < b )	return b;
	else		return a;
}


/*
 aとbを比べて小さい方を返す
 return : 小さい方の値 (等しければ aを返す)
*/
int less(int a, int b){

	if(a > b )	return b;
	if(a < b )	return a;
	else		return a;
}


/*
 通路をfieldに格納
 asl_num : 通路の数
*/
void store_aisle(int *field, Road *aisle, int aisle_num){

	for(int n = 0; n < aisle_num; n++){

		for(int i = less(aisle[n].start.y, aisle[n].end.y); i <= greater(aisle[n].start.y, aisle[n].end.y); i++){
			for(int j = less(aisle[n].start.x, aisle[n].end.x); j <= greater(aisle[n].start.x, aisle[n].end.x); j++){
				field[i * W + j] = 3;
			}
		}
	}

	return ;
}


/*
 ダンジョンの生成
*/
void dungeon(int *field){

	Mark area [AREA_MAX];		// エリア
	Mark room [AREA_MAX];		// ルーム
	Road aisle[AREA_MAX * 5];	// 通路

	int  line;					// 分割方向  0:縦, 1:横
	int  split_num;				// 分割回数
	int  aisle_num;				// 通路の数
	int  room_num;				// 部屋の数

	// 構造体の中身を全て0に設定
	memset(area , 0, sizeof(area) );
	memset(room , 0, sizeof(room) );
	memset(aisle, 0, sizeof(aisle));

	// 分割回数を確定
	split_num = AREA_MAX - rand()%3;

	// 初期分割方向を確定
	line = rand()%2;

	line = 0;

	// 分割の初期処理
	init_split(area);

	for(int n = 1; n < split_num; n++){

		// 最大面積のエリアを分割
		split_area(area, line, max_area(area, n-1), n);

		line = (line+1)%2;		// 分割方向を変える

	}

	room_num = split_num;		// 部屋の数 = 分割回数

	// 部屋を作成
	make_room(area, room, room_num);

	// 部屋をfieldに格納
	store_room((int *) field, room, room_num);

	// 部屋を枠で囲む
	frame((int *)field, room, room_num);

	// 通路を作成
	aisle_num = extend_aisle(area, aisle, room, split_num);

	// 通路を繋げる
	aisle_num = connect_aisle(aisle, aisle_num);

	// 通路を格納
	store_aisle((int *)field, aisle, aisle_num);

	return;
}


/*
 fieldをテキストファイルに格納 
 file_name : テキストファイルのパス
*/
void writing_field(char file_name[], int *field){

	FILE *fp;

	fp = fopen(file_name, "w");
	if(fp == NULL){
		perror("fopen failed (make_dungeon.c -> writing_field();)");
        exit(EXIT_FAILURE);
	}

	for(int i = 0; i < H; i++){
		for(int j = 0; j < W; j++){
			fprintf(fp, "%d", field[i * W + j]);	//　書き込み
		}
		fputc('\n', fp);
	}

	//後処理
	if(fp != NULL) fclose(fp);
	return;
}


/*
 ダンジョンの自動生成
*/
void make_dungeon(void){

	int  field[H][W] = {};	// ダンジョンの情報を格納

	// 乱数のシード地を設定
	srand((unsigned int) time(NULL));

	// ダンジョンの自動生成
	dungeon((int *) field);

	// ダンジョンの情報をテキストファイルに書き込む
	writing_field("../field/map.txt", (int *)field);

	return;
}