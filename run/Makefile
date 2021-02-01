CFLAGS = -Wall

all:	rogue

rogue:	make_dungeon.o enemy.o windows.o battle.o log.o inventory.o main.o
	cc $^ -lncurses -o $@

make_dungeon.o: ../roguelike/make_dungeon.c \
  ../roguelike/header_file/dungeon.h ../roguelike/header_file/basic.h
	cc -c ../roguelike/make_dungeon.c $(CFLAGS)

enemy.o: ../roguelike/enemy.c ../roguelike/header_file/rogue.h \
  ../roguelike/header_file/basic.h ../roguelike/header_file/dungeon.h \
  ../roguelike/header_file/enemy.h ../roguelike/header_file/inventory.h
	cc -c ../roguelike/enemy.c $(CFLAGS)

windows.o: ../roguelike/windows.c ../roguelike/header_file/rogue.h \
  ../roguelike/header_file/basic.h ../roguelike/header_file/dungeon.h \
  ../roguelike/header_file/enemy.h ../roguelike/header_file/inventory.h
	cc -c ../roguelike/windows.c $(CFLAGS)

battle.o: ../roguelike/battle.c ../roguelike/header_file/rogue.h \
  ../roguelike/header_file/basic.h ../roguelike/header_file/dungeon.h \
  ../roguelike/header_file/enemy.h ../roguelike/header_file/inventory.h
	cc -c ../roguelike/battle.c $(CFLAGS)

log.o: ../roguelike/log.c ../roguelike/header_file/rogue.h \
  ../roguelike/header_file/basic.h ../roguelike/header_file/dungeon.h \
  ../roguelike/header_file/enemy.h ../roguelike/header_file/inventory.h
	cc -c ../roguelike/log.c $(CFLAGS)

inventory.o: ../roguelike/inventory.c ../roguelike/header_file/rogue.h \
  ../roguelike/header_file/basic.h ../roguelike/header_file/dungeon.h \
  ../roguelike/header_file/enemy.h ../roguelike/header_file/inventory.h
	cc -c ../roguelike/inventory.c $(CFLAGS)

main.o: ../roguelike/main.c ../roguelike/header_file/rogue.h \
  ../roguelike/header_file/basic.h ../roguelike/header_file/dungeon.h \
  ../roguelike/header_file/enemy.h ../roguelike/header_file/inventory.h
	cc -c ../roguelike/main.c $(CFLAGS)

clean:
	-rm rogue *.o

.PHONY:	all clean



