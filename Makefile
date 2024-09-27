all: historial.c historial.h p0.c
	gcc -Wall p0.c historial.c historial.h
	./a.out

historic.o: historial.c historial.h
	gcc $(CFLAGS) -c historial.c


valgrind: all p0
	valgrind --leak-check=full --show-reachable=yes ./p0
