#include <stdio.h>
#include <stdlib.h>
#include "fixedpoint.h"
#include "tctest.h"

// Test fixture object, has some useful values for testing
typedef struct {
  Fixedpoint zero;
  Fixedpoint one;
  Fixedpoint ten;
  Fixedpoint thousand;
  Fixedpoint one_half;
  Fixedpoint one_fourth;
  Fixedpoint odd_frac1;
  Fixedpoint odd_frac2;
  Fixedpoint large1;
  Fixedpoint large2;
  Fixedpoint max;

  // TODO: add more objects to the test fixture
} TestObjs;

// functions to create and destroy the test fixture
TestObjs *setup(void);
void cleanup(TestObjs *objs);

// test functions
void test_whole_part(TestObjs *objs);
void test_frac_part(TestObjs *objs);
void test_create_from_hex(TestObjs *objs);
void test_format_as_hex(TestObjs *objs);
void test_negate(TestObjs *objs);
void test_add(TestObjs *objs);
void test_sub(TestObjs *objs);
void test_is_overflow_pos(TestObjs *objs);
void test_is_err(TestObjs *objs);
void test_create(TestObjs *objs);
void test_create_2(TestObjs *objs);
void test_halve(TestObjs *objs);
void test_double(TestObjs *objs);
void test_compare(TestObjs *objs);
void test_is_zero(TestObjs *objs);
void test_is_neg(TestObjs *objs);
void test_is_overflow_neg(TestObjs *objs);
void test_is_underflow_pos(TestObjs *objs);
void test_is_underflow_neg(TestObjs *objs);
void test_is_valid(TestObjs *objs);

// TODO: add more test functions

int main(int argc, char **argv) {
  // if a testname was specified on the command line, only that
  // test function will be executed
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  TEST(test_create);
  TEST(test_create_2);
  TEST(test_whole_part);
  TEST(test_frac_part);
  TEST(test_create_from_hex);
  TEST(test_format_as_hex);
  TEST(test_negate);
  TEST(test_add);
  TEST(test_sub);
  TEST(test_halve);
  TEST(test_double);
  TEST(test_is_overflow_pos);
  TEST(test_is_overflow_neg);
  TEST(test_is_underflow_pos);
  TEST(test_is_underflow_neg);
  TEST(test_is_err);
  TEST(test_is_zero);
  TEST(test_is_neg);
  TEST(test_compare);

  // IMPORTANT: if you add additional test functions (which you should!),
  // make sure they are included here.  E.g., if you add a test function
  // "my_awesome_tests", you should add
  //
  //   TEST(my_awesome_tests);
  //
  // here. This ensures that your test function will actually be executed.

  TEST_FINI();
}

TestObjs *setup(void) {
  TestObjs *objs = malloc(sizeof(TestObjs));

  objs->zero = fixedpoint_create(0UL);
  objs->one = fixedpoint_create(1UL);
  objs->ten = fixedpoint_create(0xaUL);
  objs->thousand = fixedpoint_create(0x3e8UL);
  objs->one_half = fixedpoint_create2(0UL, 0x8000000000000000UL);
  objs->one_fourth = fixedpoint_create2(0UL, 0x4000000000000000UL);
  objs->odd_frac1 = fixedpoint_create2(0UL, 0x4000000000000001UL);
  objs->odd_frac2 = fixedpoint_create2(0UL, 0x5738200583192347UL);
  objs->large1 = fixedpoint_create2(0x4b19efceaUL, 0xec9a1e2418UL);
  objs->large2 = fixedpoint_create2(0xfcbf3d5UL, 0x4d1a23c24fafUL);
  objs->max = fixedpoint_create2(0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL);

  return objs;
}

void cleanup(TestObjs *objs) {
  free(objs);
}

void test_create(TestObjs *objs) {
  ASSERT(fixedpoint_whole_part(objs->zero) == 0);
  ASSERT(fixedpoint_whole_part(objs->one) == 1);
  ASSERT(fixedpoint_whole_part(objs->ten) == 10);
  ASSERT(fixedpoint_whole_part(objs->thousand) == 1000);
}

void test_create_2(TestObjs *objs) {
  ASSERT(fixedpoint_whole_part(objs->one_half) == 0);
  ASSERT(fixedpoint_frac_part(objs->one_half) == 0x8000000000000000UL);
  ASSERT(fixedpoint_whole_part(objs->one_fourth) == 0);
  ASSERT(fixedpoint_frac_part(objs->one_fourth) == 0x4000000000000000UL);
  ASSERT(fixedpoint_whole_part(objs->large2) == 0xfcbf3d5UL);
  ASSERT(fixedpoint_frac_part(objs->large2) == 0x4d1a23c24fafUL);
}

void test_is_valid(TestObjs *objs) {
  Fixedpoint min = fixedpoint_negate(objs->max);
  Fixedpoint negative_one = fixedpoint_negate(objs->one);

  ASSERT(fixedpoint_is_valid(objs->zero));
  ASSERT(fixedpoint_is_valid(objs->odd_frac2));
  ASSERT(fixedpoint_is_valid(objs->large1));
  ASSERT(fixedpoint_is_valid(negative_one));
  ASSERT(fixedpoint_is_valid(min));
}

void test_is_zero(TestObjs *objs) {
  Fixedpoint neg_zero = fixedpoint_negate(objs->zero);

  ASSERT(fixedpoint_is_valid(objs->zero));
  ASSERT(fixedpoint_is_valid(neg_zero));
}

void test_is_neg(TestObjs *objs) {
  Fixedpoint min = fixedpoint_negate(objs->max);
  Fixedpoint negative_one = fixedpoint_negate(objs->one);
  Fixedpoint neg_odd1 = fixedpoint_negate(objs->odd_frac1);

  ASSERT(fixedpoint_is_valid(min));
  ASSERT(fixedpoint_is_valid(negative_one));
  ASSERT(fixedpoint_is_valid(neg_odd1));
}

void test_whole_part(TestObjs *objs) {
  ASSERT(0UL == fixedpoint_whole_part(objs->zero));
  ASSERT(1UL == fixedpoint_whole_part(objs->one));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_half));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_fourth));
  ASSERT(0x4b19efceaUL == fixedpoint_whole_part(objs->large1));
  ASSERT(0xfcbf3d5UL == fixedpoint_whole_part(objs->large2));
}

void test_frac_part(TestObjs *objs) {
  ASSERT(0UL == fixedpoint_frac_part(objs->zero));
  ASSERT(0UL == fixedpoint_frac_part(objs->one));
  ASSERT(0x8000000000000000UL == fixedpoint_frac_part(objs->one_half));
  ASSERT(0x4000000000000000UL == fixedpoint_frac_part(objs->one_fourth));
  ASSERT(0xec9a1e2418UL == fixedpoint_frac_part(objs->large1));
  ASSERT(0x4d1a23c24fafUL == fixedpoint_frac_part(objs->large2));
}

void test_create_from_hex(TestObjs *objs) {
  (void) objs;

  Fixedpoint val1 = fixedpoint_create_from_hex("f6a5865.00f2");

  ASSERT(fixedpoint_is_valid(val1));

  ASSERT(0xf6a5865UL == fixedpoint_whole_part(val1));

  ASSERT(0x00f2000000000000UL == fixedpoint_frac_part(val1));
}

void test_format_as_hex(TestObjs *objs) {
  char *s;

  s = fixedpoint_format_as_hex(objs->zero);
  ASSERT(0 == strcmp(s, "0"));
  free(s);

  s = fixedpoint_format_as_hex(objs->one);
  ASSERT(0 == strcmp(s, "1"));
  free(s);

  s = fixedpoint_format_as_hex(objs->ten);
  ASSERT(0 == strcmp(s, "a"));
  free(s);

  s = fixedpoint_format_as_hex(objs->thousand);
  ASSERT(0 == strcmp(s, "3e8"));
  free(s);

  s = fixedpoint_format_as_hex(objs->one_half);
  ASSERT(0 == strcmp(s, "0.8"));
  free(s);

  s = fixedpoint_format_as_hex(objs->one_fourth);
  ASSERT(0 == strcmp(s, "0.4"));
  free(s);

  s = fixedpoint_format_as_hex(objs->large1);
  ASSERT(0 == strcmp(s, "4b19efcea.000000ec9a1e2418"));
  free(s);

  s = fixedpoint_format_as_hex(objs->large2);
  ASSERT(0 == strcmp(s, "fcbf3d5.00004d1a23c24faf"));
  free(s);
}

void test_negate(TestObjs *objs) {
  // none of the test fixture objects are negative
  ASSERT(!fixedpoint_is_neg(objs->zero));
  ASSERT(!fixedpoint_is_neg(objs->one));
  ASSERT(!fixedpoint_is_neg(objs->one_half));
  ASSERT(!fixedpoint_is_neg(objs->one_fourth));
  ASSERT(!fixedpoint_is_neg(objs->large1));
  ASSERT(!fixedpoint_is_neg(objs->large2));

  // negate the test fixture values
  Fixedpoint zero_neg = fixedpoint_negate(objs->zero);
  Fixedpoint one_neg = fixedpoint_negate(objs->one);
  Fixedpoint one_half_neg = fixedpoint_negate(objs->one_half);
  Fixedpoint one_fourth_neg = fixedpoint_negate(objs->one_fourth);
  Fixedpoint large1_neg = fixedpoint_negate(objs->large1);
  Fixedpoint large2_neg = fixedpoint_negate(objs->large2);

  // zero does not become negative when negated
  ASSERT(!fixedpoint_is_neg(zero_neg));

  // all of the other values should have become negative when negated
  ASSERT(fixedpoint_is_neg(one_neg));
  ASSERT(fixedpoint_is_neg(one_half_neg));
  ASSERT(fixedpoint_is_neg(one_fourth_neg));
  ASSERT(fixedpoint_is_neg(large1_neg));
  ASSERT(fixedpoint_is_neg(large2_neg));

  // magnitudes should stay the same
  ASSERT(0UL == fixedpoint_whole_part(objs->zero));
  ASSERT(1UL == fixedpoint_whole_part(objs->one));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_half));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_fourth));
  ASSERT(0x4b19efceaUL == fixedpoint_whole_part(objs->large1));
  ASSERT(0xfcbf3d5UL == fixedpoint_whole_part(objs->large2));
  ASSERT(0UL == fixedpoint_frac_part(objs->zero));
  ASSERT(0UL == fixedpoint_frac_part(objs->one));
  ASSERT(0x8000000000000000UL == fixedpoint_frac_part(objs->one_half));
  ASSERT(0x4000000000000000UL == fixedpoint_frac_part(objs->one_fourth));
  ASSERT(0xec9a1e2418UL == fixedpoint_frac_part(objs->large1));
  ASSERT(0x4d1a23c24fafUL == fixedpoint_frac_part(objs->large2));
}

void test_add(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, sum;

  lhs = fixedpoint_create_from_hex("-c7252a193ae07.7a51de9ea0538c5");
  rhs = fixedpoint_create_from_hex("d09079.1e6d601");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0xc7252a0c31d8eUL == fixedpoint_whole_part(sum));
  ASSERT(0x5be47e8ea0538c50UL == fixedpoint_frac_part(sum));
}

void test_sub(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, diff;

  lhs = fixedpoint_create_from_hex("-ccf35aa3a04a3b.b105");
  rhs = fixedpoint_create_from_hex("f676e8.58");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(diff));
  ASSERT(0xccf35aa496c124UL == fixedpoint_whole_part(diff));
  ASSERT(0x0905000000000000UL == fixedpoint_frac_part(diff));
}

void test_halve(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, half1, half2;

  lhs = fixedpoint_create_from_hex("-c7252a193ae06.7a51de9ea0538c4");
  rhs = fixedpoint_create_from_hex("d09079.1e6d604");
  half1 = fixedpoint_halve(lhs);
  half2 = fixedpoint_halve(rhs);
  ASSERT(fixedpoint_is_neg(half1));
  ASSERT(0x6392950c9d703UL == fixedpoint_whole_part(half1));
  printf("%ld", fixedpoint_frac_part(half2));
  ASSERT(0x3d28ef4f5029c62UL == fixedpoint_frac_part(half1));
  ASSERT(!fixedpoint_is_neg(half2));
  ASSERT(0x68483cUL == fixedpoint_whole_part(half2));
  ASSERT(0x8000000000f36b02UL == fixedpoint_frac_part(half2));
}

void test_double(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, doub1, doub2;

  lhs = fixedpoint_create_from_hex("-c7252a193ae07.a51de9ea0538c5");
  rhs = fixedpoint_create_from_hex("d09079.1e6d601");
  doub1 = fixedpoint_double(lhs);
  doub2 = fixedpoint_double(rhs);
  ASSERT(fixedpoint_is_neg(doub1));
  ASSERT(0x18e4a543275c0fUL == fixedpoint_whole_part(doub1));
  ASSERT(0x4a3bd3d40a718aUL == fixedpoint_frac_part(doub1));
  ASSERT(!fixedpoint_is_neg(doub2));
  ASSERT(0x1a120f2UL == fixedpoint_whole_part(doub2));
  ASSERT(0x3cdac02UL == fixedpoint_frac_part(doub2));
}

void test_compare(TestObjs *objs) {
  Fixedpoint min = fixedpoint_negate(objs->max);
  Fixedpoint neg_one = fixedpoint_negate(objs->one);
  Fixedpoint neg_large2 = fixedpoint_negate(objs->large2);

  ASSERT(1 == fixedpoint_compare(objs->max, objs->one));
  ASSERT(0 == fixedpoint_compare(objs->one, objs->one));
  ASSERT(-1 == fixedpoint_compare(objs->one, objs->max));

  ASSERT(1 == fixedpoint_compare(objs->max, min));
  ASSERT(0 == fixedpoint_compare(neg_large2, neg_large2));
  ASSERT(-1 == fixedpoint_compare(min, neg_one));
}

void test_is_overflow_pos(TestObjs *objs) {
  Fixedpoint sum;

  sum = fixedpoint_add(objs->max, objs->one);
  ASSERT(fixedpoint_is_overflow_pos(sum));

  sum = fixedpoint_add(objs->one, objs->max);
  ASSERT(fixedpoint_is_overflow_pos(sum));

  Fixedpoint negative_one = fixedpoint_negate(objs->one);

  sum = fixedpoint_sub(objs->max, negative_one);
  ASSERT(fixedpoint_is_overflow_pos(sum));

  sum = fixedpoint_add(objs->max, objs->max);
  ASSERT(fixedpoint_is_overflow_pos(sum));
}

void test_is_overflow_neg(TestObjs *objs) {
  Fixedpoint dif;

  Fixedpoint negative_one = fixedpoint_negate(objs->one);
  Fixedpoint min = fixedpoint_negate(objs->max);

  dif = fixedpoint_add(negative_one, min);
  ASSERT(fixedpoint_is_overflow_neg(dif));

  dif = fixedpoint_sub(min, objs->one);
  ASSERT(fixedpoint_is_overflow_neg(dif));

  dif = fixedpoint_sub(min, objs->max);
  ASSERT(fixedpoint_is_overflow_neg(dif));
}

void test_is_underflow_pos(TestObjs *objs) {
  Fixedpoint half;

  half = fixedpoint_halve(objs->odd_frac1);
  ASSERT(fixedpoint_is_underflow_pos(half));

  half = fixedpoint_halve(objs->odd_frac2);
  ASSERT(fixedpoint_is_underflow_pos(half));
  
  half = fixedpoint_halve(objs->max);
  ASSERT(fixedpoint_is_underflow_pos(half));
}

void test_is_underflow_neg(TestObjs *objs) {
  Fixedpoint half;

  Fixedpoint neg_odd1 = fixedpoint_negate(objs->odd_frac1);
  Fixedpoint neg_odd2 = fixedpoint_negate(objs->odd_frac2);
  Fixedpoint min = fixedpoint_negate(objs->max);

  half = fixedpoint_halve(neg_odd1);
  ASSERT(fixedpoint_is_underflow_neg(half));

  half = fixedpoint_halve(neg_odd2);
  ASSERT(fixedpoint_is_underflow_neg(half));
  
  half = fixedpoint_halve(min);
  ASSERT(fixedpoint_is_underflow_neg(half));
}

void test_is_err(TestObjs *objs) {
  (void) objs;

  // too many characters
  Fixedpoint err1 = fixedpoint_create_from_hex("88888888888888889.6666666666666666");
  ASSERT(fixedpoint_is_err(err1));

  // too many characters
  Fixedpoint err2 = fixedpoint_create_from_hex("6666666666666666.88888888888888889");
  ASSERT(fixedpoint_is_err(err2));

  // this one is actually fine
  Fixedpoint err3 = fixedpoint_create_from_hex("-6666666666666666.8888888888888888");
  ASSERT(fixedpoint_is_valid(err3));
  ASSERT(!fixedpoint_is_err(err3));

  // whole part is too large
  Fixedpoint err4 = fixedpoint_create_from_hex("88888888888888889");
  ASSERT(fixedpoint_is_err(err4));

  // fractional part is too large
  Fixedpoint err5 = fixedpoint_create_from_hex("7.88888888888888889");
  ASSERT(fixedpoint_is_err(err5));

  // invalid hex digits in whole part
  Fixedpoint err6 = fixedpoint_create_from_hex("123xabc.4");
  ASSERT(fixedpoint_is_err(err6));

  // invalid hex digits in fractional part
  Fixedpoint err7 = fixedpoint_create_from_hex("7.0?4");
  ASSERT(fixedpoint_is_err(err7));
}

// TODO: implement more test functions
