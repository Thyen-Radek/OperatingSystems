CC = gcc
CFLAGS = -Wall 
$(info ************ Wykonano zadanie 2 ************)

static:
	$(CC) $(CFLAGS) simulation.c -o simulation -lrt -lpthread
	$(CC) $(CFLAGS) client.c -o client -lrt -lpthread
	$(CC) $(CFLAGS) hairdresser.c -o hairdresser -lrt -lpthread
clean:
	rm -rf client simulation hairdresser raport.txt