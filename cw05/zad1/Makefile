CC = gcc
CFLAGS = -Wall 
$(info ************ Wykonano zadanie 1 ************)

static:
	$(CC) $(CFLAGS) main.c -o main
run_example: static
	./main root example_title content
	./main sender
clean:
	rm -rf main *.txt