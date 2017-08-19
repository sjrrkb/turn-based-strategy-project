all: Gauntlet

Gauntlet: game.o shop.o heroes.o io.o boss.o parse.o fight.o
	gcc -g -Wall -Werror game.o shop.o io.o boss.o parse.o fight.o heroes.o -o Gauntlet

game.o: game.h game.c
	gcc -g -Wall -Werror -c game.c

shop.o: game.h shop.c
	gcc -g -Wall -Werror -c shop.c
	
heroes.o: game.h heroes.c
	gcc -g -Wall -Werror -c heroes.c

io.o: game.h io.c
	gcc -g -Wall -Werror -c io.c

boss.o: game.h boss.c
	gcc -g -Wall -Werror -c boss.c
	
parse.o: game.h parse.c
	gcc -g -Wall -Werror -c parse.c
	
fight.o: game.h fight.c
	gcc -g -Wall -Werror -c fight.c
	
clean:
	rm -f game.o shop.o heroes.o io.o boss.o parse.o fight.o Gauntlet
