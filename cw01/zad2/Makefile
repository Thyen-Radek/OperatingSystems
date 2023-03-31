CC = gcc
CFLAGS = -Wall -Wextra
LIB_DIR = ../zad1

# PATH_L = /Users/radek.kawa/CLionProjects/Systemy/zad1
# export DYLD_LIBRARY_PATH=$(PATH_L)

all: static shared dynamic

static:
	make -C ../zad1 static
	$(CC) $(CFLAGS) main.c $(LIB_DIR)/library.a -O0 -o static.o
	$(CC) $(CFLAGS) main.c $(LIB_DIR)/library.a -O -o staticO.o
	$(CC) $(CFLAGS) main.c $(LIB_DIR)/library.a -O2 -o staticO2.o
	$(CC) $(CFLAGS) main.c $(LIB_DIR)/library.a -Os -o staticOs.o

shared: 
	make -C ../zad1 shared
	$(CC) $(CFLAGS) main.c -L ../zad1 -llibrary.o -Wl,-rpath,../zad1 -O0 -o shared.o
	$(CC) $(CFLAGS) main.c -L ../zad1 -llibrary.o -Wl,-rpath,../zad1 -O -o sharedO.o
	$(CC) $(CFLAGS) main.c -L ../zad1 -llibrary.o -Wl,-rpath,../zad1 -O2 -o sharedO2.o
	$(CC) $(CFLAGS) main.c -L ../zad1 -llibrary.o -Wl,-rpath,../zad1 -Os -o sharedOs.o
	# $(CC) $(CFLAGS) main.c $(LIB_DIR)/library.so -O0 -o shared.o
	# $(CC) $(CFLAGS) main.c $(LIB_DIR)/library.so -O -o sharedO.o
	# $(CC) $(CFLAGS) main.c $(LIB_DIR)/library.so -O2 -o sharedO2.o
	# $(CC) $(CFLAGS) main.c $(LIB_DIR)/library.so -Os -o sharedOs.o
	
dynamic:
	make -C ../zad1 shared
	$(CC) $(CFLAGS) main.c -ldl -D DYNAMIC -O0 -o dynamic.o
	$(CC) $(CFLAGS) main.c -ldl -D DYNAMIC -O -o dynamicO.o
	$(CC) $(CFLAGS) main.c -ldl -D DYNAMIC -O2 -o dynamicO2.o
	$(CC) $(CFLAGS) main.c -ldl -D DYNAMIC -Os -o dynamicOs.o

clean:
	make -C ../zad1 clean
	rm -rf *.o *.a *.so *.gch