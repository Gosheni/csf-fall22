CC = g++
CONSERVATIVE_FLAGS = -std=c++11 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g -O0
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)

csim: csim.o 
	$(CC) -o csim csim.o 

csim.o: csim.cpp  
	$(CC) -c csim.cpp $(CFLAGS)

.PHONY: clean all
clean:
	rm -f *.o csim