#include <iostream>
#include <vector>

struct Slot {
    uint32_t tag;
    bool valid;
    uint32_t load_ts, access_ts;
};

struct Set {
    std::vector<Slot> slots; //Not dynamic (No add, rem), created up front
};

struct Cache {
    std::vector<Set> sets;
};

int main(int argc, char** argv) { 
    return 0;
}