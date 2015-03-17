make: clean helpers.o
	gcc -shared -std=c99 -o libhelpers.so  helpers.o

clean:
	rm -f helpers.o
	rm -f libhelpers.so
	rm -f test

helpers.o:
	gcc -c -fPIC -std=c99 -o $@ helpers.c