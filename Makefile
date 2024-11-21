all: historial.c historial.h p0.c
	gcc -Wall p0.c historial.c main.c p1.c file.c p2.c memlist.c
	./a.out

leaks:
	gcc -g -O0 -Wall p0.c historial.c p1.c main.c file.c p2.c memlist.c
	valgrind --leak-check=full --show-reachable=yes ./a.out

