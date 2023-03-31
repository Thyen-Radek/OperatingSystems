CC = gcc
CFLAGS = -Wall -Wextra

static:
	$(CC) $(CFLAGS) main.c -o main
run_example: static
	./main .
run_comparison: static
	./main . ; find ../zad4 | xargs wc -c
clean:
	rm -rf main *.txt 