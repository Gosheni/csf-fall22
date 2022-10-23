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
        size = 0;
    }

    Set::Set(std::vector<Slot>* s) {
        slots = *s;
        std::map<unsigned long, unsigned long> map;
        index = map;
        size = 0;
    }

    std::vector<Slot> Set::getSlots() {
        return slots;
    }

    void Set::setSlots(std::vector<Slot>* s) {
        slots = *s;
    }

    unsigned long Set::sizeOfSlots() {
        return size;
    }

    void Set::incSize() {
        size++;
    }

    std::map<unsigned long, unsigned long> Set::getIndex() {
        return index;
    }
}