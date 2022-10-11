#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <sstream>
#include <exception>

struct Slot {
    uint32_t tag;
    bool valid;
    uint32_t load_ts, access_ts;
};

struct Set {
    std::vector<Slot> slots; //Not dynamic (No add, remove), created up front
};

struct Cache {
    std::vector<Set> sets;
};

bool isPowerOfTwo(int n) {
    if (n == 0)
        return false;
    return (ceil(log2(n)) == floor(log2(n)));
}

int main(int argc, char** argv) { 
    if (argc != 7) {
        fprintf(stderr, "%s", "Error!\n");
        return 1;
    }
    
    int input1, input2, input3;
    try {
        input1 = std::stoi(argv[1]);
        input2 = std::stoi(argv[2]);
        input3 = std::stoi(argv[3]);		
    } catch(std::exception& err) {
        fprintf(stderr, "%s", "Error!\n");
        return 1;
    }
    if (!isPowerOfTwo(input1) || !isPowerOfTwo(input2) || !isPowerOfTwo(input3) || input3 < 4) {
        fprintf(stderr, "%s", "Error!\n");
        return 1;
    }
    std::string arg4 = argv[4];
    if (arg4 != "write-allocate" && arg4 != "no-write-allocate") {
        fprintf(stderr, "%s", "Error!\n");
        return 1;
    }
    std::string arg5 = argv[5];
    if (arg5 != "write-through" && arg5 != "write-back") {
        fprintf(stderr, "%s", "Error!\n");
        return 1;
    }
    std::string arg6 = argv[6];
    if (arg6 != "lru" && arg6 != "fifo") {
        fprintf(stderr, "%s", "Error!\n");
        return 1;
    }

    std::string line;
    std::vector<Slot> slots;
    std::vector<Set> sets;

    Cache cache = {sets};
    Set set = {slots};
    
    int count = 0;
    while (std::getline(std::cin, line) && (!line.empty())) {
        if (count >= input1 * input2) break;

        std::string op = line.substr(0, 1);
        std::string address = line.substr(2, 10);

        Slot s = {stoi(address, 0 ,16)};

        /*
        if (op == "l") {
            callLoad();
        } else {
            callStore();
        }
        */
        count++;
    }
    return 0;
}