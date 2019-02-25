all			:	overheads

overheads	:	overheads.o
				gcc overheads.c -o overheads.out -lpthread

testOverhead:	overheads
				for i in `seq 1 100`; do ./overheads.out; done
				python plot_overhead.py

histogram	:	histogram.o
				gcc histogram.c -o histogram.out -lpthread

histogram2	:	histogram2.o
				gcc histogram2.c -o histogram2.out -lpthread

histogram3	:	histogram3.o
				gcc histogram3.c -o histogram3.out -lpthread

testHist1	:	histogram
				./histogram.out 1000 1
				./histogram.out 1000 4
				./histogram.out 1000 8

testHist2	:	histogram
				./histogram.out 1000000 1
				./histogram.out 1000000 4
				./histogram.out 100000000 1
				./histogram.out 100000000 4

testHist3	:	histogram2
				./histogram2.out 1000000 8
				./histogram2.out 100000000 8

testHist4	:	histogram3
				./histogram3.out 1000000000 1
				./histogram3.out 1000000000 2
				./histogram3.out 1000000000 4
				./histogram3.out 1000000000 8
				./histogram3.out 10000000000 1
				./histogram3.out 10000000000 2
				./histogram3.out 10000000000 4
				./histogram3.out 10000000000 8

clean		:		
				rm -rf *.o *.out *.txt