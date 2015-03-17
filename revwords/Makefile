revwords: clean
	gcc -Wall -I../lib -L../lib -std=c99 -o revwords revwords.c -lhelpers
	LD_LIBRARY_PATH=../lib:$LD_LIBRARY_PATH
	export LD_LIBRARY_PATH

clean: 
	rm -f revwords