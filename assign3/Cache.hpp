#ifndef CACHE_HPP
#define CACHE_HPP

#include <iostream>
#include <vector>
#include "Set.hpp"
#include "Slot.hpp"

namespace Csim 
{
  class Cache {

  public:
    // This default constructor initializes a board with the standard
    // piece positions, and sets the state to white's turn
    Cache();

    Cache(std::vector<Set> s);

    Cache(std::vector<Set> s, bool allocate, bool type);
    
    // Copy constructor
    Cache(const Cache& cache);

    // Deconstructor helper function
    void delete_cache();

    // Checks if given move is allowed
    bool getAllocate();

    bool getType();

    std::vector<Set> getSet();
    
  private:
    std::vector<Set> sets;
    bool allocation;
    bool type;
  };
}

#endif