#include <stdio.h>
#include <stdlib.h>

#include "header_file/enemy.h"

void export(char file_name[], int *field){

	FILE *fp;
	int x;				// ファイルからの入力
	int index = 0;		// fieldのインデックス

	fp = fopen(file_name, "r");
	if(fp == NULL){
		perror("fopen failed export();");
        exit(EXIT_FAILURE);
	}

	while (1){

		x = fgetc(fp);				// ファイルから1文字取得 	

    	// 改行文字飛ばし
		if(x == 10) continue;		// ASCII 10: LF(改行)

		// 終了条件
    	if(x == EOF){				// 読み尽くした場合
			if(SEARCH_RANGE*SEARCH_RANGE == index)  break; 
			else exit(EXIT_FAILURE);
		}

		field[index] = x - 48;		// ファイルからfieldに格納
		index++;					// index をずらす

	}

	//後処理
	if(fp != NULL) fclose(fp);
	return;
}

void to_goal(int)

void print_around(int *around){

	for(int i = 0; i < SEARCH_RANGE; i++){
		for(int j = 0; j < SEARCH_RANGE; j++){
			printf("%d", around[i * SEARCH_RANGE + j]);
		}
		puts("");
	}

}

int main(void){

	int around[SEARCH_RANGE][SEARCH_RANGE] = {};

	export("../field/around_value.txt", (int *)around);

	print_around((int *)around);


	return 0;
}