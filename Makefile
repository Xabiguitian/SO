all: historial.c historial.h p0.c
	gcc -Wall p0.c historial.c historial.h
	./a.out