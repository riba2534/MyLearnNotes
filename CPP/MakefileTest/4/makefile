cc = g++ 
CFLAGS = -O2 -std=c++14 -Wall

all: max_main min_main

max_main: max_main.cpp bar.o foo.o
	$(cc) $(CFLAGS) max_main.cpp bar.o foo.o -o max_main

min_main: min_main.cpp bar.o foo.o
	$(cc) $(CFLAGS) min_main.cpp bar.o foo.o -o min_main

bar.o:
	$(cc) $(CFLAGS) -c bar.cpp
foo.o:
	$(cc) $(CFLAGS) -c foo.cpp
clean:
	rm *.o max_main min_main
