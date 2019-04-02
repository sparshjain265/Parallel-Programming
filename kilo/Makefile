kilo: kilo.c
	$(CC) kilo.c -o kilo -Wall -Wextra -pedantic -std=c99

test: kilo
	cp kilo.c test
	./kilo test

clean:	
	rm -rf kilo