kilo: kilo.c
	$(CC) kilo.c -o kilo -Wall -Wextra -pedantic -std=c99 -lpthread

test: kilo
	./kilo test.c

clean:	
	rm -rf kilo