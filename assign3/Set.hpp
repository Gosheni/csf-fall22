#ifndef SET_HPP
#define SET_HPP

#include <iostream>
#include <vector>
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
    
    private:
        std::vector<Slot> slots;
    };
}

#endif