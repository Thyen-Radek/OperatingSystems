CC = gcc
CFLAGS = -Wall -Wextra
$(info ************ Wykonano zadanie 1 ************)

static:
	$(CC) $(CFLAGS) main.c -o main
run: static
	./main 3 
run_raport: static
	./main 3 > raport.txt
clean:
	rm -rf main *.txt