#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include "fixedpoint.h"

const uint64_t MAX = 0xFFFFFFFFFFFFFFFFUL;

Fixedpoint fixedpoint_create(uint64_t whole) {
  Fixedpoint fp;
  fp.whole = whole;
  fp.frac = 0;
  fp.tag = Valid_Non_Negative;
  return fp;
}

Fixedpoint fixedpoint_create2(uint64_t whole, uint64_t frac) {
  Fixedpoint fp;
  fp.whole = whole;
  fp.frac = frac;
  fp.tag = Valid_Non_Negative;
  return fp;
}

Fixedpoint fixedpoint_create_from_hex(const char *hex) {
  Fixedpoint fp;

  //Malloc two char arrays for whole part and fraction part
  char* w = malloc(20);
  char* f = malloc(20);

  //Identify the negative
  if (hex[0] == '-') {
    fp.tag = Valid_Negative;
    hex++;
  } else {
    fp.tag = Valid_Non_Negative;
  }

  //Get the hex string of the whole part and check for errors
  int n = 0;
  while (*hex) {
    uint64_t c = *hex++;
    if (c == '.') break;
    if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))) fp.tag = Error;
    if (n >= 16) fp.tag = Error;
    w[n++] = c; 
  }
  if(n == 0){
    w[n] = '0';
    w[n+1] = '\0';
  }else{
    w[n] = '\0';
  }

  //Get the hex string of the frac part and check for errors
  n = 0;
  while (*hex) {
    uint64_t c = *hex++;
    if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))) fp.tag = Error;
    if (n >= 16) fp.tag = Error;
    f[n++] = c;
  }
  while (n < 16) {
    f[n++] = '0';
  }
  f[n] = '\0';

  //Copy the hex strings on to the instance variables of fp
  fp.whole = strtoul(w, NULL, 16);
  fp.frac = strtoul(f, NULL, 16);
  free(w);
  free(f);
  return fp; 
}

uint64_t fixedpoint_whole_part(Fixedpoint val) {
  return val.whole;
}

uint64_t fixedpoint_frac_part(Fixedpoint val) {
  return val.frac;
}

Fixedpoint fixedpoint_add(Fixedpoint left, Fixedpoint right) {
  assert(fixedpoint_is_valid(left));
  assert(fixedpoint_is_valid(right));
  Fixedpoint fp;

  //If the signs are different
  if (fixedpoint_is_neg(left) ^ fixedpoint_is_neg(right)) {
    if (right.whole > left.whole) {
      fp.whole = right.whole - left.whole;

      //If borrowing is required, we borrow by adding MAX + 1 to the regular difference between left.frac and right.frac
      if (left.frac > right.frac) {
        fp.frac = MAX - left.frac + right.frac + 1;
        fp.whole--;
      } else {
        fp.frac = right.frac - left.frac;
      }
      fp.tag = right.tag;
    } else if (left.whole > right.whole) {
      fp.whole = left.whole - right.whole;
      if (left.frac < right.frac) {
        fp.frac = MAX - right.frac + left.frac + 1;
        fp.whole--;
      } else {
        fp.frac = left.frac - right.frac;
      }
      fp.tag = left.tag;
    } else {
      fp.whole = 0;
      if (left.frac > right.frac) {
        fp.frac = left.frac - right.frac;
        fp.tag = left.tag;
      } else if (left.frac < right.frac) {
        fp.frac = right.frac - left.frac;
        fp.tag = right.tag;
      } else {
        fp.frac = 0;
        fp.tag = Valid_Non_Negative;
      }
    }
  } else {
    bool carry = false;

    fp.frac = left.frac + right.frac;
    fp.whole = left.whole + right.whole;

    //Increment fp.whole if there is a carry over on the frac part
    if (fp.frac < left.frac || fp.frac < right.frac) {
      fp.whole++;
      carry = true;
    }

    //Label as overflow if the sum is less than left or right
    if (fp.whole < left.whole || fp.whole < right.whole) {
      if (fixedpoint_is_neg(left)) fp.tag = Overflow_Negative;
      else fp.tag = Overflow_Positive;
    } else {
      if (fixedpoint_is_neg(left)) fp.tag = Valid_Negative;
      else fp.tag = Valid_Non_Negative;
    }

    //If both left and right are min or max AND there is a carry, we mark them as overflow,
    if (fixedpoint_is_neg(left) && carry) {
      if (MAX == left.whole && MAX == right.whole) {
        fp.tag = Overflow_Negative;
      }
    } else if (!fixedpoint_is_neg(left) && carry){
      if (MAX == left.whole && MAX == right.whole) {
        fp.tag = Overflow_Positive;
      }
    }
  }
  return fp;
}

Fixedpoint fixedpoint_sub(Fixedpoint left, Fixedpoint right) {
  assert(fixedpoint_is_valid(left));
  assert(fixedpoint_is_valid(right));

  if (fixedpoint_is_neg(right)) {
    right.tag = Valid_Non_Negative;
  } else {
    right.tag = Valid_Negative;
  }
  return fixedpoint_add(left, right);
}

Fixedpoint fixedpoint_negate(Fixedpoint val) {
  assert(fixedpoint_is_valid(val));

  if (val.whole != 0 || val.frac != 0) {
    if (val.tag == Valid_Negative) val.tag = Valid_Non_Negative;
    else if (val.tag == Valid_Non_Negative) val.tag = Valid_Negative;
  }
  return val;
}

Fixedpoint fixedpoint_halve(Fixedpoint val) {
  if (val.frac & 1UL) {
    val.tag = fixedpoint_is_neg(val) ? Underflow_Negative : Underflow_Positive;
  } 

  //If whole part is odd, we carry over the one from the whole part and add 1/2 to the fraction part
  //0x8000...UL equates to that 1/2 in this sense (hex)
  if (val.whole & 1UL) {
    val.frac >>= 1;
    val.frac |= 0x8000000000000000UL;
    val.whole >>= 1;
  } else {
    val.frac = val.frac >> 1;
    val.whole = val.whole >> 1;
  }
  return val;
}

Fixedpoint fixedpoint_double(Fixedpoint val) {
  assert(fixedpoint_is_valid(val));
  return fixedpoint_add(val, val);
}

int fixedpoint_compare(Fixedpoint left, Fixedpoint right) {

  //Positive is bigger than negative
  if (fixedpoint_is_neg(left) && !fixedpoint_is_neg(right)) {
    return -1;
  } else if (!fixedpoint_is_neg(left) && fixedpoint_is_neg(right)) {
    return 1;
  } else {

    //If right is bigger than left, check their signs
    //If they're negative, then return 1, otherwise -1. 0 when equal
    if (right.whole > left.whole) {
      if (!fixedpoint_is_neg(left) && !fixedpoint_is_neg(right)) {
        return -1;
      } else {
        return 1;
      }
    } else if (right.whole < left.whole) {
      if (fixedpoint_is_neg(left) && fixedpoint_is_neg(right)) {
        return -1;
      } else {
        return 1;
      }
    } else {
      if (right.frac > left.frac) {
        if (!fixedpoint_is_neg(left) && !fixedpoint_is_neg(right)) {
          return -1;
        } else {
          return 1;
        }
      } else if (right.frac < left.frac) {
        if (fixedpoint_is_neg(left) && fixedpoint_is_neg(right)) {
          return -1;
        } else {
          return 1;
        }
      } else {
        return 0;
      }
    }
  }
}

int fixedpoint_is_zero(Fixedpoint val) {
  if (val.tag == Valid_Non_Negative || val.tag == Valid_Negative) {
    if (val.whole == 0 && val.frac == 0) {
      return 1;
    }
  }
  return 0;
}

int fixedpoint_is_err(Fixedpoint val) {
  if(val.tag == Error){
    return 1;
  }
  return 0;
}

int fixedpoint_is_neg(Fixedpoint val) {
  if (val.tag == Valid_Negative) {
    return 1;
  }
  return 0;
}

int fixedpoint_is_overflow_neg(Fixedpoint val) {
  if (val.tag == Overflow_Negative) {
    return 1;
  }
  return 0;
}

int fixedpoint_is_overflow_pos(Fixedpoint val) {
  if (val.tag == Overflow_Positive) {
    return 1;
  }
  return 0;
}

int fixedpoint_is_underflow_neg(Fixedpoint val) {
  if (val.tag == Underflow_Negative) {
    return 1;
  }
  return 0;
}

int fixedpoint_is_underflow_pos(Fixedpoint val) {
  if (val.tag == Underflow_Positive) {
    return 1;
  }
  return 0;
}

int fixedpoint_is_valid(Fixedpoint val) {
  if (val.tag == Valid_Non_Negative || val.tag == Valid_Negative) {
    return 1;
  }
  return 0;
}

char *fixedpoint_format_as_hex(Fixedpoint val) {
  char *s = malloc(35);
  if(val.tag == Valid_Negative){
    if(val.frac == 0UL){
      sprintf(s, "-%lx", val.whole);
    }else{
      sprintf(s, "-%lx.%016lx", val.whole, val.frac);
      int i = strlen(s)-1;
      while(i >= 0 && s[i] == '0'){
        i--;
      }
      s[i+1] = '\0';
    }
  }else{
    if(val.frac == 0UL){
      sprintf(s, "%lx", val.whole);
    }else{
      sprintf(s, "%lx.%016lx", val.whole, val.frac);
      int i = strlen(s)-1;
      while(i >= 0 && s[i] == '0'){
        i--;
      }
      s[i+1] = '\0';
    }
  }
  return s;
}