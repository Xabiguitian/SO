all: historial.c historial.h p0.c
	gcc -Wall -lm p0.c historial.c main.c p1.c file.c memlist.c p2.c p3.c proclist.c searchdirlist.c
	./a.out

leaks:
	gcc -g -O0 -Wall -lm p0.c historial.c p1.c main.c file.c memlist.c p2.c p3.c
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./a.out