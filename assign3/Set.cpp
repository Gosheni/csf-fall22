#include <iostream>
#include <vector>
#include <map>
#include "Set.hpp"
#include "Slot.hpp"

namespace Csim 
{
    Set::Set() {
        std::vector<Slot> s;
        std::map<unsigned long, unsigned long> map;
        slots = s;
        index = map;
    }

    Set::Set(std::vector<Slot> s) {
        slots = s;
        std::map<unsigned long, unsigned long> map;
        index = map;
    }

    std::vector<Slot> Set::getSlots() {
        return slots;
    }

    std::map<unsigned long, unsigned long> Set::getIndex() {
        return index;
    }
}