CC = gcc
CFLAGS = -Wall -Wextra

F_STATIC = "../zad2/static"
F_SHARED = "../zad2/shared"
F_DYNAMIC = "../zad2/dynamic"

STATIC = results_static_
SHARED = results_shared_
DYNAMIC = results_dynamic_

REPORT = report.txt

COM = commands.txt
all: static shared dynamic
static:
	make -C ../zad2 static
	echo "static test with -O:" > $(STATIC)O.txt
	cat $(COM) | $(F_STATIC)O.o >> $(STATIC)O.txt

	echo "static test with -O2:" > $(STATIC)O2.txt
	cat $(COM) | $(F_STATIC)O2.o >> $(STATIC)O2.txt

	echo "static test with -Os:" > $(STATIC)Os.txt
	cat $(COM) | $(F_STATIC)Os.o >> $(STATIC)Os.txt
shared:
	make -C ../zad2 shared
	echo "shared test with -O:" > $(SHARED)O.txt
	cat $(COM) | $(F_SHARED)O.o >> $(SHARED)O.txt

	echo "shared test with -O2:" > $(SHARED)O2.txt
	cat $(COM) | $(F_SHARED)O2.o >> $(SHARED)O2.txt

	echo "shared test with -Os:" > $(SHARED)Os.txt
	cat $(COM) | $(F_SHARED)Os.o >> $(SHARED)Os.txt
dynamic:
	make -C ../zad2 dynamic
	echo "dynamic test with -O:" > $(DYNAMIC)O.txt
	cat $(COM) | $(F_DYNAMIC)O.o >> $(DYNAMIC)O.txt

	echo "dynamic test with -O2:" > $(DYNAMIC)O2.txt
	cat $(COM) | $(F_DYNAMIC)O2.o >> $(DYNAMIC)O2.txt

	echo "dynamic test with -Os:" > $(DYNAMIC)Os.txt
	cat $(COM) | $(F_DYNAMIC)Os.o >> $(DYNAMIC)Os.txt
report:
	echo "All tests:" > $(REPORT)
	echo "" >> $(REPORT)
	cat $(STATIC)O.txt >> $(REPORT)
	echo "" >> $(REPORT)
	cat $(STATIC)O2.txt >> $(REPORT)
	echo "" >> $(REPORT)
	cat $(STATIC)Os.txt >> $(REPORT)
	echo "" >> $(REPORT)
	cat $(SHARED)O.txt >> $(REPORT)
	echo "" >> $(REPORT)
	cat $(SHARED)O2.txt >> $(REPORT)
	echo "" >> $(REPORT)
	cat $(SHARED)Os.txt >> $(REPORT)
	echo "" >> $(REPORT)
	cat $(DYNAMIC)O.txt >> $(REPORT)
	echo "" >> $(REPORT)
	cat $(DYNAMIC)O2.txt >> $(REPORT)
	echo "" >> $(REPORT)
	cat $(DYNAMIC)Os.txt >> $(REPORT)
clean:
	make -C ../zad3 clean
	rm -rf *.so res*.txt