#ifndef SET_HPP
#define SET_HPP

#include <iostream>
#include <vector>
#include <map>
#include "Slot.hpp"

namespace Csim 
{
    class Set {

    public:
        // This default constructor initializes a board with the standard
        // piece positions, and sets the state to white's turn
        Set();
    
        Set(std::vector<Slot> s);
    
        // Returns true if the designated player is in mate
        std::vector<Slot> getSlots();

        std::map<unsigned long, unsigned long> getIndex();
    
    private:
        std::vector<Slot> slots;
        std::map<unsigned long, unsigned long> index; //map of tag to index of slot
    };
}

#endif