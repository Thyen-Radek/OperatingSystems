CC = gcc
CFLAGS = -Wall 
$(info ************ Wykonano zadanie 3 ************)

static:
	$(CC) $(CFLAGS) main.c -o main
	$(CC) $(CFLAGS) solve.c -o solve
run_example: static
	./main 0.1 10
clean:
	rm -rf main