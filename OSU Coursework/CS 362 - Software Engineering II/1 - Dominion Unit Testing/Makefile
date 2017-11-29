CFLAGS = -Wall -fpic -coverage -lm -std=c99 -ftest-coverage -fprofile-arcs

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)

testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out


player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player testDrawCard testBuyCard badTestDrawCard

unittestresults.out:
	echo "Compiling and running all tests....." > unittestresults.out
	
	gcc -o unittest1 unittest1.c dominion.c rngs.c -g $(CFLAGS)
	echo "Running Unittest1...." >> unittestresults.out
	./unittest1 >> unittestresults.out
	gcov dominion.c >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out
	cat dominion.c.gcov >> unittest1.gcov
	
	echo "######################################" >> unittestresults.out
	echo "######################################" >> unittestresults.out
	
	gcc -o unittest2 unittest2.c dominion.c rngs.c -g $(CFLAGS)
	echo "Running Unittest2...." >> unittestresults.out
	./unittest2 >> unittestresults.out
	gcov dominion.c >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out
	cat dominion.c.gcov >> unittest2.gcov
	
	echo "######################################" >> unittestresults.out
	echo "######################################" >> unittestresults.out
	
	gcc -o unittest3 unittest3.c dominion.c rngs.c -g $(CFLAGS)
	echo "Running Unittest3...." >> unittestresults.out
	./unittest3 >> unittestresults.out
	gcov dominion.c >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out
	cat dominion.c.gcov >> unittest3.gcov
	
	echo "######################################" >> unittestresults.out
	echo "######################################" >> unittestresults.out
	
	gcc -o unittest4 unittest4.c dominion.c rngs.c -g $(CFLAGS)
	echo "Running Unittest4...." >> unittestresults.out
	./unittest4 >> unittestresults.out
	gcov dominion.c >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out
	cat dominion.c.gcov >> unittest4.gcov
	
	echo "######################################" >> unittestresults.out
	echo "######################################" >> unittestresults.out
	
	gcc -o cardtest1 cardtest1.c dominion.c rngs.c -g $(CFLAGS)
	echo "Running Cardtest1...." >> unittestresults.out
	./cardtest1 >> unittestresults.out
	gcov dominion.c >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out
	cat dominion.c.gcov >> cardtest1.gcov
	
	echo "######################################" >> unittestresults.out
	echo "######################################" >> unittestresults.out
	
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
	
	echo "######################################" >> unittestresults.out
	echo "######################################" >> unittestresults.out
	
	gcc -o cardtest4 cardtest4.c dominion.c rngs.c -g $(CFLAGS)
	echo "Running Cardtest4...." >> unittestresults.out
	./cardtest4 >> unittestresults.out
	gcov dominion.c >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out
	cat dominion.c.gcov >> cardtest4.gcov
	
randomtests:
	gcc -o randomtestadventurer randomtestadventurer.c dominion.c rngs.c -g $(CFLAGS)
	./randomtestadventurer 29378 > randomtestadventurer.out
	gcov dominion.c >> randomtestadventurer.out
	cat dominion.c.gcov >> randomtestadventurer.out
	
	gcc -o randomtestcard1 randomtestcard1.c dominion.c rngs.c -g $(CFLAGS)
	./randomtestcard1 29378 > randomtestcard1.out
	gcov dominion.c >> randomtestcard1.out
	cat dominion.c.gcov >> randomtestcard1.out
	
	gcc -o randomtestcard2 randomtestcard2.c dominion.c rngs.c -g $(CFLAGS)
	./randomtestcard2 29378 > randomtestcard2.out
	gcov dominion.c >> randomtestcard2.out
	cat dominion.c.gcov >> randomtestcard2.out
	
buggytests:
	gcc -o cardtest2 cardtest2.c dominion.c rngs.c -g $(CFLAGS)
	
clean:
	rm -f *.o playdom.exe playdom test.exe test player player.exe testInit testInit.exe *.gcov *.gcda *.gcno *.so *.out
	rm -f randomtestadventurer randomtestcard1 randomtestcard2
