
all: gw.o
	gcc main.c gw.o
gw.o: gw.c
	gcc -c gw.c
clean:
	rm gw.o a.out
