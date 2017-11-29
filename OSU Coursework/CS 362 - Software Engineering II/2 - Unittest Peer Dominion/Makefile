CFLAGS = -Wall -fpic -coverage -lm -std=c99 -ftest-coverage -fprofile-arcs

all: assignment5

assignment5:
	echo "Compiling and running all tests....." > unittestresults.out
	
	gcc -o cardtest2 cardtest2.c dominion.c rngs.c -g $(CFLAGS)
	echo "Running Cardtest2...." >> unittestresults.out
	./cardtest2 >> unittestresults.out
	gcov dominion.c >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out
	cat dominion.c.gcov >> cardtest2.gcov
	
	echo "######################################" >> unittestresults.out
	echo "######################################" >> unittestresults.out
	
	gcc -o cardtest3 cardtest3.c dominion.c rngs.c -g $(CFLAGS)
	echo "Running Cardtest3...." >> unittestresults.out
	./cardtest3 >> unittestresults.out
	gcov dominion.c >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out
	cat dominion.c.gcov >> cardtest3.gcov
	
clean:
	rm -f *.o playdom.exe playdom test.exe test player player.exe testInit testInit.exe *.gcov *.gcda *.gcno *.so *.out
	rm -f cardtest2 cardtest3
