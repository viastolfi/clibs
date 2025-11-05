#define CT_LIB_IMPLEMENTATION
#include "../ctest.h"

ct_test(simple, test) 
{
  ct_assert(1, "success test");
}

ct_test(simple, fail)
{
  ct_assert(0, "fail test");
}

ct_test(eq, test)
{
  ct_assert_eq(1, 1, "success int eq test");
  ct_assert_eq("test", "test", "success string eq test");
}

ct_test(eq, test_fail)
{
  ct_assert_eq(1, 2, "fail int eq test");
  ct_assert_eq("test", "fail", "fail string eq test");
}
