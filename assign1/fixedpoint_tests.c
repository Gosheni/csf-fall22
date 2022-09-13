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

  //X.Y
  Fixedpoint val1 = fixedpoint_create_from_hex("f6a5865.00f2");
  ASSERT(fixedpoint_is_valid(val1));
  ASSERT(0xf6a5865UL == fixedpoint_whole_part(val1));
  ASSERT(0x00f2000000000000UL == fixedpoint_frac_part(val1));

  //Capital Letters
  Fixedpoint val2 = fixedpoint_create_from_hex("F6a5865");
  ASSERT(fixedpoint_is_valid(val2));
  ASSERT(0xF6a5865UL == fixedpoint_whole_part(val2));
  ASSERT(0UL == fixedpoint_frac_part(val2));

  //0.Y
  Fixedpoint zeroWhole = fixedpoint_create_from_hex("0.00f2");
  ASSERT(fixedpoint_is_valid(zeroWhole));
  ASSERT(!fixedpoint_is_neg(zeroWhole));
  ASSERT(0 == fixedpoint_whole_part(zeroWhole));
  ASSERT(0x00f2000000000000UL == fixedpoint_frac_part(zeroWhole));

  //-0.Y
  Fixedpoint zeroWhole_neg = fixedpoint_create_from_hex("-0.00f2");
  ASSERT(fixedpoint_is_valid(zeroWhole_neg));
  ASSERT(fixedpoint_is_neg(zeroWhole_neg));
  ASSERT(0 == fixedpoint_whole_part(zeroWhole_neg));
  ASSERT(0x00f2000000000000UL == fixedpoint_frac_part(zeroWhole_neg));

  //X.0
  Fixedpoint zeroFrac = fixedpoint_create_from_hex("f6a5865.0");
  ASSERT(fixedpoint_is_valid(zeroFrac));
  ASSERT(!fixedpoint_is_neg(zeroFrac));
  ASSERT(0xf6a5865UL == fixedpoint_whole_part(zeroFrac));
  ASSERT(0 == fixedpoint_frac_part(zeroFrac));

  //-X.0
  Fixedpoint zeroFrac_neg = fixedpoint_create_from_hex("-f6a5865.0");
  ASSERT(fixedpoint_is_valid(zeroFrac_neg));
  ASSERT(fixedpoint_is_neg(zeroFrac_neg));
  ASSERT(0xf6a5865UL == fixedpoint_whole_part(zeroFrac_neg));
  ASSERT(0 == fixedpoint_frac_part(zeroFrac_neg));

  //-X.000
  Fixedpoint zerosFrac_neg = fixedpoint_create_from_hex("-f6a5865.000");
  ASSERT(fixedpoint_is_valid(zerosFrac_neg));
  ASSERT(fixedpoint_is_neg(zerosFrac_neg));
  ASSERT(0xf6a5865UL == fixedpoint_whole_part(zerosFrac_neg));
  ASSERT(0 == fixedpoint_frac_part(zerosFrac_neg));

  //.Y
  Fixedpoint noWhole = fixedpoint_create_from_hex(".00f2");
  ASSERT(fixedpoint_is_valid(noWhole));
  ASSERT(!fixedpoint_is_neg(noWhole));
  ASSERT(0 == fixedpoint_whole_part(noWhole));
  ASSERT(0x00f2000000000000UL == fixedpoint_frac_part(noWhole));

  //-.Y
  Fixedpoint noWhole_neg = fixedpoint_create_from_hex("-.00f2");
  ASSERT(fixedpoint_is_valid(noWhole_neg));
  ASSERT(fixedpoint_is_neg(noWhole_neg));
  ASSERT(0 == fixedpoint_whole_part(noWhole_neg));
  ASSERT(0x00f2000000000000UL == fixedpoint_frac_part(noWhole_neg));

  //X.
  Fixedpoint noFrac = fixedpoint_create_from_hex("f6a5865.");
  ASSERT(fixedpoint_is_valid(noFrac));
  ASSERT(!fixedpoint_is_neg(noFrac));
  ASSERT(0xf6a5865UL == fixedpoint_whole_part(noFrac));
  ASSERT(0 == fixedpoint_frac_part(noFrac));

  //-X.
  Fixedpoint noFrac_neg = fixedpoint_create_from_hex("-f6a5865.");
  ASSERT(fixedpoint_is_valid(noFrac_neg));
  ASSERT(fixedpoint_is_neg(noFrac_neg));
  ASSERT(0xf6a5865UL == fixedpoint_whole_part(noFrac_neg));
  ASSERT(0 == fixedpoint_frac_part(noFrac_neg));
}

void test_format_as_hex(TestObjs *objs) {
  char *s;

  // non-negative values
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

  // negative values
  s = fixedpoint_format_as_hex(fixedpoint_negate(objs->one));
  ASSERT(0 == strcmp(s, "-1"));
  free(s);

  s = fixedpoint_format_as_hex(fixedpoint_negate(objs->one_fourth));
  ASSERT(0 == strcmp(s, "-0.4"));
  free(s);

  s = fixedpoint_format_as_hex(fixedpoint_negate(objs->large1));
  ASSERT(0 == strcmp(s, "-4b19efcea.000000ec9a1e2418"));
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
  //p + p
  lhs = fixedpoint_create_from_hex("c7252a193ae07.7a51de9ea0538c5");
  rhs = fixedpoint_create_from_hex("d09079.1e6d601");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(!fixedpoint_is_neg(sum));
  ASSERT(0xc7252a2643e80UL == fixedpoint_whole_part(sum));
  ASSERT(0x98bf3eaea0538c50UL == fixedpoint_frac_part(sum));

  //n + p
  lhs = fixedpoint_create_from_hex("-c7252a193ae07.7a51de9ea0538c5");
  rhs = fixedpoint_create_from_hex("d09079.1e6d601");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0xc7252a0c31d8eUL == fixedpoint_whole_part(sum));
  ASSERT(0x5be47e8ea0538c50UL == fixedpoint_frac_part(sum));
  
  //p + n
  lhs = fixedpoint_create_from_hex("c7252a193ae07.7a51de9ea0538c5");
  rhs = fixedpoint_create_from_hex("-d09079.1e6d601");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(!fixedpoint_is_neg(sum));
  ASSERT(0xc7252a0c31d8eUL == fixedpoint_whole_part(sum));
  ASSERT(0x5be47e8ea0538c50UL == fixedpoint_frac_part(sum));

  //n + n
  lhs = fixedpoint_create_from_hex("-c7252a193ae07.7a51de9ea0538c5");
  rhs = fixedpoint_create_from_hex("-d09079.1e6d601");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0xc7252a2643e80UL == fixedpoint_whole_part(sum));
  ASSERT(0x98bf3eaea0538c50UL == fixedpoint_frac_part(sum));

  //n + 0
  lhs = fixedpoint_create_from_hex("-c7252a193ae07.7a51de9ea0538c5");
  rhs = fixedpoint_create_from_hex("0");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0xc7252a193ae07UL == fixedpoint_whole_part(sum));
  ASSERT(0x7a51de9ea0538c50UL == fixedpoint_frac_part(sum));

  //0 + p
  lhs = fixedpoint_create_from_hex("0");
  rhs = fixedpoint_create_from_hex("d09079.1e6d601");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(!fixedpoint_is_neg(sum));
  ASSERT(0xd09079UL == fixedpoint_whole_part(sum));
  ASSERT(0x1e6d601000000000UL == fixedpoint_frac_part(sum));

  //0 + 0
  lhs = fixedpoint_create_from_hex("0");
  rhs = fixedpoint_create_from_hex("0");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_zero(sum));
  ASSERT(0 == fixedpoint_whole_part(sum));
  ASSERT(0 == fixedpoint_frac_part(sum));
}

void test_sub(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, diff;

  //p - p
  lhs = fixedpoint_create_from_hex("ccf35aa3a04a3b.b105");
  rhs = fixedpoint_create_from_hex("f676e8.58");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(!fixedpoint_is_neg(diff));
  ASSERT(0xccf35aa2a9d353UL == fixedpoint_whole_part(diff));
  ASSERT(0x5905000000000000UL == fixedpoint_frac_part(diff));

  //n - p
  lhs = fixedpoint_create_from_hex("-ccf35aa3a04a3b.b105");
  rhs = fixedpoint_create_from_hex("f676e8.58");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(diff));
  ASSERT(0xccf35aa496c124UL == fixedpoint_whole_part(diff));
  ASSERT(0x0905000000000000UL == fixedpoint_frac_part(diff));
  
  //p - n
  lhs = fixedpoint_create_from_hex("ccf35aa3a04a3b.b105");
  rhs = fixedpoint_create_from_hex("-f676e8.58");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(!fixedpoint_is_neg(diff));
  ASSERT(0xccf35aa496c124UL == fixedpoint_whole_part(diff));
  ASSERT(0x0905000000000000UL == fixedpoint_frac_part(diff));

  //n - n
  lhs = fixedpoint_create_from_hex("-ccf35aa3a04a3b.b105");
  rhs = fixedpoint_create_from_hex("-f676e8.58");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(diff));
  ASSERT(0xccf35aa2a9d353UL == fixedpoint_whole_part(diff));
  ASSERT(0x5905000000000000UL == fixedpoint_frac_part(diff));

  //0 - p
  lhs = fixedpoint_create_from_hex("0");
  rhs = fixedpoint_create_from_hex("f676e8.58");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(diff));
  ASSERT(0xf676e8UL == fixedpoint_whole_part(diff));
  ASSERT(0x5800000000000000UL == fixedpoint_frac_part(diff));

  //n - 0
  lhs = fixedpoint_create_from_hex("-ccf35aa3a04a3b.b105");
  rhs = fixedpoint_create_from_hex("0");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(diff));
  ASSERT(0xccf35aa3a04a3bUL == fixedpoint_whole_part(diff));
  ASSERT(0xb105000000000000UL == fixedpoint_frac_part(diff));

  //0 - 0
  lhs = fixedpoint_create_from_hex("0");
  rhs = fixedpoint_create_from_hex("0");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_zero(diff));
  ASSERT(0 == fixedpoint_whole_part(diff));
  ASSERT(0 == fixedpoint_frac_part(diff));
}

void test_halve(TestObjs *objs) {
  (void) objs;

  Fixedpoint s, halfPositive, halfNegative, halfZero;

  //positive
  s = fixedpoint_create_from_hex("d09079.1e6d604");
  halfPositive = fixedpoint_halve(s);
  ASSERT(!fixedpoint_is_neg(halfPositive));
  ASSERT(0x68483cUL == fixedpoint_whole_part(halfPositive));
  ASSERT(0x8f36b02000000000UL == fixedpoint_frac_part(halfPositive));

  //negative
  s = fixedpoint_create_from_hex("-c7252a193ae06.7a51de9ea0538c4");
  halfNegative = fixedpoint_halve(s);
  ASSERT(fixedpoint_is_neg(halfNegative));
  ASSERT(0x6392950c9d703UL == fixedpoint_whole_part(halfNegative));
  ASSERT(0x3d28ef4f5029c620UL == fixedpoint_frac_part(halfNegative));
  
  //zero
  s = fixedpoint_create_from_hex("0");
  halfZero = fixedpoint_halve(s);
  ASSERT(fixedpoint_is_zero(halfZero));
  ASSERT(0 == fixedpoint_whole_part(halfZero));
  ASSERT(0 == fixedpoint_frac_part(halfZero));
}

void test_double(TestObjs *objs) {
  (void) objs;

  Fixedpoint s, doublePositive, doubleNegative, doubleZero;

  //positive
  s = fixedpoint_create_from_hex("d09079.1e6d601");
  doublePositive = fixedpoint_double(s);
  ASSERT(!fixedpoint_is_neg(doublePositive));
  ASSERT(0x1a120f2UL == fixedpoint_whole_part(doublePositive));
  ASSERT(0x3cdac02000000000UL == fixedpoint_frac_part(doublePositive));

  //negative
  s = fixedpoint_create_from_hex("-c7252a193ae07.a51de9ea0538c5");
  doubleNegative = fixedpoint_double(s);
  ASSERT(fixedpoint_is_neg(doubleNegative));
  ASSERT(0x18e4a543275c0fUL == fixedpoint_whole_part(doubleNegative));
  ASSERT(0x4a3bd3d40a718a00UL == fixedpoint_frac_part(doubleNegative));
  
  //zero
  s = fixedpoint_create_from_hex("0");
  doubleZero = fixedpoint_double(s);
  ASSERT(fixedpoint_is_zero(doubleZero));
  ASSERT(0 == fixedpoint_whole_part(doubleZero));
  ASSERT(0 == fixedpoint_frac_part(doubleZero));
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