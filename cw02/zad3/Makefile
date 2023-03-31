CC = gcc
CFLAGS = -Wall -Wextra

static:
	$(CC) $(CFLAGS) main.c -o main
run: static
	./main
run_comparison: static
	./main ; wc -c * 
clean:
	rm -rf main *.txt 