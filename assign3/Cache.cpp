#include <iostream>
#include <vector>
#include "Cache.hpp"
#include "Set.hpp"
#include "Slot.hpp"

namespace Csim {
    Cache::Cache(){}

    Cache::Cache(std::vector<Set> sets) {
        sets = sets;
    }

    Cache::Cache(std::vector<Set> sets, bool all, bool typ) {
        sets = sets;
        allocation = all;
        type = typ;
    }

    //Copy constructor
    Cache::Cache(const Cache& cache) {
      sets = cache.sets;
    }

    // Deconstructor helper function
    void Cache::delete_cache() {
      sets.clear();
    }

    bool Cache::getAllocate() {
      return allocation;
    }

    bool Cache::getType() {
      return type;
    }

    std::vector<Set> Cache::getSet() {
      return sets;
    }
}