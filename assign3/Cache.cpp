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

    bool Cache::callLoad(uint32_t ad, uint32_t index, size_t n) {
      //Load
      bool hit = false;
      int toRem = -1;
      Slot slot = {ad, true, 0};
      std::vector<Slot> block = sets[index].getSlots();
      for (size_t i = 0; i < block.size(); i++) {
        if (block[i].getTs() == n-1) toRem = i;
        if (block[i].getTag() == ad) {
          block[i].resetTs();
          hit = true;
        } else {
          block[i].incTs();
        }
      }
      if (!hit) {
        if (toRem > -1) block.erase(block.begin()+toRem);
        block.push_back(slot); // Pushed to the end of the block
      }
      return hit ? 1 : 0;
    }

    bool Cache::callStore(uint32_t ad, uint32_t index, size_t n) {
      //Load
      callLoad(ad, index, n);
      return 0;
    }

    bool Cache::callFullLoad(uint32_t ad, size_t n) {
      //Load
      bool hit = false;
      int toRem = -1;

      Csim::Slot slot; //Initializing slot
      slot.setTag(ad);
      slot.resetTs();
      slot.makeValid();

      std::vector<Slot> block = sets[0].getSlots();
      for (size_t i = 0; i < block.size(); i++) {
        //std::cout << i << std::endl;
        //std::cout << block[i].getTag() << std::endl;
        if (block[i].getTs() == n-1) toRem = i;
        if (block[i].getTag() == ad) {
          block[i].resetTs();
          hit = true;
        } else {
          block[i].incTs();
        }
      }
      if (!hit) {
        if (toRem > -1) block.erase(block.begin()+toRem);
        block.push_back(slot); // Pushed to the end of the block
      }
      int index = block.size()-1; 
      sets[0].getIndex()[ad] = index;
      sets[0].setSlots(&block);
      return hit ? 1 : 0;
    }

    bool Cache::callFullStore(uint32_t ad, size_t n) {
      //Load
      callFullLoad(ad, n);
      return 0;
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