#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "csim.hpp"

bool loadStore(Cache &c, uint32_t ad, uint32_t ind, size_t n, bool dirty, bool store){
    bool hit = false;
    int toRem = -1;

    //Initializing slot
    Slot slot; 
    slot.tag = ad;
    slot.valid = !dirty;

    std::vector<Slot> block = c.sets[ind].slots;
    if (c.type == 3) { //Full-associative
        auto it = c.sets[0].index.find(ad);
        if (it != c.sets[0].index.end()) { //hit
            toRem = it->second;
            hit = true;
        } else { //miss
            //Find n-1 (See if set is full)
            for (int i = 0; i < c.sets[0].size; i++) {
                if (block[i].timestamp == n-1) {
                    toRem = i;
                    break;
                }
            }
        }
    } else { //Direct or Set-associative
        for (int i = 0; i < c.sets[ind].size; i++) {
            if (block[i].timestamp == n-1) { //Save index, if n-1 found
                toRem = i;
            }
            if (block[i].tag == ad) { //hit
                toRem = i;
                hit = true;
                break;
            }
        }
    }

    if (!hit) {
        if (store && !c.allocate && c.write) { // store & no-allocate & write-through
            return false;
        }
        if (c.type == 3 && toRem > -1) { //Full-associative and found n-1
            c.sets[0].index.erase(block[toRem].tag);
        }
        if (toRem > -1 && !block[toRem].valid) c.cycles += c.byte/4*100;
        else if(toRem == -1) { //If not found, then can increase size
            toRem = c.sets[ind].size;
            c.sets[ind].size++;
            slot.timestamp = n-1;
            block[toRem] = slot;
        }
        if (c.type == 3) { //Full-associative
            c.sets[0].index[ad] = toRem;
        }
    } 
    
    if (c.lru || !hit) {
        // Increase ts of all blocks with ts less than parameter
        for (int i = 0; i < c.sets[ind].size; i++) {
            if (block[i].timestamp < block[toRem].timestamp) {
                block[i].timestamp++;
            }
        }
        //Set slot and assign
        slot.timestamp = 0;
        block[toRem] = slot;
        c.sets[ind].slots = block;
    }
    return hit;
}

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
    
    std::vector<Set> sets(inp1);
    for (int i = 0; i < inp1; i++) { // Fill with empty pre-sized Sets of empty pre-sized Slots
        std::vector<Slot> slots(inp2);
        Set s;
        s.slots = slots;
        s.size = 0;
        sets[i] = s;
    }
    int t = inp2 == 1 ? 1 : (inp1 == 1 ? 3 : 2); //Set type of cache
    if (t == 3) { // Full-associative map initialization
        std::map<uint32_t, int> map;
        sets[0].index = map;
    }

    //Initialize cache
    Cache cache;
    cache.sets = sets;
    cache.byte = inp3;
    cache.allocate = inp4-1;
    cache.write = inp5-1;
    cache.lru = inp6-1;
    cache.type = t;
    cache.cycles = 0;
    
    unsigned long load = 0, store = 0, loadHit = 0, loadMiss = 0, storeHit = 0, storeMiss = 0, cycles = 0;
    std::string op;
    uint32_t ad;
    int dummy;

    while (std::cin >> op) {

        std::cin >> std::hex >> ad;
        std::cin >> dummy; //Unused third argument

        ad >>= logTwo(inp3); // Get rid of the offset
        uint32_t index = ad % inp1; // Get index
        ad >>= logTwo(inp1); // Get the rest which is the tag
        
        bool dirty = (op == "s" && inp5 == 1); //store and write-back
        bool hit = loadStore(cache, ad, index, (size_t)inp2, dirty, op == "s");

        if (op == "l") { //Load
            if (hit) {
                loadHit++;
                cycles++;
            } else {
                loadMiss++;
                cycles += inp3/4*100;
            }
            load++;
        } else { //Store
            if (hit) storeHit++;
            else storeMiss++;
            if (inp5 == 2) { //write-through
                cycles += 100;
                if (inp4 == 2 && !hit) { //write-allocate and miss
                    cycles += inp3/4*100; 
                }
            } else { //write-back
                if (hit) cycles++;
                else cycles += inp3/4*100;
            }
            store++;
        }
    }
    std::cout << "Total loads: " << load << std::endl;
    std::cout << "Total stores: " << store << std::endl;
    std::cout << "Load hits: " << loadHit << std::endl;
    std::cout << "Load misses: " << loadMiss << std::endl;
    std::cout << "Store hits: " << storeHit << std::endl;
    std::cout << "Store misses: " << storeMiss << std::endl;
    std::cout << "Total cycles: " << cycles + cache.cycles << std::endl;
    return 0;
}