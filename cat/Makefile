cat:
	gcc -Wall -I../lib -L../lib -o cat cat.c -lhelpers
	LD_LIBRARY_PATH=../lib:$LD_LIBRARY_PATH
	export LD_LIBRARY_PATH

clean: 
	rm -f cat