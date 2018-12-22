cc = clang++ 
CFLAGS = -O2 -std=c++14 -Wall
main: main.cpp bar.o foo.o
	$(cc) $(CFLAGS) main.cpp bar.o foo.o -o main
bar.o:
	$(cc) $(CFLAGS) -c bar.cpp
foo.o:
	$(cc) $(CFLAGS) -c foo.cpp
clean:
	rm *.o main
