#ifndef SLOT_HPP
#define SLOT_HPP

#include <iostream>

namespace Csim 
{
  class Slot {

  public:
    // This default constructor initializes a board with the standard
    // piece positions, and sets the state to white's turn
    Slot();

    Slot(uint32_t tag, bool valid, unsigned long ts);
    
    // Returns true if the designated player is in mate
    bool isValid();

    uint32_t getTag();

    void setTag(uint32_t t);

    unsigned long getTs();

    void incTs();

    void resetTs();

    void makeValid();

    void makeInvalid();
    
  private:
    uint32_t tag;
    bool valid;
    unsigned long timestamp;
  };
}

#endif