CC = gcc
CFLAGS = -Wall
$(info ************ Wykonano zadanie 2 ************)

static:
	$(CC) $(CFLAGS) main.c -o main
raport: static
	./main > raport.txt
clean:
	rm -rf main raport.txt