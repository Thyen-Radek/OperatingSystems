CC = gcc
CFLAGS = -Wall -Wextra

static:
	$(CC) $(CFLAGS) main.c -o main
run_example: static 
	./main files/in.txt files/out.txt
run_test: static
	./main files/in.txt tmp1;./main tmp1 tmp2;diff -s tmp2 files/in.txt
run_test_2: static
	# Don't know if works (i'm using OS'X, don't have tac command)
	./main files/in.txt files/out.txt;tac < files/out.txt | rev > tmp;diff -s tmp files/out.txt
clean:
	rm -rf main *.txt tmp tmp1 tmp2 
