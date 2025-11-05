#define CTEST_BEFORE_EACH
#define CTEST_LIB_IMPLEMENTATION
#include "../ctest.h"

/*
 * Example on how the lib works with before_each function
 * Before each ct_test function, the before_each function will be called
 * This before_each function set a reusable int value called value
 * The before_each function will always be called with a int param called entry
 * The type and name of the reusable value and the definition of the param can be customized on need
 */
before_each(int, value, int entry) {
  value = entry + 3;
}

// Here, we set the `value` var by calling the before_each function with `1`
ct_test(simple, before_each, 1) 
{
  ct_assert_eq(4, value, "success test with before each");
}

// Here, we set the `value` var by calling the before_each function with `2`
ct_test(second, before_each, 2)
{
  ct_assert_eq(5, value, "second success test with before each");
  ct_assert_eq(6, value, "fail test with before each");
}
