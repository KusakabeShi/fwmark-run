all:
	gcc -Wall -Wextra -Wwrite-strings -fPIC -c -o ld_fwmark.o fwmark_ld.c
	gcc -Wall -Wextra -Wwrite-strings -shared -o ld_fwmark.so ld_fwmark.o -ldl

clean:
	rm -f *.so *.o
