#define CTEST_LIB_IMPLEMENTATION
#include "../ctest.h"

ct_test(simple, test) 
{
  ct_assert(1, "success test");
}

ct_test(simple, fail)
{
  ct_assert(0, "fail test");
}

ct_test(simple, not_null) 
{
  ct_assert_not_null("test", "successfull not null test");
  ct_assert_not_null(NULL, "failing not null test");
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
