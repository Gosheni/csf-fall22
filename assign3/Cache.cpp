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
        cycles = 0;
    }

    //Copy constructor
    Cache::Cache(const Cache& cache) {
      sets = cache.sets;
      allocate = cache.allocate;
      write = cache.write;
      lru = cache.lru;
      type = cache.type;
      cycles = 0;
    }

    // Deconstructor helper function
    void Cache::delete_cache() {
      sets.clear();
    }

    bool Cache::callLoad(uint32_t ad, uint32_t index, size_t n) {
      //Load
      bool hit = false;
      int toRem = -1;
      
      Csim::Slot slot = Slot(); //Initializing slot
      slot.setTag(ad);
      slot.resetTs();
      slot.makeValid();

      std::vector<Slot> block = sets[index].getSlots();
      for (size_t i = 0; i < sets[index].sizeOfSlots(); i++) {
        if (block[i].getTs() == n-1) toRem = i;
        if (block[i].getTag() == ad) {
          block[i].resetTs();
          hit = true;
        } else {
          block[i].incTs();
        }
      }
      if (!hit) {
        if (toRem > -1) {
          block.erase(block.begin()+toRem);
        } else {
          toRem = sets[index].sizeOfSlots();
        }
        block[toRem] = slot;
        sets[index].incSize();
      }
      sets[index].setSlots(&block);
      return hit;
    }

    bool Cache::callStore(uint32_t ad, uint32_t index, size_t n) {
      //Load
      callLoad(ad, index, n);
      return 0;
    }

    bool Cache::callLoadFull(uint32_t ad, size_t n) {
      //Load
      bool hit = false;
      int toRem = -1;

      Csim::Slot slot; //Initializing slot
      slot.setTag(ad);
      slot.resetTs();
      slot.makeValid();

      std::map<unsigned long, unsigned long> temp = sets[0].getIndex();
      std::vector<Slot> block = sets[0].getSlots();
      for (size_t i = 0; i < sets[0].sizeOfSlots(); i++) {
        if (block[i].getTs() == n-1) toRem = i; // Save the index of highest ts
        if (block[i].getTag() == ad) { // If it's a hit
          block[i].resetTs(); // Reset ts to 0
          hit = true;
        } else {
          block[i].incTs(); // Else inc ts
        }
      }
      if (!hit) {
        if (toRem > -1) {
          block.erase(block.begin()+toRem);
        } else {
          toRem = sets[0].sizeOfSlots();
        }
        block[toRem] = slot;
        sets[0].incSize();
        temp[ad] = toRem;
        sets[0].setMap(&temp);
      } 
      sets[0].setSlots(&block);
      return hit;
    }

    bool Cache::callStoreFull(uint32_t ad, size_t n) {
      //Load
      callLoadFull(ad, n);
      return 0;
    }

    bool Cache::storeMemory(uint32_t ad, uint32_t index) {
      //Load
      bool hit = false;
      std::vector<Slot> block = sets[index].getSlots();
      for (size_t i = 0; i < sets[index].sizeOfSlots(); i++) {
        if (block[i].getTag() == ad) { // If it's a hit
          block[i].resetTs(); // Reset ts to 0
          hit = true;
        } else {
          block[i].incTs(); // Else inc ts
        }
      }
      if (hit) sets[0].setSlots(&block); // If it's store miss, sets doesn't get updated
      return hit;
    }

    bool Cache::storeMemoryFull(uint32_t ad) {

      std::map<unsigned long, unsigned long> m = sets[0].getIndex();
      std::map<unsigned long, unsigned long>::iterator it = m.find(ad); // Check if map gets updated
      if (m.empty() || it == sets[0].getIndex().end()) return false; // Store miss 

      std::vector<Slot> block = sets[0].getSlots();
      for (size_t i = 0; i < sets[0].sizeOfSlots(); i++) {
        if (block[i].getTag() == ad) { // If it's a hit
          block[i].resetTs(); // Reset ts to 0
        } else {
          block[i].incTs(); // Else inc ts
        }
      }
      sets[0].setSlots(&block);
      return true; // Store hit
    }

    bool Cache::dirty(uint32_t ad, uint32_t index, size_t n, uint32_t byte, bool l) {
      //Load
      bool hit = false;
      int toRem = -1;
      
      Csim::Slot slot; //Initializing slot
      slot.setTag(ad);
      slot.resetTs();
      l ? slot.makeValid() : slot.makeInvalid();

      std::vector<Slot> block = sets[index].getSlots();
      for (size_t i = 0; i < sets[index].sizeOfSlots(); i++) {
        if (block[i].getTs() == n-1) toRem = i;
        if (block[i].getTag() == ad) {
          if (!l) block[i].makeInvalid();
          block[i].resetTs();
          hit = true;
        } else {
          block[i].incTs();
        }
      }
      if (!hit) {
        if (toRem > -1) {
          if (!block[toRem].isValid()) cycles += byte/4*100;
          block.erase(block.begin()+toRem);
        } else {
          toRem = sets[index].sizeOfSlots();
        }
        block[toRem] = slot;
        sets[index].incSize();
      } 
      sets[index].setSlots(&block);
      return hit;
    }

    bool Cache::dirtyFull(uint32_t ad, size_t n, uint32_t byte, bool l) {
      //Load
      bool hit = false;
      int toRem = -1;

      Csim::Slot slot; //Initializing slot
      slot.setTag(ad);
      slot.resetTs();
      l ? slot.makeValid() : slot.makeInvalid();

      std::map<unsigned long, unsigned long> temp = sets[0].getIndex();
      std::vector<Slot> block = sets[0].getSlots();
      for (size_t i = 0; i < sets[0].sizeOfSlots(); i++) {
        if (block[i].getTs() == n-1) toRem = i; // Save the index of highest ts
        if (block[i].getTag() == ad) { // If it's a hit
          if (!l) block[i].makeInvalid();
          block[i].resetTs(); // Reset ts to 0
          hit = true;
        } else {
          block[i].incTs(); // Else inc ts
        }
      }
      if (!hit) {
        if (toRem > -1) {
          if (!block[toRem].isValid()) cycles += byte/4*100;
          block.erase(block.begin()+toRem);
        } else {
          toRem = sets[0].sizeOfSlots();
        }
        block[toRem] = slot;
        sets[0].incSize();
        temp[ad] = toRem;
        sets[0].setMap(&temp);
      } 
      sets[0].setSlots(&block);
      return hit;
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

    unsigned long Cache::getCycles() {
      return cycles;
    }

    std::vector<Set> Cache::getSet() {
      return sets;
    }
}