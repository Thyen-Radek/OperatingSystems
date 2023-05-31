CC=gcc
CFLAGS=-Wall
$(info ************ Wykonano zadanie 1 ************)

all:
	$(CC) $(CFLAGS) client.c -o client -pthread
	$(CC) $(CFLAGS) server.c -o server -pthread

clean:
	rm -f client server