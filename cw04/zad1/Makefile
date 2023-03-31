CC = gcc
CFLAGS = -Wall -Wextra
$(info ************ Wykonano zadanie 1 ************)

static:
	$(CC) $(CFLAGS) main_fork.c -o main_fork
	$(CC) $(CFLAGS) main_exec.c -o main_exec
	$(CC) $(CFLAGS) child.c -o child
raport: static
	echo "**** Method Fork ****" > raport.txt
	./main_fork ignore >> raport.txt
	./main_fork handler >> raport.txt
	./main_fork mask >> raport.txt
	./main_fork pending >> raport.txt
	echo "**** Method Exec ****" >> raport.txt
	./main_exec ignore >> raport.txt
	./main_exec mask >> raport.txt
	./main_exec pending >> raport.txt
clean:
	rm -rf main *.txt main_fork main_exec child