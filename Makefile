all: historial.c historial.h p0.c
	gcc -Wall p0.c historial.c main.c p1.c file.c
	./a.out

leaks:
	gcc -g -O0 -Wall p0.c historial.c p1.c main.c file.c
	valgrind --leak-check=full --show-reachable=yes ./a.out