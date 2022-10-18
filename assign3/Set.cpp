#include <iostream>
#include <vector>
#include "Set.hpp"
#include "Slot.hpp"

namespace Csim 
{
    Set::Set(){}

    Set::Set(std::vector<Slot> s) {
        slots = s;
    }
    
    std::vector<Slot> Set::getSlots() {
        return slots;
    }
}