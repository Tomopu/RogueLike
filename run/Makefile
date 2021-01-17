DIR = ../roguelike/
CFLAGS = -Wall

all:	rogue

rogue:	make_dungeon.o main.o enemy.o
	cc $^ -lncurses -o $@

make_dungeon.o:	 ../roguelike/make_dungeon.c ../roguelike/dungeon.h ../roguelike/basic.h
	cc -c ../roguelike/make_dungeon.c -Wall

main.o:	../roguelike/main.c ../roguelike/dungeon.h ../roguelike/basic.h
	cc -c ../roguelike/main.c -Wall

enemy.o: ../roguelike/enemy.c ../roguelike/enemy.h ../roguelike/basic.h
	cc -c ../roguelike/enemy.c -Wall

clean:
		-rm rogue *.o

.PHONY:	all clean

