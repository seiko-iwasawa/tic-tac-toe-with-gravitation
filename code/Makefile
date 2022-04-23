CC=g++
CFLAGS=-g -I include/ -c -Wall -Wextra -Werror

all: tic-tac-toe

obj/:
	mkdir obj/

obj/ai.o: include/ai.h src/ai.cpp | obj/
	$(CC) $(CFLAGS) src/ai.cpp -o $@

obj/data-base.o: include/data-base.h src/data-base.cpp | obj/
	$(CC) $(CFLAGS) src/data-base.cpp -o $@

obj/field.o: include/field.h src/field.cpp | obj/
	$(CC) $(CFLAGS) src/field.cpp -o $@

obj/game.o: include/game.h src/game.cpp | obj/
	$(CC) $(CFLAGS) src/game.cpp -o $@

obj/prepare.o: include/prepare.h src/prepare.cpp | obj/
	$(CC) $(CFLAGS) src/prepare.cpp -o $@

obj/main.o: src/main.cpp | obj/
	$(CC) $(CFLAGS) src/main.cpp -o $@

tic-tac-toe: obj/ai.o obj/data-base.o obj/field.o obj/game.o obj/prepare.o obj/main.o
	$(CC) $^ -o $@

clean:
	rm -rf obj
	rm -f tic-tac-toe

.PHONY: all clean
