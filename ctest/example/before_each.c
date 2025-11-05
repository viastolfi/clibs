#define CTEST_BEFORE_EACH
#define CTEST_LIB_IMPLEMENTATION
#include "../ctest.h"

before_each(int, value, int entry) {
  value = entry + 3;
}

ct_test(simple, before_each, 1) 
{
  ct_assert_eq(4, value, "success test with before each");
}

ct_test(second, before_each, 2)
{
  ct_assert_eq(5, value, "second success test with before each");
  ct_assert_eq(6, value, "fail test with before each");
}
