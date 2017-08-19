all: gtest

easy: modTest boss.o parse.o
	gcc $^ -g -Wall -Werror -o gtest

modTest: test.c game.h
	gcc test.c -c -g -DEASY -Wall -Werror -o $@

gtest: test.o boss.o parse.o
	gcc $^ -g -Wall -Werror -o $@

%.o: %.c game.h
	gcc $< -c -g -Wall -Werror -o $@

clean:
	rm *.o gtest
