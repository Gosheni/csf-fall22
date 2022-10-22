#include <iostream>
#include <vector>
#include "Cache.hpp"
#include "Set.hpp"
#include "Slot.hpp"

namespace Csim {
    Cache::Cache(){}

    Cache::Cache(std::vector<Set> sets, bool all, bool wr, bool l, int t) {
        sets = sets;
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