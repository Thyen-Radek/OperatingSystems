CC = gcc
CFLAGS = -Wall
$(info ************ Wykonano zadanie 2 ************)

all: life
	
life: grid.c life.c
	$(CC) $(CFLAGS) grid.c life.c -o life -lncursesw -lpthread
	
clean:
	rm -f life
