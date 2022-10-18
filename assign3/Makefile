CC = g++
CONSERVATIVE_FLAGS = -std=c++11 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g -O0
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)

csim: csim.o Cache.o Set.o Slot.o
	$(CC) -o csim csim.o Cache.o Set.o Slot.o 

Cache.o: Cache.cpp Set.hpp Slot.hpp
	$(CC) -c Cache.cpp $(CFLAGS)

Set.o: Set.hpp Slot.hpp
	$(CC) -c Set.cpp $(CFLAGS)

Slot.o: Slot.hpp
	$(CC) -c Slot.cpp $(CFLAGS)

csim.o: csim.cpp Cache.hpp Set.hpp Slot.hpp
	$(CC) -c csim.cpp $(CFLAGS)

.PHONY: clean all
clean:
	rm -f *.o csim