CC = gcc
CFLAGS = -Wall -Wextra
$(info ************ Wykonano zadanie 3 ************)

static:
	$(CC) $(CFLAGS) main.c -o main
run: static
	./main ../.. \#include
run_raport: static
	./main ../.. \#include > raport.txt 
clean:
	rm -rf main *.txt

