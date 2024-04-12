all: my_program

my_program: main.o function.o
	gcc -o my_program main.o function.o

main.o: main.c
	gcc -c main.c

function.o: function.c
	gcc -c function.c

clean:
	rm -f my_program main.o function.o
