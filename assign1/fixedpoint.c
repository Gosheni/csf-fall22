#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "fixedpoint.h"

// You can remove this once all of the functions are fully implemented
static Fixedpoint DUMMY;

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
  char* w = malloc(20);
  char* f = malloc(20);
  if (hex[0] == '-') {
    fp.tag = Valid_Negative;
    hex++;
  } else {
    fp.tag = Valid_Non_Negative;
  }
  int n = 0;
  while (*hex) {
    uint64_t c = *hex++;
    if (c == '.') {
      break;
    }
    if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))) fp.tag = Error;
    if (n >= 16) fp.tag = Error;
    w[n++] = c; 
  }
  w[n] = '\0';
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
  char* ptr;
  char* ptr2;
  fp.whole = strtoul(w, &ptr, 16);
  fp.frac = strtoul(f, &ptr2, 16);
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

  Fixedpoint half = fixedpoint_create(0x800000000000000UL);
  
  if (fixedpoint_is_neg(left) ^ fixedpoint_is_neg(right)) {
    if (right.whole > left.whole) {
      fp.whole = right.whole - left.whole;
      if (left.frac > right.frac) {
        fp.frac = left.frac + right.frac + half.whole;
        fp.whole--;
      } else {
        fp.frac = right.frac - left.frac;
      }
      fp.tag = right.tag;
    } else if (left.whole > right.whole) {
      fp.whole = left.whole - right.whole;
      if (left.frac < right.frac) {
        fp.frac = right.frac + left.frac + half.whole;
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
    uint64_t sum_frac = left.frac + right.frac;
    uint64_t sum = left.whole + right.whole;

    Fixedpoint max = fixedpoint_create2(0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL);

    if (sum_frac < left.frac || sum_frac < right.frac){
      fp.frac = sum_frac;
      sum++;
    }
    if (sum < left.whole || sum < right.whole) {
      if (fixedpoint_is_neg(left)) fp.tag = Overflow_Negative;
      else fp.tag = Overflow_Positive;
    } else {
      if (fixedpoint_is_neg(left)) fp.tag = Valid_Negative;
      else fp.tag = Valid_Non_Negative;
    }
    fp.whole = sum;

    //If both left and right are min or max
    if (fixedpoint_is_neg(left)) {
      Fixedpoint min = fixedpoint_negate(max);
      if (fixedpoint_compare(min, left) == 0 && fixedpoint_compare(min, right) == 0) {
        fp.tag = Overflow_Negative;
      }
    } else {
      if (fixedpoint_compare(max, left) == 0 && fixedpoint_compare(max, right) == 0) {
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
  if (val.whole & 1UL) {
    val.frac >>= 1;
    val.frac |= 0x8000000000000000UL;
    val.whole >>= 1;
  } else {
    val.frac = val.frac >> 1;
    val.whole = val.whole >> 1;
  }
  while (val.frac % 16 == 0) {
    val.frac /= 16;
  }
  return val;
}

Fixedpoint fixedpoint_double(Fixedpoint val) {
  assert(fixedpoint_is_valid(val));
  return fixedpoint_add(val, val);
}

int fixedpoint_compare(Fixedpoint left, Fixedpoint right) {
  if (fixedpoint_is_neg(left) && !fixedpoint_is_neg(right)) {
    return -1;
  } else if (!fixedpoint_is_neg(left) && fixedpoint_is_neg(right)) {
    return 1;
  } else if (!fixedpoint_is_neg(left) && !fixedpoint_is_neg(right)) {
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
  } else {
    if (right.whole > left.whole) {
      return 1;
    } else if (right.whole < left.whole) {
      return -1;
    } else {
      if (right.frac > left.frac) {
        return 1;
      } else if (right.frac < left.frac) {
        return -1;
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
  printf("-------");
  char *s = malloc(20);
  char *w = malloc(20);
  if (val.whole == 0) {
    strcpy(w, "0");
  } else {
    //whole
    uint64_t whole = val.whole;
    int i = 0;
    while (whole != 0) {
      uint8_t c = whole % 16;
      char integer_string[2];
      if (c >= 10 && c <= 15) {
        c = (c - 10) + 'a'; 
      } else {
        c = '0' + c;
      }
      *(w+i) = c;
      i++;
      whole /= 16;
    }
    for (size_t i = 0; i < strlen(w)/2; i++) {
      char temp = w[i];
      w[i] = w[strlen(w) - i - 1];
      w[strlen(w) - i - 1] = temp;
    }
  }
  printf("\n%s\n", w);
  //fraction
  if(val.frac == 0){
    return w;
  }else{
    strcat(w, ".");
    char *f = malloc(100);
    uint64_t frac = val.frac;
    //printf("%" PRIu64 "\n", frac);
    int add_value = 0;
    int extra_zeros = 0;
    int i = 0;
    while (frac != 0) {
      uint8_t c = frac % 16;
      if (c >= 10 && c <= 15) {
        add_value++;
        c = (c - 10) + 'a';
      } else if(c != 0 || add_value){
        add_value++;
        c = '0' + c;
      } else {
        extra_zeros++;
      }
      if(add_value){
        //printf("==%c\n",c);
        *(f+i) = c;
        i++;
      }
      frac /= 16;
    }
    printf("valid_values: %d\n",add_value);
    printf("extra_zeros: %d\n",extra_zeros);
    for(int j = 0; j < (16-add_value-extra_zeros); j++){
      *(f+i+j) = '0';
    }
    printf("%s\n", f);
    for (size_t i = 0; i < strlen(f)/2; i++) {
      char temp = f[i];
      f[i] = f[strlen(f) - i - 1];
      f[strlen(f) - i - 1] = temp;
    }
    f[strlen(f)] = '\0';
    printf("%s\n", f);
    //printf("%s\n", strcat(w, f));
    return strcat(w, f);
  }
}