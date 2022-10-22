#include <iostream>
#include "Slot.hpp"

namespace Csim 
{
    Slot::Slot(){}

    Slot::Slot(uint32_t tag) {
        tag = tag;
        valid = true;
        load_ts = 0;
        access_ts = 0;
    }

    Slot::Slot(uint32_t tag, bool valid) {
        tag = tag;
        valid = valid;
        load_ts = 0;
        access_ts = 0;
    }

    Slot::Slot(uint32_t tag, bool valid, unsigned long load_ts, unsigned long access_ts) {
        tag = tag;
        valid = valid;
        load_ts = load_ts;
        access_ts = access_ts;
    }

    bool Slot::isValid() {
        return valid;
    }

    uint32_t Slot::getTag() {
        return tag;
    }

    unsigned long Slot::getLoad_ts() {
        return load_ts;
    }

    unsigned long Slot::getAccess_ts() {
        return access_ts;
    }
}