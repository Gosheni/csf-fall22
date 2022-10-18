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
    
    Slot(uint32_t tag);

    Slot(uint32_t tag, bool valid);

    Slot(uint32_t tag, bool valid, unsigned long load_ts, unsigned long access_ts);
    
    // Returns true if the designated player is in mate
    bool isValid();

    uint32_t getTag();

    unsigned long getLoad_ts();

    unsigned long getAccess_ts();
    
  private:
    uint32_t tag;
    bool valid;
    unsigned long load_ts, access_ts;
  };
}

#endif