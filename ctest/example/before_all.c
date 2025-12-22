#define CTEST_BEFORE_ALL
#define CTEST_LIB_IMPLEMENTATION
#include "../ctest.h"

before_all(int, v) {
  v = 4;
}

ct_test(before_all, success) {
  ct_assert_eq(v, 4, "Success test using var defined in before all func");
}

ct_test(before_all, faile) {
  ct_assert_eq(v, 2, "Failing test using var defined in before all func");
}
