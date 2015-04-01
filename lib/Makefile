make: clean helpers bufio 
	gcc -shared -std=c99 -o libhelpers.so  helpers.o

clean:
	rm -f helpers.o
	rm -f libhelpers.so
	rm -f bufio.o
	rm -f libbufio.so
	rm -f test

helpers: helpers.o
	gcc -shared -std=c99 -o libhelpers.so  helpers.o	

helpers.o:
	gcc -c -fPIC -std=c99 -o $@ helpers.c

bufio: bufio.o
	gcc -shared -std=c99 -o libbufio.so  bufio.o	

bufio.o:
	gcc -c -fPIC -std=c99 -o $@ bufio.c
