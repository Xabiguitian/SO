all: historial.c historial.h p0.c
	gcc -Wall p0.c historial.c historial.h main.c
	./a.out

leaks:
	gcc -g -O0 -Wall p0.c historial.c historial.h
	valgrind --leak-check=full --show-reachable=yes ./a.out