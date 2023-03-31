CC = gcc
CFLAGS = -Wall -Wextra

static:
	$(CC) $(CFLAGS) main.c -o main
run_example: static 
	./main l j files/in.txt files/out.txt
check_indenticity: static 
	./main l j files/in.txt files/out.txt;tr l j < files/in.txt > tmp;diff -s tmp files/out.txt > conclusions.txt
clean:
	rm -rf main *.txt tmp
