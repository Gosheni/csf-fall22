#ifndef CSIM_HPP
#define CSIM_HPP

#include <iostream>
#include <vector>
#include <map>

struct Slot {
    uint32_t tag;
    bool valid;
    unsigned long timestamp;
};

struct Set {
    std::vector<Slot> slots;
    std::map<uint32_t, int> index; //map of tag to index of slot
    int size; //Size of the set
};

struct Cache {
    std::vector<Set> sets;
    uint32_t byte; 
    bool allocate; // true -> write-allocate, false -> no-write-allocate
    bool write; // true -> write-through, false -> write-back
    bool lru; // true -> LRU, false -> FIFO
    int type; // 1 -> Direct, 2 -> Set, 3 -> Full 
    unsigned long cycles;
};

/*
Takes in the address and index and processes the store or the load 
based on the configurations of the cache and edits the cache

Parameters: 
    c - memory address of cache
    ad - uint32_t value of tag
    ind - uint32_t value of index
    n - size_t value that represents max size of each set
    dirty - bool value that decides if slot should be marked dirty
    store - bool value that determines store or load

Returns:
    A bool value that determines if it's a hit or a miss
*/
bool loadStore(Cache &c, uint32_t ad, uint32_t ind, size_t n, bool dirty, bool store);

/*
Takes in an integer paramter and returns log two value of it. Returns -1 if not possible.

Parameters: 
    n - int value to take log two of

Returns:
    An int log two value of n, or -1 if n is not power of 2 or less than or equal to 0
*/
int logTwo(int n);

/*
Checks if input is valid, as well as convert it to int value

Parameters: 
    argv - pointer to command line input

Returns:
    An integer value of the first input if it's valid, a -1 if it's an error
*/
int checkInput1(char** argv);


/*
Checks if input is valid, as well as convert it to int value

Parameters: 
    argv - pointer to command line input

Returns:
    An integer value of the second input if it's valid, a -1 if it's an error
*/
int checkInput2(char** argv);


/*
Checks if input is valid, as well as convert it to int value

Parameters: 
    argv - pointer to command line input

Returns:
    An integer value of the third input if it's valid, a -1 if it's an error
*/
int checkInput3(char** argv);

/*
Checks if the fourth input is the right argument for cache simulation

Parameters: 
    argv - pointer to command line input

Returns:
    A 1 or 2 if the command line input is valid, a -1 if it's an error
*/
int checkInput4(char** argv);

/*
Checks if the fifth input is the right argument for cache simulation

Parameters: 
    argv - pointer to command line input

Returns:
    A 1 or 2 if the command line input is valid, a -1 if it's an error
*/
int checkInput5(char** argv);

/*
Checks if the sixth input is the right argument for cache simulation

Parameters: 
    argv - pointer to command line input

Returns:
    A 1 or 2 if the command line input is valid, a -1 if it's an error
*/
int checkInput6(char** argv);

#endif