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

int checkInput1(char** argv) {
    std::string arg1 = argv[1];
    for (char const &ch : arg1) {
        if (isdigit(ch) == 0) return -1;
    }
    int input = std::stoi(argv[1]);
    if (logTwo(input) == -1) return -1;
    return input;
}

int checkInput2(char** argv) {
    std::string arg2 = argv[2];
    for (char const &ch : arg2) {
        if (isdigit(ch) == 0) return -1;
    }
    int input = std::stoi(argv[2]);
    if (logTwo(input) == -1) return -1;
    return input;
}

int checkInput3(char** argv) {
    std::string arg3 = argv[3];
    for (char const &ch : arg3) {
        if (isdigit(ch) == 0) return -1;
    }
    int input = std::stoi(argv[3]);
    if (logTwo(input) == -1 || input < 4) return -1;
    return input;
}

int checkInput4(char** argv) {
    std::string arg4 = argv[4];
    if (arg4 == "write-allocate") return 2;
    else if (arg4 == "no-write-allocate") return 1;
    else return -1;
}

int checkInput5(char** argv) {
    std::string arg5 = argv[5];
    if (arg5 == "write-through") return 2;
    else if (arg5 == "write-back") return 1;
    else return -1;
}

int checkInput6(char** argv) {
    std::string arg6 = argv[6];
    if (arg6 == "lru") return 2;
    else if (arg6 == "fifo") return 1;
    else return -1;
}

int main(int argc, char** argv) { 
    if (argc != 7) {
        fprintf(stderr, "%s", "Error!\n");
        return 1;
    }
    int inp1 = checkInput1(argv), inp2 = checkInput2(argv), inp3 = checkInput3(argv);
    int inp4 = checkInput4(argv), inp5 = checkInput5(argv), inp6 = checkInput6(argv);
    if (inp1 < 0 || inp2 < 0 || inp3 < 0 || inp4 < 0 || inp5 < 0 || inp6 < 0) {
        fprintf(stderr, "%s", "Error!\n");
        return 1;
    }
    if (inp4 == 1 && inp5 == 1) {
        fprintf(stderr, "%s", "Error!\n");
        return 1;
    }
    
    std::vector<Csim::Set> sets(inp1);
    for (int i = 0; i < inp1; i++) { // Fill with empty pre-sized Sets of empty pre-sized Slots
        std::vector<Csim::Slot> slots(inp2);
        Csim::Set s;
        s.setSlots(&slots);
        s.resetSize();
        sets[i] = s;
    }
    int t = inp2 == 1 ? 1 : (inp1 == 1 ? 3 : 2);
    if (t == 3) {
        std::map<unsigned long, unsigned long> map;
        sets[0].setMap(&map);
    }
    Csim::Cache cache(&sets, inp4-1, inp5-1, inp6-1, t);
    
    unsigned long load = 0, store = 0, loadHit = 0, loadMiss = 0, storeHit = 0, storeMiss = 0, cycles = 0;
    std::string op;
    uint32_t ad;
    int dummy;

    while (std::cin >> op) {

        std::cin >> std::hex >> ad;
        std::cin >> dummy;

        // std::cout << op << " - " << std::stol(address, 0 ,16) << std::endl;
        // uint32_t = stol(address, 0 ,16);

        ad >>= logTwo(inp3); // Get rid of the offset
        uint32_t index = ad % inp1; // Get index
        ad >>= logTwo(inp1); // Get the rest which is the tag

        
        if (inp6 == 2) { //LRU implementation
            if (inp4 == 1 && inp5 == 2) { // No-write-allocate & write-through
                if (op == "l") { // Load
                    if (t == 3) { // Full-associative
                        if (cache.callLoadFull(ad, (size_t)inp2)) {
                            loadHit++;
                            cycles++;
                        } else {
                            loadMiss++;
                            cycles += inp3/4*100;
                        }
                    } else {
                        if (cache.callLoad(ad, index, (size_t)inp2)) { // Others
                            loadHit++;
                            cycles++;
                        } else {
                            loadMiss++;
                            cycles += inp3/4*100;
                        }
                    }
                    load++;
                } else { // Store
                    if (t == 3) cache.storeMemoryFull(ad) ? storeHit++ : storeMiss++; //Full-associative
                    else cache.storeMemory(ad, index) ? storeHit++ : storeMiss++; //Others
                    store++;
                    cycles += 100;
                }
            }
            if (inp4 == 2 && inp5 == 2) { // write-allocate & write-through
                if (op == "l") { // Load
                    if (t == 3) { // Full-associative
                        if (cache.callLoadFull(ad, (size_t)inp2)) {
                            loadHit++;
                            cycles++;
                        } else {
                            loadMiss++;
                            cycles += inp3/4*100;
                        }
                    } else {
                        if (cache.callLoad(ad, index, (size_t)inp2)) { // Others
                            loadHit++;
                            cycles++;
                        } else {
                            loadMiss++;
                            cycles += inp3/4*100;
                        }
                    }
                    load++;
                } else { // Store
                    if (t == 3) { // Full-associative
                        if (cache.callStoreFull(ad, (size_t)inp2)) {
                            storeHit++;
                            cycles += 100;
                        } else {
                            storeMiss++;
                            cycles += inp3/4*100 + 100;
                        }
                    } else {
                        if (cache.callStore(ad, index, (size_t)inp2)) { // Others
                            storeHit++;
                            cycles += 100;
                        } else {
                            storeMiss++;
                            cycles += inp3/4*100 + 100;
                        }
                    }
                    store++;
                }
            }
            if (inp4 == 2 && inp5 == 1) { // write-allocate & write-back
                if (op == "l") { // Load
                    bool l = true;
                    if (t == 3) { // Full-associative
                        if (cache.dirtyFull(ad, (size_t)inp2, inp3, l)) {
                            loadHit++;
                            cycles++;
                        } else {
                            loadMiss++;
                            cycles += inp3/4*100;
                        }
                    } else {
                        if (cache.dirty(ad, index, (size_t)inp2, inp3, l)) { // Others
                            loadHit++;
                            cycles++;
                        } else {
                            loadMiss++;
                            cycles += inp3/4*100;
                        }
                    }
                    load++;
                } else { // Store
                    bool l = false;
                    if (t == 3) { // Full-associative
                        if (cache.dirtyFull(ad, (size_t)inp2, inp3, l)) {
                            storeHit++;
                            cycles++;
                        } else {
                            storeMiss++;
                            cycles += inp3/4*100;
                        }
                    } else {
                        if (cache.dirty(ad, index, (size_t)inp2, inp3, l)) { // Others
                            storeHit++;
                            cycles++;
                        } else {
                            storeMiss++;
                            cycles += inp3/4*100;
                        }
                    }
                    store++;
                }
            }
        }
    }
    std::cout << "Total loads: " << load << std::endl;
    std::cout << "Total stores: " << store << std::endl;
    std::cout << "Load hits: " << loadHit << std::endl;
    std::cout << "Load misses: " << loadMiss << std::endl;
    std::cout << "Store hits: " << storeHit << std::endl;
    std::cout << "Store misses: " << storeMiss << std::endl;
    std::cout << "Total cycles: " << cycles + cache.getCycles() << std::endl;
    return 0;
}

//Questions: 
//1. Expel the block with lowest ts, does this mean iterating through every element?
//2. Why access_ts (lru) (Reset ts for same address, same set) AND load_ts (fifo) (Don't change ts)
//write-through (Replace same tag, +100 cycles) write-back (Set same tag invalid, add new tag, when being replaced, if slot is valid, +100 cycles)
//write-allocate, load into cache, no-write-allocate only +100  cycles
//load miss, load into cache from memory, +100 cycles