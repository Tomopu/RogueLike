DIR = ../roguelike/
CFLAGS = -Wall

all:	rogue

rogue:	make_dungeon.o enemy.o windows.o battle.o main.o
	cc $^ -lncurses -o $@

make_dungeon.o: ../roguelike/make_dungeon.c ../roguelike/header_file/dungeon.h ../roguelike/header_file/basic.h
	cc -c ../roguelike/make_dungeon.c -Wall

enemy.o: ../roguelike/enemy.c ../roguelike/header_file/enemy.h ../roguelike/header_file/basic.h
	cc -c ../roguelike/enemy.c -Wall

windows.o: ../roguelike/windows.c ../roguelike/header_file/rogue.h ../roguelike/header_file/dungeon.h \
  ../roguelike/header_file/basic.h ../roguelike/header_file/enemy.h
	cc -c ../roguelike/windows.c -Wall

battle.o: ../roguelike/battle.c ../roguelike/header_file/rogue.h \
  ../roguelike/header_file/dungeon.h ../roguelike/header_file/basic.h \
  ../roguelike/header_file/enemy.h
	cc -c ../roguelike/battle.c -Wall

main.o: ../roguelike/main.c ../roguelike/header_file/rogue.h ../roguelike/header_file/dungeon.h \
  ../roguelike/header_file/basic.h ../roguelike/header_file/enemy.h
	cc -c ../roguelike/main.c -Wall

clean:
		-rm rogue *.o

.PHONY:	all clean




