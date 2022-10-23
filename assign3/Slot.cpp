#include <iostream>
#include "Slot.hpp"

namespace Csim 
{
    Slot::Slot() {
        tag = 0;
        valid = true;
        timestamp = 0;
    }

    Slot::Slot(uint32_t tag, bool valid, unsigned long ts) {
        tag = tag;
        valid = valid;
        timestamp = ts;
    }

    bool Slot::isValid() {
        return valid;
    }

    uint32_t Slot::getTag() {
        return tag;
    }

    void Slot::setTag(uint32_t t) {
        tag = t;
    }

    unsigned long Slot::getTs() {
        return timestamp;
    }

    void Slot::incTs() {
        timestamp++;
    }

    void Slot::resetTs() {
        timestamp = 0;
    }

    void Slot::makeValid() {
        valid = true;
    }
}