#include <iostream>
#include <vector>
#include <map>
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

    std::map<unsigned long, unsigned long> Set::getIndex() {
        return index;
    }
}