LEX    = flex
LFLAGS = 
CC     = gcc
CFLAGS = -std=c99 -Wall -pedantic

testurlscanner: urlscanner.o testurlscanner.o
	$(CC) $(CFLAGS) $^ -o $@

testurlscanner.o: urlscanner.h
urlscanner.o: urlscanner.h

clean:
	rm -f *.o testurlscanner *.exe
