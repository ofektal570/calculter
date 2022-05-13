CC = gcc
CFLAGS = -ansi -pedantic-errors -Wall -Wextra -g

a.out: calc.o c_stack.o d_stack.o calc_main.o
	$(CC) $(CFLAGS) $^ -lm

calc.o: calc.c calc.h c_stack.h d_stack.h 
	$(CC) $(CFLAGS) -c calc.c

c_stack.o: c_stack.c c_stack.h
	$(CC) $(CFLAGS) -c c_stack.c

d_stack.o: d_stack.c d_stack.h
	$(CC) $(CFLAGS) -c d_stack.c

clean:
	rm *.o