#test: make
#	gcc -std=c99 -L$(PWD) -Wall test.c -o test -lhelpers
#	LD_LIBRARY_PATH=$PWD:$(LD_LIBRARY_PATH) ./test

make: clean helpers.o
	gcc -shared -o libhelpers.so  helpers.o

clean:
	rm -f helpers.o
	rm -f libhelpers.so
	rm -f test

helpers.o:
	gcc -c -fPIC -std=c99 -o $@ helpers.c