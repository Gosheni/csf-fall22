#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <sstream>
#include <exception>
#include "Cache.hpp"
#include "Set.hpp"
#include "Slot.hpp"

int logTwo(int n) {
    if (n == 0) return -1;
    int count = 0;
    while ((n % 2) == 0) {
        n >>= 1;
        count++;
    }
    return n == 1 ? count : -1;
}

bool callLoad(){
    return 0;
}

bool callStore(){
    return 0;
}

bool checkInput(char** argv) {
    std::string arg4 = argv[4];
    if (arg4 != "write-allocate" && arg4 != "no-write-allocate") return false;

    std::string arg5 = argv[5];
    if (arg5 != "write-through" && arg5 != "write-back") return false;
    if (arg4 == "no-write-allocate" && arg5 == "write-back") return false;

    std::string arg6 = argv[6];
    if (arg6 != "lru" && arg6 != "fifo") return false;

    return true;
}

int main(int argc, char** argv) { 
    if (argc != 7) {
        fprintf(stderr, "%s", "Error!\n");
        return 1;
    }
    std::string arg1 = argv[1], arg2 = argv[2], arg3 = argv[3];   
    for (char const &ch : arg1) {
        if (isdigit(ch) == 0) {
            fprintf(stderr, "%s", "Error!\n");
            return 1;
        }
    }
    for (char const &ch : arg2) {
        if (isdigit(ch) == 0) {
            fprintf(stderr, "%s", "Error!\n");
            return 1;
        }
    }
    for (char const &ch : arg3) {
        if (isdigit(ch) == 0) {
            fprintf(stderr, "%s", "Error!\n");
            return 1;
        }
    }  
    int input1 = std::stoi(argv[1]), input2 = std::stoi(argv[2]), input3 = std::stoi(argv[3]);
    int size1 = logTwo(input1), size2 = logTwo(input2), size3 = logTwo(input3);
    if (size1 == -1 || size2 == -1 || size3 == -1 || input3 < 4) {
        fprintf(stderr, "%s", "Error!\n");
        return 1;
    }
    if (!checkInput(argv)) {
        fprintf(stderr, "%s", "Error!\n");
        return 1;
    }

    std::string line;
    std::vector<Csim::Slot> slots(input2);
    std::vector<Csim::Set> sets(size1);

    Csim::Cache ca = {sets};
    Csim::Set set = {slots};
    
    int count = 0;
    unsigned long load = 0, store = 0, loadHit = 0, loadMiss = 0, storeHit = 0, storeMiss = 0;

    while (std::getline(std::cin, line) && (!line.empty())) {
        if (count >= input1 * input2) break;

        std::string op = line.substr(0, 1);
        std::string address = line.substr(2, 10);
        // std::cout << op << " - " << std::stol(address, 0 ,16) << std::endl;
        // uint32_t = stol(address, 0 ,16);
        uint32_t ad = stoi(address, 0, 16);

        ad >>= size3; // Get rid of the offset
        uint32_t index = ad % input1; // Get index
        ad >>= size1; // Get the rest which is the tag
        Csim::Slot s = {ad, true}; // Initialize slot

        /*if (ca.sets.at(index) == NULL) {
            std::vector<Slot> slots(input2);
            Set st = {slots};
            sets.at(index) = st;
            st.slots.push_back(s);
        } else {
            ca.sets.at(index).slots.push_back(s);
        }*/
        
        if (op == "l") {
            (callLoad()) ? loadHit++ : loadMiss++;
            load++;
        } else {
            (callStore()) ? storeHit++ : storeMiss++;
            store++;
        }
        count++;
    }
    std::cout << "Total loads: " << load << std::endl;
    std::cout << "Total stores: " << store << std::endl;
    std::cout << "Load hits: " << loadHit << std::endl;
    std::cout << "Load misses: " << loadMiss << std::endl;
    std::cout << "Store hits: " << storeHit << std::endl;
    std::cout << "Store misses: " << storeMiss << std::endl;
    std::cout << "Total cycles: " << load << std::endl;
    return 0;
}

//Questions: 
//1. How to utilize enum in string inputs
//2. Why access_ts AND load_ts?