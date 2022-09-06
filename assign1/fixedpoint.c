#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "fixedpoint.h"

// You can remove this once all of the functions are fully implemented
static Fixedpoint DUMMY;
const uint64_t MAX = 0xFFFFFFFFFFFFFFFFUL;

Fixedpoint fixedpoint_create(uint64_t whole) {
  Fixedpoint fp;
  fp.whole = whole;
  fp.frac = 0;
  fp.tag = Valid_Positive;
  return fp;
}

Fixedpoint fixedpoint_create2(uint64_t whole, uint64_t frac) {
  Fixedpoint fp;
  fp.whole = whole;
  fp.frac = frac;
  fp.tag = Valid_Positive;
  return fp;
}

Fixedpoint fixedpoint_create_from_hex(const char *hex) {
  Fixedpoint fp;
  char hex2[strlen(hex) + 1];
  strcpy(hex2, hex);
  if (hex[0] == '-') {
    fp.tag = Valid_Negative;
    memcpy(hex2, &hex[1], strlen(hex));
  } else {
    fp.tag = Valid_Positive;
    memcpy(hex2, &hex[0], strlen(hex)+1);
  }
  char* token = strtok(hex2, ".");
  fp.whole = (uint64_t) strtol(token, NULL, 16);
  token = strtok(NULL, ".");
  fp.frac = (uint64_t) strtol(token, NULL, 16);
  int i = (16 - strlen(token)) * 4;
  fp.frac <<= i;
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
  if (fixedpoint_is_neg(left) && fixedpoint_is_neg(right)) {
    uint64_t sum_frac = left.frac + right.frac;
    uint64_t sum = left.whole + right.whole;
    if(sum_frac < left.frac || sum_frac < right.frac){
      fp.frac = sum_frac;
      sum++;
    }
    if (sum < left.whole || sum < right.whole) {
      fp.tag = Overflow_Negative;
    } else {
      fp.tag = Valid_Negative;
    }
    fp.whole = sum;
  } else if (!fixedpoint_is_neg(left) && !fixedpoint_is_neg(right)) {
    uint64_t sum_frac = left.frac + right.frac;
    uint64_t sum = left.whole + right.whole;
    if(sum_frac < left.frac || sum_frac < right.frac){
      fp.frac = sum_frac;
      sum++;
    }
    if (sum < left.whole || sum < right.whole) {
      fp.tag = Overflow_Positive;
    } else {
      fp.tag = Valid_Positive;
    }
    fp.whole = sum;
  } else {
    if (right.whole > left.whole) {
      fp.whole = right.whole - left.whole;
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
        fp.tag = Valid_Positive;
      }
    }
  }
  return fp;
}

Fixedpoint fixedpoint_sub(Fixedpoint left, Fixedpoint right) {
  assert(fixedpoint_is_valid(left));
  assert(fixedpoint_is_valid(right));
  if (fixedpoint_is_neg(right)) {
    right.tag = Valid_Positive;
  } else {
    right.tag = Valid_Negative;
  }
  return fixedpoint_add(left, right);
}

Fixedpoint fixedpoint_negate(Fixedpoint val) {
  assert(fixedpoint_is_valid(val));
  if (val.whole != 0 || val.frac != 0) {
    if (val.tag == Valid_Negative) val.tag = Valid_Positive;
    else if (val.tag == Valid_Positive) val.tag = Valid_Negative;
  }
  return val;
}

Fixedpoint fixedpoint_halve(Fixedpoint val) {
  Fixedpoint fp;
  if (val.frac % 2 == 1) {
    fp.tag = fixedpoint_is_neg(val) ? Underflow_Negative : Underflow_Positive;
  } else {
    fp.tag = val.tag;
  }
  if (val.whole % 2 == 1) {
    fp.frac = val.frac + (MAX - val.frac + 1) / 2;
    fp.whole = (val.whole-1) / 2;
  } else {
    fp.frac = val.frac / 2;
    fp.whole = val.whole / 2;
  }
  return fp;
}

Fixedpoint fixedpoint_double(Fixedpoint val) {
  assert(fixedpoint_is_valid(val));
  return fixedpoint_add(val, val);
}

int fixedpoint_compare(Fixedpoint left, Fixedpoint right) {
  assert(fixedpoint_is_valid(left));
  assert(fixedpoint_is_valid(right));
  if (fixedpoint_is_neg(left) && !fixedpoint_is_neg(right)) {
    return -1;
  } else if (!fixedpoint_is_neg(left) && fixedpoint_is_neg(right)) {
    return 1;
  } else {
    if (right.whole > left.whole) {
      return -1;
    } else if (right.whole < left.whole) {
      return 1;
    } else {
      if (right.frac > left.frac) {
        return -1;
      } else if (right.frac < left.frac) {
        return 1;
      } else {
        return 0;
      }
    }
  }
}

int fixedpoint_is_zero(Fixedpoint val) {
  if (val.tag == Valid_Positive || val.tag == Valid_Negative) {
    if (val.whole == 0 && val.frac == 0) {
      return 1;
    }
  }
  return 0;
}

int fixedpoint_is_err(Fixedpoint val) {
  // TODO: implement
  assert(0);
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
  if (val.tag == Valid_Positive || val.tag == Valid_Negative) {
    return 1;
  }
  return 0;
}

char *fixedpoint_format_as_hex(Fixedpoint val) {
  // TODO: implement
  assert(0);
  char *s = malloc(20);
  strcpy(s, "<invalid>");
  return s;
}
