#include <iostream>
#include <vector>
#include "Cache.hpp"
#include "Set.hpp"
#include "Slot.hpp"

namespace Csim 
{
    Cache::Cache(){}

    Cache::Cache(std::vector<Set>* s, bool all, bool wr, bool l, int t) {
        sets = *s;
        allocate = all;
        write = wr;
        lru = l;
        type = t;
    }

    //Copy constructor
    Cache::Cache(const Cache& cache) {
      sets = cache.sets;
      allocate = cache.allocate;
      write = cache.write;
      lru = cache.lru;
      type = cache.type;
    }

    // Deconstructor helper function
    void Cache::delete_cache() {
      sets.clear();
    }

    bool Cache::callLoad(Csim::Cache cache, uint32_t ad, uint32_t index, size_t n) {
      std::cout << ad << std::endl;
      //Load
      std::vector<Set> s = cache.getSet();
      Slot slot = {ad, true};
      if (cache.getType() == 3) {
        if (s[0].getSlots().size() >= n) evict(s[0].getSlots());
        s[0].getSlots().push_back(slot);
        int ind = s[0].getSlots().size()-1;
        s[0].getIndex()[ad] = ind;
      } else {
        if (s[index].getSlots().size() >= n) evict(s[index].getSlots());
        s[index].getSlots().push_back(slot);
      }
      return 0;
    }

    bool Cache::callStore(Csim::Cache cache, uint32_t ad, uint32_t index, size_t n) {
      //Load
      callLoad(cache, ad, index, n);
      return 0;
    }

    void Cache::evict(std::vector<Slot> slots) {
      //Find index of block using ts
      int index = 0;
      std::cout << 0 << std::endl;
      slots.erase(slots.begin() + index);
    }

    bool Cache::getAllocate() {
      return allocate;
    }

    bool Cache::getWrite() {
      return write;
    }

    bool Cache::getLru() {
      return lru;
    }

    int Cache::getType() {
      return type;
    }

    std::vector<Set> Cache::getSet() {
      return sets;
    }
}