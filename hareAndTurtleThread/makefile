%.o:	%.cpp
		g++ $< -o $@ -lpthread -std=c++17

all:	main.o

clean:	
		rm *.o

test:	all test1 test2 test3

test1:	all
		bash sim1.sh | ./main.o

test2:	all
		bash sim2.sh | ./main.o

test3:	all
		bash sim3.sh | ./main.o
		
