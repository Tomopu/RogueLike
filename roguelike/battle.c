#include <ncurses.h>
#include <locale.h>		// setlocale()
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>		// usleep()

#include "header_file/rogue.h"

void player_attack(int *field, Chara *player, Chara *enemy, int dire){

	if(dire == 0){
		if(UP == 6){
			enemy->status.hp -= player->status.attack;
		}
	}else if(dire == 3){
		if(UP == 6){
			enemy->status.hp -= player->status.attack;
		}
	}else if(dire == 6){
		if(UP == 6){
			enemy->status.hp -= player->status.attack;
		}
	}else if(dire == 9){
		if(UP == 6){
			enemy->status.hp -= player->status.attack;
		}
	}else{
		return;
	}
	
	return;
}