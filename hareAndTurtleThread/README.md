# Hare and Turtle

The program simulates the famous race between hare and turtle by spawning 3 threads to simulate hare, turtle and reporter each. The main thread simulates the God.

Both hare and turtle take steps every 50 milliseconds

Step size for turtle is 1, while for hare it is randomly chosen everytime between 2 and 6 (both inclusive)

If hare is well ahead of turtle (defined as 50 steps ahead) it sleeps for a random duration {50, 60, .. , 140}


The race starts at position 0 and ends at position 500, the first to cross 500 wins

The reporter displays the position of hare and turtle every 250 milliseconds

The God reads the terminal constantly, He ends the race (and the program) if it reads 'q'

If 'p' is read, next 2 inputs are interpreted as absolute position for hare and turtle (in that order) 

In that case, He replaces hare and turtle to the corresponding position

## Prerequisites
* Requires support for thread library (compile using -lpthread option)
* Requires support for C++17 Standard Library (compile using -std=c++17 option : available from g++ 8.0)

## Test Cases
There are 3 test cases:

* test1 : let the race go by itself
* test2 : after 3 seconds, replace 10 10, after 10 seconds, replace 10 100
* test3 : replace 5 5, 5 10, 5 20, 5 40, 100 200 ever 2 seconds, replace 200 400 after 5 seconds

To simulate all 3 test cases
```
make test
``` 
To simulate xth test case
```
make testx
```