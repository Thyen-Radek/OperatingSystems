CC = gcc
CFLAGS = -Wall -Wextra

F_STATIC = "../zad2/static.o"
F_SHARED = "../zad2/shared.o"
F_DYNAMIC = "../zad2/dynamic.o"

STATIC = results_static.txt
SHARED = results_shared.txt
DYNAMIC = results_dynamic.txt

REPORT = report.txt

# PATH_L = /Users/radek.kawa/CLionProjects/Systemy/zad1
# export DYLD_LIBRARY_PATH=$(PATH_L)

COM = commands.txt

all: static shared dynamic

static:
	make -C ../zad2 static
	echo "static test:" > $(STATIC)
	cat $(COM) | $(F_STATIC) >> $(STATIC)

shared:
	make -C ../zad2 shared
	echo "shared test:" > $(SHARED)
	cat $(COM) | $(F_SHARED) >> $(SHARED)
dynamic:
	make -C ../zad2 dynamic
	echo "dynamic test:" > $(DYNAMIC)
	cat $(COM) | $(F_DYNAMIC) >> $(DYNAMIC)
report:
	echo "All tests:" > $(REPORT)
	echo "" >> $(REPORT)
	cat $(STATIC) >> $(REPORT)
	echo "" >> $(REPORT)
	cat $(SHARED) >> $(REPORT)
	echo "" >> $(REPORT)
	cat $(DYNAMIC) >> $(REPORT)
clean:
	make -C ../zad2 clean
	rm -rf *.so res*.txt